#include "groupnavmodel.h"

GroupNavModel::GroupNavModel(QObject *parent)
    : QAbstractListModel{parent}
{
//    m_groups = DataManager::instance().groups();
    connect(&DataManager::instance(), &DataManager::dataChanged, this, &GroupNavModel::onDataChanged);
    onDataChanged();
}

int GroupNavModel::rowCount(const QModelIndex&) const
{
    return m_groups.size() + 1;
}

QVariant GroupNavModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_groups.size())
        return QVariant();
    const GroupInfo& g = m_groups[index.row()];
    if (role == Qt::DisplayRole) {
        int count = 0;
        if (g.id == DataManager::UNGROUPED_ID) {
            count = DataManager::instance().ungroupMemberCount();
        } else {
            count = DataManager::instance().groupMemberCount(g.id);
        }
        return QString("%1 (%2)").arg(g.name).arg(count);
    }
    if (role == Qt::UserRole) {
        return g.id;
    }
    return QVariant();
}

int GroupNavModel::groupIdAt(int row) const
{
    if (row < 0 || row >= m_groups.size())
        return DataManager::UNGROUPED_ID;
    return m_groups[row].id;
}

void GroupNavModel::onDataChanged()
{
    beginResetModel();
    m_groups = DataManager::instance().groups();
    endResetModel();
}
