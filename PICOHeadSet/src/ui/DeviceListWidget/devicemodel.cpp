#include "devicemodel.h"
#include <QModelIndex>
#include <QDebug>
#include <algorithm>

DeviceModel::DeviceModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

int DeviceModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_devices.size();
}

QVariant DeviceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const DeviceInfo &device = m_devices.at(index.row());
    switch (role) {
    case DeviceIdRole: return device.id;
    case DeviceStateRole:  return device.state;
    default: return QVariant();
    }
    return QVariant();
}

bool DeviceModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_devices.size())
        return false;

    DeviceInfo &device = m_devices[index.row()];
    switch (role) {
    case DeviceStateRole:
        device.state = static_cast<RotatingButton::State>(value.toInt());
        emit dataChanged(index, index, {DeviceStateRole});
        return true;
    default:
        break;
    }
    return false;
}

QHash<int, QByteArray> DeviceModel::roleNames() const
{
    return {
        {DeviceIdRole, "deviceId"},
        {DeviceStateRole, "deviceState"}
    };
}

void DeviceModel::addDevice(const QString &name)
{
    if (m_idIndexMap.contains(name)) return;
    beginInsertRows(QModelIndex(), m_devices.size(), m_devices.size());
    m_devices.append({name, RotatingButton::Disconnected});
    m_idIndexMap[name] = m_devices.size() - 1;
    endInsertRows();
}

void DeviceModel::updateDeviceState(const QString &deviceId, RotatingButton::State state)
{
    if (m_idIndexMap.contains(deviceId)) {
        int i = m_idIndexMap[deviceId];
        QModelIndex idx = createIndex(i, 0);
        setData(idx, static_cast<int>(state), DeviceStateRole);
    }
}

bool DeviceModel::exists(const QString &deviceId) const
{
//    return std::any_of(m_devices.begin(), m_devices.end(),
//                       [&](const DeviceInfo &d) {return d.id == deviceId;});
    return m_idIndexMap.contains(deviceId);
}

void DeviceModel::removeDevice(const QString &deviceId)
{
    if (m_idIndexMap.contains(deviceId)) {
        int row = m_idIndexMap[deviceId];
        beginRemoveRows(QModelIndex(), row, row);
        m_devices.removeAt(row);
        m_idIndexMap.remove(deviceId);
        for (int i = row; i < m_devices.size(); ++i) {
            m_idIndexMap[m_devices[i].id] = i;
        }
        endRemoveRows();
    }
}

void DeviceModel::clear()
{
    beginResetModel();
    m_devices.clear();
    m_idIndexMap.clear();
    endResetModel();
}
