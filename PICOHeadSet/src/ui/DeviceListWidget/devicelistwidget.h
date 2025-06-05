#ifndef DEVICELISTWIDGET_H
#define DEVICELISTWIDGET_H

#include <QWidget>
#include <QListView>
#include "devicemodel.h"
#include "devicedelegate.h"

class DeviceListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceListWidget(QWidget *parent = nullptr);

public slots:
    void addDevice(const QString &deviceId);
    void removeDevice(const QString &deviceId);
    void clearAllDevices();
    void updateDeviceState(const QString &deviceId, RotatingButton::State state);

    void onConnectRequest(const QString &deviceId);
    void onDisconnectRequest(const QString &deviceId);

signals:
    void deviceConnectRequested(const QString &deviceId);
    void deviceDisconnectRequested(const QString &deviceId);

private:
    void initUI();
    void connectSignals();

private:
    QListView *m_listView;
    DeviceModel *m_model;
    DeviceDelegate *m_delegate;
};

#endif // DEVICELISTWIDGET_H
