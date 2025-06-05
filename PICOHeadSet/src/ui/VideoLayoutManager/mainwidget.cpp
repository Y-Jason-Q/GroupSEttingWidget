#include "mainwidget.h"
#include "videolayoutmanager.h"
#include "videocontroller.h"
#include "videoitemwrapper.h"
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QMimeData>
#include <QTimer>
#include <QDebug>
#include <QStackedWidget>
#include "../initpagewidget.h"
#include "displaywidget.h"
#include "../SettingPage/groupsettingwidget.h"
#include "../bottombar.h"
#include "../layoutchoosewidget.h"
#include "../DeviceListWidget/devicelistwidget.h"
#include "../devicelistpopwindow.h"
#include "../../PXREAClientWrapper.h"
#include "../yuvplayer.h"

#define GRID_SPACING    4

MainWidget::MainWidget(QWidget *parent)
    : FramelessWindow(parent)
{
    initUI();
    initPicoSDK();
    initConnections();
}

MainWidget::~MainWidget()
{
    if (m_devListPopWidget)
        delete m_devListPopWidget;
    if (m_layoutWidget)
        delete m_layoutWidget;
}

void MainWidget::initUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(centerWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    m_fullWidget = new QWidget;
    m_fullWidget->setWindowFlag(Qt::FramelessWindowHint);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    m_fullWidget->setLayout(layout);
    m_fullWidget->hide();

    initBottomBar();

    m_stackWidget = new QStackedWidget(centerWidget);
    m_initWidget = new InitPageWidget;
    m_displayWidget = new DisplayWidget;
    m_groupSettingWidget = new GroupSettingWidget;

    m_stackWidget->addWidget(m_initWidget);
    m_stackWidget->addWidget(m_displayWidget);
    m_stackWidget->addWidget(m_groupSettingWidget);

    mainLayout->addWidget(m_stackWidget);
    mainLayout->addWidget(m_bottomWidget);
    m_bottomWidget->hide();

    m_initWidget->setupUI();

    m_layoutType = 1;
    m_isFullWindow = false;

    m_displayWidget->initialize();
    createVideoItems();
    m_displayWidget->setWidgetList(m_widgetList);
    m_displayWidget->updateLayout(m_layoutType);

    m_layoutWidget = new LayoutChooseWidget;
    m_devListPopWidget = new DeviceListPopWindow;
    m_layoutWidget->hide();
    m_devListPopWidget->hide();

    m_stackWidget->setCurrentIndex(0);
    setTitleButtonType(MIN_CLOSE);
    resize(960, 600);

    buttonEnable(false);

    setTitleName(QStringLiteral("分组设置"));
    setLogoIcon(":/images/setting_btn.png");

    // TEST
    m_stackWidget->setCurrentIndex(1);
    m_bottomWidget->show();
    showFullScreen();
    setTitleButtonType(ALL);

    // 添加几个未分组成员
    DataManager::instance().addMember(QStringLiteral("小明"));
    DataManager::instance().addMember(QStringLiteral("小红"));
    DataManager::instance().addMember(QStringLiteral("小刚"));
}

