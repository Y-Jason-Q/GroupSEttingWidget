#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QComboBox>
#include <QGridLayout>
#include "../framelesswindow.h"
#include "../../PXREAClientWrapper.h"
#include "../DeviceListWidget/rotatingbutton.h"

class QStackedWidget;
class DisplayWidget;
class InitPageWidget;
class GroupSettingWidget;
class BottomBar;

class LayoutChooseWidget;
class DeviceListPopWindow;

class PXREAClientWrapper;

class YuvPlayer;

class MainWidget : public FramelessWindow
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    void initUI();
    void initBottomBar();
    void initPicoSDK();
    void initConnections();
    void createVideoItems();

    void buttonEnable(bool showBack);

signals:
    // 主控同步所有设备列表状态
    void updateAllDeviceState(const QString &id, RotatingButton::State state);
    // 设备发现
    void sigAddDevice(const QString &id);

    // 连接/断开结果
    void deviceConnectSuccess(const QString &id);
    void deviceConnectFailed(const QString &id);

    void deviceDisconnectSuccess(const QString &id);
    void deviceDisconnectFailed(const QString &id);

public slots:
    void onConnectRequest(const QString &id);
    void onDisconnectRequest(const QString &id);

    void onDeviceFound(const QString &id);

    void onConnectSuccess(const QString &id);

    void onScreenFrameReceived(const QString &devID, const uint8_t *yuvData, int width, int height);
    void onStreamFrameReceived(const QString &devID, const uint8_t *yuvData, int width, int height);

    void onUpdateLayout(int layoutType);

    void onShowLayoutWidget();
    void onShowDeviceListWidget();

    void onFullScreen(const QString &id, bool isLinked, bool isFull);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QList<YuvPlayer*> m_widgetList;
    QGridLayout *m_layout;

    QStackedWidget *m_stackWidget;
    InitPageWidget *m_initWidget;
    DisplayWidget  *m_displayWidget;
    GroupSettingWidget *m_groupSettingWidget;
//    BottomBar      *m_bottomBar;

    LayoutChooseWidget *m_layoutWidget;
    DeviceListPopWindow *m_devListPopWidget;

    PXREAClientWrapper m_picoSdkClient;

    int m_layoutType;

    QWidget *m_bottomWidget;
    QPushButton *m_layoutButton;
    QLabel      *m_layoutLabel;
    QPushButton *m_devlistButton;
    QLabel      *m_devlistLabel;
    QPushButton *m_settingButton;
    QLabel      *m_settingLabel;
    QPushButton *m_backButton;
    QLabel      *m_backLabel;
    QHBoxLayout *m_bottomLayout;

    QWidget *m_fullWidget;
    YuvPlayer *m_fullPlayer = nullptr;
    bool m_isFullWindow;
};

#endif // MAINWIDGET_H
