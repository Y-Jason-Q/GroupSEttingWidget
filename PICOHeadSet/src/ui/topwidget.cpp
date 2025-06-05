#include "topwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include "dotwidget.h"

TopWidget::TopWidget(QWidget *parent)
    : QWidget{parent}
{
    initUI();
}

void TopWidget::initUI()
{
    m_leftLabel = new QLabel(this);
    m_midWidget = new DotWidget(this);
    m_rightLabel = new QLabel(this);
    m_leftLabel->setMinimumSize(64, 64);
    m_midWidget->setMinimumSize(100, 64);
    m_rightLabel->setMinimumSize(64, 64);

    m_statusLabel = new QLabel(this);
    m_infoLabel = new QLabel(this);

    QHBoxLayout *hlayout = new QHBoxLayout;
    QVBoxLayout *vlayout = new QVBoxLayout;

    hlayout->setSpacing(20);
    hlayout->addStretch();
    hlayout->addWidget(m_leftLabel);
    hlayout->addWidget(m_midWidget);
    hlayout->addWidget(m_rightLabel);
    hlayout->addStretch();

    vlayout->setSpacing(20);
    vlayout->addStretch();
    vlayout->addLayout(hlayout);
    vlayout->addWidget(m_statusLabel);
    vlayout->addWidget(m_infoLabel);
    vlayout->addStretch();
    this->setLayout(vlayout);

    m_statusLabel->setText(QStringLiteral("连接中"));
    m_infoLabel->setText(QStringLiteral("请确认与虚仿一体机连接在同一网络下\n在\"可连接列表\"中,找到设备名称,点击\"连接\"即可"));

    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_infoLabel->setAlignment(Qt::AlignCenter);

    QPixmap pixmap(":/images/localhost.png");
    m_leftLabel->setPixmap(pixmap.scaled(80, 80, Qt::KeepAspectRatio)); // 保持比例
    m_leftLabel->setAlignment(Qt::AlignCenter); // 居中

    QPixmap pixmap2(":/images/device_connecting.png");
    m_rightLabel->setPixmap(pixmap2.scaled(80, 80, Qt::KeepAspectRatio)); // 保持比例
    m_rightLabel->setAlignment(Qt::AlignCenter); // 居中

    m_leftLabel->setStyleSheet("background-color: transparent;");
    m_rightLabel->setStyleSheet("background-color: transparent;");

    m_statusLabel->setStyleSheet("background-color: transparent; font-family: Microsoft YaHei; font-size: 18px; font-weight: bold; color: #BBBBBB;");
    m_infoLabel->setStyleSheet("background-color: transparent; font-family: Microsoft YaHei; font-size: 12px; color: #BBBBBB;");

//    m_midWidget->start(200);

    updateStatus(DISCONNECT);
}

void TopWidget::updateStatus(ConnectStatus status)
{
    if (status == DISCONNECT) {
        m_midWidget->stop();
        m_midWidget->hide();
        m_leftLabel->hide();

        QPixmap pixmap2(":/images/device_disconnect.png");
        m_rightLabel->setPixmap(pixmap2.scaled(80, 80, Qt::KeepAspectRatio)); // 保持比例
        m_rightLabel->setAlignment(Qt::AlignCenter); // 居中
        m_statusLabel->setText(QStringLiteral("未连接"));
    }
    else if (status == CONNECTING) {
        m_midWidget->start(200);

        QPixmap pixmap2(":/images/device_connecting.png");
        m_rightLabel->setPixmap(pixmap2.scaled(80, 80, Qt::KeepAspectRatio)); // 保持比例
        m_rightLabel->setAlignment(Qt::AlignCenter); // 居中
        m_statusLabel->setText(QStringLiteral("连接中"));
    }
    else if (status == CONNECTED) {
        m_midWidget->stop();
        m_midWidget->hide();
        m_leftLabel->hide();

        QPixmap pixmap2(":/images/device_connect.png");
        m_rightLabel->setPixmap(pixmap2.scaled(80, 80, Qt::KeepAspectRatio)); // 保持比例
        m_rightLabel->setAlignment(Qt::AlignCenter); // 居中
        m_statusLabel->setText(QStringLiteral("待机中"));
    }
}
