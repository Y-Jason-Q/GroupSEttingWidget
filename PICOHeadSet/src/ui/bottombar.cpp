#include "bottombar.h"
#include <QHBoxLayout>

BottomBar::BottomBar(QWidget *parent)
    : QWidget{parent}
{
    initUI();
    initConnection();
}

void BottomBar::initUI()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(32);

    m_layoutButton = new QPushButton();
    m_devlistButton = new QPushButton();
    m_layoutButton->setFixedSize(100, 50);
    m_devlistButton->setFixedSize(100, 50);

    layout->addStretch();
    layout->addWidget(m_layoutButton);
    layout->addWidget(m_devlistButton);
    layout->addStretch();

    this->setLayout(layout);

    this->setStyleSheet("background-color: #1E1E1E;");

    m_layoutButton->setStyleSheet("QPushButton{ background-color: #282828; qproperty-icon: url(:/images/layout_btn.png); qproperty-iconSize: 24px; border-radius: 25px;}"
                                  "QPushButton:hover{ background-color: #393939;}"
                                  "QPushButton:pressed{ background-color: #1E1E1E;}");
    m_devlistButton->setStyleSheet("QPushButton{ background-color: #282828; qproperty-icon: url(:/images/devlist_btn.png); qproperty-iconSize: 32px; border-radius: 25px; }"
                                   "QPushButton:hover{ background-color: #393939;}"
                                   "QPushButton:pressed { background-color: #1E1E1E;}");

    m_layoutType = 1;
}

void BottomBar::initConnection()
{
    connect(m_layoutButton, &QPushButton::clicked, this, &BottomBar::sigClickedLayoutBtn);
    connect(m_devlistButton, &QPushButton::clicked, this, &BottomBar::sigClickedDevListBtn);
}
