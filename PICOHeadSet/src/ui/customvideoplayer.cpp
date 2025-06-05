#include "customvideoplayer.h"
#include <QPainter>
#include <QMouseEvent>
#include <QColor>
#include <QDebug>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>

CustomVideoPlayer::CustomVideoPlayer(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);

    m_titleText = tr("PICO NEO 3");

    setupUI();
}

void CustomVideoPlayer::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_yuvPlayerWidget = new YuvPlayer(this);
    layout->addWidget(m_yuvPlayerWidget);

//    connect(m_yuvPlayerWidget, &YuvPlayer::switchButtonClicked, this, &CustomVideoPlayer::switchRequested);
//    connect(m_yuvPlayerWidget, &YuvPlayer::maxButtonClicked, this, &CustomVideoPlayer::fullscreenRequested);
//    connect(this, &CustomVideoPlayer::updateFrame, m_yuvPlayerWidget, &YuvPlayer::updateYuvFrame);
}

void CustomVideoPlayer::setTitleName(const QString &titleName)
{
    m_titleText = titleName;
    m_yuvPlayerWidget->setTitleName(titleName);
}

void CustomVideoPlayer::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void CustomVideoPlayer::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}
