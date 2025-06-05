#ifndef PICOCALLER_H
#define PICOCALLER_H

#include <QLibrary>
#include <QObject>
#include <QImage>
#include <QJsonObject>
#include "PXREAClientSDK.h"

class PXREAClientWrapper : public QObject
{
    Q_OBJECT
public:
//    enum CallbackType
//    {
//        PXREAFrameCaptureStart      = 1,
//        PXREAFrameCaptureStop       = 1<<1,
//        /// @brief 服务已连接
//        PXREAServerConnect          = 1<<2,
//        /// @brief 服务已断开
//        PXREAServerDisconnect       = 1<<3,
//        /// @brief 设备上线
//        PXREADeviceFind             = 1<<4,
//        /// @brief 设备离线
//        PXREADeviceMissing          = 1<<5,
//        /// @brief 设备电量
//        PXREADeviceBattery          = 1<<6,
//        /// @brief 设备已锁定
//        PXREADeviceLock             = 1<<7,
//        /// @brief 设备已解锁
//        PXREADeviceUnlock           = 1<<8,
//        /// @brief 设备连接
//        PXREADeviceConnect          = 1<<9,
//        /// @brief 设备佩戴传感器
//        PXREADeviceSensor           = 1<<10,
//        /// @brief 设备关闭
//        PXREADeviceShutdown         = 1<<11,
//        /// @brief 设备唤醒
//        PXREADeviceResume           = 1<<12,
//        /// @brief 画面监控参数返回
//        /// @deprecated 已废弃 被具体的流类型取代
//        PXREADeviceMonitorParameter = 1<<13,
//        /// @brief 监控画面帧
//        /// @deprecated 已废弃 被具体的流类型取代
//        PXREADeviceMonitor          = 1<<14,
//        /// @brief 设备型号
//        PXREADeviceModel            = 1<<15,
//        /// @brief 设备当前应用程序
//        PXREACurrentApplication     = 1<<16,
//        /// @brief 手柄电量
//        PXREAControllerBattery      = 1<<17,
//        /// @brief 当前屏幕状态
//        PXREAScreenState            = 1<<18,
//        /// @brief 播控指令执行结果
//        PXREAVideoControlResult     = 1<<19,
//        /// @brief 屏幕监控参数返回
//        PXREADeviceScreenMonitorParameter       = 1<<20,
//        /// @brief 屏幕监控画面帧
//        PXREADeviceScreenMonitor                = 1<<21,
//        /// @brief 自定义流监控参数返回
//        PXREADeviceStreamMonitorParameter       = 1<<22,
//        /// @brief 自定义流监控画面帧
//        PXREADeviceStreamMonitor                = 1<<23,
//        /// @brief 设备别名
//        PXREADeviceAlias                        = 1<<24,
//        /// @brief 设备状态Json描述
//        PXREADeviceStateJson                    = 1<<25,
//        /// @brief 掩码,用于开启全部回调
//        PXREADeviceCustomMessage                = 1<<26,
//        /// @brief 自定义消息
//        PXREAFullMask               = 0xffffffff
//    };
//    Q_ENUM(CallbackType)


    explicit PXREAClientWrapper(QObject *parent = nullptr);
    ~PXREAClientWrapper();

    bool initialize(const QString &dllpath = "PXREAClient.dll");
    bool isInitialize() const;
    QString lastError() const;

