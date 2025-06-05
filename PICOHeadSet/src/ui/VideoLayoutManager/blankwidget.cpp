#include "blankwidget.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QFont>

BlankWidget::BlankWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

//    m_titleLabel = new QLabel(this);
//    m_imageLabel = new QLabel(this);
//    m_textLabel = new QLabel(this);

//    mainLayout->addWidget(m_titleLabel, 0, Qt::AlignTop | Qt::AlignLeft);
//    mainLayout->addWidget(m_imageLabel, 0, Qt::AlignCenter);
//    mainLayout->addWidget(m_textLabel, 0, Qt::AlignCenter);

//    m_imageLabel->setStyleSheet("background-image: url(:/images/device_disconnect.png);");
//    m_textLabel->setText(QStringLiteral("未连接"));
//    m_textLabel->setStyleSheet("background-color: transparent; color: #BBBBBB; font-size: 18px;");
//    m_titleLabel->setStyleSheet("background-color: transparent; color: #BBBBBB; font-size: 20px; font-weight: bold;");

    m_titleText = tr("PICO");
    m_statusText = QStringLiteral("未连接");
    m_image = QImage(":/images/device_disconnect2.png");
}

void BlankWidget::setTitleName(const QString &titleName)
{
    m_titleText = titleName;
}

void BlankWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    painter.fillRect(rect(), "#393939");

    QRect rt = this->rect();
    /***************************** 绘制画面 *****************************/
    if (!m_image.isNull()) {
        QImage scaledImg = m_image.scaled(size() / 3, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPoint topLeft((width() - scaledImg.width()) / 2, (height() - scaledImg.height()) / 2);
        painter.drawImage(topLeft, scaledImg);
    }

    /***************************** 绘制左上角标题文字 *****************************/
    painter.save();
    painter.setPen(Qt::white);
    QFont font("Microsoft YaHei", 14, QFont::Bold);
    painter.setFont(font);
    painter.drawText(20, 30, m_titleText);
    painter.restore();

    painter.save();
    painter.setPen(QColor("#BBBBBB"));
    QFont font2("Microsoft YaHei", 18, QFont::Bold);
    painter.setFont(font);
    // 计算下方1/3高度的区域
    qreal widgetHeight = height();
    qreal textAreaY = (1.0 / 2.0) * widgetHeight + 10;
    qreal textAreaHeight = widgetHeight / 3.0;
    QRectF textRect(0, textAreaY, width(), textAreaHeight);
    // 在矩形区域内居中绘制文字
    painter.drawText(textRect, Qt::AlignCenter, m_statusText);
    painter.restore();
}
