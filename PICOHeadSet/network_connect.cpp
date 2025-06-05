#include "network_connect.h"
#include <QNetworkAddressEntry>

//#pragma comment(lib, "wlanapi.lib")
//#pragma comment(lib, "iphlpapi.lib")

bool isRunningAsAdmin()
{
    BOOL isAdmin = false;
    PSID adminGroup = nullptr;

    // 创建管理员组的SID
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    bool ret = AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup);
    if (ret) {
        CheckTokenMembership(nullptr, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    return isAdmin == TRUE;
}

bool isVirtualInterface(const QNetworkInterface &iface)
{
    static const QStringList virtualKeywords = {
        "virtual", "vmware", "loopback", "pseudo", "tunneling",
        "tap", "vpn", "hyper-v", "vbox", "teredo", "桥接",
        "vethernet", "default switch", "nat", "container"
    };

    QString name = iface.name().toLower();
    QString desc = iface.humanReadableName().toLower();
    QString mac = iface.hardwareAddress().toLower();

    for (const QString &key : virtualKeywords) {
        if (name.contains(key) || desc.contains(key) || mac.contains(key))
            return true;
    }
    // 00:00:00:00:00:00 通常为虚拟接口
    if (iface.hardwareAddress().toLower() == "00:00:00:00:00:00")
        return true;

    return false;
}

bool isUsefulInterface(const QNetworkInterface &iface)
{
    // 过滤掉回环、无MAC的接口
    if (iface.flags().testFlag(QNetworkInterface::IsLoopBack))
        return false;
    if (!(iface.type() == QNetworkInterface::Ethernet || iface.type() == QNetworkInterface::Wifi))
        return false;
    if (isVirtualInterface(iface))
        return false;
    return true;
}

QString decodeSSID(const DOT11_SSID& dot11SSID) {
    return QString::fromUtf8(
               reinterpret_cast<const char*>(dot11SSID.ucSSID),
               dot11SSID.uSSIDLength
               ).trimmed();
}

bool updateConnectionStatus(const WinNetworkInterface &stInterface)
{
//    for (const QNetworkAddressEntry &entry : stInterface.qtInterface.addressEntries()) {
//        QString ip = entry.ip().toString();
//        // 过滤掉无效IP、本地链路IP（169.254.x.x）
//        if ((entry.ip().protocol() == QAbstractSocket::IPv4Protocol ||
//             entry.ip().protocol() == QAbstractSocket::IPv6Protocol) &&
//            !ip.isEmpty() &&
//            !ip.startsWith("169.254") &&         // 本地链路自动分配
//            ip != "127.0.0.1" &&                 // 回环
//            !ip.startsWith("::1"))               // IPv6回环
//        {
//            return true;
//        }
//    }

    QString ipv4 = "";
    QString ipv6 = "";
    if (stInterface.ipv4Address.size() > 0)
        ipv4 = stInterface.ipv4Address.at(0);
    if (stInterface.ipv6Address.size() > 0)
        ipv6 = stInterface.ipv6Address.at(0);
    if ((ipv4.isEmpty() && ipv6.isEmpty()) || ipv4.startsWith("169.254") || ipv4 == "127.0.0.1" || ipv6.startsWith("::1"))
    {
        return false;
    }

    return true;
}

bool fillWifiDetails(WinNetworkInterface  &stInterface)
{
    HANDLE hClient = NULL;
    DWORD dwMaxClient = 2;  // WLAN API版本
    DWORD dwCurVersion = 0;
    DWORD dwResult = 0;
    bool success = false;

    dwResult = WlanOpenHandle(dwMaxClient, nullptr, &dwCurVersion, &hClient);
    if (dwResult != ERROR_SUCCESS) {
        return false;
    }

    PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
    dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
    if (dwResult != ERROR_SUCCESS) {
        WlanCloseHandle(hClient, NULL);
        return false;
    }

    QString ssidName = "";

    for (int i = 0; i < (int)pIfList->dwNumberOfItems; ++i) {
        PWLAN_INTERFACE_INFO pIfInfo = &pIfList->InterfaceInfo[i];

        // 转换为QT接口名称格式（GUID转为适配器名称）
//        QString winDeviceName = QString::fromWCharArray(pIfInfo->strInterfaceDescription);
//        if (winDeviceName != stInterface.internalName)
//            continue;

        // 获取连接属性
        PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = nullptr;
        DWORD connectInfoSize = sizeof(WLAN_CONNECTION_ATTRIBUTES);
        WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;

        dwResult = WlanQueryInterface(hClient,
                                      &pIfInfo->InterfaceGuid,
                                      wlan_intf_opcode_current_connection,
                                      nullptr,
                                      &connectInfoSize,
                                      (PVOID*)&pConnectInfo,
                                      &opCode);
        if (dwResult == ERROR_SUCCESS) {
            if (pConnectInfo && pConnectInfo->isState == wlan_interface_state_connected) {
                // 提取SSID
                DOT11_SSID ssid = pConnectInfo->wlanAssociationAttributes.dot11Ssid;
                QString ssidName = decodeSSID(ssid);

                // 提取信号强度
                int signalQuality = pConnectInfo->wlanAssociationAttributes.wlanSignalQuality;

                // BSSID转换
                const DOT11_MAC_ADDRESS &bssid = pConnectInfo->wlanAssociationAttributes.dot11Bssid;
                QString  strBSSid = QString("%1:%2:%3:%4:%5:%6")
                                       .arg(bssid[0], 2, 16, QLatin1Char('0'))
                                       .arg(bssid[1], 2, 16, QLatin1Char('0'))
                                       .arg(bssid[2], 2, 16, QLatin1Char('0'))
                                       .arg(bssid[3], 2, 16, QLatin1Char('0'))
                                       .arg(bssid[4], 2, 16, QLatin1Char('0'))
                                       .arg(bssid[5], 2, 16, QLatin1Char('0')).toUpper();

                // 认证类型
                QString strAuthorType = "";
                switch (pConnectInfo->wlanSecurityAttributes.dot11AuthAlgorithm) {
                case DOT11_AUTH_ALGO_80211_OPEN:
                    strAuthorType = "Open"; //开放网络(Open)";
                    break;
                case DOT11_AUTH_ALGO_80211_SHARED_KEY:
                    strAuthorType = "Shared Key"; //"WEB共享密钥(Shared Key)";
                    break;
                case DOT11_AUTH_ALGO_WPA:
                    strAuthorType = "WPA-Enterprise"; //"WPA-企业版(WPA-Enterprise)";
                    break;
                case DOT11_AUTH_ALGO_WPA_PSK:
                    strAuthorType = "WPA-Personal"; //"WPA-个人版(WPA-Personal)";
                    break;
                case DOT11_AUTH_ALGO_WPA_NONE:
                    strAuthorType = "WPA-None";
                    break;
                case DOT11_AUTH_ALGO_RSNA:
                    strAuthorType = "WPA2-Enterprise"; // "WPA2-企业版(WPA2-Enterprise)";
                    break;
                case DOT11_AUTH_ALGO_RSNA_PSK:
                    strAuthorType = "WPA2-Personal"; // "WPA2-个人版(WPA2-Personal)";
                    break;
                case DOT11_AUTH_ALGO_WPA3:
                    strAuthorType = "WPA3-Enterprise"; // "WPA3-企业版(WPA3-Enterprise)";
                    break;
                case DOT11_AUTH_ALGO_WPA3_SAE:
                    strAuthorType = "SAE"; // "WPA3-个人版(SAE)";
                    break;
                case DOT11_AUTH_ALGO_OWE:
                    strAuthorType = "OWE"; //"机会无线加密(OWE)";
                    break;
                case DOT11_AUTH_ALGO_IHV_START:
                    strAuthorType = QString("MCC(0x%1)").arg(pConnectInfo->wlanSecurityAttributes.dot11AuthAlgorithm, 0, 16);
                    break;
                case DOT11_AUTH_ALGO_IHV_END:
                    strAuthorType = QString("MCC(0x%1)").arg(pConnectInfo->wlanSecurityAttributes.dot11AuthAlgorithm, 0, 16);
                    break;
                default:
                    strAuthorType = QString("UnKnown(0x%1)").arg(pConnectInfo->wlanSecurityAttributes.dot11AuthAlgorithm, 0, 16);
                    break;
                }

                stInterface.wifiSSID = ssidName;
                stInterface.wifiSignalQuality = signalQuality;
                stInterface.wifiBSSID = strBSSid;
                stInterface.wifiAuthType = strAuthorType;

                success = true;
                WlanFreeMemory(pConnectInfo);
            }
        }
    }

    if (pIfList) {
        WlanFreeMemory(pIfList);
    }

    WlanCloseHandle(hClient, NULL);

    return success;
}

QList<WinNetworkInterface> getNetworkInterfaces()
{
    QList<WinNetworkInterface> result;

    foreach (const QNetworkInterface &qinterface, QNetworkInterface::allInterfaces()) {
        if (!isUsefulInterface(qinterface))
            continue;

        WinNetworkInterface winterface;
        winterface.qtInterface = qinterface;

        // 基础信息
        winterface.name = qinterface.name();
        winterface.internalName = qinterface.humanReadableName();
        winterface.macAddress = qinterface.hardwareAddress();
        winterface.type = qinterface.type();

        // 分离IP地址
        foreach (const QNetworkAddressEntry &entry, qinterface.addressEntries()) {
            QString ip = entry.ip().toString();
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                winterface.ipv4Address.append(ip);
            }
            else if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol) {
                winterface.ipv6Address.append(ip);
            }
        }

        winterface.isConnected = updateConnectionStatus(winterface);

        if (winterface.isConnected == false)
            continue;

        // 获取WIFI详细信息
        if (winterface.type == QNetworkInterface::Wifi) {
            if (!fillWifiDetails(winterface)) {
                qWarning() << "Failed to get WIFI details for " << winterface.name;
                continue;
            }
        }

        result.append(winterface);
    }

    return result;
}

