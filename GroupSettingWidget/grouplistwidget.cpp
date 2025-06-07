#include "grouplistwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

GroupListWidget::GroupListWidget(QWidget *parent)
    : QWidget{parent}
{
    m_model = new GroupListModel(this);
    m_delegate = new GroupItemDelegate(this);
    m_listView = new QListView(this);
    m_listView->setModel(m_model);
    m_listView->setItemDelegate(m_delegate);
    m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_listView->setSelectionMode(QAbstractItemView::NoSelection);
    // 样式设置
    m_listView->setFrameShape(QFrame::NoFrame);
    m_listView->setSpacing(4);
    m_listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_label = new QLabel(this);
    m_btn = new QPushButton(this);
    m_btn->setFixedSize(QSize(120, 50));

    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->setContentsMargins(10, 0, 10, 0);
    topLayout->addWidget(m_label);
    topLayout->addStretch();
    topLayout->addWidget(m_btn);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);
    layout->addLayout(topLayout);
    layout->addWidget(m_listView);
    setLayout(layout);

    m_label->setStyleSheet("background-color: transparent; color: #FFFFFF; font-size: 18px;");
    m_btn->setStyleSheet("background-color: #3D8BFF; color: #FFFFFF; font-size: 18px; border-radius: 16px;");
    this->setStyleSheet("background-color: transparent;");

    connect(m_btn, &QPushButton::clicked, this, &GroupListWidget::buttonClicked);
    connect(m_model, &GroupListModel::checkedItemsChanged, this, &GroupListWidget::checkedItemsChanged);
}

void GroupListWidget::setLabel(const QString& label)
{
    m_label->setText(label);
}
void GroupListWidget::setButtonText(const QString& text)
{
    m_btn->setText(text);
}
void GroupListWidget::setMembers(const QVector<MemberInfo>& members)
{
    m_model->setMembers(members);
}
void GroupListWidget::setCheckedIds(const QSet<int>& checkedIds)
{
    m_model->setCheckedIds(checkedIds);
}
QSet<int> GroupListWidget::checkedIds() const
{
    return m_model->checkedIds();
}
void GroupListWidget::hideIfEmpty(bool hide)
{
    setVisible(!hide || m_model->rowCount() > 0);
}

void GroupListWidget::setButtonVisible(bool visible)
{
    m_btn->setVisible(visible);
}
