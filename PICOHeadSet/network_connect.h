#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include <QCoreApplication>
#include <QNetworkInterface>
#include <QNetworkConfigurationManager>
#include <QTimer>
#include <QDebug>
#include <QVector>
#include <QString>
#include <QMap>

#ifdef _WIN32
#include <windows.h>
#include <iphlpapi.h>
#include <wlanapi.h>
#include <sddl.h>
#include <shellapi.h>
//#pragma comment(lib, "wlanapi.lib")
#else
#endif

struct WinNetworkInterface
{
    QString name;               // 接口名称
    QString internalName;       // 系统内部名称
    QString macAddress;         // MAC地址
    QNetworkInterface::InterfaceType type;  // 类型（Wifi/Ethernet等）
    bool isConnected;           // 是否已连接

    QList<QString> ipv4Address; // IPv4地址列表
    QList<QString> ipv6Address; // IPv6地址列表

    QString wifiSSID;           // 连接的WiFi名称
    int wifiSignalQuality;      // 信号强度(0-100)
    QString wifiBSSID;          // 路由器MAC地址
    QString wifiAuthType;       // 加密类型(如WPA2)

    QNetworkInterface qtInterface;
};

// 检查管理员权限
bool isRunningAsAdmin();

bool isVirtualInterface(const QNetworkInterface &iface);

//过滤掉无用接口（如回环、虚拟网卡等）
bool isUsefulInterface(const QNetworkInterface &iface);

// 辅助函数：将 Windows API 的 DOT11_SSID 转换为 QString
QString decodeSSID(const DOT11_SSID& dot11SSID);

bool updateConnectionStatus(const WinNetworkInterface &stInterface);

// 获取无线网络详细信息
bool fillWifiDetails(WinNetworkInterface  &stInterface);

// 获取所有网络接口信息
QList<WinNetworkInterface> getNetworkInterfaces();

// 打印网络信息
void printNetworkInfo();


// 网络监听
class NetworkMonitor : public QObject
{
    Q_OBJECT
public:
    NetworkMonitor();

signals:
    void networkChanged();

private slots:
    void onNetworkChanged(const QNetworkConfiguration &config);

public:
    QNetworkConfigurationManager m_manger;
};

#endif // NETWORK_CONNECT_H
