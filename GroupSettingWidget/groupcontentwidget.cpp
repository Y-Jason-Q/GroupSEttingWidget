#include "groupcontentwidget.h"
#include "DataManager.h"
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

    connect(m_groupList, &GroupListWidget::buttonClicked, [this](){
        emit removeMembersFromGroup(m_groupList->checkedIds());
    });
    connect(m_unGroupList, &GroupListWidget::buttonClicked, [this](){
        emit addMembersToGroup(m_unGroupList->checkedIds());
    });

    connect(&DataManager::instance(), &DataManager::dataChanged, [this]{
        setGroupId(m_groupId);
    });
}

void GroupContentWidget::setGroupId(int groupId)
{
    m_groupId = groupId;
    auto& mgr = DataManager::instance();

    if (groupId == DataManager::UNGROUPED_ID) {
        // 只显示未分组成员列表
        QVector<MemberInfo> ungroupList = mgr.ungroupedMembers();
        m_unGroupList->setMembers(ungroupList);
        // 判断是否显示
        m_unGroupList->setVisible(!ungroupList.isEmpty());
        m_groupList->setVisible(false);
    } else {
        QVector<MemberInfo> groupList = mgr.groupMembers(groupId);
        QVector<MemberInfo> ungroupList = mgr.ungroupedMembers();
        m_groupList->setMembers(groupList);
        m_unGroupList->setMembers(ungroupList);
        // 判断是否显示
        m_groupList->setVisible(!groupList.isEmpty());
        m_unGroupList->setVisible(!ungroupList.isEmpty());
    }
}
