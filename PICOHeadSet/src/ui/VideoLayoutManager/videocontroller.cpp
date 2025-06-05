#include "videocontroller.h"
#include "videoitemwrapper.h"
#include "videolayoutmanager.h"

VideoController::VideoController(VideoLayoutManager *layoutManager, QObject *parent)
    : QObject{parent}
    , m_layoutManager(layoutManager)
{

}

void VideoController::handleFullScreenRequest(VideoItemWrapper *item)
{
    if (m_fullscreenItem) {
        m_fullscreenItem = nullptr;
        m_layoutManager->setContainerSize(m_normalSize);
        m_layoutManager->setLayout(VideoLayoutManager::Layout4); // 恢复默认4窗口布局
    }
    else {
        m_fullscreenItem = item;
        m_layoutManager->setContainerSize(m_fullscreenSize);
        m_layoutManager->setLayout(VideoLayoutManager::Layout1);
        m_layoutManager->addVideoItem(item);
    }
    m_layoutManager->updateLayout();
}

void VideoController::handleSwitchRequest()
{
    // 发送轮训
    m_layoutManager->cycleVideoSources();
    m_layoutManager->updateLayout();
}

void VideoController::changeLayout(int layoutMode)
{
    if (m_fullscreenItem) {
        m_fullscreenItem = nullptr;
    }
    auto mode = static_cast<VideoLayoutManager::LayoutMode>(layoutMode);
    m_layoutManager->setLayout(mode);
//    m_layoutManager->updateLayout();
}