void printNetworkInfo()
{
    QList<WinNetworkInterface> interfaces = getNetworkInterfaces();

    qDebug() << "\n======= 网络接口信息 =======";
    foreach (const WinNetworkInterface &winterface, interfaces) {
        qDebug() << "net name:" << winterface.name;
        qDebug() << "sys name:" << winterface.internalName;
        qDebug() << "mac addr:" << winterface.macAddress;
        qDebug() << "net type:" << (winterface.type == QNetworkInterface::Wifi ? "wifi" : "wlan");
        qDebug() << "link status:" << (winterface.isConnected ? "connected" : "disconnected");

        if (!winterface.ipv4Address.isEmpty()) {
            qDebug() << QStringLiteral("IPv4 Address:");
            foreach (const QString &ip, winterface.ipv4Address) {
                qDebug() << "  " << ip;
            }
        }

        if (!winterface.ipv6Address.isEmpty()) {
            qDebug() << QStringLiteral("IPv6 Address:");
            foreach (const QString &ip, winterface.ipv6Address) {
                qDebug() << "  " << ip;
            }
        }

        if (winterface.type == QNetworkInterface::Wifi) {
            qDebug() << QStringLiteral("SSID:") << winterface.wifiSSID;
            qDebug() << QStringLiteral("Signal Quality:") << winterface.wifiSignalQuality << "%";
            qDebug() << QStringLiteral("BSSID:") << winterface.wifiBSSID;
            qDebug() << QStringLiteral("cipherType:") << winterface.wifiAuthType;
        }

        qDebug() << "--------------------------";
    }
}


NetworkMonitor::NetworkMonitor()
{
    connect(&m_manger, &QNetworkConfigurationManager::configurationChanged,
            this, &NetworkMonitor::onNetworkChanged);
}

void NetworkMonitor::onNetworkChanged(const QNetworkConfiguration &config) {
    qDebug() << "\n[网络变化检测]";
    qDebug() << "变更接口:" << config.name();
    printNetworkInfo();

    emit networkChanged();
}