void MainWidget::initBottomBar()
{
    m_bottomWidget = new QWidget(this);
    m_bottomWidget->setFixedHeight(100);
    m_bottomLayout = new QHBoxLayout(this);
    m_bottomLayout->setSpacing(32);

    m_layoutButton = new QPushButton();
    m_devlistButton = new QPushButton();
    m_settingButton = new QPushButton();
    m_backButton = new QPushButton();
    m_layoutButton->setFixedSize(100, 50);
    m_devlistButton->setFixedSize(100, 50);
    m_settingButton->setFixedSize(100, 50);
    m_backButton->setFixedSize(100, 50);

    m_layoutLabel = new QLabel();
    m_devlistLabel = new QLabel();
    m_settingLabel = new QLabel();
    m_backLabel = new QLabel();

    m_layoutLabel->setAlignment(Qt::AlignCenter);
    m_devlistLabel->setAlignment(Qt::AlignCenter);
    m_settingLabel->setAlignment(Qt::AlignCenter);
    m_backLabel->setAlignment(Qt::AlignCenter);

    m_layoutLabel->setText(QStringLiteral("画面布局"));
    m_devlistLabel->setText(QStringLiteral("设备列表"));
    m_settingLabel->setText(QStringLiteral("分组设置"));
    m_backLabel->setText(QStringLiteral("返回"));

    QVBoxLayout *lytLayout = new QVBoxLayout;
    QVBoxLayout *devLayout = new QVBoxLayout;
    QVBoxLayout *setLayout = new QVBoxLayout;
    QVBoxLayout *bakLayout = new QVBoxLayout;

    lytLayout->setContentsMargins(0, 10, 0, 10);
    devLayout->setContentsMargins(0, 10, 0, 10);
    setLayout->setContentsMargins(0, 10, 0, 10);
    bakLayout->setContentsMargins(0, 10, 0, 10);

    lytLayout->setSpacing(5);
    devLayout->setSpacing(5);
    setLayout->setSpacing(5);
    bakLayout->setSpacing(5);

    lytLayout->addWidget(m_layoutButton);
    lytLayout->addWidget(m_layoutLabel);

    devLayout->addWidget(m_devlistButton);
    devLayout->addWidget(m_devlistLabel);

    setLayout->addWidget(m_settingButton);
    setLayout->addWidget(m_settingLabel);

    bakLayout->addWidget(m_backButton);
    bakLayout->addWidget(m_backLabel);

    m_bottomLayout->setContentsMargins(0, 0, 0, 0);
    m_bottomLayout->addStretch();
    m_bottomLayout->addLayout(lytLayout);
    m_bottomLayout->addLayout(devLayout);
    m_bottomLayout->addLayout(setLayout);
    m_bottomLayout->addLayout(bakLayout);
    m_bottomLayout->addStretch();

    m_bottomWidget->setLayout(m_bottomLayout);

    this->setStyleSheet("background-color: #1E1E1E;");

    m_layoutButton->setStyleSheet("QPushButton{ background-color: #282828; qproperty-icon: url(:/images/layout_btn.png); qproperty-iconSize: 24px; border-radius: 24px;}"
                                  "QPushButton:hover{ background-color: #393939;}"
                                  "QPushButton:pressed{ background-color: #1E1E1E;}");
    m_devlistButton->setStyleSheet("QPushButton{ background-color: #282828; qproperty-icon: url(:/images/devlist_btn.png); qproperty-iconSize: 32px; border-radius: 24px; }"
                                   "QPushButton:hover{ background-color: #393939;}"
                                   "QPushButton:pressed { background-color: #1E1E1E;}");
    m_settingButton->setStyleSheet("QPushButton{ background-color: #282828; qproperty-icon: url(:/images/setting_btn.png); qproperty-iconSize: 24px; border-radius: 24px; }"
                                   "QPushButton:hover{ background-color: #393939;}"
                                   "QPushButton:pressed { background-color: #1E1E1E;}");
    m_backButton->setStyleSheet("QPushButton{ background-color: #282828; qproperty-icon: url(:/images/back_btn.png); qproperty-iconSize: 24px; border-radius: 24px; }"
                                   "QPushButton:hover{ background-color: #393939;}"
                                   "QPushButton:pressed { background-color: #1E1E1E;}");

    m_layoutLabel->setStyleSheet("QLabel{background-color: transparent; color: white;}");
    m_devlistLabel->setStyleSheet("QLabel{background-color: transparent; color: white;}");
    m_settingLabel->setStyleSheet("QLabel{background-color: transparent; color: white;}");
    m_backLabel->setStyleSheet("QLabel{background-color: transparent; color: white;}");

    m_layoutType = 1;
}

void MainWidget::initPicoSDK()
{
    bool ret = m_picoSdkClient.initialize("./SDK/PXREAClientSDK.dll");
    if (!ret) {
        qDebug() << "Initialize PICO SDK Failed!";
        return;
    }

    // 连接状态
    connect(&m_picoSdkClient, &PXREAClientWrapper::serverConnected, []() {});

    // 查找设备
    connect(&m_picoSdkClient, &PXREAClientWrapper::deviceFound, this, &MainWidget::onDeviceFound);

    // 设备电量更新
    connect(&m_picoSdkClient, &PXREAClientWrapper::deviceBatteryUpdated, [](const QString &devID, int battery) {
        qDebug() << "[MainWidget] deviceBatteryUpdated - devID:" << devID << ", battery: " << battery;
    });

    // 屏幕帧接收
    connect(&m_picoSdkClient, &PXREAClientWrapper::screenFrameReceived, this, &MainWidget::onScreenFrameReceived);
    // 流帧接收
    connect(&m_picoSdkClient, &PXREAClientWrapper::streamFrameReceived, this, &MainWidget::onStreamFrameReceived);
}

