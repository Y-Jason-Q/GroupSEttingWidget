#include "initpagewidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "../network_connect.h"

NetworkCheckThread::NetworkCheckThread(QObject *parent)
    : QThread(parent)
{

}

void NetworkCheckThread::run()
{
    QList<WinNetworkInterface> interfaces = getNetworkInterfaces();

    QString activeNetworkName = "No active network";
    bool isConnect = false;
    for (int i = 0; i < interfaces.size(); ++i)
    {
        WinNetworkInterface inter = interfaces.at(i);
        if (inter.isConnected) {
            if (inter.type == QNetworkInterface::Wifi && !inter.wifiSSID.isEmpty()) {
                activeNetworkName = "WIFI: " + inter.wifiSSID;
            }
            else {
                activeNetworkName = "Ethernet: " + inter.name;
            }
            isConnect = true;
            break;
        }
        else {
            continue;
        }
    }

    emit networkStatusUpdated(activeNetworkName, isConnect);
}


InitPageWidget::InitPageWidget(QWidget *parent)
    : QWidget(parent) //FramelessWindow(parent)
{
}

void InitPageWidget::setupUI()
{
    m_topWidget = new TopWidget(this);
    m_nwDisplayWidget = new NetworkDisplayWidget(this);
    m_devListWidget = new DeviceLinkWidget(this);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *hLayout = new QHBoxLayout(this);

    hLayout->addWidget(m_nwDisplayWidget);
    hLayout->addWidget(m_devListWidget);
    hLayout->setSpacing(96);
    hLayout->setContentsMargins(0, 0, 0, 0);

    vLayout->addWidget(m_topWidget);
    vLayout->addLayout(hLayout);
    vLayout->setContentsMargins(0, 0, 0, 40);

    this->setLayout(vLayout);

    m_nwDisplayWidget->setNetworkName("TP-Link 2.000");

    m_networkThread = new NetworkCheckThread(this);
    connect(m_networkThread, &NetworkCheckThread::networkStatusUpdated, m_nwDisplayWidget, &NetworkDisplayWidget::onNetworkStatus);
    connect(m_networkThread, &NetworkCheckThread::networkStatusUpdated, this, &InitPageWidget::updateNetworkStatus);
    m_networkThread->start();

    m_monitor = new NetworkMonitor();
    connect(m_monitor, &NetworkMonitor::networkChanged, this, [this](){
        if (!m_networkThread->isRunning()) {
            m_networkThread->start();
        }
    });

    connect(&m_monitor->m_manger, &QNetworkConfigurationManager::configurationChanged, [this]() {
        qDebug() << "Network Changed ------>";
        if (!m_networkThread->isRunning()) {
            m_networkThread->start();
        }
    });

    connect(this, &InitPageWidget::sigAddDevice, m_devListWidget, &DeviceLinkWidget::onAddDevice);

    connect(m_devListWidget, &DeviceLinkWidget::connectRequest, this, &InitPageWidget::onConnectRequest);
    connect(m_devListWidget, &DeviceLinkWidget::disconnectRequest, this, &InitPageWidget::onDisconnectRequest);
}

void InitPageWidget::addDevice(const QString &id)
{
    m_devListWidget->onAddDevice(id);
}

void InitPageWidget::updateDeviceState(const QString &id, RotatingButton::State state)
{
    m_devListWidget->updateDeviceState(id, state);
}

void InitPageWidget::setTitleButtonDispaly(TITLE_BUTTONTYPE type)
{
//    setTitleButtonType(type);
}

void InitPageWidget::onConnectRequest(const QString &id)
{
//    qDebug() << "[InitPageWidget] onConnectRequest = id" << id;
    emit connectRequest(id);
}

void InitPageWidget::onDisconnectRequest(const QString &id)
{
//    qDebug() << "[InitPageWidget] onDisconnectRequest = id" << id;
    emit disconnectRequest(id);
}

void InitPageWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    QRect rt = this->rect();
    m_nwDisplayWidget->setFixedSize(rt.width() * 0.3, rt.height() * 0.3);
    m_devListWidget->setFixedSize(rt.width() * 0.3, rt.height() * 0.3);
}
