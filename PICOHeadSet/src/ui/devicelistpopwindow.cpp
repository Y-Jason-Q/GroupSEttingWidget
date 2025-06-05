#include "devicelistpopwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>

DeviceListPopWindow::DeviceListPopWindow(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::FramelessWindowHint, true);
    setStyleSheet("background-color: #282828; border-radius: 8px;");

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 120));
    setGraphicsEffect(shadow);

    m_devLinkWidget = new DeviceLinkWidget(this);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addWidget(m_devLinkWidget);

    this->setLayout(hlayout);

    resize(420, 300);

    connect(this, &DeviceListPopWindow::sigAddDevice, m_devLinkWidget, &DeviceLinkWidget::onAddDevice);

    connect(m_devLinkWidget, &DeviceLinkWidget::connectRequest, this, [this](const QString &id) {
        emit deviceStateChanged(id, RotatingButton::State::Connected);
    });
    connect(m_devLinkWidget, &DeviceLinkWidget::disconnectRequest, this, [this](const QString &id) {
        emit deviceStateChanged(id, RotatingButton::State::Disconnected);
    });
}

void DeviceListPopWindow::onAddDevice(const QString &devId)
{
//    m_devLinkWidget->onAddDevice(devId);
    emit sigAddDevice(devId);
}