void MainWidget::initConnections()
{
    // 设备新增同步
    connect(this, &MainWidget::sigAddDevice, m_initWidget, &InitPageWidget::addDevice);
    connect(this, &MainWidget::sigAddDevice, m_devListPopWidget->devListWidget(), &DeviceListWidget::addDevice);

    // 所有设备列表的连接/断开请求
    connect(m_initWidget, &InitPageWidget::connectRequest, this, &MainWidget::onConnectRequest);
    connect(m_initWidget, &InitPageWidget::disconnectRequest, this, &MainWidget::onDisconnectRequest);

//    connect(m_devListPopWidget, &DeviceListPopWindow::deviceStateChanged, this, &MainWidget::updateAllDeviceState);
    connect(m_devListPopWidget, &DeviceListPopWindow::deviceStateChanged, this, [this](const QString &devId, RotatingButton::State state){
        if (m_stackWidget->currentIndex() == 1) {
            m_displayWidget->clickedLinkButton(devId, state);
        }
    });

    // 统一状态分发，所有窗口同步
    connect(this, &MainWidget::updateAllDeviceState, m_initWidget, &InitPageWidget::updateDeviceState);
    connect(this, &MainWidget::updateAllDeviceState, m_devListPopWidget->devListWidget(), &DeviceListWidget::updateDeviceState);

    // 页面切换，连接成功切换主显示页
    connect(this, &MainWidget::deviceConnectSuccess, [this](const QString &id) {
        m_displayWidget->addWidget(id, false);
        m_displayWidget->updateLayout(m_layoutType);
        if (m_stackWidget->currentIndex() == 0) {
            m_stackWidget->setCurrentIndex(1);
        }
    });

//    connect(m_initWidget, &InitPageWidget::updateDeviceList, this, [this](){});
    connect(m_stackWidget, &QStackedWidget::currentChanged, this, [this](int index){
        if (index == 0) {
            m_bottomWidget->hide();
        }
        else if (index == 1) {
            this->showFullScreen();
            m_bottomWidget->show();
            setTitleButtonType(ALL);
            buttonEnable(false);
        }
        else if (index == 2) {
            buttonEnable(true);
        }
    });

    connect(m_layoutButton, &QPushButton::clicked, this, &MainWidget::onShowLayoutWidget);
    connect(m_devlistButton, &QPushButton::clicked, this, &MainWidget::onShowDeviceListWidget);

    connect(m_displayWidget, &DisplayWidget::connectRequest, this, &MainWidget::onConnectRequest);
    connect(m_displayWidget, &DisplayWidget::disconnectRequest, this, &MainWidget::onDisconnectRequest);
    connect(m_displayWidget, &DisplayWidget::sigFullScreen, this, &MainWidget::onFullScreen);

    connect(m_layoutWidget, &LayoutChooseWidget::clickedButtonIndex, this, &MainWidget::onUpdateLayout);

    connect(m_settingButton, &QPushButton::clicked, [this]() {
        m_stackWidget->setCurrentIndex(2);
//        showFullScreen();
    });

    connect(m_backButton, &QPushButton::clicked, [this]() {
        m_stackWidget->setCurrentIndex(1);
//        showFullScreen();
    });
}

void MainWidget::buttonEnable(bool showBack)
{
    if (showBack) {
        m_backButton->show();
        m_backLabel->show();
        m_layoutButton->hide();
        m_layoutLabel->hide();
        m_devlistButton->hide();
        m_devlistLabel->hide();
        m_settingButton->hide();
        m_settingLabel->hide();

        showSettingTitle(true);
    } else {
        m_backButton->hide();
        m_backLabel->hide();
        m_layoutButton->show();
        m_layoutLabel->show();
        m_devlistButton->show();
        m_devlistLabel->show();
        m_settingButton->show();
        m_settingLabel->show();

        showSettingTitle(false);
    }
}

void MainWidget::onFullScreen(const QString &id, bool isLinked, bool isFull)
{
    if (!isFull) {
        m_fullWidget->hide();
        m_displayWidget->updateLayout(m_layoutType);
        m_isFullWindow = false;
        this->showFullScreen();
        setTitleButtonType(ALL);
    } else {
        if (m_fullPlayer) {
            if (m_fullPlayer->titleName() != id) {
                m_fullPlayer->setFullValue(false);
            }
        }
        m_fullPlayer = m_displayWidget->yuvPlayer(id);
        m_fullWidget->layout()->addWidget(m_fullPlayer);
        m_isFullWindow = true;
        m_fullPlayer->setFullValue(true);
        m_fullWidget->showFullScreen();
        this->hide();
    }
}

