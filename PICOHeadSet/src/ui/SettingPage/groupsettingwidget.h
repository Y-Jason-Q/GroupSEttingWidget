#ifndef GROUPSETTINGWIDGET_H
#define GROUPSETTINGWIDGET_H

#include <QWidget>
#include "GroupNavWidget.h"
#include "GroupContentWidget.h"

class GroupSettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupSettingWidget(QWidget *parent = nullptr);

private:
    GroupNavWidget* m_nav;
    GroupContentWidget* m_content;
    int m_currentGroupId = DataManager::UNGROUPED_ID;
};

#endif // GROUPSETTINGWIDGET_H
