#include "groupsettingwidget.h"
#include "../../DataManager/datamanager.h"
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>

GroupSettingWidget::GroupSettingWidget(QWidget *parent)
    : QWidget{parent}
{
    m_nav = new GroupNavWidget(this);
    m_content = new GroupContentWidget(this);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_nav, 1);
    layout->addWidget(m_content, 4);
    setLayout(layout);

    connect(m_nav, &GroupNavWidget::groupSelected, this, [this](int groupId){
        m_currentGroupId = groupId;
        m_content->setGroupId(groupId);
    }, Qt::UniqueConnection);

    connect(m_nav, &GroupNavWidget::addGroupRequested, this, [this]{
        // 获取所有分组名（排除“未分组”）
        QSet<int> usedNums;
        const QString prefix = QStringLiteral("分组");
        const auto groups = DataManager::instance().groups();
        for (const auto& g : groups) {
            if (g.name.startsWith(prefix)) {
                bool ok = false;
                int n = g.name.mid(prefix.size()).toInt(&ok);
                if (ok && n > 0) usedNums.insert(n);
            }
        }

        // 找最小未用编号
        int idx = 1;
        while (usedNums.contains(idx)) ++idx;
        QString name = prefix + QString::number(idx);
        DataManager::instance().addGroup(name);
    }, Qt::UniqueConnection);

    connect(m_nav, &GroupNavWidget::deleteGroupRequested, this, [this](int groupId){
        if (groupId == DataManager::UNGROUPED_ID) return;
        {
            // 判断当前选中项是否为要删除的分组
            bool isDeletingCurrent = (m_currentGroupId == groupId);
            DataManager::instance().removeGroup(groupId);

            if (isDeletingCurrent) {
                // 删除的是当前选中分组：选中"未分组"，刷新内容
                m_currentGroupId = DataManager::UNGROUPED_ID;
                m_content->setGroupId(m_currentGroupId);
            }
        }
    }, Qt::UniqueConnection);

    connect(m_content, &GroupContentWidget::addMembersToGroup, this, [this](const QSet<int>& ids, int groupId){
            for (int id : ids)
                DataManager::instance().assignMemberToGroup(id, groupId);
    }, Qt::UniqueConnection);

    connect(m_content, &GroupContentWidget::removeMembersFromGroup, this, [this](const QSet<int>& ids, int){
        for (int id : ids)
            DataManager::instance().assignMemberToGroup(id, DataManager::UNGROUPED_ID); // 1为未分组
    }, Qt::UniqueConnection);

    m_content->setGroupId(m_currentGroupId);
}
