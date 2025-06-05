#include "yuvplayer.h"
#include <QImage>
#include <QPainter>
#include <QVector>
#include <libyuv.h>
#include <QMouseEvent>
#include <QDebug>
#include <QCoreApplication>
#include <QThreadPool>

YuvConvertTask::YuvConvertTask(const QByteArray &yuvData, int width, int height, int dstW, int dstH, QAtomicInt *cancelFlag)
    : m_yuv(yuvData), m_width(width), m_height(height), m_dstW(dstW), m_dstH(dstH), m_cancelFlag(cancelFlag)
{
}

void YuvConvertTask::run()
{
    if (!m_yuv.size() || m_width <= 0 || m_height <= 0 || m_dstW <= 0 || m_dstH <= 0) return;
    if (m_cancelFlag && int(*m_cancelFlag) != 0) return;

    // 分离YUV平面
    const uint8_t *yPlane = reinterpret_cast<const uint8_t*>(m_yuv.constData());
    const uint8_t *uPlane = yPlane + m_width * m_height;
    const uint8_t *vPlane = uPlane + (m_width / 2) * (m_height / 2);

    int dstW = m_dstW;
    int dstH = m_dstH;
    if (dstW % 2)
        --dstW;
    if (dstH % 2)
        --dstH;

    int dst_stride_y = dstW;
    int dst_stride_u = dstW / 2;
    int dst_stride_v = dstW / 2;

    QVector<uint8_t> scaledY(dst_stride_y * dstH);
    QVector<uint8_t> scaledU(dst_stride_u * (dstH / 2));
    QVector<uint8_t> scaledV(dst_stride_v * (dstH / 2));

    int ret = libyuv::I420Scale(
        yPlane, m_width,
        uPlane, m_width / 2,
        vPlane, m_width / 2,
        m_width, m_height,
        scaledY.data(), dst_stride_y,
        scaledU.data(), dst_stride_u,
        scaledV.data(), dst_stride_v,
        dstW, dstH,
        libyuv::kFilterBilinear
        );
    if (ret != 0) {
        qDebug() << "I420Scale Failed.";
    }

    if (m_cancelFlag && int(*m_cancelFlag) != 0) return;

    // 计算所需缓冲区大小
    int stride = m_width * 4;
    int stride_size = stride * m_height;
    int targetW = m_width;
    int targetH = m_height;
    if (ret == 0) {
        stride = dstW * 4;
        stride_size = stride * dstH;
        targetW = dstW;
        targetH = dstH;
    }
//    QVector<uchar> rgbBuffer(stride * m_height);
    QVector<uchar> rgbBuffer(stride_size);

    if (ret == 0) {
        libyuv::I420ToARGB(
            scaledY.data(), dst_stride_y,
            scaledU.data(), dst_stride_u,
            scaledV.data(), dst_stride_v,
            rgbBuffer.data(), stride,
            targetW, targetH);
    } else {
        libyuv::I420ToARGB(
            yPlane, m_width,
            uPlane, m_width / 2,
            vPlane, m_width / 2,
            rgbBuffer.data(), stride,
            targetW, targetH);
    }

    if (m_cancelFlag && int(*m_cancelFlag) != 0) return;

    // 创建图像并发送
    QImage image(rgbBuffer.data(), targetW, targetH, stride, QImage::Format_ARGB32);

    emit frameConverted(image.copy());
}

