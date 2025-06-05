#include "displaywidget.h"
#include <QGridLayout>
#include <QDebug>
//#include "blankwidget.h"
#include "../yuvplayer.h"

#define GRID_SPACING    4

DisplayWidget::DisplayWidget(QWidget *parent)
    : QWidget(parent)
{
//    initialize();
}

void DisplayWidget::initialize()
{
    m_layout = new QGridLayout(this);

    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(GRID_SPACING);
    this->setLayout(m_layout);
    this->setStyleSheet("background-color: #141414;");
}

void DisplayWidget::setWidgetList(QList<YuvPlayer*> widgetlist)
{
    m_stWidgetInfoList.clear();
    for (auto *widget : widgetlist) {
        WidgetInfo info;
        info.widget = widget;
        info.devID = widget->titleName();
        info.isBlank = true;
        m_stWidgetInfoList.append(info);
        connect(widget, &YuvPlayer::linkButtonClicked, this, &DisplayWidget::onUpdateLinkStatus);
        connect(widget, &YuvPlayer::maxButtonClicked, this, &DisplayWidget::onFullScreen);
    }
}

void DisplayWidget::onFullScreen(const QString &id, bool isLinked, bool isFull)
{
    emit sigFullScreen(id, isLinked, isFull);
}

bool DisplayWidget::containsID(const QString& targetID) {
    return std::any_of(m_stWidgetInfoList.begin(), m_stWidgetInfoList.end(),
                       [&](const WidgetInfo& info) {
                           return info.devID == targetID;
                       });
}

void DisplayWidget::addWidget(const QString &id, bool isBlank)
{
    for (auto &info : m_stWidgetInfoList)
    {
        if (info.devID == id) {
            // 已激活但断开，重新连接
//            if (info.isBlank == false && !info.widget->isLinked()) {
                info.widget->updateLinkStatus(true); // 这会内部将m_isLinked置为true
                // 可选：你也可以在WidgetInfo加一个isLinked字段同步
//            }
            return;
        }
    }
    // 否则找第一个空白窗口激活
    for (auto &info : m_stWidgetInfoList) {
        if (info.isBlank) {
            info.isBlank = false;
            info.devID = id;
            info.widget->setTitleName(id);
            info.widget->updateLinkStatus(true);
            return;
        }
    }
}

void DisplayWidget::onUpdateLinkStatus(const QString &devID, bool isLinked)
{
//    if (!isLinked) {
//        emit disconnectRequest(devID);
//    } else {
//        emit connectRequest(devID);
//    }

    for (auto &info : m_stWidgetInfoList) {
        if (info.devID == devID) {
            info.widget->updateLinkStatus(isLinked);
            info.isBlank = !isLinked;
            break;
        }
    }

    if (!isLinked) {
        emit disconnectRequest(devID);
    } else {
        emit connectRequest(devID);
    }
}

void DisplayWidget::updateFrame(const QString &devID, const uint8_t *yuvData, int width, int height)
{
    for (auto &info : m_stWidgetInfoList) {

        if (info.devID == devID) {
            if (!info.widget->isLinked())
                return;
            info.widget->updateYuvFrame(yuvData, width, height);
            info.widget->updateLinkStatus(true);
            return;
        }
    }
}

YuvPlayer *DisplayWidget::yuvPlayer(const QString &devID)
{
    for (auto &info : m_stWidgetInfoList) {
        if (info.devID == devID) {
            return info.widget;
        }
    }
    return nullptr;
}

void DisplayWidget::clickedLinkButton(const QString &devID, RotatingButton::State state)
{
    auto widget = yuvPlayer(devID);
    if (!widget) return;

    emit widget->linkButtonClicked(devID, !widget->isLinked());
}

void DisplayWidget::updateLayout(int layoutType)
{
    clearAll();
    int n = std::min(layoutType, m_stWidgetInfoList.size());
    switch(layoutType)
    {
    case 1: layout1(n);  break;
    case 2: layout2(n);  break;
    case 4: layout4(n);  break;
    case 6: layout6(n);  break;
    case 8: layout8(n);  break;
    case 9: layout9(n);  break;
    default: break;
    }
//    update();
}

void DisplayWidget::layout1(int n)
{
    if (m_stWidgetInfoList.empty()) {
        fillBlankWidget(1);
    }
    else {
        auto &info = m_stWidgetInfoList[0];
        m_layout->addWidget(info.widget, 0, 0, 1, 1);
        info.widget->show();
        if (info.isBlank)
            info.widget->updateLinkStatus(false);
    }

}

void DisplayWidget::layout2(int n)
{
    if (m_stWidgetInfoList.empty() || m_stWidgetInfoList.size() < 2) {
        fillBlankWidget(2);
    }
    else {
        for (int i = 0; i < 2 && i < m_stWidgetInfoList.size(); ++i) {
            auto &info = m_stWidgetInfoList[i];
            m_layout->addWidget(info.widget, 0, i, 1, 1);
            info.widget->show();
            if (info.isBlank)
                info.widget->updateLinkStatus(false);
        }
    }
}

void DisplayWidget::layout4(int n)
{
    int pos[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    for (int i = 0; i < n; ++i) {
        auto &info = m_stWidgetInfoList[i];
        m_layout->addWidget(info.widget, pos[i][0], pos[i][1], 1, 1);
        info.widget->show();
        if (info.isBlank)
            info.widget->updateLinkStatus(false);
    }
}

void DisplayWidget::layout6(int n)
{
    int pos[6][2] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}};
    for (int i = 0; i < n; ++i) {
        auto &info = m_stWidgetInfoList.at(i);
        m_layout->addWidget(info.widget, pos[i][0], pos[i][1], 1, 1);
        info.widget->show();
        if (info.isBlank)
            info.widget->updateLinkStatus(false);
    }
}

void DisplayWidget::layout8(int n)
{
    for (int i = 0; i < n; ++i) {
        auto &info = m_stWidgetInfoList.at(i);

        int row = 0;
        int col = 0;
        int rowspan = 1;
        int colspan = 1;
        if (i == 0) {
            rowspan = 3;
            colspan = 3;
        }
        switch (i) {
        case 0: row = 0; col = 0; break;
        case 1: row = 0; col = 3; break;
        case 2: row = 1; col = 3; break;
        case 3: row = 2; col = 3; break;
        case 4: row = 3; col = 0; break;
        case 5: row = 3; col = 1; break;
        case 6: row = 3; col = 2; break;
        case 7: row = 3; col = 3; break;
        }
        m_layout->addWidget(info.widget, row, col, rowspan, colspan);
        info.widget->show();

        if (info.isBlank)
            info.widget->updateLinkStatus(false);
    }
}

void DisplayWidget::layout9(int n)
{
    int idx = 0;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col){
            if (idx >= m_stWidgetInfoList.size()) return;
            auto &info = m_stWidgetInfoList[idx];
            m_layout->addWidget(info.widget, row, col, 1, 1);
            info.widget->show();
            if (info.isBlank)
                info.widget->updateLinkStatus(false);
            ++idx;
        }
    }
}

void DisplayWidget::clearAll()
{
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != nullptr) {
        if (child->widget())
            child->widget()->hide();
//        m_layout->removeWidget(child->widget());
//        delete child;
    }
}

void DisplayWidget::fillBlankWidget(int layoutType)
{
    int count2 = m_stWidgetInfoList.size();
    for (int i = count2; i < layoutType; ++i)
    {
        YuvPlayer *widget = new YuvPlayer;
        widget->setTitleName(tr("PICO %1").arg(i + 1));
        widget->updateLinkStatus(false);
        addWidget(widget->titleName(), true);
    }
}
