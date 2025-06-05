#ifndef CUSTOMVIDEOPLAYER_H
#define CUSTOMVIDEOPLAYER_H

#include <QObject>
#include <QWidget>
#include <QRect>
#include <QPaintEvent>
#include <QOpenGLWidget>
#include "yuvplayer.h"
#include "framelesswindow.h"

class CustomVideoPlayer : public /*QOpenGLWidget */QWidget
{
    Q_OBJECT
public:
    explicit CustomVideoPlayer(QWidget *parent = nullptr);
    ~CustomVideoPlayer() {}

    void setTitleName(const QString &titleName);

    YuvPlayer* yuvPlayer() const {return m_yuvPlayerWidget;}

signals:
    void linkButtonClicked();
    void switchRequested();
    void fullscreenRequested();

    void updateFrame(const uint8_t *yuvData, int width, int height);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void setupUI();
    void handleButtonEvents(QMouseEvent *event);

private:
    QString m_titleText;

    FramelessWindow *m_framelessWindow;
    YuvPlayer *m_yuvPlayerWidget;
};

#endif // CUSTOMVIDEOPLAYER_H
