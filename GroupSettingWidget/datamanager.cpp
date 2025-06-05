#include "datamanager.h"

DataManager &DataManager::instance()
{
    static DataManager mgr;
    return mgr;
}

DataManager::DataManager(QObject *parent)
    : QObject{parent}
    , m_nextGroupId(1)
    , m_nextMemberId(1)
{
    // 添加未分组，id 固定为 UNGROUPED_ID
    GroupInfo ungrouped;
    ungrouped.id = UNGROUPED_ID;
    ungrouped.name = QStringLiteral("未分组");
    m_groups << ungrouped;
}

QVector<GroupInfo> DataManager::groups() const
{
    QReadLocker locker(&m_lock);
    return m_groups;
}

int DataManager::addGroup(const QString &name)
{
    if (name.trimmed().isEmpty()) return -1;
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
    {
        QWriteLocker locker(&m_lock);
        if (groupId == UNGROUPED_ID) return;
        auto it = std::remove_if(m_groups.begin(), m_groups.end(),
                                 [groupId](const GroupInfo& g) { return g.id == groupId; });
        m_groups.erase(it, m_groups.end());
        for (auto& m : m_members) {
            if (m.groupId == groupId)
                m.groupId = UNGROUPED_ID;
        }
    } // 锁作用域结束
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
    MemberInfo m{m_nextMemberId++, name, UNGROUPED_ID};
    m_members.append(m);
    emit dataChanged();
    return m.id;
}

void DataManager::removeMember(int memberId) {
    QWriteLocker locker(&m_lock);
    auto it = std::remove_if(m_members.begin(), m_members.end(),
                             [memberId](const MemberInfo& m) { return m.id == memberId; });
    m_members.erase(it, m_members.end());
    emit dataChanged();
}

QVector<MemberInfo> DataManager::groupMembers(int groupId) const
{
    QReadLocker locker(&m_lock);
    QVector<MemberInfo> result;
    for (const auto& m : m_members) {
        if (m.groupId == groupId)
            result.append(m);
    }
    return result;
}

QVector<MemberInfo> DataManager::ungroupedMembers() const
{
    return groupMembers(UNGROUPED_ID);
}

void DataManager::assignMemberToGroup(int memberId, int groupId)
{
    if (groupId == UNGROUPED_ID) return;
    QWriteLocker locker(&m_lock);
    for (auto& m : m_members) {
        if (m.id == memberId) {
            m.groupId = groupId;
            break;
        }
    }
    emit dataChanged();
}

void DataManager::removeMemberFromGroup(int memberId)
{
    QWriteLocker locker(&m_lock);
    for (auto& m : m_members) {
        if (m.id == memberId) {
            m.groupId = UNGROUPED_ID;
            break;
        }
    }
    emit dataChanged();
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
    return groupMembers(groupId).size();
}

int DataManager::ungroupMemberCount() const
{
    return ungroupedMembers().size();
}

