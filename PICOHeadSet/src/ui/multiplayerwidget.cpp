#include "multiplayerwidget.h"
#include <QSplitter>

#define H_MARGIN 4
#define V_MARGIN 4
#define H_SPACE 4
#define V_SPACE 4

MultiPlayerWidget::MultiPlayerWidget(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
}

void MultiPlayerWidget::setupUI()
{
    widget_1 = new QWidget(this);
    widget_2 = new QWidget(this);
    widget_3 = new QWidget(this);
    widget_4 = new QWidget(this);
    widget_5 = new QWidget(this);
    widget_6 = new QWidget(this);
    widget_7 = new QWidget(this);
    widget_8 = new QWidget(this);
    widget_9 = new QWidget(this);
    widget_10 = new QWidget(this);
    widget_11 = new QWidget(this);
    widget_12 = new QWidget(this);
    widget_13 = new QWidget(this);
    widget_14 = new QWidget(this);
    widget_15 = new QWidget(this);
    widget_16 = new QWidget(this);

    m_widgetList.append(widget_1);
    m_widgetList.append(widget_2);
    m_widgetList.append(widget_3);
    m_widgetList.append(widget_4);
    m_widgetList.append(widget_5);
    m_widgetList.append(widget_6);
    m_widgetList.append(widget_7);
    m_widgetList.append(widget_8);
    m_widgetList.append(widget_9);
    m_widgetList.append(widget_10);
    m_widgetList.append(widget_11);
    m_widgetList.append(widget_12);
    m_widgetList.append(widget_13);
    m_widgetList.append(widget_14);
    m_widgetList.append(widget_15);
    m_widgetList.append(widget_16);

    m_hLayout = new QHBoxLayout(this);
}

void MultiPlayerWidget::setViewWindowLayout(bw::ScreenLayout lyt)
{
    switch(lyt) {
    case bw::ScreenLayout::SINGLE_SCREEN:
        singleLayout();
        break;
    case bw::ScreenLayout::DOUBLE_SCREEN:
        doubleLayout();
        break;
    case bw::ScreenLayout::TRIPLE_SCREEN:
        tripleLayout();
        break;
    case bw::ScreenLayout::FOUR_SCREEN:
        fourLayout();
        break;
    case bw::ScreenLayout::SIX_SCREEN:
        sixLayout();
        break;
    case bw::ScreenLayout::EIGHT_SCREEN:
        eightLayout();
        break;
    case bw::ScreenLayout::NINE_SCREEN:
        nineLayout();
        break;
    }
}

void MultiPlayerWidget::singleLayout()
{
    m_hLayout->addWidget(widget_1);
    m_hLayout->setContentsMargins(H_MARGIN, V_MARGIN, H_MARGIN, V_MARGIN);
}

void MultiPlayerWidget::doubleLayout()
{
    int width = rect().width();

    int viewWidth = width / 2;
    int viewHeight = viewWidth * 3 / 4;

    widget_1->resize(viewWidth, viewHeight);
    widget_2->resize(viewWidth, viewHeight);

    m_hLayout->addWidget(widget_1);
    m_hLayout->addWidget(widget_2);
    widget_1->setStyleSheet("QWidget{background-color: white;}");
    widget_2->setStyleSheet("QWidget{background-color: red;}");
    m_hLayout->setContentsMargins(H_MARGIN, V_MARGIN, H_MARGIN, V_MARGIN);
    m_hLayout->setSpacing(H_SPACE);
    this->setLayout(m_hLayout);
    widget_1->show();
    widget_2->show();
}

void MultiPlayerWidget::tripleLayout()
{
    // No Triple layout
}

void MultiPlayerWidget::fourLayout()
{

}

void MultiPlayerWidget::sixLayout()
{

}

void MultiPlayerWidget::eightLayout()
{

}

void MultiPlayerWidget::nineLayout()
{

}
