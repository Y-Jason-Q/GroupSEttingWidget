#ifndef DEVICELISTPOPWINDOW_H
#define DEVICELISTPOPWINDOW_H

#include <QWidget>
#include "devicelinkwidget.h"

class DeviceListPopWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceListPopWindow(QWidget *parent = nullptr);
    DeviceListWidget *devListWidget() const {return m_devLinkWidget->deviceListWidget();}

signals:
    void sigAddDevice(const QString &devID);
    void deviceStateChanged(const QString &devId, RotatingButton::State state);

public slots:
    void onAddDevice(const QString &devId);

private:
    DeviceLinkWidget *m_devLinkWidget;

};

#endif // DEVICELISTPOPWINDOW_H
