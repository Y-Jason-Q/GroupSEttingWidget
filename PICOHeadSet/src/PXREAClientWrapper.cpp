#include "PXREAClientWrapper.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMetaObject>
#include <QString>
#include <QDebug>
#include <QBuffer>
#include <QFileInfo>

PXREAClientWrapper::PXREAClientWrapper(QObject *parent)
    : QObject(parent)
{
}

PXREAClientWrapper::~PXREAClientWrapper()
{
    if (m_initialized)
        deinit();
}

bool PXREAClientWrapper::initialize(const QString &dllpath)
{
    qDebug() << "Loading SDK form: " << QFileInfo(dllpath).absoluteFilePath();

    m_lib.setFileName(dllpath);
    if (!m_lib.load()) {
        qWarning() << "Load DLL failed:" << m_lib.errorString();
        return false;
    }

    qDebug() << "DLL Loaded Successfully";

    // 解析所有函数指针
    m_initFunc = reinterpret_cast<InitFunc>(m_lib.resolve("Init"));
    m_deinitFunc = reinterpret_cast<DeinitFunc>(m_lib.resolve("Deinit"));
    m_sendSharedTexHandleFunc = reinterpret_cast<SendSharedTexHandleFunc>(m_lib.resolve("SendSharedTexHandle"));
    m_getBatteryFunc = reinterpret_cast<GetBatteryFunc>(m_lib.resolve("GetBattery"));
    m_shutDownDeviceFunc = reinterpret_cast<ShutDownDeviceFunc>(m_lib.resolve("ShutDownDevice"));
    m_restartDeviceFunc = reinterpret_cast<RestartDeviceFunc>(m_lib.resolve("RestartDevice"));
    m_lockDeviceFunc = reinterpret_cast<LockDeviceFunc>(m_lib.resolve("LockDevice"));
    m_unlockDeviceFunc = reinterpret_cast<UnlockDeviceFunc>(m_lib.resolve("UnlockDevice"));
    m_startControlDeviceFunc = reinterpret_cast<StartControlDeviceFunc>(m_lib.resolve("StartControlDevice"));
    m_stopControlDeviceFunc = reinterpret_cast<StopControlDeviceFunc>(m_lib.resolve("StopControlDevice"));
    m_startPlayVideoFunc = reinterpret_cast<StartPlayVideoFunc>(m_lib.resolve("StartPlayVideo"));
    m_stopPlayVideoFunc = reinterpret_cast<StopPlayVideoFunc>(m_lib.resolve("StopPlayVideo"));
    m_seekVideoTimeFunc = reinterpret_cast<SeekVideoTimeFunc>(m_lib.resolve("SeekVideoTime"));
    m_pauseVideoFunc = reinterpret_cast<PauseVideoFunc>(m_lib.resolve("PauseVideo"));
    m_continueVideoFunc = reinterpret_cast<ContinueVideoFunc>(m_lib.resolve("ContinueVideo"));
    m_setDeviceVolumnFunc = reinterpret_cast<SetDeviceVolumnFunc>(m_lib.resolve("SetDeviceVolumn"));
    m_startPlayAppFunc = reinterpret_cast<StartPlayAppFunc>(m_lib.resolve("StartPlayApp"));
    m_stopPlayAppFunc = reinterpret_cast<StopPlayAppFunc>(m_lib.resolve("StopPlayApp"));
    m_startPlayPictureFunc = reinterpret_cast<StartPlayPictureFunc>(m_lib.resolve("StartPlayPicture"));
    m_stopPlayPictureFunc = reinterpret_cast<StopPlayPictureFunc>(m_lib.resolve("StopPlayPicture"));
    m_sendBytesToDeviceFunc = reinterpret_cast<SendBytesToDeviceFunc>(m_lib.resolve("SendBytesToDevice"));
    m_sendBytesToRoomFunc = reinterpret_cast<SendBytesToRoomFunc>(m_lib.resolve("SendBytesToRoom"));
    m_startMonitorFunc = reinterpret_cast<StartMonitorFunc>(m_lib.resolve("StartMonitor"));
    m_stopMonitorFunc = reinterpret_cast<StopMonitorFunc>(m_lib.resolve("StopMonitor"));
    m_startMonitorScreenFunc = reinterpret_cast<StartMonitorScreenFunc>(m_lib.resolve("StartMonitorScreen"));
    m_stopMonitorScreenFunc = reinterpret_cast<StopMonitorScreenFunc>(m_lib.resolve("StopMonitorScreen"));
    m_startMonitorStreamFunc = reinterpret_cast<StartMonitorStreamFunc>(m_lib.resolve("StartMonitorStream"));
    m_stopMonitorStreamFunc = reinterpret_cast<StopMonitorStreamFunc>(m_lib.resolve("StopMonitorStream"));
    m_getCurrentAppFunc = reinterpret_cast<GetCurrentAppFunc>(m_lib.resolve("GetCurrentApp"));
    m_getDeviceModelFunc = reinterpret_cast<GetDeviceModelFunc>(m_lib.resolve("GetDeviceModel"));
    m_getControllerBatteryFunc = reinterpret_cast<GetControllerBatteryFunc>(m_lib.resolve("GetControllerBattery"));
    m_setScreenStateFunc = reinterpret_cast<SetScreenStateFunc>(m_lib.resolve("SetScreenState"));
    m_getScreenStateFunc = reinterpret_cast<GetScreenStateFunc>(m_lib.resolve("GetScreenState"));
    m_customControllerHomeButtonFunctionFunc = reinterpret_cast<CustomControllerHomeButtonFunctionFunc>(m_lib.resolve("CustomControllerHomeButtonFunction"));
    m_recenterViewFunc = reinterpret_cast<RecenterViewFunc>(m_lib.resolve("RecenterView"));
    m_setDeviceAliasFunc = reinterpret_cast<SetDeviceAliasFunc>(m_lib.resolve("SetDeviceAlias"));
    m_getDeviceAliasFunc = reinterpret_cast<GetDeviceAliasFunc>(m_lib.resolve("GetDeivceAlias"));
    m_deviceControlJsonFunc = reinterpret_cast<DeviceControlJsonFunc>(m_lib.resolve("DeviceControlJson"));


    // 验证关键函数
    if (!m_initFunc || !m_lockDeviceFunc || !m_unlockDeviceFunc) {
        m_error = "Failed to resolve core functions";
        m_lib.unload();
        return false;
    }

    // 初始化SDK回调
    int ret = m_initFunc(this, &PXREAClientWrapper::clientCallback, PXREAFullMask);
    if (ret != 0) {
        m_error = QString("SDK init failed with code: %1").arg(ret);
        m_lib.unload();
        return false;
    }

    m_initialized = true;

    return true;
}