YuvPlayer::YuvPlayer(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_isLinked(false)
    , m_isActive(false)
    , m_hovered(false)
    , m_isFull(false)
    , m_texture(nullptr)
    , m_hideTimer(nullptr)
{
    setMinimumSize(320, 240);
    m_blankImage = QImage(":/images/device_disconnect2.png");
    m_maxBtnPixmap = QPixmap(":/images/fullscreen_btn.png");

    m_cancelFlag = 0;
    m_processingFrame = 0;
    QThreadPool::globalInstance()->setMaxThreadCount(qMax(QThread::idealThreadCount(), 2));
}

YuvPlayer::~YuvPlayer()
{
    stopConverter();
    if (m_texture) { delete m_texture; m_texture = nullptr; }
    if (m_hideTimer) { m_hideTimer->stop(); m_hideTimer->deleteLater(); m_hideTimer = nullptr; }
}

void YuvPlayer::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void YuvPlayer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void YuvPlayer::paintGL()
{
    qDebug() << "paintGL called" << m_titleText;

    glClear(GL_COLOR_BUFFER_BIT);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if (!m_isLinked) {
        painter.fillRect(rect(), QColor("#393939"));
        if (!m_blankImage.isNull()) {
            QImage scaledImg = m_blankImage.scaled(size() / 4, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QPoint topLeft((width() - scaledImg.width()) / 2, (height() - scaledImg.height()) / 2);
            painter.drawImage(topLeft, scaledImg);
        }
        painter.save();
        painter.setPen(Qt::white);
        QFont font("Microsoft YaHei", 12, QFont::Bold);
        painter.setFont(font);
        painter.drawText(20, 30, m_titleText);
        painter.restore();

        painter.save();
        painter.setPen(QColor("#BBBBBB"));
        QFont font2("Microsoft YaHei", 14, QFont::Bold);
        painter.setFont(font2);
        qreal widgetHeight = height();
        qreal textAreaY = (1.0 / 2.0) * widgetHeight + 10;
        qreal textAreaHeight = widgetHeight / 3.0;
        QRectF textRect(0, textAreaY, width(), textAreaHeight);
        painter.drawText(textRect, Qt::AlignCenter, QStringLiteral("未连接"));
        painter.restore();

//        if (m_hovered || m_isActive) {
//            drawButtons(painter);
//        }

        return;
    }

    QImage toDraw;
    {
        QMutexLocker locker(&m_frameMutex);
        toDraw = m_currentFrame;
    }
    qDebug() << "paintGL toDraw.isNull()=" << toDraw.isNull() << "size=" << toDraw.size();
    if (toDraw.isNull()) {
        painter.fillRect(rect(), Qt::black);
    } else {
        QImage scaledImg = toDraw.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPoint topLeft((width() - scaledImg.width()) / 2, (height() - scaledImg.height()) / 2);
        painter.drawImage(topLeft, scaledImg);
        qDebug() << "paintGL called drawImage88888888888888888888888888888888888888";
    }

    drawTitle(painter);

    if (m_hovered || m_isActive) {
        drawButtons(painter);
    }
}

void YuvPlayer::handleConvertedFrame(const QImage &image)
{
    m_processingFrame.deref();
    if (!m_isLinked) return; // 断开时不再处理
    QMutexLocker locker(&m_frameMutex);
    m_currentFrame = image;
    update();
}

void YuvPlayer::updateYuvFrame(const uint8_t *yuvData, int width, int height)
{
    qDebug() << "updateYuvFrame called"
             << "this=" << this
             << "m_isLinked=" << m_isLinked
             << "yuvData=" << yuvData
             << "width=" << width
             << "height=" << height
             << "titlename = " << titleName();

    if (!m_isLinked || !yuvData || width <= 0 || height <= 0) {
        qDebug() << "updateYuvFrame early return: m_isLinked=" << m_isLinked
                 << "yuvData=" << yuvData
                 << "width=" << width
                 << "height=" << height;
        return;
    }

//    if (m_processingFrame.fetchAndAddRelaxed(1) > 0) {
//        m_processingFrame.deref();
//        return;
//    }

    m_cancelFlag.store(0);
    QByteArray yuvCopy(reinterpret_cast<const char *>(yuvData), width * height * 3 / 2);

    int dstW = this->width();
    int dstH = this->height();
    if (dstW % 2) --dstW;
    if (dstH % 2) --dstH;

    qDebug() << "updateYuvFrame, dstW =" << dstW << ", dstH =" << dstH;

    YuvConvertTask *task = new YuvConvertTask(yuvCopy, width, height, dstW, dstH, &m_cancelFlag);
    connect(task, SIGNAL(frameConverted(QImage)), this, SLOT(handleConvertedFrame(QImage)), Qt::QueuedConnection);
    task->setAutoDelete(true);

    QThreadPool::globalInstance()->start(task);
}

void YuvPlayer::setTitleName(const QString &name)
{
    if (m_titleText != name) {
        m_titleText = name;
        update();
    }
}

void YuvPlayer::updateLinkStatus(bool linked)
{
    if (m_isLinked != linked) {
        m_isLinked = linked;
        if (!m_isLinked) {
            stopConverter();
            QMutexLocker locker(&m_frameMutex);
            m_currentFrame = QImage();
        }
        update();
    }
}

void YuvPlayer::stopConverter()
{
    // 请求所有正在转换的任务尽量停止（任务会快速自检 m_cancelFlag）
    m_cancelFlag.store(1);
    // 等待所有在处理的帧完成（保证不会再有 handleConvertedFrame 回调）
//    while (int(m_processingFrame) > 0) {
//        QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
//    }
}

void YuvPlayer::setActive(bool active)
{
    if (m_isActive == active) return;
    m_isActive = active;
    update();
}

void YuvPlayer::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    m_hovered = true;
    if (m_hideTimer) {
        m_hideTimer->stop();
        m_hideTimer->deleteLater();
        m_hideTimer = nullptr;
    }
    update();
}

void YuvPlayer::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    if (m_hideTimer) {
        m_hideTimer->stop();
        m_hideTimer->deleteLater();
        m_hideTimer = nullptr;
    }
    m_hideTimer = new QTimer(this);
    m_hideTimer->setSingleShot(true);
    m_hideTimer->setInterval(1000);
    connect(m_hideTimer, &QTimer::timeout, [this]() {
        m_hovered = false;
        update();
        m_hideTimer->deleteLater();
        m_hideTimer = nullptr;
    });
    m_hideTimer->start();
}

