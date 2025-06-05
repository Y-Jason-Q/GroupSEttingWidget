#include "networkdisplaywidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>

NetworkDisplayWidget::NetworkDisplayWidget(QWidget *parent)
    : QWidget{parent}
{
    initUI();
}

void NetworkDisplayWidget::initUI()
{
    m_iconLabel = new QLabel(this);
    m_titleLabel = new QLabel(this);
    m_networkNameLabel = new QLabel(this);

    m_iconLabel->setObjectName("Network-IconLabel");
    m_titleLabel->setObjectName("Network-TitleLabel");
    m_networkNameLabel->setObjectName("Network-NameLabel");
    this->setObjectName("Network-MainWidget");
    this->setAttribute(Qt::WA_StyledBackground);

    m_iconLabel->setFixedSize(32, 32);
    m_titleLabel->setFixedHeight(32);

    int margin = 16;

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(margin);
    hLayout->addWidget(m_iconLabel);
    hLayout->addWidget(m_titleLabel);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(margin, margin, margin, margin);
    vLayout->setSpacing(margin);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(m_networkNameLabel);
    vLayout->addStretch();

    m_titleLabel->setText(QStringLiteral("网络连接"));
    m_networkNameLabel->setText(QStringLiteral("当前连接网络: %1"));

    QPixmap pixmap(":/images/network.png");
    m_iconLabel->setPixmap(pixmap);
    m_iconLabel->setScaledContents(true);

    m_iconLabel->setStyleSheet("background: transparent;");
    m_titleLabel->setStyleSheet("background: transparent; font-family: \"Microsoft YaHei-Bold\"; font-weight: bold; font-size: 16px; color: #BBBBBB;");
    m_networkNameLabel->setStyleSheet("background: transparent; font-family: \"Microsoft YaHei-Regular\"; font-size: 12px; color: #BBBBBB;");
    this->setStyleSheet(""
                        "#Network-MainWidget {"
                        "   background-color: #282828;"
                        "   border-radius: 16px;"
                        "}");
}

void NetworkDisplayWidget::setNetworkName(const QString &name)
{
    QString labelName = QString(QStringLiteral("当前连接网络: \n")) + name;
    m_networkNameLabel->setText(labelName);
}

void NetworkDisplayWidget::onNetworkStatus(const QString &name, bool isConnected)
{
    if (isConnected) {
        setNetworkName(name);
    }
    else {
        m_networkNameLabel->setText(QStringLiteral("未找到可连接的网络"));
    }
}