bool PXREAClientWrapper::isInitialize() const
{
    return m_initialized;
}

QString PXREAClientWrapper::lastError() const
{
    return m_error;
}

// 功能函数实现
int PXREAClientWrapper::init(void *context, unsigned mask)
{
    return m_initFunc ? m_initFunc(context, &PXREAClientWrapper::clientCallback, mask) : -1;
}

int PXREAClientWrapper::deinit()
{
    return m_deinitFunc ? m_deinitFunc() : -1;
}

int PXREAClientWrapper::sendSharedTexHandle(int64_t handle)
{
    return m_sendSharedTexHandleFunc ? m_sendSharedTexHandleFunc(handle) : -1;
}

int PXREAClientWrapper::getBattery(const QString &devID)
{
    return m_getBatteryFunc ? m_getBatteryFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::shutDownDevice(const QString &devID)
{
    return m_shutDownDeviceFunc ? m_shutDownDeviceFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::restartDevice(const QString &devID)
{
    return m_restartDeviceFunc ? m_restartDeviceFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::lockDevice(const QString &devID)
{
    return m_lockDeviceFunc ? m_lockDeviceFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::unlockDevice(const QString &devID)
{
    return m_unlockDeviceFunc ? m_unlockDeviceFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::startControlDevice(const QString &devID)
{
    return m_startControlDeviceFunc ? m_startControlDeviceFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::stopControlDevice(const QString &devID)
{
    return m_stopControlDeviceFunc ? m_stopControlDeviceFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::startPlayVideo(const QString &devID, const QString &videoPath)
{
    return m_startPlayVideoFunc ? m_startPlayVideoFunc(devID.toUtf8().constData(), videoPath.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::stopPlayVideo(const QString &devID)
{
    return m_stopPlayVideoFunc ? m_stopPlayVideoFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::seekVideoTime(const QString &devID, int millseconds)
{
    return m_seekVideoTimeFunc ? m_seekVideoTimeFunc(devID.toUtf8().constData(), millseconds) : -1;
}

int PXREAClientWrapper::pauseVideo(const QString &devID)
{
    return m_pauseVideoFunc ? m_pauseVideoFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::continueVideo(const QString &devID)
{
    return m_continueVideoFunc ? m_continueVideoFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::setDeviceVolumn(const QString &devID, int vol)
{
    return m_setDeviceVolumnFunc ? m_setDeviceVolumnFunc(devID.toUtf8().constData(), vol) : -1;
}

int PXREAClientWrapper::startPlayApp(const QString &devID, const QString &appName)
{
    return m_startPlayAppFunc ? m_startPlayAppFunc(devID.toUtf8().constData(), appName.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::stopPlayApp(const QString &devID, const QString &appName)
{
    return m_stopPlayAppFunc ? m_stopPlayAppFunc(devID.toUtf8().constData(), appName.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::startPlayPicture(const QString &devID, const QString &picPath)
{
    return m_startPlayPictureFunc ? m_startPlayPictureFunc(devID.toUtf8().constData(), picPath.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::stopPlayPicture(const QString &devID)
{
    return m_stopPlayPictureFunc ? m_stopPlayPictureFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::sendBytesToDevice(const QString &devID, const char *data, unsigned len)
{
    return m_sendBytesToDeviceFunc ?
                m_sendBytesToDeviceFunc(devID.toUtf8().constData(), data, len) : -1;
}

int PXREAClientWrapper::sendBytesToRoom(const char *data, unsigned len)
{
    return m_sendBytesToRoomFunc ?
                m_sendBytesToRoomFunc(data, len) : -1;
}

int PXREAClientWrapper::startMonitor(const QString &devID)
{
    return m_startMonitorFunc ? m_startMonitorFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::stopMonitor(const QString &devID)
{
    return m_stopMonitorFunc ? m_stopMonitorFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::startMonitorScreen(const QString &devID, PXREAMonitorQuality quality)
{
    return m_startMonitorScreenFunc ?
                m_startMonitorScreenFunc(devID.toUtf8().constData(), quality) : -1;
}

int PXREAClientWrapper::stopMonitorScreen(const QString &devID)
{
    return m_stopMonitorScreenFunc ?
                m_stopMonitorScreenFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::startMonitorStream(const QString &devID)
{
    return m_startMonitorStreamFunc ?
                m_startMonitorStreamFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::stopMonitorStream(const QString &devID)
{
    return m_stopMonitorStreamFunc ? m_stopMonitorStreamFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::getCurrentApp(const QString &devID)
{
    return m_getCurrentAppFunc ? m_getCurrentAppFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::getDeviceModel(const QString &devID)
{
    return m_getDeviceModelFunc ? m_getDeviceModelFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::getControllerBattery(const QString &devID, int controllerID)
{
    return m_getControllerBatteryFunc ?
                m_getControllerBatteryFunc(devID.toUtf8().constData(), controllerID) : -1;
}

int PXREAClientWrapper::setScreenState(const QString &devID, int screenState)
{
    return m_setScreenStateFunc ? m_setScreenStateFunc(devID.toUtf8().constData(), screenState) : -1;
}

int PXREAClientWrapper::getScreenState(const QString &devID)
{
    return m_getScreenStateFunc ? m_getScreenStateFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::CustomControllerHomeButtonFunction(const QString &devID, bool isControlMode, PXREAHomeButtonType btnType, PXREAHomeFunction homeFunc)
{
    return m_customControllerHomeButtonFunctionFunc ?
                m_customControllerHomeButtonFunctionFunc(devID.toUtf8().constData(), isControlMode, btnType, homeFunc) : -1;
}

int PXREAClientWrapper::recenterView(const QString &devID)
{
    return m_recenterViewFunc ? m_recenterViewFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::setDeviceAlias(const QString &devID, const char *alias)
{
    return m_setDeviceAliasFunc ?
                m_setDeviceAliasFunc(devID.toUtf8().constData(), alias) : -1;
}

int PXREAClientWrapper::getDeviceAlias(const QString &devID)
{
    return m_getDeviceAliasFunc ? m_getDeviceAliasFunc(devID.toUtf8().constData()) : -1;
}

int PXREAClientWrapper::deviceControlJson(const QString &devID, const char *parameterJson)
{
    return m_deviceControlJsonFunc ?
                m_deviceControlJsonFunc(devID.toUtf8().constData(), parameterJson) : -1;
}

// 回调处理核心
void PXREAClientWrapper::clientCallback(void *context, PXREAClientCallbackType type, int status, void *userData)
{
    qDebug() << "==== [ClientCallback] ==== Type:" << type << ", Status:" << status;

    auto wrapper = qobject_cast<PXREAClientWrapper*>(static_cast<QObject*>(context));
    if (!wrapper) {
        qDebug() << "Invalid context";
        return;
    }

    // 类型转换
//    CallbackType type = static_cast<CallbackType>(sdkType);

    switch(type) {
    //---------------- 设备连接状态 -----------------
    case PXREAServerConnect:
    {
        emit wrapper->serverConnected();
        break;
    }
    case PXREAServerDisconnect:
    {
        emit wrapper->serverDisconnected();
        break;
    }

    //---------------- 设备发现与丢失 -----------------
    case PXREADeviceConnect:
    case PXREADeviceFind:
    {
        const char* devID = static_cast<const char*>(userData);
        emit wrapper->deviceFound(QString::fromLocal8Bit(devID));
        break;
    }
    case PXREADeviceMissing:
    {
        const char* devID = static_cast<const char*>(userData);
        emit wrapper->deviceLost(QString::fromLocal8Bit(devID));
        break;
    }

        //---------------- 设备锁定状态 -----------------
    case PXREADeviceLock:
    {
        const char* devID = static_cast<const char*>(userData);
        emit wrapper->deviceLocked(QString::fromLocal8Bit(devID), true);
        break;
    }
    case PXREADeviceUnlock:
    {
        const char* devID = static_cast<const char*>(userData);
        emit wrapper->deviceLocked(QString::fromLocal8Bit(devID), false);
        break;
    }
    case PXREADeviceShutdown:
    {
        const char* devID = static_cast<const char*>(userData);
        emit wrapper->deviceLocked(QString::fromLocal8Bit(devID), true);
        break;
    }
    case PXREADeviceResume:
    {
        const char* devID = static_cast<const char*>(userData);
        emit wrapper->deviceLocked(QString::fromLocal8Bit(devID), false);
        break;
    }

        //---------------- 电量信息 -----------------
    case PXREADeviceBattery:
    {
        auto battery = static_cast<PXREADevBattery*>(userData);
        emit wrapper->deviceBatteryUpdated(
                    QString::fromLocal8Bit(battery->devID),
                    battery->battery);
        break;
    }
    case PXREAControllerBattery:
    {
        auto battery = static_cast<PXREACtrllerBattery*>(userData);
        emit wrapper->controllerBatteryUpdated(
                    QString::fromLocal8Bit(battery->devID),
                    battery->controllerID, battery->battery);
        break;
    }

        //---------------- 监控参数 -----------------
    case PXREADeviceScreenMonitorParameter:
    {
        auto params = static_cast<PXREAMonitorPara*>(userData);
        emit wrapper->screenParamsReady(QString::fromLocal8Bit(params->devID),
                                        params->width, params->height);
        break;
    }
    case PXREADeviceStreamMonitorParameter:
    {
        auto params = static_cast<PXREAMonitorPara*>(userData);
        emit wrapper->streamParamsReady(QString::fromLocal8Bit(params->devID),
                                        params->width, params->height);
        break;
    }

        //---------------- 视频帧处理 -----------------
    case PXREADeviceScreenMonitor:
    {
        wrapper->processFrame(static_cast<PXREAFrameBlobMultiplyMonitor*>(userData), true);
        break;
    }
    case PXREADeviceStreamMonitor:
    {
        wrapper->processFrame(static_cast<PXREAFrameBlobMultiplyMonitor*>(userData), false);
        break;
    }

        //---------------- 应用状态 -----------------
    case PXREACurrentApplication:
    {
        auto app = static_cast<PXREACurrentApp*>(userData);
        emit wrapper->currentAppChanged(
                    QString::fromLocal8Bit(app->devID),
                    QString::fromLocal8Bit(app->appName));
        break;
    }

        //---------------- 控制结果 -----------------
    case PXREAVideoControlResult:
    {
        auto result = static_cast<PXREAVideoCtrlResult*>(userData);
        emit wrapper->videoControlResult(
                    QString::fromLocal8Bit(result->devID),
                    result->result,
                    QString::fromLocal8Bit(result->detail));
        break;
    }

        //---------------- 设备信息 -----------------
    case PXREADeviceModel:
    {
        auto model = static_cast<PXREADevModel*>(userData);
        emit wrapper->deviceModelReceived(
                    QString::fromLocal8Bit(model->devID),
                    QString::fromLocal8Bit(model->devModel));
        break;
    }

        //---------------- 高级功能 -----------------
    case PXREADeviceStateJson:
    {
        auto stateJson = static_cast<PXREADevStateJson*>(userData);
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(
                    QByteArray(stateJson->stateJson, 256), &error);
        if (error.error == QJsonParseError::NoError) {
            emit wrapper->deviceStateJsonReceived(
                        QString::fromLocal8Bit(stateJson->devID),
                        doc.object());
        }
        else {
            qWarning() << "JSON解析错误:" << error.errorString();
        }
        break;
    }

        //---------------- 其他回调 -----------------
    case PXREAScreenState:
    {
//        const char* state = status == PXREA_SCREEN_ON ? "1" : "0";
        emit wrapper->screenStateChanged(
                    QString::fromLocal8Bit(static_cast<const char*>(userData)),
                    status == PXREA_SCREEN_ON);
        break;
    }

    default:
    {
        qWarning() << "Unhandled callback type:" << type << ", with status:" << status;
        if (userData) {
            qDebug() << "Raw userData: " << QByteArray(
                            static_cast<const char*>(userData), 64).toHex();
        }
        break;
    }
    }
}

// 画面帧处理
void PXREAClientWrapper::processFrame(PXREAFrameBlobMultiplyMonitor *frame, bool isScreen)
{
    // 检查帧参数有效性
    if (!frame || frame->width < 1 || frame->height < 1 || !frame->addr)
    {
        qWarning() << "Invalid frame data received.";
        return;
    }

    QImage image = convertYUV420P(
        reinterpret_cast<const uint8_t*>(frame->addr),
        frame->width, frame->height);

    if (!image.isNull()) {
        if (isScreen) {
            //                emit screenFrameReceived(QString::fromLocal8Bit(frame->devID), image);
            emit screenFrameReceived(
                QString::fromLocal8Bit(frame->devID),
                reinterpret_cast<const uint8_t*>(frame->addr),
                frame->width, frame->height);
        }
        else {
            //                emit streamFrameReceived(QString::fromLocal8Bit(frame->devID), image);
            emit streamFrameReceived(
                QString::fromLocal8Bit(frame->devID),
                reinterpret_cast<const uint8_t*>(frame->addr),
                frame->width, frame->height);
        }
    }
}

QImage PXREAClientWrapper::convertYUV420P(const uint8_t *yuvData, int width, int height)
{
    QImage image(width, height, QImage::Format_RGB32);
    const uint8_t *yPlane = yuvData;
    const uint8_t *uPlane = yuvData + width * height;
    const uint8_t *vPlane = yuvData + width * height + (width / 2) * (height / 2);

//#pragma omp parallel for //  启用OpenMP 多线程加速
    for (int y = 0; y < height; ++y)
    {
        const int uvY = y / 2;
        uint8_t *rgb = image.scanLine(y);

        for (int x = 0; x < width; ++x)
        {
            const int uvX = x / 2;
            const int yIndex = y * width + x;
            const int uIndex = uvY * (width / 2) + uvX;
            const int vIndex = uvY * (width / 2) + uvX;

            // YUV转RGB公式（ITU-R BT.601）
            const int yVal = yPlane[yIndex];
            const int uVal = uPlane[uIndex] - 128;
            const int vVal = vPlane[vIndex] - 128;

            int r = qBound(0, (int)(yVal + 1.402 * vVal), 255);
            int g = qBound(0, (int)(yVal - 0.344 * uVal - 0.714 * vVal), 255);
            int b = qBound(0, (int)(yVal + 1.772 * uVal), 255);

            rgb[x * 4]      = static_cast<uint8_t>(b);
            rgb[x * 4 + 1]  = static_cast<uint8_t>(g);
            rgb[x * 4 + 2]  = static_cast<uint8_t>(r);
            rgb[x * 4 + 3]  = 0xFF;
        }
    }
    return image;
}







