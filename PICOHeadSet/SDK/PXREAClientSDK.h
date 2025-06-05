#ifndef PXREACLIENTSDK_H
#define PXREACLIENTSDK_H
#ifdef _WIN32
#if defined(PXREACLIENTSDK_LIBRARY)
#  define PXREACLIENTSDK_EXPORT __declspec(dllexport)
#else
#  define PXREACLIENTSDK_EXPORT __declspec(dllimport)
#endif
#endif

#ifdef __linux__
#if defined(PXREACLIENTSDK_LIBRARY)
#  define PXREACLIENTSDK_EXPORT __attribute__((visibility("default")))
#else
#  define PXREACLIENTSDK_EXPORT __attribute__((visibility("default")))
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include<stdint.h>

enum PXREAClientCallbackType
{
    PXREAFrameCaptureStart      = 1,
    PXREAFrameCaptureStop       = 1<<1,
    /// @brief 服务已连接
    PXREAServerConnect          = 1<<2,
    /// @brief 服务已断开
    PXREAServerDisconnect       = 1<<3,
    /// @brief 设备上线
    PXREADeviceFind             = 1<<4,
    /// @brief 设备离线
    PXREADeviceMissing          = 1<<5,
    /// @brief 设备电量
    PXREADeviceBattery          = 1<<6,
    /// @brief 设备已锁定
    PXREADeviceLock             = 1<<7,
    /// @brief 设备已解锁
    PXREADeviceUnlock           = 1<<8,
    /// @brief 设备连接
    PXREADeviceConnect          = 1<<9,
    /// @brief 设备佩戴传感器
    PXREADeviceSensor           = 1<<10,
    /// @brief 设备关闭
    PXREADeviceShutdown         = 1<<11,
    /// @brief 设备唤醒
    PXREADeviceResume           = 1<<12,
    /// @brief 画面监控参数返回
    /// @deprecated 已废弃 被具体的流类型取代
    PXREADeviceMonitorParameter = 1<<13,
    /// @brief 监控画面帧
    /// @deprecated 已废弃 被具体的流类型取代
    PXREADeviceMonitor          = 1<<14,
    /// @brief 设备型号
    PXREADeviceModel            = 1<<15,
    /// @brief 设备当前应用程序
    PXREACurrentApplication     = 1<<16,
    /// @brief 手柄电量
    PXREAControllerBattery      = 1<<17,
    /// @brief 当前屏幕状态
    PXREAScreenState            = 1<<18,
    /// @brief 播控指令执行结果
    PXREAVideoControlResult     = 1<<19,
    /// @brief 屏幕监控参数返回
    PXREADeviceScreenMonitorParameter       = 1<<20,
    /// @brief 屏幕监控画面帧
    PXREADeviceScreenMonitor                = 1<<21,
    /// @brief 自定义流监控参数返回
    PXREADeviceStreamMonitorParameter       = 1<<22,
    /// @brief 自定义流监控画面帧
    PXREADeviceStreamMonitor                = 1<<23,
    /// @brief 设备别名
    PXREADeviceAlias                        = 1<<24,
    /// @brief 设备状态Json描述
    PXREADeviceStateJson                    = 1<<25,
    /// @brief 掩码,用于开启全部回调
    PXREADeviceCustomMessage                = 1<<26,
    /// @brief 自定义消息
    PXREAFullMask               = 0xffffffff
};

/// @brief home键按键类型
enum PXREAHomeButtonType {
    /// @brief 单击
    HomeFunction_singleClick = 0,
    /// @brief 双击
    HomeFunction_doubleClick,
    /// @brief 长按
    HomeFunction_longPress
};

/// @brief home键按键功能
enum PXREAHomeFunction {
    /// @brief 返回主界面
    HomeFunction_backToHome = 0,
    /// @brief 系统默认功能
    HomeFunction_default,
    /// @brief 无效
    HomeFunction_null
};
/// @brief 画面监控质量
enum PXREAMonitorQuality {
    /// @brief 854x480
    Quality480P = 0,
    /// @brief 1280x720
    Quality720P,
    /// @brief 1920x1080
    Quality1080P,
    /// @brief 2560x1440
    Quality2K,
    /// @brief 640x360
    Quality360p,
    /// @brief 分辨率从key-setting配置文件中读取
    QualityUserConfig = 1000
};

