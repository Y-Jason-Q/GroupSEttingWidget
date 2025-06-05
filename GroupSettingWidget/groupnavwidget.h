#ifndef GROUPNAVWIDGET_H
#define GROUPNAVWIDGET_H

#include <QWidget>
#include <QListView>
#include <QPushButton>
#include <QLabel>
#include "GroupNavModel.h"

class GroupNavWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupNavWidget(QWidget* parent = nullptr);

signals:
    void groupSelected(int groupId);
    void addGroupRequested();
    void deleteGroupRequested(int groupId);

private:
    QListView* m_listView;
    QPushButton* m_addBtn;
    QWidget *m_btnArea;
    QLabel *m_label;
    GroupNavModel* m_model;
};

#endif // GROUPNAVWIDGET_H
