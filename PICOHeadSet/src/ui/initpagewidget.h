#ifndef INITPAGEWIDGET_H
#define INITPAGEWIDGET_H

#include "framelesswindow.h"
#include "../../stdafx.h"
#include "networkdisplaywidget.h"
#include "devicelinkwidget.h"
#include "topwidget.h"

class NetworkMonitor;

class NetworkCheckThread : public QThread
{
    Q_OBJECT
public:
    explicit NetworkCheckThread(QObject* parent = nullptr);

signals:
    void networkStatusUpdated(const QString &networkName, bool isConnected);

protected:
    void run() override;
};

class InitPageWidget : public QWidget//FramelessWindow
{
    Q_OBJECT
public:
    explicit InitPageWidget(QWidget *parent = nullptr);

    void setupUI();
    void setTitleButtonDispaly(TITLE_BUTTONTYPE type);

    DeviceListWidget *devListWidget() const { return m_devListWidget->deviceListWidget(); }

signals:
    void updateNetworkStatus(const QString &networkName, bool isConnected);
    void updateDeviceList();
    void connectSuccess();

    void sigAddDevice(const QString &id);

    void connectRequest(const QString &id);
    void disconnectRequest(const QString &id);

public slots:
    void addDevice(const QString &id);
    void updateDeviceState(const QString &id, RotatingButton::State state);
    void onConnectRequest(const QString &id);
    void onDisconnectRequest(const QString &id);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    TopWidget *m_topWidget;
    NetworkDisplayWidget *m_nwDisplayWidget;
    DeviceLinkWidget *m_devListWidget;

    NetworkCheckThread *m_networkThread;
    NetworkMonitor *m_monitor;
};

#endif // INITPAGEWIDGET_H