typedef struct {
    unsigned width;
    unsigned height;
    unsigned fps;
}PXREAVrParam;

/// @brief 设备电量
typedef struct {
    /// @brief 设备sn
    char devID[32];
    /// @brief 设备电量
    unsigned battery;
}PXREADevBattery;

/// @brief 画面监控帧信息
/// @deprecated 已废弃
typedef struct {
    /// @brief 设备sn
    char devID[32];
    /// @brief 帧数据起始地址
    const char* addr;
    /// @brief 帧数据长度
    unsigned size;
    /// @brief 帧宽度
    unsigned width;
    /// @brief 帧高度
    unsigned height;
    /// @brief 帧行宽
    unsigned bytesPerLine;
    /// @brief 帧画面格式,当前仅支持YUV420P
    int pixFormat;
}PXREAFrameBlob;

/// @brief 多路画面监控帧信息
typedef struct {
    /// @brief 设备sn
    char devID[32];
    /// @brief 流类型 0 stream 1 screen
    int isScreen;
    /// @brief 帧数据起始地址
    const char* addr;
    /// @brief 帧数据长度
    unsigned size;
    /// @brief 帧宽度
    unsigned width;
    /// @brief 帧高度
    unsigned height;
    /// @brief 帧行宽
    unsigned bytesPerLine;
    /// @brief 帧画面格式,当前仅支持YUV420P
    int pixFormat;
}PXREAFrameBlobMultiplyMonitor;

/// @brief 画面监控启动信息
typedef struct {
    /// @brief 设备sn
    char devID[32];
    /// @brief 帧宽度
    unsigned width;
    /// @brief 帧高度
    unsigned height;
}PXREAMonitorPara;

/// @brief 设备型号信息
typedef struct {
    /// @brief 设备sn
    char devID[32];
    /// @brief 设备型号
    char devModel[32];
}PXREADevModel;

/// @brief 设备应用信息
typedef struct {
    /// @brief 设备sn
    char devID[32];
    /// @brief 应用包名
    char appName[128];
}PXREACurrentApp;

/// @brief 手柄电量信息
typedef struct {
    /// @brief 设备sn
    char devID[32];
    /// @brief 手柄ID 0 左手柄 1 右手柄
    int controllerID;
    /// @brief 手柄电量 0-100
    unsigned battery;
}PXREACtrllerBattery;

/// @brief 播控指令返回结果
typedef struct {
    /// @brief 设备sn
    char devID[32];
    /// @brief 播控类型
    /// @brief openApp 打开应用
    /// @brief closeApp 关闭应用
    /// @brief openVideo 打开视频
    /// @brief closeVideo 关闭视频
    /// @brief openImage 打开图像
    /// @brief closeImage 关闭图像
    char action[32];
    /// @brief 播控是否成功，同status
    /// @brief 0 成功 对应宏 #PXREA_VIDEO_CONTROL_SUCCESS
    /// @brief -1 失败 对应宏 #PXREA_VIDEO_CONTROL_FAIL
    int  result;
    /// @brief 错误详情
    char detail[128];
    /// @brief 错误码 某些播控类型没有错误码，则返回0
    int  errorcode;
}PXREAVideoCtrlResult;

/// @brief 设备别名
typedef struct {
    /// @brief 设备sn
    char devID[32];
    /// @brief 设备别名
    char alias[128];
}PXREADevAlias;

/// @brief 设备状态json描述
typedef struct {
    /// @brief 设备sn
    char devID[32];
    /// @brief json格式的设备状态
    char stateJson[256];
}PXREADevStateJson;

typedef struct {
    /// @brief 设备sn
    char devID[32];
    /// @brief 数据长度
    uint64_t dataSize;
    /// @brief 数据指针，回调内有效
    const char* dataPtr;
}PXREADevCustomMessage;





/**
 * @brief 客户端回调，用户接收服务端消息
 * @param context 回调上下文，由 #Init 参数1 context 传入
 * @param type 回调类型
 * @param status 回调状态码
 * @param userData 回调数据指针，由参数2 type 决定
 */
typedef void(*pfPXREAClientCallback)(void* context,PXREAClientCallbackType type,int status,void* userData);

