#include "devicedelegate.h"
#include "devicemodel.h"
#include "deviceitem.h"

#include <QDebug>

DeviceDelegate::DeviceDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

QWidget *DeviceDelegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    QString deviceId = index.data(DeviceModel::DeviceIdRole).toString();
    DeviceItem *editor = new DeviceItem(deviceId, parent);
    editor->setFocusPolicy(Qt::NoFocus);
    connect(editor, &DeviceItem::connectRequested, this, &DeviceDelegate::onConnectRequest);
    connect(editor, &DeviceItem::disconnectRequested, this, &DeviceDelegate::onDisconnectRequest);
    return editor;
}

void DeviceDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (auto *item = qobject_cast<DeviceItem*>(editor)) {
        auto state = static_cast<RotatingButton::State>(index.data(DeviceModel::DeviceStateRole).toInt());
        item->updateState(state);
    }
}

void DeviceDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

QSize DeviceDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(), 40);
}

void DeviceDelegate::onConnectRequest(const QString &deviceId)
{
//    qDebug() << "[DeviceDelegate] onConnectRequest - deviceId:" << deviceId;
    emit connectClicked(deviceId);
}

void DeviceDelegate::onDisconnectRequest(const QString &deviceId)
{
//    qDebug() << "[DeviceDelegate] onDisconnectRequest - deviceId:" << deviceId;
    emit disconnectClicked(deviceId);
}
