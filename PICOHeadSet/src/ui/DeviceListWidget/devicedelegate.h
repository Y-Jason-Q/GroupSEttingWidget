#ifndef DEVICEDELEGATE_H
#define DEVICEDELEGATE_H

#include <QStyledItemDelegate>

class DeviceDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DeviceDelegate(QObject *parent = nullptr);

    // 重写以下标准方法
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:
    void connectClicked(const QString &deviceId);
    void disconnectClicked(const QString &deviceId);

public slots:
    void onConnectRequest(const QString &deviceId);
    void onDisconnectRequest(const QString &deviceId);
};

#endif // DEVICEDELEGATE_H