#define PXREA_SCREEN_ON 1
#define PXREA_SCREEN_OFF 0
#define PXREA_VIDEO_CONTROL_SUCCESS 0
#define PXREA_VIDEO_CONTROL_FAIL -1

/**
 * @brief SDK初始化接口
 * @brief 连接服务，同时注册回调
 * @param context 回调上下文，用于为回调函数传入用户自定义数据
 * @param cliCallback 回调函数指针，用于监听服务端消息
 * @param mask 回调掩码，用于屏蔽某些服务端消息
 */
PXREACLIENTSDK_EXPORT int Init(void* context,pfPXREAClientCallback cliCallback,unsigned mask);
/**
 * @brief 终止接口
 * @brief 断开服务连接
 */
PXREACLIENTSDK_EXPORT int Deinit();
PXREACLIENTSDK_EXPORT int SendSharedTexHandle(int64_t handle);
/**
 * @brief 查询设备电量
 * @param devID 设备sn
 * @return 0 成功
 * @return 返回结果参见 PXREADevBattery
 * @return -1 失败
 */
PXREACLIENTSDK_EXPORT int GetBattery(const char* devID);
/**
 * @brief 关闭设备
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 */
PXREACLIENTSDK_EXPORT int ShutDownDevice(const char* devID);
/**
 * @brief 重启设备
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 */
PXREACLIENTSDK_EXPORT int RestartDevice(const char* devID);
/**
 * @brief 锁定设备
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 * @note 锁定指令发出后，锁定结果通过回调 @link PXREAClientCallbackType::PXREADeviceLock PXREADeviceLock @endlink 通知客户端
 */
PXREACLIENTSDK_EXPORT int LockDevice(const char* devID);
/**
 * @brief 解除设备锁定
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 * @note 向VR头戴发送任何播控指令之前,必须锁定设备
 * @note 锁定指令发出后，锁定解除结果通过回调 @link PXREAClientCallbackType::PXREADeviceUnlock PXREADeviceUnlock @endlink 通知客户端
 */
PXREACLIENTSDK_EXPORT int UnlockDevice(const char* devID);
/**
 * @brief 显示受控提示
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 * @note 受控提示框的作用是提示VR设备的使用者,设备已受控,但是它与锁定不同,不是设备播控的必要条件
 * @note 此指令无法在C端设备上使用,用户需自行开发受控界面App,并在设备发现后启动
 */
PXREACLIENTSDK_EXPORT int StartControlDevice(const char* devID);
/**
 * @brief 取消受控提示
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 * @note 发送该指令之前，需调用 #LockDevice 锁定设备
 */
PXREACLIENTSDK_EXPORT int StopControlDevice(const char* devID);
/**
 * @brief 控制VR端播放一个视频
 * @param devID 设备sn
 * @param videoPath 视频路径 <br>
 * 视频路径格式为 视频地址|格式 例如 /sdcard/pre_resource/1.mp4|2D <br>
 * 目前支持以下两种格式 <br>
 * RTSP 地址 示例 <br>
 * http://39.134.115.163:8080/PLTV/88888910/224/3221225642/index.m3u8 <br>
 * 内部存储视频地址 示例 <br>
 * /sdcard/pre_resource/1.mp4 <br>
 * 已支持的格式包括 <br>
 * | 格式 | 说明 | <br>
 * | 2D | 普通2D模式 | <br>
 * | 3D_LR | 左右视角3D模式,需视频文件支持 | <br>
 * | 3D_TB | 上下视角3D模式,需视频文件支持 | <br>
 * | 180 | 180广角模式,需视频文件支持 | <br>
 * | 180_LR | 180左右广角3D模式,需视频文件支持 | <br>
 * | 180_TB | 180上下广角3D模式,需视频文件支持 | <br>
 * | 360 | 360广角模式,需视频文件支持 | <br>
 * | 360_LR | 360左右广角3D模式,需视频文件支持 | <br>
 * | 360_TB | 360上下广角3D模式,需视频文件支持 | <br>
 * | Youtube | Youtube专有2D视频格式 | <br>
 * | Youtube3D | Youtube专有3D视频格式 | <br>
 * @return 0 成功
 * @return -1 失败
 * @note 发送该指令之前，需调用 #LockDevice 锁定设备
 */
