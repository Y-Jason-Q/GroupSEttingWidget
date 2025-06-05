#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include <QObject>
#include <QSize>

class VideoLayoutManager;
class VideoItemWrapper;

class VideoController : public QObject
{
    Q_OBJECT
public:
    explicit VideoController(VideoLayoutManager *layoutManager, QObject *parent = nullptr);

signals:

public slots:
    void handleFullScreenRequest(VideoItemWrapper *item);
    void handleSwitchRequest();
    void changeLayout(int layoutMode);

private:
    VideoLayoutManager *m_layoutManager;
    VideoItemWrapper *m_fullscreenItem = nullptr;

    QSizeF m_normalSize = QSizeF(800, 600);
    QSizeF m_fullscreenSize = QSizeF(1920, 1080);

};

#endif // VIDEOCONTROLLER_H
