#include "grouplistmodel.h"

GroupListModel::GroupListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void GroupListModel::setMembers(const QVector<MemberInfo> &members)
{
    beginResetModel();
    m_members = members;
    m_checkedIds.clear();
    endResetModel();
    emit checkedItemsChanged();
}

void GroupListModel::setCheckedIds(const QSet<int>& checkedIds)
{
    m_checkedIds = checkedIds;
    emit checkedItemsChanged();
    emit dataChanged(index(0), index(rowCount()-1), {Qt::CheckStateRole});
}

int GroupListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_members.size();
}

QVariant GroupListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_members.size())
        return QVariant();

    const auto& m = m_members[index.row()];
    if (role == Qt::DisplayRole)
        return m.name;
    if (role == Qt::CheckStateRole)
        return m_checkedIds.contains(m.id) ? Qt::Checked : Qt::Unchecked;
    return QVariant();
}

bool GroupListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_members.size())
        return false;

    if (role == Qt::CheckStateRole) {
        int id = m_members[index.row()].id;
        if (value.toInt() == Qt::Checked)
            m_checkedIds.insert(id);
        else
            m_checkedIds.remove(id);

        emit checkedItemsChanged();
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags GroupListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
}

int GroupListModel::memberIdAt(int row) const
{
    return m_members[row].id;
}

QSet<int> GroupListModel::checkedIds() const
{
    return m_checkedIds;
}