PXREACLIENTSDK_EXPORT int StartPlayVideo(const char* devID,const char* videoPath);
/**
 * @brief 控制VR端停止播放视频
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 * @note 发送该指令之前，需调用 #LockDevice 锁定设备
 */
PXREACLIENTSDK_EXPORT int StopPlayVideo(const char* devID);
/**
 * @brief 视频播放跳转至指定时间
 * @param devID 设备sn
 * @param millseconds 跳转时间点,单位是毫秒
 * @return 0 成功
 * @return -1 失败
 * @note 发送该指令之前，需调用 #LockDevice 锁定设备
 */
PXREACLIENTSDK_EXPORT int SeekVideoTime(const char* devID,int millseconds);
/**
 * @brief 视频播放暂停
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 * @note 发送该指令之前，需调用 #LockDevice 锁定设备
 */
PXREACLIENTSDK_EXPORT int PauseVideo(const char* devID);
/**
 * @brief 视频播放继续
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 * @note 发送该指令之前，需调用 #LockDevice 锁定设备
 */
PXREACLIENTSDK_EXPORT int ContinueVideo(const char* devID);
/**
 * @brief 控制设备音量
 * @param devID 设备sn
 * @param vol 设备音量 范围0-100
 * @return 0 成功
 * @return -1 失败
 * @note 发送该指令之前，需调用 #LockDevice 锁定设备
 */
PXREACLIENTSDK_EXPORT int SetDeviceVolumn(const char* devID,int vol);
/**
 * @brief 启动应用
 * @param devID 设备sn
 * @param appName 应用名称和参数 <br>
 *                支持无参启动应用和含参启动应用 <br>
 *                无参启动应用,传入包名即可，如：<br>
 *                com.tencent.mm <br>
 *                含参启动应用，使用json字符串，如：<br>
 *                { "packageName": "com.tencent.mm", "extra": { "arg1": "arg1Data" } } <br>
 *                packageName对应包名 <br>
 *                extra对应应用参数，可以是一个或者多个 <br>
 * @return 0 成功
 * @return -1 失败
 * @note 发送该指令之前，需调用 #LockDevice 锁定设备
 */
PXREACLIENTSDK_EXPORT int StartPlayApp(const char* devID, const char* appName);
/**
 * @brief 退出当前应用
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 * @note 发送该指令之前，需调用 #LockDevice 锁定设备
 */
PXREACLIENTSDK_EXPORT int StopPlayApp(const char* devID, const char* appName=NULL);
/**
 * @brief 播放全景图片
 * @param devID 设备sn
 * @param picPath 图片路径 <br>
 *                图片路径格式为 url:类型代码 例如 /sdcard/pre_resource/1.png:1 <br>
 *                url目前仅支持内部存储图片路径 <br>
 *                已支持的类型包括 <br>
 *                | 类型代码 | 说明 | <br>
 *                | 0 | 360模式,需图片内容支持 | <br>
 *                | 1 | 普通2D模式 | <br>
 *                | 2 | 360左右3D模式,需图片格式支持 | <br>
 *                | 3 | 360上下3D模式,需图片格式支持 | <br>
 * @return 0 成功
 * @return -1 失败
 * @note 发送该指令之前，需调用 #LockDevice 锁定设备
 */
PXREACLIENTSDK_EXPORT int StartPlayPicture(const char* devID,const char* picPath);
/**
 * @brief 停止播放当前全景图
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 * @note 发送该指令之前，需调用 #LockDevice 锁定设备
 */
PXREACLIENTSDK_EXPORT int StopPlayPicture(const char* devID);
/**
 * @brief 向指定设备发送字节流
 * @param devID 设备sn
 * @param data 字节流起始地址
 * @param len 字节流长度
 * @return 0 成功
 * @return -1 失败
 * @note 此命令适用于sdk调用者自定义消息
 */
PXREACLIENTSDK_EXPORT int SendBytesToDevice(const char* devID,const char* data,unsigned len);
/**
 * @brief 向房间内所有用户广播字节流
 * @param data 字节流起始地址
 * @param len 字节流长度
 * @return 0 成功
 * @return -1 失败
 * @note 此命令适用于sdk调用者自定义消息
 */
