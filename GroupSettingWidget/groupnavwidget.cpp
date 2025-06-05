#include "groupnavwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>

#include "GroupNavDelegate.h"

GroupNavWidget::GroupNavWidget(QWidget *parent)
    : QWidget(parent)
{
    m_model = new GroupNavModel(this);
    m_listView = new QListView(this);
    m_listView->setModel(m_model);
    auto* delegate = new GroupNavDelegate(this);
    m_listView->setItemDelegate(delegate);
    m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_listView->setSelectionMode(QAbstractItemView::SingleSelection);

    // 样式设置
    m_listView->setFrameShape(QFrame::NoFrame);
    m_listView->setSpacing(4);
    m_listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 按钮区域
    m_btnArea = new QWidget(this);
    m_addBtn = new QPushButton(QStringLiteral("添加分组"), this);
    m_addBtn->setFixedSize(120, 60);

    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignLeft);
    m_label->setFixedHeight(40);
//    m_label->hide();

    QHBoxLayout *btnLayout = new QHBoxLayout(m_btnArea);
    btnLayout->setContentsMargins(0, 0, 24, 8);
    btnLayout->setSpacing(0);
    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(m_addBtn);
    m_btnArea->setLayout(btnLayout);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_label);
    layout->addWidget(m_listView);
    layout->addWidget(m_btnArea);
    this->setLayout(layout);

    this->setStyleSheet("background-color: #141414;");
    m_btnArea->setStyleSheet("background-color: #141414;");
    m_addBtn->setStyleSheet("background-color: #3D8BFF; color: #FFFFFF; font-size: 18px; border-radius: 16px;");
    m_label->setStyleSheet("background-color: #141414; color: #FFFFFF; font-size: 18px;");

    connect(m_listView->selectionModel(), &QItemSelectionModel::currentChanged, [this](const QModelIndex& idx){
        if (idx.isValid()) emit groupSelected(m_model->groupIdAt(idx.row()));
    });
    connect(m_addBtn, &QPushButton::clicked, [this]{
        emit addGroupRequested();
    });
    connect(delegate, &GroupNavDelegate::deleteClicked, [this](int row){
        emit deleteGroupRequested(m_model->groupIdAt(row));
    });

    m_listView->setCurrentIndex(m_model->index(0, 0));
}
