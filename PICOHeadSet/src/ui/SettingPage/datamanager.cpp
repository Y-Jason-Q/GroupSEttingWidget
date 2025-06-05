#include "datamanager.h"
#include <algorithm>

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
    m_groups.append(ungrouped);
}



int DataManager::addGroup(const QString &name)
{
    if (name.trimmed().isEmpty()) return -1;
    int id = -1;
    {
        QWriteLocker locker(&m_lock);
        for (const auto &g : m_groups)
            if (g.name == name) return -1;
        GroupInfo g{m_nextGroupId.fetchAndAddRelaxed(1), name};
        m_groups.append(g);
        id = g.id;
    }
    emit dataChanged();
    return id;
}

void DataManager::removeGroup(int groupId)
{
    if (groupId == UNGROUPED_ID) return;
    bool changed = false;
    {
        QWriteLocker locker(&m_lock);
        auto it = std::remove_if(m_groups.begin(), m_groups.end(),
                                 [groupId](const GroupInfo& g) { return g.id == groupId; });
        if (it != m_groups.end()) {
            m_groups.erase(it, m_groups.end());
            for (auto &m : m_members) {
                if (m.groupId == groupId)
                    m.groupId = UNGROUPED_ID;
            }
            changed = true;
        }
    } // 锁作用域结束
    if (changed) emit dataChanged();
}


int DataManager::addMember(const QString& name)
{
    if (name.trimmed().isEmpty()) return -1;
    int id = -1;
    {
        QWriteLocker locker(&m_lock);
        for (const auto &m : m_members)
            if (m.name == name) return -1;
        MemberInfo mi{m_nextMemberId++, name, UNGROUPED_ID};
        m_members.append(mi);
        id = mi.id;
    }
    emit dataChanged();
    return id;
}

void DataManager::removeMember(int memberId)
{
    bool changed = false;
    {
        QWriteLocker locker(&m_lock);
        auto it = std::remove_if(m_members.begin(), m_members.end(),
                                 [memberId](const MemberInfo& m) { return m.id == memberId; });
        if (it != m_members.end()) {
            m_members.erase(it, m_members.end());
            changed = true;
        }
    }
    if (changed) emit dataChanged();
}

void DataManager::assignMemberToGroup(int memberId, int groupId)
{
    if (groupId != UNGROUPED_ID)  {
        QReadLocker gLocker(&m_lock);
        auto grpIt = std::find_if(m_groups.begin(), m_groups.end(),
                                  [groupId](const GroupInfo &g) { return g.id == groupId; });
        if (grpIt == m_groups.end()) return;
    }
    bool changed = false;
    {
        QWriteLocker locker(&m_lock);
        for (auto& m : m_members) {
            if (m.id == memberId) {
                m.groupId = groupId;
                changed = true;
                break;
            }
        }
    }
    if (changed) emit dataChanged();
}

QVector<GroupInfo> DataManager::groups() const
{
    QReadLocker locker(&m_lock);
    return m_groups;
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

int DataManager::groupCount() const
{
    QReadLocker locker(&m_lock);
    return m_groups.size();
}

int DataManager::memberCount() const
{
    QReadLocker locker(&m_lock);
    return m_members.size();
}

int DataManager::groupMemberCount(int groupId) const
{
    QReadLocker locker(&m_lock);
    int count = 0;
    for (const auto& m : m_members)
        if (m.groupId == groupId)
            ++count;
    return count;
}

int DataManager::ungroupMemberCount() const
{
    return groupMemberCount(UNGROUPED_ID);
}