    // 封装SDK函数
//    int init(void *context, pfPXREAClientCallback callback, unsigned mask);
    int init(void *context = nullptr, unsigned mask = PXREAFullMask);
    int deinit();
    int sendSharedTexHandle(int64_t handle);
    int getBattery(const QString &devID);
    int shutDownDevice(const QString &devID);
    int restartDevice(const QString &devID);
    int lockDevice(const QString &devID);
    int unlockDevice(const QString &devID);
    int startControlDevice(const QString &devID);
    int stopControlDevice(const QString &devID);
    int startPlayVideo(const QString &devID, const QString &videoPath);
    int stopPlayVideo(const QString &devID);
    int seekVideoTime(const QString &devID, int millseconds);
    int pauseVideo(const QString &devID);
    int continueVideo(const QString &devID);
    int setDeviceVolumn(const QString &devID, int vol);
    int startPlayApp(const QString &devID, const QString &appName);
    int stopPlayApp(const QString &devID, const QString &appName = NULL);
    int startPlayPicture(const QString &devID, const QString &picPath);
    int stopPlayPicture(const QString &devID);
    int sendBytesToDevice(const QString &devID, const char* data, unsigned len);
    int sendBytesToRoom(const char *data, unsigned len);
    int startMonitor(const QString &devID);
    int stopMonitor(const QString &devID);
    int startMonitorScreen(const QString &devID, PXREAMonitorQuality quality);
    int setMonitorScreenQuality(const QString &devID, PXREAMonitorQuality quality);
    int stopMonitorScreen(const QString &devID);
    int startMonitorStream(const QString &devID);
    int stopMonitorStream(const QString &devID);
    int getCurrentApp(const QString &devID);
    int getDeviceModel(const QString &devID);
    int getControllerBattery(const QString &devID, int controllerID);
    int setScreenState(const QString &devID, int screenState);
    int getScreenState(const QString &devID);
    int CustomControllerHomeButtonFunction(const QString &devID, bool isControlMode, PXREAHomeButtonType btnType, PXREAHomeFunction homeFunc);
    int recenterView(const QString &devID);
    int setDeviceAlias(const QString &devID, const char* alias);
    int getDeviceAlias(const QString &devID);
    int deviceControlJson(const QString &devID, const char* parameterJson);


signals:
    /************* 定义回调信号 *************/
    // 基础状态
    void serverConnected();
    void serverDisconnected();

    // 设备状态
    void deviceFound(const QString &devID);
    void deviceLost(const QString &devID);
    void deviceLocked(const QString &devID, bool isLocked);

    // 状态信息
    void deviceBatteryUpdated(const QString &devID, int battery);
    void controllerBatteryUpdated(const QString &devID, int controllerID, int battery);

    void currentAppChanged(const QString &devID, const QString &appName);

    void videoControlResult(const QString &devID, int result, const QString &detail);

    // 监控参数
    void screenParamsReady(const QString &devID, int width, int height);
    void streamParamsReady(const QString &devID, int width, int height);

    // 视频帧
//    void screenFrameReceived(const QString &devID, const QImage &frame);
//    void streamFrameReceived(const QString &devID, const QImage &frame);
    void screenFrameReceived(const QString &devID, const uint8_t *yuvData, int width, int height);
    void streamFrameReceived(const QString &devID, const uint8_t *yuvData, int width, int height);

    void deviceModelReceived(const QString &devID, const QString &model);
    void deviceStateJsonReceived(const QString &devID, const QJsonObject &state);

    void customMessageReceived(const QString &devID, const QByteArray &data);

    void screenStateChanged(const QString &devID, bool isOn);

private:
    QLibrary m_lib;
    bool m_initialized = false;
    QString m_error;

    // 函数指针定义(与SDK头文件中的函数一一对应)
    typedef int (*InitFunc)(void *, pfPXREAClientCallback, unsigned);
    typedef int (*DeinitFunc)();
    typedef int (*SendSharedTexHandleFunc)(int64_t);
    typedef int (*GetBatteryFunc)(const char*);
    typedef int (*ShutDownDeviceFunc)(const char*);
    typedef int (*RestartDeviceFunc)(const char*);
    typedef int (*LockDeviceFunc)(const char*);
    typedef int (*UnlockDeviceFunc)(const char*);
    typedef int (*StartControlDeviceFunc)(const char*);
    typedef int (*StopControlDeviceFunc)(const char*);
    typedef int (*StartPlayVideoFunc)(const char*, const char*);
    typedef int (*StopPlayVideoFunc)(const char*);
    typedef int (*SeekVideoTimeFunc)(const char*, int);
    typedef int (*PauseVideoFunc)(const char*);
    typedef int (*ContinueVideoFunc)(const char*);
    typedef int (*SetDeviceVolumnFunc)(const char*, int);
    typedef int (*StartPlayAppFunc)(const char*, const char*);
    typedef int (*StopPlayAppFunc)(const char*, const char*);
    typedef int (*StartPlayPictureFunc)(const char*, const char*);
    typedef int (*StopPlayPictureFunc)(const char*);
    typedef int (*SendBytesToDeviceFunc)(const char*, const char*, unsigned);
    typedef int (*SendBytesToRoomFunc)(const char*, unsigned);
    typedef int (*StartMonitorFunc)(const char*);
    typedef int (*StopMonitorFunc)(const char*);
    typedef int (*StartMonitorScreenFunc)(const char*, PXREAMonitorQuality);
    typedef int (*SetMonitorScreenQualityFunc)(const char*, PXREAMonitorQuality);
    typedef int (*StopMonitorScreenFunc)(const char*);
    typedef int (*StartMonitorStreamFunc)(const char*);
    typedef int (*StopMonitorStreamFunc)(const char*);
    typedef int (*GetCurrentAppFunc)(const char*);
    typedef int (*GetDeviceModelFunc)(const char*);
    typedef int (*GetControllerBatteryFunc)(const char*, int);
    typedef int (*SetScreenStateFunc)(const char*, int);
    typedef int (*GetScreenStateFunc)(const char*);
    typedef int (*CustomControllerHomeButtonFunctionFunc)(const char*, bool, PXREAHomeButtonType, PXREAHomeFunction);
    typedef int (*RecenterViewFunc)(const char*);
    typedef int (*SetDeviceAliasFunc)(const char*, const char*);
    typedef int (*GetDeviceAliasFunc)(const char*);
    typedef int (*DeviceControlJsonFunc)(const char*, const char*);