void MainWidget::onShowLayoutWidget()
{
    QTimer::singleShot(0, centerWidget, [this]() {
        m_layoutWidget->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
        m_layoutWidget->setAttribute(Qt::WA_DontShowOnScreen, true); // 不显示到屏幕
        m_layoutWidget->show();
        m_layoutWidget->hide();
        m_layoutWidget->setAttribute(Qt::WA_DontShowOnScreen, false); // 恢复

        QPoint btnPosInCenter = m_layoutButton->mapTo(centerWidget, QPoint(0,0));
        QPoint buttonPos = centerWidget->mapToGlobal(btnPosInCenter);

        // 计算弹出位置（按钮上方居中）
        int popupX = buttonPos.x() + m_layoutButton->width()/2 - m_layoutWidget->width()/2;
        int popupY = buttonPos.y() - m_layoutWidget->height() - 20;

        QScreen *screen = QApplication::screenAt(buttonPos);
        if (!screen) {
            qWarning() << "No screen found for button position";
            return;
        }
        QRect screenGeometry = screen->availableGeometry();

        // 智能位置调整 - 优先考虑垂直方向
        if (popupY < screenGeometry.top()) {
            // 上方空间不足：尝试显示在按钮下方
            popupY = buttonPos.y() + m_layoutButton->height();

            // 检查下方空间是否足够
            if (popupY + m_layoutWidget->height() > screenGeometry.bottom()) {
                // 上下空间都不足：优先显示在按钮上方，但限制在屏幕顶部
                popupY = screenGeometry.top();
            }
        }
        else if (popupY + m_layoutWidget->height() > screenGeometry.bottom()) {
            // 按钮上方空间足够但弹出窗口会超出底部
            popupY = screenGeometry.bottom() - m_layoutWidget->height();
        }

        // 水平方向调整
        if (popupX < screenGeometry.left()) {
            popupX = screenGeometry.left();
        }
        else if (popupX + m_layoutWidget->width() > screenGeometry.right()) {
            popupX = screenGeometry.right() - m_layoutWidget->width();
        }


        m_layoutWidget->move(popupX, popupY);
        m_layoutWidget->show();
    });
}

void MainWidget::onShowDeviceListWidget()
{
    QTimer::singleShot(0, centerWidget, [this]() {
        m_layoutWidget->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
        m_layoutWidget->setAttribute(Qt::WA_DontShowOnScreen, true); // 不显示到屏幕
        m_layoutWidget->show();
        m_layoutWidget->hide();
        m_layoutWidget->setAttribute(Qt::WA_DontShowOnScreen, false); // 恢复

        //    QPoint buttonPos = m_devlistButton->mapToGlobal(QPoint(0, 0));
        QPoint btnPosInCenter = m_devlistButton->mapTo(centerWidget, QPoint(0,0));
        QPoint buttonPos = centerWidget->mapToGlobal(btnPosInCenter);

        // 计算弹出位置（按钮上方居中）
        int popupX = buttonPos.x() + m_devlistButton->width()/2 - m_devListPopWidget->width()/2;
        int popupY = buttonPos.y() - m_devListPopWidget->height() - 10;

        QScreen *screen = QApplication::screenAt(buttonPos);
        if (!screen) {
            qWarning() << "No screen found for button position";
            return;
        }
        QRect screenGeometry = screen->availableGeometry();

        // 智能位置调整 - 优先考虑垂直方向
        if (popupY < screenGeometry.top()) {
            // 上方空间不足：尝试显示在按钮下方
            popupY = buttonPos.y() + m_devlistButton->height();

            // 检查下方空间是否足够
            if (popupY + m_layoutWidget->height() > screenGeometry.bottom()) {
                // 上下空间都不足：优先显示在按钮上方，但限制在屏幕顶部
                popupY = screenGeometry.top();
            }
        }
        else if (popupY + m_layoutWidget->height() > screenGeometry.bottom()) {
            // 按钮上方空间足够但弹出窗口会超出底部
            popupY = screenGeometry.bottom() - m_devListPopWidget->height();
        }

        // 水平方向调整
        if (popupX < screenGeometry.left()) {
            popupX = screenGeometry.left();
        }
        else if (popupX + m_layoutWidget->width() > screenGeometry.right()) {
            popupX = screenGeometry.right() - m_devListPopWidget->width();
        }

        m_devListPopWidget->move(popupX, popupY);
        m_devListPopWidget->show();
    });
}

