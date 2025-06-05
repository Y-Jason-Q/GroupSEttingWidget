#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include <QSet>
#include <QReadWriteLock>

/*!
 * \brief 成员信息结构体
 */
struct MemberInfo {
    int id;
    QString name;
    int groupId;    // 当前成员所属分组id
};

/*!
 * \brief 分组信息结构体
 */
struct GroupInfo {
    int id;
    QString name;
};

/*!
 * \brief 数据管理器，线程安全，单例
 */
class DataManager : public QObject
{
    Q_OBJECT
public:
    static constexpr int UNGROUPED_ID = 0;
    static DataManager &instance();

    // 分组操作
    int addGroup(const QString &name);  // 添加分组
    void removeGroup(int groupId);      // 删除分组

    // 成员操作
    int addMember(const QString &name); // 添加成员
    void removeMember(int memberId);    // 删除成员
    void assignMemberToGroup(int memberId, int groupId);    // 成员分组变更

    // 查询
    QVector<GroupInfo> groups() const;  // 获取所有分组（含未分组）
    QVector<MemberInfo> groupMembers(int groupId) const;    // 指定分组成员
    QVector<MemberInfo> ungroupedMembers() const; // 未分组成员

    // 统计
    int groupCount() const;                         // 分组总数（含未分组）
    int memberCount() const;                        // 总成员数
    int groupMemberCount(int groupId) const;        // 某分组成员数
    int ungroupMemberCount() const;                 // 未分组成员数

signals:
    void dataChanged();

private:
    explicit DataManager(QObject* parent = nullptr);
    Q_DISABLE_COPY(DataManager)

    mutable QReadWriteLock m_lock;
    QVector<GroupInfo> m_groups;
    QVector<MemberInfo> m_members;
    QAtomicInt m_nextGroupId;
    QAtomicInt m_nextMemberId;
};

#endif // DATAMANAGER_H