    // 函数指针成员
    InitFunc                    m_initFunc                  = nullptr;
    DeinitFunc                  m_deinitFunc                = nullptr;
    SendSharedTexHandleFunc     m_sendSharedTexHandleFunc   = nullptr;
    GetBatteryFunc              m_getBatteryFunc            = nullptr;
    ShutDownDeviceFunc          m_shutDownDeviceFunc        = nullptr;
    RestartDeviceFunc           m_restartDeviceFunc         = nullptr;
    LockDeviceFunc              m_lockDeviceFunc            = nullptr;
    UnlockDeviceFunc            m_unlockDeviceFunc          = nullptr;
    StartControlDeviceFunc      m_startControlDeviceFunc    = nullptr;
    StopControlDeviceFunc       m_stopControlDeviceFunc     = nullptr;
    StartPlayVideoFunc          m_startPlayVideoFunc        = nullptr;
    StopPlayVideoFunc           m_stopPlayVideoFunc         = nullptr;
    SeekVideoTimeFunc           m_seekVideoTimeFunc         = nullptr;
    PauseVideoFunc              m_pauseVideoFunc            = nullptr;
    ContinueVideoFunc           m_continueVideoFunc         = nullptr;
    SetDeviceVolumnFunc         m_setDeviceVolumnFunc       = nullptr;
    StartPlayAppFunc            m_startPlayAppFunc          = nullptr;
    StopPlayAppFunc             m_stopPlayAppFunc           = nullptr;
    StartPlayPictureFunc        m_startPlayPictureFunc      = nullptr;
    StopPlayPictureFunc         m_stopPlayPictureFunc       = nullptr;
    SendBytesToDeviceFunc       m_sendBytesToDeviceFunc     = nullptr;
    SendBytesToRoomFunc         m_sendBytesToRoomFunc       = nullptr;
    StartMonitorFunc            m_startMonitorFunc          = nullptr;
    StopMonitorFunc             m_stopMonitorFunc           = nullptr;
    StartMonitorScreenFunc      m_startMonitorScreenFunc    = nullptr;
    StopMonitorScreenFunc       m_stopMonitorScreenFunc     = nullptr;
    StartMonitorStreamFunc      m_startMonitorStreamFunc    = nullptr;
    StopMonitorStreamFunc       m_stopMonitorStreamFunc     = nullptr;
    GetCurrentAppFunc           m_getCurrentAppFunc         = nullptr;
    GetDeviceModelFunc          m_getDeviceModelFunc        = nullptr;
    GetControllerBatteryFunc    m_getControllerBatteryFunc  = nullptr;
    SetScreenStateFunc          m_setScreenStateFunc        = nullptr;
    GetScreenStateFunc          m_getScreenStateFunc        = nullptr;
    CustomControllerHomeButtonFunctionFunc m_customControllerHomeButtonFunctionFunc = nullptr;
    RecenterViewFunc            m_recenterViewFunc          = nullptr;
    SetDeviceAliasFunc          m_setDeviceAliasFunc        = nullptr;
    GetDeviceAliasFunc          m_getDeviceAliasFunc        = nullptr;
    DeviceControlJsonFunc       m_deviceControlJsonFunc     = nullptr;


    // 函数指针转发函数
    static void clientCallback(void *context, PXREAClientCallbackType type, int status, void *userData);
    void processFrame(PXREAFrameBlobMultiplyMonitor *frame, bool isScreen);
    QImage convertYUV420P(const uint8_t* yuvData, int width, int height);
};

#endif // PICOCALLER_H
