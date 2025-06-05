#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "rotatingbutton.h"

struct DeviceInfo {
    QString id;
    RotatingButton::State state;
};

class DeviceModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(Roles)
public:
    enum Roles { DeviceIdRole = Qt::UserRole + 1, DeviceStateRole };

    explicit DeviceModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    void addDevice(const QString &name);
    void updateDeviceState(const QString &deviceId, RotatingButton::State state);

    bool exists(const QString &deviceId) const;
    void removeDevice(const QString &deviceId);
    void clear();

private:
    QVector<DeviceInfo> m_devices;
    QHash<QString, int> m_idIndexMap;
};

#endif // DEVICEMODEL_H