void YuvPlayer::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit fullScreenRequested(property("playerIndex").toInt());
    }
    QOpenGLWidget::mouseDoubleClickEvent(event);
}

void YuvPlayer::mousePressEvent(QMouseEvent *event)
{
    QPoint pt = event->pos();
    if (m_linkBtnRect.contains(pt)) {
        emit linkButtonClicked(m_titleText, !m_isLinked);
    }
    else if (m_switchBtnRect.contains(pt)) {
        emit switchButtonClicked();
    }
    else if (m_maxBtnRect.contains(pt)) {
        m_isFull = !m_isFull;
        emit maxButtonClicked(m_titleText, m_isLinked, m_isFull);
    }
//    update();
    QOpenGLWidget::mousePressEvent(event);
}

void YuvPlayer::drawButtons(QPainter &painter)
{
    const int btnW = 60;
    const int btnH = 30;
    const int spacing = 10;
    int right = width() - spacing;
    int top = height() - btnH - spacing;

    m_maxBtnRect = QRect(right - btnW, top, btnH, btnH);
    m_switchBtnRect = QRect(right - 2 * btnW - spacing, top, btnW, btnH);
    m_linkBtnRect = QRect(right - 3 * btnW - 2 * spacing, top, btnW, btnH);

    qDebug() << "DrawButton: " << m_isLinked;
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_isLinked ? QColor("#28a745") : QColor("#0078D7"));
    painter.drawRoundedRect(m_linkBtnRect, 5, 5);
    painter.setPen(Qt::white);
    painter.drawText(m_linkBtnRect, Qt::AlignCenter, m_isLinked ? "UnLink" : "Link");
    painter.restore();

//    painter.save();
//    painter.setPen(Qt::NoPen);
//    painter.setBrush(QColor("#0078D7"));
//    painter.drawRoundedRect(m_switchBtnRect, 5, 5);
//    painter.setPen(Qt::white);
//    painter.drawText(m_switchBtnRect, Qt::AlignCenter, "-");
//    painter.restore();

    painter.save();
    painter.setPen(Qt::NoPen);
//    painter.setBrush(QColor("#0078D7"));
    painter.drawRoundedRect(m_maxBtnRect, btnH, btnH);
//    painter.setPen(Qt::white);
//    painter.drawText(m_maxBtnRect, Qt::AlignCenter, "o");
    painter.drawPixmap(m_maxBtnRect, m_maxBtnPixmap);
    painter.restore();
}

void YuvPlayer::drawTitle(QPainter &painter)
{
    painter.save();
    painter.setPen(Qt::white);
    QFont font("Arial", 12, QFont::Bold);
    painter.setFont(font);
    painter.drawText(20, 30, m_titleText);
    painter.restore();
}

//YuvPlayer::YuvPlayer(QWidget *parent)
//    : QWidget{parent}
//{
//    setMinimumSize(320, 240);

//    m_blankImage = QImage(":/images/device_disconnect2.png");
//}

//void YuvPlayer::setTitleName(const QString &name)
//{
//    if (m_titleText != name) {
//        m_titleText = name;
//        update();
//    }
//}

//void YuvPlayer::updateLinkStatus(bool isLink)
//{
//    m_isLinked = isLink;
//    update();
//}

//void YuvPlayer::updateYuvFrame(const uint8_t *yuvData, int width, int height)
//{
//    if (!m_isLinked)
//        return;

//    if (!yuvData || width <= 0 || height <= 0)
//        return;

//    int stride = width * 4;
//    int requiredSize = stride * height;

//    if (m_rgbBuffer.size() < requiredSize)
//        m_rgbBuffer.resize(requiredSize);

//    const uint8_t *yPlane = yuvData;
//    const uint8_t *uPlane = yuvData + width * height;
//    const uint8_t *vPlane = yuvData + width * height + (width / 2) * (height / 2);

//    // 使用libyuv 将 I420 转为 ARGB
//    libyuv::I420ToARGB(
//        yPlane, width,
//        uPlane, width / 2,
//        vPlane, width / 2,
//        m_rgbBuffer.data(), stride,
//        width, height);

//    m_image = QImage(m_rgbBuffer.data(), width, height, stride, QImage::Format_ARGB32).copy();

//    update();
//}

//void YuvPlayer::paintEvent(QPaintEvent *event)
//{
//    Q_UNUSED(event)

//    QPainter painter(this);
//    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

//    setAutoFillBackground(true);


