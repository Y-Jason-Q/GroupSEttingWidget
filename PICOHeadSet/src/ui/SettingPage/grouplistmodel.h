#ifndef GROUPLISTMODEL_H
#define GROUPLISTMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include "datamanager.h"

struct GroupItemData {
    QString name;
    bool checked;
};

class GroupListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit GroupListModel(QObject *parent = nullptr);

    void setMembers(const QVector<MemberInfo> &members);
    void setCheckedIds(const QSet<int> &checkedIds);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    int memberIdAt(int row) const;
    QSet<int> checkedIds() const;

signals:
    void checkedItemsChanged();

private:
    QVector<MemberInfo> m_members;
    QSet<int> m_checkedIds;
};

#endif // GROUPLISTMODEL_H
