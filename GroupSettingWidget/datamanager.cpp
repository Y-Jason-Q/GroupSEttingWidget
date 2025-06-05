#include "datamanager.h"

DataManager &DataManager::instance()
{
    static DataManager mgr;
    return mgr;
}

DataManager::DataManager(QObject *parent)
    : QObject{parent}
{
    // 添加未分组，id 固定为 UNGROUPED_ID
    GroupInfo ungrouped;
    ungrouped.id = UNGROUPED_ID;
    ungrouped.name = QStringLiteral("未分组");
    m_groups << ungrouped;

    // 示例数据：可移除
    int mid1 = addMember(QStringLiteral("用户A"));
    int mid2 = addMember(QStringLiteral("用户B"));
    int mid3 = addMember(QStringLiteral("用户C"));
    int gid1 = addGroup(QStringLiteral("分组一"));
    int gid2 = addGroup(QStringLiteral("分组二"));
    assignMemberToGroup(mid1, gid1);
    assignMemberToGroup(mid2, gid2);
}

QVector<GroupInfo> DataManager::groups() const
{
    QReadLocker locker(&m_lock);
    return m_groups;
}

int DataManager::addGroup(const QString &name)
{
    int id;
    {
        QWriteLocker locker(&m_lock);
        GroupInfo g{m_nextGroupId++, name};
        m_groups.append(g);
        id = g.id;
    }
    emit dataChanged();
    return id;
}

void DataManager::removeGroup(int groupId)
{
    QWriteLocker locker(&m_lock);
    if (groupId == UNGROUPED_ID) return; // “未分组”不允许删除
    auto it = std::remove_if(m_groups.begin(), m_groups.end(),
                             [groupId](const GroupInfo& g) { return g.id == groupId; });
    m_groups.erase(it, m_groups.end());
    // 分组成员挪回未分组
    if (m_groupMembers.contains(groupId)) {
        QSet<int> ids = m_groupMembers[groupId];
        for (int mid : ids) {
            m_groupMembers[UNGROUPED_ID].insert(mid);
        }
        m_groupMembers.remove(groupId);
    }
    emit dataChanged();
}

QString DataManager::nextAutoGroupName() const
{
    QReadLocker locker(&m_lock);
    QStringList exist;
    for (const auto& g : m_groups)
        exist << g.name;

    int idx = 1;
    while (true) {
        QString base = QStringLiteral("分组%1").arg(idx);
        if (!exist.contains(base)) return base;
        ++idx;
    }
}

QVector<MemberInfo> DataManager::members() const
{
    QReadLocker locker(&m_lock);
    return m_members;
}

int DataManager::addMember(const QString& name)
{
    QWriteLocker locker(&m_lock);
    MemberInfo m{m_nextMemberId++, name};
    m_members.append(m);
    m_groupMembers[UNGROUPED_ID].insert(m.id);
    emit dataChanged();
    return m.id;
}

void DataManager::removeMember(int memberId) {
    QWriteLocker locker(&m_lock);
    auto it = std::remove_if(m_members.begin(), m_members.end(),
                             [memberId](const MemberInfo& m) { return m.id == memberId; });
    m_members.erase(it, m_members.end());

    for (auto& ids : m_groupMembers) {
        ids.remove(memberId);
    }
    emit dataChanged();
}

QSet<int> DataManager::groupMemberIds(int groupId) const
{
    QReadLocker locker(&m_lock);
    return m_groupMembers.value(groupId, {});
}

void DataManager::assignMemberToGroup(int memberId, int groupId)
{
    if (groupId == UNGROUPED_ID) return; // 不能手动分配回未分组
    QWriteLocker locker(&m_lock);
    // 先从所有分组移除（保证唯一归属）
    for (auto& ids : m_groupMembers) {
        ids.remove(memberId);
    }
    // 添加到指定分组
    m_groupMembers[groupId].insert(memberId);
    emit dataChanged();
}

void DataManager::removeMemberFromGroup(int memberId, int groupId)
{
    if (groupId == UNGROUPED_ID) return; // 未分组不能移除成员
    QWriteLocker locker(&m_lock);
    m_groupMembers[groupId].remove(memberId);
    m_groupMembers[UNGROUPED_ID].insert(memberId);
    emit dataChanged();
}

QSet<int> DataManager::ungroupedMemberIds() const
{
    QReadLocker locker(&m_lock);
    return m_groupMembers.value(UNGROUPED_ID, {});
}

MemberInfo DataManager::memberInfo(int memberId) const
{
    QReadLocker locker(&m_lock);
    for (const auto& m : m_members) {
        if (m.id == memberId)
            return m;
    }
    return MemberInfo{};
}

GroupInfo DataManager::groupInfo(int groupId) const
{
    QReadLocker locker(&m_lock);
    for (const auto& g : m_groups) {
        if (g.id == groupId)
            return g;
    }
    return GroupInfo{};
}

// 分组/未分组成员数量统计
int DataManager::groupMemberCount(int groupId) const
{
    return groupMemberIds(groupId).size();
}
int DataManager::ungroupMemberCount() const
{
    return ungroupedMemberIds().size();
}