//    /***************************** 绘制画面 *****************************/
//    if (!m_isLinked) {
//        painter.fillRect(rect(), "#393939");
//        /***************************** 绘制画面 *****************************/
//        if (!m_blankImage.isNull()) {
//            QImage scaledImg = m_blankImage.scaled(size() / 3, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//            QPoint topLeft((width() - scaledImg.width()) / 2, (height() - scaledImg.height()) / 2);
//            painter.drawImage(topLeft, scaledImg);
//        }

//        /***************************** 绘制左上角标题文字 *****************************/
//        painter.save();
//        painter.setPen(Qt::white);
//        QFont font("Microsoft YaHei", 14, QFont::Bold);
//        painter.setFont(font);
//        painter.drawText(20, 30, m_titleText);
//        painter.restore();

//        painter.save();
//        painter.setPen(QColor("#BBBBBB"));
//        QFont font2("Microsoft YaHei", 18, QFont::Bold);
//        painter.setFont(font);
//        // 计算下方1/3高度的区域
//        qreal widgetHeight = height();
//        qreal textAreaY = (1.0 / 2.0) * widgetHeight + 10;
//        qreal textAreaHeight = widgetHeight / 3.0;
//        QRectF textRect(0, textAreaY, width(), textAreaHeight);
//        // 在矩形区域内居中绘制文字
//        painter.drawText(textRect, Qt::AlignCenter, QStringLiteral("未连接"));
//        painter.restore();
//    }
//    else {
//        painter.fillRect(rect(), Qt::black);

//        if (!m_image.isNull()) {
//            //        painter.drawImage(rect(), m_image);

//            QImage scaledImg = m_image.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
//            QPoint topLeft((width() - scaledImg.width()) / 2, (height() - scaledImg.height()) / 2);
//            painter.drawImage(topLeft, scaledImg);
//        }

//        /***************************** 绘制左上角标题文字 *****************************/
//        painter.save();
//        painter.setPen(Qt::white);
//        QFont font("Arial", 16, QFont::Bold);
//        painter.setFont(font);
//        painter.drawText(20, 30, m_titleText);
//        painter.restore();

//        /***************************** 绘制按钮 *****************************/
//        drawButtons(painter);
//    }
//}

//void YuvPlayer::drawButtons(QPainter &painter)
//{
//    const int btnW = 60;
//    const int btnH = 30;
//    const int spacing = 10;
//    int right = width() - spacing;
//    int top = spacing;

//    // 最大化按钮
//    m_maxBtnRect = QRect (right - btnW, top, btnW, btnH);
//    // 切换按钮
//    m_switchBtnRect = QRect (right - 2 * btnW - spacing, top, btnW, btnH);
//    // 连接按钮
//    m_linkBtnRect = QRect (right - 3 * btnW - 2 * spacing, top, btnW, btnH);

//    // 绘制按钮背景
//    painter.setPen(Qt::NoPen);

//    // 连接按钮颜色
//    painter.setBrush(m_isLinked ? QColor("#28a745") : QColor("#0078D7"));
//    painter.drawRoundedRect(m_linkBtnRect, 5, 5);
//    painter.setPen(Qt::white);
//    painter.drawText(m_linkBtnRect, Qt::AlignCenter, m_isLinked ? "UnLink" : "Link");

//    // 切换按钮蓝色
//    painter.setPen(Qt::NoPen);
//    painter.setBrush(QColor("#0078D7"));
//    painter.drawRoundedRect(m_switchBtnRect, 5, 5);
//    painter.setPen(Qt::white);
//    painter.drawText(m_switchBtnRect, Qt::AlignCenter, "-");

//    // 最大化按钮蓝色
//    painter.setPen(Qt::NoPen);
//    painter.setBrush(QColor("#0078D7"));
//    painter.drawRoundedRect(m_maxBtnRect, 5, 5);
//    painter.setPen(Qt::white);
//    painter.drawText(m_maxBtnRect, Qt::AlignCenter, "o");
//}

//void YuvPlayer::mousePressEvent(QMouseEvent *event)
//{
//    QPoint pt = event->pos();

//    if (m_linkBtnRect.contains(pt)) {
//        qDebug() << "link button clicked!";
//        m_isLinked = !m_isLinked;
//        emit linkButtonClicked(m_titleText, m_isLinked);
//    }
//    else if (m_switchBtnRect.contains(pt)) {
//        qDebug() << "switch button clicked!";
//        emit switchButtonClicked();
//    }
//    else if (m_maxBtnRect.contains(pt)) {
//        qDebug() << "max button clicked!";
//        emit maxButtonClicked();
//    }

//    update();

//    QWidget::mousePressEvent(event);
//}

//void YuvPlayer::mouseMoveEvent(QMouseEvent *event)
//{
//    QWidget::mouseMoveEvent(event);
//}
