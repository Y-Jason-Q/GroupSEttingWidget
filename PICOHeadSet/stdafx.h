#ifndef STDAFX_H
#define STDAFX_H

#include <QtWidgets>
#include <WinUser.h>

#define BORDER_WIDTH 5
#define TITLE_HEIGHT 60
#define BUTTON_WIDTH 32

enum TITLE_BUTTONTYPE {
    Unknow = -1,
    ONLY_CLOSE,
    MIN_CLOSE,
    ALL,
};

#define DEFAULT_STYLE ":/StyleWindow/Resources/default.qss"

extern void RefreshStyle(QWidget* widget, const char* name, QVariant value);
extern void LoadStyle(QString filePath);

#endif // STDAFX_H
