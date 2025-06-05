#include "deviceitem.h"
#include <QHBoxLayout>
#include <QDebug>

DeviceItem::DeviceItem(const QString &name, QWidget *parent)
    : QWidget(parent)
    , m_devName(name)
{
    initUI();
}

void DeviceItem::initUI()
{
    m_devNameLabel = new QLabel(m_devName, this);
    m_linkButton = new RotatingButton(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 0, 5, 0);
    layout->setSpacing(0);
    layout->addWidget(m_devNameLabel);
    layout->addStretch();
    layout->addWidget(m_linkButton);
    setLayout(layout);

    m_devNameLabel->setStyleSheet("background: transparent; font-family: \"Microsoft YaHei\"; font-size: 12px; color: #BBBBBB;");

    connect(m_linkButton, &QToolButton::clicked, [this](){
        qDebug() << "[DeviceItem] clicked: " << m_devName;
        if (m_linkButton->state() == RotatingButton::Disconnected) {
            emit connectRequested(m_devName);
        }
        else if (m_linkButton->state() == RotatingButton::Connected) {
            emit disconnectRequested(m_devName);
        }

        if (m_linkButton->state() == RotatingButton::Disconnected)
            updateState(RotatingButton::Connecting);
        else if (m_linkButton->state() == RotatingButton::Connected)
            updateState(RotatingButton::Disconnecting);
    });
}

void DeviceItem::updateState(RotatingButton::State state)
{
    if (state == RotatingButton::Connecting) {
        m_linkButton->startConnectingAnim();
    } else {
        m_linkButton->stopConnectingAnim(state != RotatingButton::Connecting);
    }
    m_linkButton->setState(state);
}
