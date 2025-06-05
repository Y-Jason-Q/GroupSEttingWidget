#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include <QSet>
#include <QReadWriteLock>

struct MemberInfo {
    int id;
    QString name;
    int groupId;    // 当前成员所属分组id
};

struct GroupInfo {
    int id;
    QString name;
};

class DataManager : public QObject
{
    Q_OBJECT
public:
    static constexpr int UNGROUPED_ID = 0;

    static DataManager &instance();

    // 分组管理
    QVector<GroupInfo> groups() const;
    int addGroup(const QString &name); // 返回新分组ID
    void removeGroup(int groupId);
    QString nextAutoGroupName() const; // 自动生成唯一分组名（分组一，分组二...）

    // 成员管理
    QVector<MemberInfo> members() const;
    int addMember(const QString &name);
    void removeMember(int memberId);

    // 分配成员到分组
    QSet<int> groupMemberIds(int groupId) const;            // 某分组下成员ID集合
    void assignMemberToGroup(int memberId, int groupId);    // 把成员分到分组（自动移出其它分组）
    void removeMemberFromGroup(int member, int groupId);    // 从分组移除成员

    // 查询未分组成员
    QSet<int> ungroupedMemberIds() const;

    // 查询id查找
    MemberInfo memberInfo(int memberId) const;
    GroupInfo groupInfo(int groupId) const;

    // 分组/未分组成员数量统计
    int groupMemberCount(int groupId) const;
    int ungroupMemberCount() const;

signals:
    void dataChanged();

private:
    explicit DataManager(QObject* parent = nullptr);
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

    mutable QReadWriteLock m_lock;
    QVector<GroupInfo> m_groups;
    QVector<MemberInfo> m_members;
    QMap<int, QSet<int>> m_groupMembers; // groupId -> memberIds
    int m_nextGroupId;
    int m_nextMemberId;
};

#endif // DATAMANAGER_H