PXREACLIENTSDK_EXPORT int SendBytesToRoom(const char* data,unsigned len);
/**
 * @brief 开启设备画面监视
 * @deprecated 已过时，参见 ::StartMonitorScreen
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 * @note 画面监控开启后，首先触发回调@link PXREAClientCallbackType::PXREADeviceScreenMonitorParameter PXREADeviceScreenMonitorParameter @endlink
 * @note 此回调用于回传监控画面的基本信息，用户可以利用这些信息初始化播放器
 * @note 回调对应的数据结构为 PXREAMonitorPara
 * @note 画面监控过程中，数据帧将通过回调@link PXREAClientCallbackType::PXREADeviceScreenMonitor PXREADeviceScreenMonitor @endlink持续返回画面帧
 * @note 画面帧对应的数据结构为 PXREAFrameBlobMultiplyMonitor
 */
PXREACLIENTSDK_EXPORT int StartMonitor(const char* devID);
/**
 * @brief 关闭设备画面监视
 * @deprecated 已过时，参见 ::StopMonitorScreen
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 */
PXREACLIENTSDK_EXPORT int StopMonitor(const char* devID);
/**
 * @brief 开启设备屏幕画面监视
 * @param devID 设备sn
 * @param quality 画面监控质量,参见 #PXREAMonitorQuality
 * @return 0 成功
 * @return -1 失败
 * @note 画面监控开启后，首先触发回调@link PXREAClientCallbackType::PXREADeviceScreenMonitorParameter PXREADeviceScreenMonitorParameter @endlink
 * @note 此回调用于回传监控画面的基本信息，用户可以利用这些信息初始化播放器
 * @note 回调对应的数据结构为 PXREAMonitorPara
 * @note 画面监控过程中，数据帧将通过回调@link PXREAClientCallbackType::PXREADeviceScreenMonitor PXREADeviceScreenMonitor @endlink持续返回画面帧
 * @note 画面帧对应的数据结构为 PXREAFrameBlobMultiplyMonitor
 */
PXREACLIENTSDK_EXPORT int StartMonitorScreen(const char* devID, PXREAMonitorQuality quality);
/**
 * @brief 修改设备屏幕画面监控质量
 * @param devID 设备sn
 * @param quality 画面监控质量,参见 #PXREAMonitorQuality
 * @return 0 成功
 * @return -1 失败
 * @note 仅当设备画面监控开启后,才能调用此函数调整画面监控质量
 * @note 画面监控过程中，数据帧将通过回调@link PXREAClientCallbackType::PXREADeviceScreenMonitor PXREADeviceScreenMonitor @endlink持续返回画面帧
 * @note 画面帧对应的数据结构为 PXREAFrameBlobMultiplyMonitor
 */
PXREACLIENTSDK_EXPORT int SetMonitorScreenQuality(const char* devID, PXREAMonitorQuality quality);
/**
 * @brief 关闭设备屏幕画面监视
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 */
PXREACLIENTSDK_EXPORT int StopMonitorScreen(const char* devID);
/**
 * @brief 开启设备摄像头监视
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 * @note 画面监控开启后，首先触发回调@link PXREAClientCallbackType::PXREADeviceStreamMonitorParameter PXREADeviceStreamMonitorParameter @endlink
 * @note 此回调用于回传监控画面的基本信息，用户可以利用这些信息初始化播放器
 * @note 回调对应的数据结构为 PXREAMonitorPara
 * @note 画面监控过程中，数据帧将通过回调@link PXREAClientCallbackType::PXREADeviceStreamMonitor PXREADeviceStreamMonitor @endlink持续返回画面帧
 * @note 画面帧对应的数据结构为 PXREAFrameBlobMultiplyMonitor
 */
PXREACLIENTSDK_EXPORT int StartMonitorStream(const char* devID);
/**
 * @brief 关闭设备摄像头监视
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 */
PXREACLIENTSDK_EXPORT int StopMonitorStream(const char* devID);
/**
 * @brief 查询当前头戴内的前台应用
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 * @note 当前前台应用的包名将通过回调@link PXREAClientCallbackType::PXREACurrentApplication PXREACurrentApplication @endlink返回
 * @note 返回结果参见 PXREACurrentApp
 */
