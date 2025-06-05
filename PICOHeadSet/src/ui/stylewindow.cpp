#include "stylewindow.h"
#include <QHBoxLayout>

StyleWindow::StyleWindow(QWidget *parent)
    : FramelessWindow(parent)
{
    resize(800, 600);

//    m_videoPlayer = new CustomVideoPlayer;

    QHBoxLayout *layout = new QHBoxLayout;
//    layout->addWidget(m_videoPlayer);
    setLayout(layout);
}
