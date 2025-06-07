#include "groupcontentwidget.h"
//#include "DataManager.h"
#include "../../DataManager/datamanager.h"
#include <QVBoxLayout>

GroupContentWidget::GroupContentWidget(QWidget *parent)
    : QWidget{parent}
{
    m_groupList = new GroupListWidget(this);
    m_groupList->setLabel(QStringLiteral("分组列表"));
    m_groupList->setButtonText(QStringLiteral("移除分组"));

    m_unGroupList = new GroupListWidget(this);
    m_unGroupList->setLabel(QStringLiteral("未分组列表"));
    m_unGroupList->setButtonText(QStringLiteral("添加至分组"));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(70, 0, 70, 0);
    layout->setSpacing(20);
    layout->addWidget(m_groupList);
    layout->addWidget(m_unGroupList);
    setLayout(layout);

    connect(m_groupList, &GroupListWidget::buttonClicked, this, &GroupContentWidget::onRemoveMembers, Qt::UniqueConnection);
    connect(m_unGroupList, &GroupListWidget::buttonClicked, this, &GroupContentWidget::onAddMembers, Qt::UniqueConnection);
    connect(&DataManager::instance(), &DataManager::dataChanged, this, &GroupContentWidget::refreshLists, Qt::UniqueConnection);

    this->setStyleSheet("background-color: #282828;");
}

void GroupContentWidget::setGroupId(int groupId)
{
    m_groupId = groupId;
    refreshLists();
}

void GroupContentWidget::refreshLists()
{
    auto& mgr = DataManager::instance();
    QVector<MemberInfo> groupList = mgr.groupMembers(m_groupId);
    QVector<MemberInfo> ungroupList = mgr.ungroupedMembers();

    m_groupList->setMembers(groupList);
    m_unGroupList->setMembers(ungroupList);

    m_groupList->setVisible(m_groupId != DataManager::UNGROUPED_ID && !groupList.isEmpty());
    m_unGroupList->setVisible(!ungroupList.isEmpty());

    if (m_groupId == DataManager::UNGROUPED_ID) {
        m_unGroupList->setButtonVisible(false);
    } else {
        m_unGroupList->setButtonVisible(true);
    }
}

void GroupContentWidget::onAddMembers()
{
    QSet<int> ids = m_unGroupList->checkedIds();
    if (!ids.isEmpty())
        emit addMembersToGroup(ids, m_groupId);
}

void GroupContentWidget::onRemoveMembers()
{
    QSet<int> ids = m_groupList->checkedIds();
    if (!ids.isEmpty())
        emit removeMembersFromGroup(ids, m_groupId);
}
