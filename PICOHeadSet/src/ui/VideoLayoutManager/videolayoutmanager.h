#ifndef VIDEOLAYOUTMANAGER_H
#define VIDEOLAYOUTMANAGER_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include "videoitemwrapper.h"

class VideoLayoutManager : public QObject
{
    Q_OBJECT
public:
    enum LayoutMode {
        Layout1 = 1,
        Layout2 = 2,
        Layout4 = 4,
        Layout6 = 6,
        Layout8 = 8,
        Layout9 = 9,
    };
    Q_ENUM(LayoutMode)

    explicit VideoLayoutManager(QGraphicsScene* scene, QObject *parent = nullptr);

    void setLayout(LayoutMode mode);
    void addVideoItem(VideoItemWrapper* item);
    void cycleVideoSources();
    void setContainerSize(const QSizeF &size);

    QList<VideoItemWrapper*> items() const { return m_items; }
    QGraphicsScene * scene() const { return m_scene; }

public slots:
    void updateLayout();

signals:
    void layoutUpdated();

private:
    void clearLayout();

    void createGridLayout(int rows, int cols);
    void createSpecial8Layout();
    void maintainAspectRatios();

private:
    QGraphicsScene *m_scene;
    QList<VideoItemWrapper*> m_items;
    QSizeF m_containerSize;
    LayoutMode m_currentLayout = Layout1;

    QGraphicsWidget *m_container = nullptr;

    const float ASPECT_RATIO = 4.0f / 3.0f;
    const int SPACING = 5;
    int m_currentCycleIndex = 0;
};

#endif // VIDEOLAYOUTMANAGER_H
