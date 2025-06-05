#ifndef GROUPLISTWIDGET_H
#define GROUPLISTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListView>
#include "grouplistmodel.h"
#include "groupitemdelegate.h"

class GroupListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupListWidget(QWidget *parent = nullptr);

    void setLabel(const QString& label);
    void setButtonText(const QString& text);
    void setMembers(const QVector<MemberInfo>& members);
    void setCheckedIds(const QSet<int>& checkedIds);
    QSet<int> checkedIds() const;
    void hideIfEmpty(bool hide = true);

    void setButtonVisible(bool visible);

signals:
    void buttonClicked();
    void checkedItemsChanged();

private:
    QLabel* m_label;
    QPushButton* m_btn;
    QListView* m_listView;
    GroupListModel* m_model;
    GroupItemDelegate* m_delegate;
};

#endif // GROUPLISTWIDGET_H
