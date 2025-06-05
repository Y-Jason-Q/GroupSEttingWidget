#include "videoitemwrapper.h"
#include <QDrag>
#include <QMimeData>
#include <QCursor>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QDebug>
#include "../yuvplayer.h"

VideoItemWrapper::VideoItemWrapper(QWidget *parent)
    : QGraphicsWidget(nullptr)
    , m_id(QUuid::createUuid())
{
    m_player = new YuvPlayer(nullptr); // parent 必须为 nullptr
    createProxyWidget();
    setupConnections();
    setMinimumSize(200, 150);
}

void VideoItemWrapper::setGeometry(const QRectF &rect)
{
    QGraphicsWidget::setGeometry(rect);
    if (m_proxy) {
        m_proxy->resize(rect.size());
        m_proxy->setPos(0, 0);
    }
}

void VideoItemWrapper::setGeometry(qreal x, qreal y, qreal w, qreal h)
{
    setGeometry(QRectF(x, y, w, h));
}

void VideoItemWrapper::createProxyWidget()
{
    m_proxy = new QGraphicsProxyWidget(this);
    m_proxy->setWidget(m_player);
    m_proxy->setGeometry(QRectF(0, 0, 200, 150));
    m_proxy->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void VideoItemWrapper::setupConnections()
{
    connect(m_player, &YuvPlayer::switchButtonClicked, this, &VideoItemWrapper::switchRequested);
    connect(m_player, &YuvPlayer::maxButtonClicked, [this]{ emit fullscreenRequested(this); });
    connect(m_player, &YuvPlayer::linkButtonClicked, this, [this](const QString &, bool linked){
        emit linkRequested(linked, this);
    });
}

void VideoItemWrapper::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit fullscreenRequested(this);
}

void VideoItemWrapper::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragStartPos = event->pos();
        m_dragging = false;
    }
    QGraphicsWidget::mousePressEvent(event);
}

void VideoItemWrapper::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_dragging &&
        (event->pos() - m_dragStartPos).manhattanLength() > QApplication::startDragDistance())
    {
        m_dragging = true;

        QDrag *drag = new QDrag(m_player);
        QMimeData  *mimeData = new QMimeData;

//        QByteArray ba;
//        ba.append(reinterpret_cast<const char*>(this), sizeof(this));
//        mimeData->setData("application/x-videoitemwrapper", ba);
        // 使用Quuid进行拖拽
        mimeData->setData("application/x-videoitemwrapper-uuid", m_id.toByteArray());

        drag->setMimeData(mimeData);
        drag->exec(Qt::MoveAction);
    }

    QGraphicsWidget::mouseMoveEvent(event);
}

void VideoItemWrapper::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_dragging = false;
    QGraphicsWidget::mouseReleaseEvent(event);
}

void VideoItemWrapper::swapVideoWith(VideoItemWrapper *other)
{
    if (!other || other == this)
        return;

    // 交换YuvPlayer中的视频数据(示意，具体实现需你补充)
    // 比如有接口：getCurrentYuvData(), setYuvData(...)
    // 这里简单示意用信号槽或函数调用交换内容

    // 先获取两个player的视频缓存指针或状态，交换后调用update
    // 示例：
    // auto tempData = this->m_player->currentYuvData();
    // this->m_player->setYuvData(other->m_player->currentYuvData());
    // other->m_player->setYuvData(tempData);

    // 你可以根据你视频数据结构具体实现此功能
    // 这里暂时用日志代替
    qDebug() << "swap video content between two VideoItemWrapper";
}
