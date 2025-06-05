#include "devicelistwidget.h"
#include <QVBoxLayout>
#include <QTimer>
#include <QDebug>

DeviceListWidget::DeviceListWidget(QWidget *parent)
    : QWidget(parent)
{
    initUI();
    connectSignals();
}

void DeviceListWidget::initUI()
{
    m_listView = new QListView(this);
    m_model = new DeviceModel(this);
    m_delegate = new DeviceDelegate(this);

    m_listView->setModel(m_model);
    m_listView->setItemDelegate(m_delegate);
    m_listView->setSpacing(0);
    m_listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_listView);
    layout->setContentsMargins(0, 0, 0, 0);

    m_listView->setStyleSheet("QListView{border: none; background-color: transparent;}");
}

void DeviceListWidget::connectSignals()
{
    connect(m_delegate, &DeviceDelegate::connectClicked, this, &DeviceListWidget::onConnectRequest);
    connect(m_delegate, &DeviceDelegate::disconnectClicked, this, &DeviceListWidget::onDisconnectRequest);
}

void DeviceListWidget::addDevice(const QString &deviceId)
{
    if (!m_model->exists(deviceId)) {
        m_model->addDevice(deviceId);
        QModelIndex idx = m_model->index(m_model->rowCount() - 1);
        m_listView->openPersistentEditor(idx);  // 让DeviceItem常驻显示
    }
}

void DeviceListWidget::removeDevice(const QString &deviceId)
{
    m_model->removeDevice(deviceId);
}

void DeviceListWidget::clearAllDevices()
{
    m_model->clear();
}

void DeviceListWidget::updateDeviceState(const QString &deviceId, RotatingButton::State state)
{
    qDebug() << "[DeviceListWidget::updateDeviceState] deviceId = " << deviceId << ", state = " << state;
    m_model->updateDeviceState(deviceId, state);
}

void DeviceListWidget::onConnectRequest(const QString &deviceId)
{
//    qDebug()<< "[DeviceListWidget] onConnectRequest - deviceId: " << deviceId;
    updateDeviceState(deviceId, RotatingButton::Connecting);

    emit deviceConnectRequested(deviceId);
}

void DeviceListWidget::onDisconnectRequest(const QString &deviceId)
{
//    qDebug()<< "[DeviceListWidget] onDisconnectRequest - deviceId: " << deviceId;
    updateDeviceState(deviceId, RotatingButton::Disconnected);

    emit deviceDisconnectRequested(deviceId);
}
