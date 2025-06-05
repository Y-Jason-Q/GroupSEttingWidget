#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QDebug>
#include <QMetaEnum>
#include <QThread>
#include <QTimer>
#include <QPushButton>
#include "src/ui/customvideoplayer.h"
#include "src/ui/stylewindow.h"
#include "network_connect.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    if (!isRunningAsAdmin()) {
//        qCritical() << "请以管理员权限运行此程序";
//    }

//    printNetworkInfo();

//    NetworkMonitor monitor;
//    QTimer timer;
//    QObject::connect(&timer, &QTimer::timeout, []{
//        qDebug() << "\n[定时刷新网络信息]";
//        printNetworkInfo();
//    });
//    timer.start(5000);

//    m_videoPlayer = new CustomVideoPlayer;

    window = new StyleWindow;
    window->show();

    QPushButton *openLinkPico = new QPushButton(tr("Link PICO"), this);
    connect(openLinkPico, &QPushButton::clicked, this, [&, this](){
//        m_videoPlayer->setTitleName(tr("Test 1"));
//        m_videoPlayer->resize(600, 400);
//        m_videoPlayer->show();

        window->show();
    });

    openLinkPico->move(20, 20);
    openLinkPico->show();

    QPushButton *closeLinkPico = new QPushButton(tr("Close Link"), this);
    connect(closeLinkPico, &QPushButton::clicked, this, [this]() {
//        if (m_videoPlayer->isVisible())
//            m_videoPlayer->hide();
        if (window->isVisible())
            window->close();
    });
    closeLinkPico->move(120, 20);
    closeLinkPico->show();

    if (!m_client.initialize("./SDK/PXREAClientSDK.dll")) {
        qFatal("Initialize failed: %s", qPrintable(m_client.lastError()));
    }

    // 连接所有信号
    connect(&m_client, &PXREAClientWrapper::serverConnected, [](){
        qDebug() << "PICO hatSet connect successful";
    });

    connect(&m_client, &PXREAClientWrapper::deviceFound, [this](const QString &id) {
        qDebug() << "Found device: " << id;
        if (m_client.lockDevice(id) == 0) {
            int ret = m_client.startMonitorScreen(id, Quality1080P);
            if (ret != 0) {
                qDebug() << "[startMonitorScreen] return : " << ret << ", error = " << m_client.lastError();
            }
            else {
                qDebug() << "[startMonitorScreen] successful";
            }

            int battery = m_client.getBattery(id);
            qDebug() << "battery = " << battery;
        }
    });

    connect(&m_client, &PXREAClientWrapper::deviceBatteryUpdated, [](const QString &devID, int battery){
        qDebug() << "current devID: " << devID << ", battery = " << battery;
    });

    connect(&m_client, &PXREAClientWrapper::screenFrameReceived, this, &MainWindow::onScreenFrameReceived, Qt::QueuedConnection);
    connect(&m_client, &PXREAClientWrapper::streamFrameReceived, this, &MainWindow::onStreamFrameReceived, Qt::QueuedConnection);

//    QTimer::singleShot(0, []{
//        qDebug() << "=== Keep Alive ===";
//    });

    this->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onScreenFrameReceived(const QString &devID, const uint8_t *yuvData, int width, int height)
{
    qDebug() << "devID:" << devID;
    m_videoPlayer->setTitleName(devID);
    emit m_videoPlayer->updateFrame(yuvData, width, height);
}

void MainWindow::onStreamFrameReceived(const QString &devID, const uint8_t *yuvData, int width, int height)
{
    qDebug() << "devID:" << devID;
    m_videoPlayer->setTitleName(devID);
    emit m_videoPlayer->updateFrame(yuvData, width, height);
}
