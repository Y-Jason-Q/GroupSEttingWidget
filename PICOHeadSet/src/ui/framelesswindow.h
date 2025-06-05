#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QWidget>
#include <Windows.h>

#include "stdafx.h"
#include "titlebar.h"

class FramelessWindow : public QWidget
{
    Q_OBJECT
public:
    explicit FramelessWindow(QWidget *parent = nullptr);
    ~FramelessWindow();

    void setTitleButtonType(TITLE_BUTTONTYPE type);
    QWidget *getCenterWidget();

    void showSettingTitle(bool show);
    void setLogoIcon(const QString &iconpath);
    void setTitleName(const QString &titleName);

signals:
    void sigMaxBtnClicked();

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    void changeEvent(QEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    bool isWindowMaximized(HWND hWnd);
    void monitorNCCALCSIZE(MSG *msg);

    TitleBar *titleBar;
    MONITORINFO monitorInfo;
    RECT monitor_rect;
    RECT work_area;
    QVBoxLayout *m_layout;

public:
    QWidget *centerWidget;

    bool m_readyForPopup = false;
};

#endif // FRAMELESSWINDOW_H
