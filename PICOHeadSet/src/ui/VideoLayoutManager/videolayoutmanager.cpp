#include "videolayoutmanager.h"
#include <QGraphicsWidget>
#include <QGraphicsGridLayout>
#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <QtMath>
#include <QDebug>

VideoLayoutManager::VideoLayoutManager(QGraphicsScene *scene, QObject *parent)
    : QObject(parent)
    , m_scene(scene)
{
    m_containerSize = QSizeF(800, 600);

    m_container = new QGraphicsWidget();
    m_scene->addItem(m_container);
}

void VideoLayoutManager::setLayout(LayoutMode mode)
{
    m_currentLayout = mode;
    updateLayout();
}

void VideoLayoutManager::addVideoItem(VideoItemWrapper *item)
{
    qDebug() << "[VideoLayoutManager] addVideoItem";
    if (!m_items.contains(item)) {
        m_items.append(item);
    }
    updateLayout();
}

void VideoLayoutManager::updateLayout()
{
    clearLayout();

    switch (m_currentLayout) {
    case Layout1:   createGridLayout(1, 1);  break;
    case Layout2:   createGridLayout(1, 2);  break;
    case Layout4:   createGridLayout(2, 2);  break;
    case Layout6:   createGridLayout(2, 3);  break;
    case Layout8:   createSpecial8Layout();  break;
    case Layout9:   createGridLayout(3, 3);  break;
    }

    maintainAspectRatios();
    emit layoutUpdated();
}

void VideoLayoutManager::createGridLayout(int rows, int cols)
{
    if (!m_container)
        return;

    auto *layout = new QGraphicsGridLayout();

    double itemWidth = (m_containerSize.width() - (cols - 1) * SPACING) / cols;
    double itemHeight = itemWidth / ASPECT_RATIO;

    int count = 0;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (count >= m_items.size())
                break;

            VideoItemWrapper *item = m_items[count];
            item->setMinimumSize(itemWidth, itemHeight);
            item->setPreferredSize(itemWidth, itemHeight);
            item->setVisible(true);
            item->setParentItem(m_container);
            layout->addItem(item, i, j);
            ++count;
        }
    }

    m_container->setLayout(layout);
    m_container->setPos(0, 0);
    m_container->setMinimumSize(m_containerSize);
    m_container->setPreferredSize(m_containerSize);
}

void VideoLayoutManager::createSpecial8Layout()
{
    if (!m_container)
        return;
    if (m_items.size() < 8)
        return;

    // 计算格局
    const double totalWidth = m_containerSize.width();
    const double totalHeight = m_containerSize.height();

    // 主窗口宽高
    double mainW = totalWidth * 0.75;
    double mainH = totalHeight * 0.75;

    // 右侧竖排窗口
    double sideW = totalWidth - mainW;
    double sideH = mainH / 3;

    // 底部窗口
    double bottomH = totalHeight - mainH;
    double bottomW = totalWidth / 4.0;

    // 1号主窗口
    VideoItemWrapper *item1 = m_items[0];
    item1->setGeometry(QRectF(0, 0, mainW, mainH));
    item1->setVisible(true);
    item1->setParentItem(m_container);

    // 2/3/4号右侧竖排
    for (int i = 0; i < 3; ++i) {
        VideoItemWrapper *item = m_items[1 + i];
        item->setGeometry(QRectF(mainW, i * sideH, sideW, sideH));
        item->setVisible(true);
        item->setParentItem(m_container);
    }

    // 5/6/7/8号底部
    for (int i = 0; i < 4; ++i) {
        VideoItemWrapper *item = m_items[4 + i];
        item->setGeometry(QRectF(i * bottomW, mainH, bottomW, bottomH));
        item->setVisible(true);
        item->setParentItem(m_container);
    }

    // 隐藏多余窗口
    for (int i = 8; i < m_items.size(); ++i) {
        m_items[i]->setVisible(false);
    }

    m_container->setMinimumSize(m_containerSize);
    m_container->setPreferredSize(m_containerSize);
}

void VideoLayoutManager::cycleVideoSources()
{
    if (m_items.isEmpty())
        return;

    m_currentCycleIndex = (m_currentCycleIndex + 1) % m_items.size();

    // 获取当前可见项最大索引
    int maxVisibleIndex = 0;
    switch (m_currentLayout) {
    case Layout1: maxVisibleIndex = 0; break;
    case Layout2: maxVisibleIndex = 1; break;
    case Layout4: maxVisibleIndex = 3; break;
    case Layout6: maxVisibleIndex = 5; break;
    case Layout8: maxVisibleIndex = 7; break;
    case Layout9: maxVisibleIndex = 8; break;
    }

    if (m_currentCycleIndex > maxVisibleIndex) {
        std::swap(m_items[m_currentCycleIndex], m_items[maxVisibleIndex]);
        updateLayout();
    }
}

void VideoLayoutManager::setContainerSize(const QSizeF &size)
{
    m_containerSize = size;
    m_containerSize.setHeight(size.width() / ASPECT_RATIO);
    updateLayout();
    emit layoutUpdated();
}

void VideoLayoutManager::maintainAspectRatios()
{
    foreach (auto item, m_items)
    {
        QRectF rect = item->geometry();
        double targetHeight = rect.width() / ASPECT_RATIO;
        item->setMinimumHeight(targetHeight);
    }
}

void VideoLayoutManager::clearLayout()
{
    if (m_container && m_container->layout())
    {
        m_container->setLayout(nullptr);
    }

    foreach (auto item, m_scene->items())
    {
        item->setVisible(false);
        item->setParentItem(nullptr);
    }
}




































