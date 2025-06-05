#ifndef GROUPCONTENTWIDGET_H
#define GROUPCONTENTWIDGET_H

#include <QWidget>
#include "GroupListWidget.h"

class GroupContentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupContentWidget(QWidget *parent = nullptr);

    void setGroupId(int groupId);

private:
    void refreshLists();

private slots:
    void onAddMembers();
    void onRemoveMembers();

signals:
    void addMembersToGroup(const QSet<int>& memberIds, int groupId);
    void removeMembersFromGroup(const QSet<int>& memberIds, int groupId);

private:
    int m_groupId = DataManager::UNGROUPED_ID;
    GroupListWidget* m_groupList;
    GroupListWidget* m_unGroupList;

};

#endif // GROUPCONTENTWIDGET_H
