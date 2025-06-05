#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "src/PXREAClientWrapper.h"
#include "src/ui/customvideoplayer.h"

namespace Ui {
class MainWindow;
}

class StyleWindow;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onScreenFrameReceived(const QString &devID, const uint8_t *yuvData, int width, int height);
    void onStreamFrameReceived(const QString &devID, const uint8_t *yuvData, int width, int height);

private:
    Ui::MainWindow *ui;

    PXREAClientWrapper m_client;

    CustomVideoPlayer *m_videoPlayer;

    StyleWindow *window;
};

#endif // MAINWINDOW_H
