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

signals:
    void removeMembersFromGroup(const QSet<int>& memberIds);
    void addMembersToGroup(const QSet<int>& memberIds);

private:
    int m_groupId = DataManager::UNGROUPED_ID;
    GroupListWidget* m_groupList;
    GroupListWidget* m_unGroupList;

};

#endif // GROUPCONTENTWIDGET_H
