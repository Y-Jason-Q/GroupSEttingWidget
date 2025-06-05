#include "devicelinkwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QDebug>

#include "DeviceListWidget/devicelistwidget.h"

DeviceLinkWidget::DeviceLinkWidget(QWidget *parent)
    : QWidget{parent}
{
    initUI();
    initConnect();
}

void DeviceLinkWidget::initUI()
{
    m_iconLabel = new QLabel;
    m_titleLabel = new QLabel;
    m_refreshButton = new QToolButton(this);
    m_deviceListWidget = new DeviceListWidget;

    m_iconLabel->setFixedSize(32, 32);
    m_titleLabel->setFixedHeight(32);
    m_refreshButton->setFixedSize(32, 32);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(16);
    hLayout->addWidget(m_iconLabel);
    hLayout->addWidget(m_titleLabel);
    hLayout->addStretch();
    hLayout->addWidget(m_refreshButton);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(16, 16, 16, 16);
    vLayout->setSpacing(16);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(m_deviceListWidget);

    this->setObjectName("DeviceLink-mainWidget");
    this->setAttribute(Qt::WA_StyledBackground);

    this->setStyleSheet(""
                        "#DeviceLink-mainWidget {"
                        "    background-color: #282828;"
                        "    border-radius: 16;"
                        "}");

    QPixmap pixmap(":/images/device_small.png");
    m_iconLabel->setPixmap(pixmap);
    m_iconLabel->setScaledContents(true);
    m_iconLabel->setStyleSheet("background-color: transparent;");

    m_titleLabel->setStyleSheet("background: transparent; font-family: \"Microsoft YaHei-Bold\"; font-weight: bold; font-size: 16px; color: #BBBBBB;");

    m_titleLabel->setText(QStringLiteral("可连接设备"));

    m_refreshButton->setAttribute(Qt::WA_AlwaysStackOnTop);
    m_refreshButton->setIcon(QIcon(":/images/refresh.png"));
    m_refreshButton->setIconSize(QSize(24, 24));
    m_refreshButton->setStyleSheet("background: transparent; font-family: \"Microsoft YaHei-Regular\"; font-size: 12px; color: #BBBBBB; text-decoration: underline;");

    m_rotateAnimation = new QPropertyAnimation(m_refreshButton, "rotation");
    m_rotateAnimation->setDuration(1200);
    m_rotateAnimation->setStartValue(0);
    m_rotateAnimation->setEndValue(360);
    m_rotateAnimation->setLoopCount(-1);

    m_refreshButton->hide();
}

void DeviceLinkWidget::initConnect()
{
    connect(m_refreshButton, &QToolButton::clicked, this, &DeviceLinkWidget::onRefreshButtonClicked);

    connect(m_deviceListWidget, &DeviceListWidget::deviceConnectRequested, this, &DeviceLinkWidget::onConnectRequest);
    connect(m_deviceListWidget, &DeviceListWidget::deviceDisconnectRequested, this, &DeviceLinkWidget::onDisconnectRequest);
}

void DeviceLinkWidget::updateDeviceState(const QString &id, RotatingButton::State state)
{
    m_deviceListWidget->updateDeviceState(id, state);
}

void DeviceLinkWidget::onRefreshButtonClicked()
{
    if (m_isLoading)
        return;

    if (m_rotateAnimation->state() == QAbstractAnimation::Running)
        return;

    m_isLoading = true;

    m_refreshButton->setIcon(QIcon(":/images/waiting.png"));
    m_rotateAnimation->start();

    QTimer::singleShot(3000, this, &DeviceLinkWidget::onRefreshFinished);
}

void DeviceLinkWidget::onRefreshFinished()
{
    m_isLoading = false;

    m_rotateAnimation->stop();

    QPropertyAnimation *resetAnim = new QPropertyAnimation(m_refreshButton, "rotation");
    resetAnim->setDuration(500);
    resetAnim->setStartValue(0);
    resetAnim->setEndValue(360);
    resetAnim->setEasingCurve(QEasingCurve::OutQuad);
    connect(resetAnim, &QPropertyAnimation::finished, [=]() {
        m_refreshButton->setIcon(QIcon(":/images/refresh.png"));
//        m_refreshButton->setIcon(QIcon(""));
//        m_refreshButton->setText(QStringLiteral("connected"));
    });
    resetAnim->start(QAbstractAnimation::DeleteWhenStopped);
}

void DeviceLinkWidget::onAddDevice(const QString &id)
{
    if (m_deviceListWidget) {
        m_deviceListWidget->addDevice(id);
    }
}

void DeviceLinkWidget::onConnectRequest(const QString &id)
{
//    qDebug() << "[DeviceLinkWidget] onConnectRequest = id" << id;
    emit connectRequest(id);
}

void DeviceLinkWidget::onDisconnectRequest(const QString &id)
{
//    qDebug() << "[DeviceLinkWidget] onDisconnectRequest = id" << id;
    emit disconnectRequest(id);
}