void MainWidget::createVideoItems()
{
    m_widgetList.clear();
    for (int i = 0; i < 9; i++)
    {
        YuvPlayer *player = new YuvPlayer;
        player->setTitleName(tr("PICO %1").arg(i + 1));
        player->updateLinkStatus(false);    // 初始未连接
        m_widgetList.append(player);
    }
}

void MainWidget::onUpdateLayout(int layoutType)
{
    qDebug() << "[MainWidget] onUpdateLayout - layoutType:" << layoutType;
    if (m_layoutType != layoutType) {
        m_layoutType = layoutType;
        m_displayWidget->updateLayout(m_layoutType);
    }
    Sleep(1);

    if (layoutType == 8)
        m_bottomWidget->show();
}

void MainWidget::onDeviceFound(const QString &id)
{
    emit sigAddDevice(id);
}

void MainWidget::onConnectRequest(const QString &id)
{
    qDebug() << "[MainWidget] onConnectRequest - id:" << id;

    emit updateAllDeviceState(id, RotatingButton::Connecting);

    bool ok = false;
    if (m_picoSdkClient.lockDevice(id) == 0) {
        PXREAMonitorQuality quality = Quality480P;
        if (m_layoutType == 1 || m_layoutType == 2)
            quality = Quality1080P;
        int val = m_picoSdkClient.startMonitorScreen(id, quality);
        if (val == 0)
            ok = true;
        else
            qDebug() << "startMonitorScreen failed。" << val;
    }

    if (ok) {
        emit deviceConnectSuccess(id);
        emit updateAllDeviceState(id, RotatingButton::Connected);
    }
    else {
        emit deviceConnectFailed(id);
        emit updateAllDeviceState(id, RotatingButton::Disconnected);
    }
}

void MainWidget::onConnectSuccess(const QString &id)
{
    // 新增一个YuvWidget
    m_displayWidget->addWidget(id, false);
    m_displayWidget->updateLayout(m_layoutType);

    if (m_stackWidget->currentIndex() == 0) {
        m_stackWidget->setCurrentIndex(1);
    }
}

void MainWidget::onDisconnectRequest(const QString &id)
{    
    emit updateAllDeviceState(id, RotatingButton::Disconnecting);

    bool ok = false;
    if (m_picoSdkClient.lockDevice(id) == 0) {
        int val = m_picoSdkClient.stopMonitorScreen(id);
        if (val == 0) {
            m_picoSdkClient.unlockDevice(id);
            ok = true;
        } else {
            qDebug() << "stopMonitorScreen failed: " << val;
        }
    }
    if (ok) {
//        emit deviceDisconnectSuccess(id);
        emit updateAllDeviceState(id, RotatingButton::Disconnected);
    } else {
//        emit deviceDisconnectFailed(id);
        emit updateAllDeviceState(id, RotatingButton::Connected);
    }
}

void MainWidget::onScreenFrameReceived(const QString &devID, const uint8_t *yuvData, int width, int height)
{
//    qDebug() << "[MainWidget] onScreenFrameReceived - devID:" << devID;
    qDebug() << "[MainWidget::onScreenFrameReceived] m_isFullWindow ：" << m_isFullWindow;
    if (m_isFullWindow) {
        m_fullPlayer = m_displayWidget->yuvPlayer(devID);
        m_fullPlayer->updateYuvFrame(yuvData, width, height);
        m_fullPlayer->updateLinkStatus(true);
    }
    else {
        m_displayWidget->updateFrame(devID, yuvData, width, height);
    }
}

void MainWidget::onStreamFrameReceived(const QString &devID, const uint8_t *yuvData, int width, int height)
{
    qDebug() << "[MainWidget] onStreamFrameReceived - devID:" << devID;
    m_displayWidget->updateFrame(devID, yuvData, width, height);
}


void MainWidget::mousePressEvent(QMouseEvent *event)
{
    if (!m_layoutWidget->rect().contains(event->pos())) {
        m_layoutWidget->hide();
    }
    if (!m_devListPopWidget->rect().contains(event->pos())) {
        m_devListPopWidget->hide();
    }
    QWidget::mousePressEvent(event);
}









