#ifndef STYLEWINDOW_H
#define STYLEWINDOW_H

#include <QWidget>
#include "framelesswindow.h"
#include "customvideoplayer.h"

class StyleWindow : public FramelessWindow
{
    Q_OBJECT
public:
    StyleWindow(QWidget *parent = nullptr);

private:
//    CustomVideoPlayer *m_videoPlayer;
};

#endif // STYLEWINDOW_H
