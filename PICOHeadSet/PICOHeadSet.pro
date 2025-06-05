#-------------------------------------------------
#
# Project created by QtCreator 2025-04-29T09:47:51
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CODECFORSRC = UTF-8

TARGET = PICOHeadSet
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        network_connect.cpp \
        src/PXREAClientWrapper.cpp \
        src/ui/DeviceListWidget/devicedelegate.cpp \
        src/ui/DeviceListWidget/deviceitem.cpp \
        src/ui/DeviceListWidget/devicelistwidget.cpp \
        src/ui/DeviceListWidget/devicemodel.cpp \
        src/ui/DeviceListWidget/rotatingbutton.cpp \
        src/ui/SettingPage/datamanager.cpp \
        src/ui/SettingPage/groupcontentwidget.cpp \
        src/ui/SettingPage/groupitemdelegate.cpp \
        src/ui/SettingPage/grouplistmodel.cpp \
        src/ui/SettingPage/grouplistwidget.cpp \
        src/ui/SettingPage/groupnavdelegate.cpp \
        src/ui/SettingPage/groupnavmodel.cpp \
        src/ui/SettingPage/groupnavwidget.cpp \
        src/ui/SettingPage/groupsettingwidget.cpp \
        src/ui/VideoLayoutManager/blankwidget.cpp \
        src/ui/VideoLayoutManager/displaywidget.cpp \
        src/ui/VideoLayoutManager/mainwidget.cpp \
        src/ui/VideoLayoutManager/videocontroller.cpp \
        src/ui/VideoLayoutManager/videoitemwrapper.cpp \
        src/ui/VideoLayoutManager/videolayoutmanager.cpp \
        src/ui/bottombar.cpp \
        src/ui/customvideoplayer.cpp \
        src/ui/devicelinkwidget.cpp \
        src/ui/devicelistpopwindow.cpp \
        src/ui/dotwidget.cpp \
        src/ui/framelesswindow.cpp \
        src/ui/initpagewidget.cpp \
        src/ui/layoutchoosewidget.cpp \
        src/ui/multiplayerwidget.cpp \
        src/ui/networkdisplaywidget.cpp \
        src/ui/stylewindow.cpp \
        src/ui/titlebar.cpp \
        src/ui/topwidget.cpp \
        src/ui/windoweffect.cpp \
        src/ui/yuvplayer.cpp \
        stdafx.cpp

HEADERS  += mainwindow.h \
        common.h \
        myinclude.h \
        network_connect.h \
        src/PXREAClientWrapper.h \
        src/ui/DeviceListWidget/devicedelegate.h \
        src/ui/DeviceListWidget/deviceitem.h \
        src/ui/DeviceListWidget/devicelistwidget.h \
        src/ui/DeviceListWidget/devicemodel.h \
        src/ui/DeviceListWidget/rotatingbutton.h \
        src/ui/SettingPage/datamanager.h \
        src/ui/SettingPage/groupcontentwidget.h \
        src/ui/SettingPage/groupitemdelegate.h \
        src/ui/SettingPage/grouplistmodel.h \
        src/ui/SettingPage/grouplistwidget.h \
        src/ui/SettingPage/groupnavdelegate.h \
        src/ui/SettingPage/groupnavmodel.h \
        src/ui/SettingPage/groupnavwidget.h \
        src/ui/SettingPage/groupsettingwidget.h \
        src/ui/VideoLayoutManager/blankwidget.h \
        src/ui/VideoLayoutManager/displaywidget.h \
        src/ui/VideoLayoutManager/mainwidget.h \
        src/ui/VideoLayoutManager/videocontroller.h \
        src/ui/VideoLayoutManager/videoitemwrapper.h \
        src/ui/VideoLayoutManager/videolayoutmanager.h \
        src/ui/bottombar.h \
        src/ui/customvideoplayer.h \
        src/ui/devicelinkwidget.h \
        src/ui/devicelistpopwindow.h \
        src/ui/dotwidget.h \
        src/ui/framelesswindow.h \
        src/ui/initpagewidget.h \
        src/ui/layoutchoosewidget.h \
        src/ui/multiplayerwidget.h \
        src/ui/networkdisplaywidget.h \
        src/ui/stylewindow.h \
        src/ui/titlebar.h \
        src/ui/topwidget.h \
        src/ui/windoweffect.h \
        src/ui/yuvplayer.h \
        stdafx.h

FORMS    += mainwindow.ui


# PICO SDK
INCLUDEPATH += $$PWD/SDK/
DEPENDPATH += $$PWD/SDK

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/SDK/ -lPXREAClientSDK
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/SDK/ -lPXREAClientSDK


# libyuv
INCLUDEPATH += $$PWD/3rdparty/libyuv/include
DEPENDPATH += $$PWD/3rdparty/libyuv/lib/x64

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/3rdparty/libyuv/lib/x64/ -lyuv
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/3rdparty/libyuv/lib/x64/ -lyuv


LIBS += -lDwmapi -ladvapi32 -lshell32 -lwlanapi -liphlpapi
#LIBS += -lwlanapi

RESOURCES += \
    Resource.qrc

RC_FILE = myicon.rc

