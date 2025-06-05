#include "framelesswindow.h"

#include <QMouseEvent>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QWindow>
#include "windoweffect.h"
#include "../../stdafx.h"

#pragma comment(lib, "user32.lib")

FramelessWindow::FramelessWindow(QWidget *parent)
    : QWidget(parent)
    , m_readyForPopup(false)
{
    memset(&monitorInfo, 0, sizeof(MONITORINFO));
    monitorInfo.cbSize = sizeof(MONITORINFO);

    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0, 0, 0, 0);

    titleBar = new TitleBar(this);
    titleBar->setObjectName("titleBar");
    m_layout->addWidget(titleBar);
    
    centerWidget = new QWidget;
    centerWidget->setStyleSheet("background: #1E1E1E;");
    m_layout->addWidget(centerWidget);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    WindowEffect::addWindowAnimation((HWND)winId());
    WindowEffect::addShadowEffect((HWND)winId());

    connect(windowHandle(), &QWindow::screenChanged, this, [=] {
        auto hWnd = reinterpret_cast<HWND>(windowHandle()->winId());
        SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
    });

    connect(this, &FramelessWindow::sigMaxBtnClicked, titleBar, &TitleBar::slotMaxBtnClicked);
}

FramelessWindow::~FramelessWindow()
{
}

QWidget* FramelessWindow::getCenterWidget()
{
    return centerWidget;
}

void FramelessWindow::setTitleButtonType(TITLE_BUTTONTYPE type)
{
    titleBar->setButtonType(type);
}

void FramelessWindow::setLogoIcon(const QString &iconpath)
{
    titleBar->setIcon(iconpath);
}

void FramelessWindow::setTitleName(const QString &titleName)
{
    titleBar->setTitleName(titleName);
}

void FramelessWindow::showSettingTitle(bool isShow)
{
    titleBar->showTitleName(isShow);
}

bool FramelessWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *msg = reinterpret_cast<MSG*>(message);
    if (msg->message == WM_NCHITTEST)
    {
        int xPos = (LOWORD(msg->lParam) - frameGeometry().x() ) %  65535;
        int yPos = HIWORD(msg->lParam) - frameGeometry().y();

        int w = width();
        int h = height();
        bool lx = xPos < BORDER_WIDTH;
        bool rx = xPos > w - BORDER_WIDTH;
        bool ty = yPos < BORDER_WIDTH;
        bool by = yPos > h - BORDER_WIDTH;

        if (lx && ty) {
            *result = HTTOPLEFT;
            return true;
        }
        else if (rx && by) {
            *result = HTBOTTOMRIGHT;
            return true;
        }
        else if (rx && ty) {
            *result = HTTOPRIGHT;
            return true;
        }
        else if (lx && by) {
            *result = HTBOTTOMLEFT;
            return true;
        }
        else if (ty) {
            *result = HTTOP;
            return true;
        }
        else if (by) {
            *result = HTBOTTOM;
            return true;
        }
        else if (lx) {
            *result = HTLEFT;
            return true;
        }
        else if (rx) {
            *result = HTRIGHT;
            return true;
        }
    }
    else if (msg->message == WM_NCCALCSIZE)
    {
        if (isWindowMaximized(msg->hwnd)) {
            monitorNCCALCSIZE(msg);
        }
        *result = HTNOWHERE;
        return true;
    }
    else if (msg->message == WM_GETMINMAXINFO)
    {
        if (isWindowMaximized(msg->hwnd)) {
            RECT window_rect;
            if (!GetWindowRect(msg->hwnd, &window_rect)) {
                *result = HTNOWHERE;
                return false;
            }

            HMONITOR hMonitor = MonitorFromRect(&window_rect, MONITOR_DEFAULTTONULL);
            if (!hMonitor) {
                *result = HTNOWHERE;
                return false;
            }

            GetMonitorInfoW(hMonitor, &monitorInfo);
            RECT monitor_rect = monitorInfo.rcMonitor;
            RECT work_area = monitorInfo.rcWork;

            LPMINMAXINFO info = reinterpret_cast<LPMINMAXINFO>(msg->lParam);

            info->ptMaxSize.x = work_area.right - work_area.left;
            info->ptMaxSize.y = work_area.bottom - work_area.top;
            info->ptMaxTrackSize.x = info->ptMaxSize.x;
            info->ptMaxTrackSize.y = info->ptMaxSize.y;

            info->ptMaxPosition.x = abs(window_rect.left - monitor_rect.left);
            info->ptMaxPosition.y = abs(window_rect.top - monitor_rect.top);

            *result = HTCLIENT;
            return true;
        }
    }

    return QWidget::nativeEvent(eventType, message, result);
}

void FramelessWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {

    }

    return QWidget::changeEvent(event);
}

void FramelessWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    m_readyForPopup = true;
}

void FramelessWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_readyForPopup = true;
}

bool FramelessWindow::isWindowMaximized(HWND hWnd)
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    if (!GetWindowPlacement(hWnd, &wp))
    {

    }

    return wp.showCmd == SW_MAXIMIZE;
}

void FramelessWindow::monitorNCCALCSIZE(MSG *msg)
{
    HMONITOR hMonitor = MonitorFromWindow(msg->hwnd, MONITOR_DEFAULTTONULL);
    if (hMonitor != NULL) {
        GetMonitorInfoW(hMonitor, &monitorInfo);
    }

    NCCALCSIZE_PARAMS *params = reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
    params->rgrc[0].left = monitorInfo.rcWork.left;
    params->rgrc[0].top = monitorInfo.rcWork.top;
    params->rgrc[0].right = monitorInfo.rcWork.right;
    params->rgrc[0].bottom = monitorInfo.rcWork.bottom;
}