PXREACLIENTSDK_EXPORT int GetCurrentApp(const char* devID);
/**
 * @brief 查询设备型号
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 * @note 当前设备型号将通过回调@link PXREAClientCallbackType::PXREADeviceModel PXREADeviceModel @endlink返回
 * @note 返回结果参见 PXREADevModel
 */
PXREACLIENTSDK_EXPORT int GetDeviceModel(const char* devID);
/**
 * @brief 此接口用于查询VR手柄剩余电量
 * @param devID 设备sn
 * @param controllerID 手柄ID 0 左手柄 1 右手柄
 * @return 0 成功
 * @return -1 失败
 * @note 当前设备型号将通过回调@link PXREAClientCallbackType::PXREAControllerBattery PXREAControllerBattery @endlink返回
 * @note 返回结果参见 PXREACtrllerBattery
 * @note 当手柄处于未连接状态时,电量查询会失败
 */
PXREACLIENTSDK_EXPORT int GetControllerBattery(const char* devID,int controllerID);
/**
 * @brief 设置屏幕状态
 * @param devID 设备sn
 * @param screenState 屏幕状态 <br>
 *                    0 息屏 对应宏 #PXREA_SCREEN_OFF <br>
 *                    1 亮屏 对应宏 #PXREA_SCREEN_ON
 * @return 0 成功
 * @return -1 失败
 * @note 发送该指令之前，需调用 #LockDevice 锁定设备
 */
PXREACLIENTSDK_EXPORT int SetScreenState(const char *devID, int screenState);
/**
 * @brief 获取屏幕状态
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 * @note 当前设备型号将通过回调@link PXREAClientCallbackType::PXREAScreenState PXREAScreenState @endlink返回
 * @note 解析返回数据时 userData 需转化为字符串常量
 * @note "0" 屏幕处于息屏状态
 * @note "1" 屏幕处于亮屏状态
 * @note 发送该指令之前，需调用 #LockDevice 锁定设备
 */
PXREACLIENTSDK_EXPORT int GetScreenState(const char *devID);
/**
 * @brief 设置home键功能
 * @param devID 设备sn
 * @param isControlMode 设置的场景是受控模式还是非受控模式
 * @param btnType 按键类型 参考 #PXREAHomeButtonType
 * @param homeFunc 功能类型 参考 #PXREAHomeFunction
 * @return 0 成功
 * @return -1 失败
 * @note home键功能设置后，如果企业套件VR端退出，设置将失效，需要重新设置
 */
PXREACLIENTSDK_EXPORT int CustomControllerHomeButtonFunction(const char *devID,bool isControlMode,PXREAHomeButtonType btnType,PXREAHomeFunction homeFunc);
/**
 * @brief 重置设备视角
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 */
PXREACLIENTSDK_EXPORT int RecenterView(const char* devID);
/**
 * @brief 设置设备别名
 * @param devID 设备sn
 * @param alias 设备别名
 * @return 0 成功
 * @return -1 失败
 */
PXREACLIENTSDK_EXPORT int SetDeviceAlias(const char *devID, const char *alias);
/**
 * @brief 获取设备别名
 * @param devID 设备sn
 * @return 0 成功
 * @return -1 失败
 * @note 设备别名将通过回调@link PXREAClientCallbackType::PXREADeviceAlias PXREADeviceAlias @endlink返回
 * @note 返回结果参见 PXREADevAlias
 */
PXREACLIENTSDK_EXPORT int GetDeviceAlias(const char *devID);
/**
 * @brief 向设备发送json格式指令
 * @param devID 设备sn
 * @param parameterJson 功能及参数,json格式,具体用法参考企业套件SDK文档
 * @return 0 成功
 * @return -1 失败
 */
PXREACLIENTSDK_EXPORT int DeviceControlJson(const char *devID,const char *parameterJson);
/**
 * @brief 向设备发送json格式指令
 * @param devID 设备sn
 * @param parameterJson 功能及参数,json格式,具体用法参考企业套件SDK文档
 * @return 0 成功
 * @return -1 失败
 */
#ifdef __cplusplus
}
#endif

#endif // PXREACLIENTSDK_H
