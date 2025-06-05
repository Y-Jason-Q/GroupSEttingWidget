#ifndef DEVICELINKWIDGET_H
#define DEVICELINKWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QMovie>
#include <QPropertyAnimation>
#include "DeviceListWidget/devicelistwidget.h"

class DeviceLinkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceLinkWidget(QWidget *parent = nullptr);
    DeviceListWidget *deviceListWidget() { return m_deviceListWidget; }
    void updateDeviceState(const QString &id, RotatingButton::State state);

private:
    void initUI();
    void initConnect();

signals:
    void sigRefreshButtonClicked();
    void updateDeviceList();
    void connectSuccess();

    void connectRequest(const QString &id);
    void disconnectRequest(const QString &id);

public slots:
    void onRefreshButtonClicked();
    void onRefreshFinished();
    void onAddDevice(const QString &id);

    void onConnectRequest(const QString &id);
    void onDisconnectRequest(const QString &id);

private:
    QLabel *m_iconLabel;
    QLabel *m_titleLabel;
    QToolButton *m_refreshButton;
    DeviceListWidget *m_deviceListWidget;

    QMovie *m_loadingMovie;
    QPropertyAnimation *m_rotateAnimation;
    bool m_isLoading = false;
};

#endif // DEVICELINKWIDGET_H
