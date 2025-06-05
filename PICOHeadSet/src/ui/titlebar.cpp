#include "titlebar.h"
#include <Windows.h>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

TitleBar::TitleBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(TITLE_HEIGHT);
    setAttribute(Qt::WA_StyledBackground);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(24);
    layout->setContentsMargins(0, 0, 24, 0);

    m_logoWidget = new QWidget(this);
    m_logoLabel = new QLabel(this);
    m_logoLabel->setObjectName("logoLabel");

    m_titleLabel = new QLabel(this);
    m_titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QHBoxLayout *titleLayout = new QHBoxLayout(m_logoWidget);
    titleLayout->setContentsMargins(20, 0, 0, 0);
    titleLayout->setSpacing(16);
    titleLayout->addWidget(m_logoLabel);
    titleLayout->addWidget(m_titleLabel);
    titleLayout->addStretch();
    m_logoWidget->setLayout(titleLayout);

    layout->addWidget(m_logoWidget);
    layout->addStretch();

    minBtn = new QToolButton(this);
    maxBtn = new QToolButton(this);
    clsBtn = new QToolButton(this);
    minBtn->setFixedSize(BUTTON_WIDTH, BUTTON_WIDTH);
    maxBtn->setFixedSize(BUTTON_WIDTH, BUTTON_WIDTH);
    clsBtn->setFixedSize(BUTTON_WIDTH, BUTTON_WIDTH);


    minBtn->setObjectName("minButton");
    layout->addWidget(minBtn);

    maxBtn->setObjectName("maxButton");
    layout->addWidget(maxBtn);

    clsBtn->setObjectName("closeButton");
    layout->addWidget(clsBtn);

    QPixmap minPixmap(":/images/min.png");
    minBtn->setIcon(minPixmap);

    QPixmap maxPixmap(":/images/max.png");
    maxBtn->setIcon(maxPixmap);

    QPixmap clsPixmap(":/images/close.png");
    clsBtn->setIcon(clsPixmap);

    minBtn->setStyleSheet("QToolButton{ background-color: transparent; border: none; } QToolButton:hover{ background-color: #282828; }");
    maxBtn->setStyleSheet("QToolButton{ background-color: transparent; border: none; } QToolButton:hover{ background-color: #282828; }");
    clsBtn->setStyleSheet("QToolButton{ background-color: transparent; border: none; } QToolButton:hover{ background-color: red;}");

    this->setStyleSheet("background: #1E1E1E;");
    m_logoWidget->setStyleSheet("background: #141414;");
    m_logoLabel->setStyleSheet("background: transparent; ");
    m_titleLabel->setStyleSheet("background: transparent; color: #FFFFFF; font-size: 18px; font-famliy: Microsoft YaHei-Bold; font-weight: bold;");

    connect(minBtn, &QToolButton::clicked, this->window(), &QWidget::showMinimized);
    connect(maxBtn, &QToolButton::clicked, this, &TitleBar::toggleMaxState);
    connect(clsBtn, &QToolButton::clicked, this->window(), &QWidget::close);

    showTitleName(false);
}

TitleBar::~TitleBar()
{

}

void TitleBar::setButtonType(TITLE_BUTTONTYPE type)
{
    switch (type) {
    case ONLY_CLOSE:
    {
        minBtn->setVisible(false);
        maxBtn->setVisible(false);
        clsBtn->setVisible(true);
        break;
    }
    case MIN_CLOSE:
    {
        minBtn->setVisible(true);
        maxBtn->setVisible(false);
        clsBtn->setVisible(true);
        break;
    }
    case ALL:
    {
        minBtn->setVisible(true);
        maxBtn->setVisible(true);
        clsBtn->setVisible(true);
        break;
    }
    default:
        minBtn->setVisible(false);
        maxBtn->setVisible(false);
        clsBtn->setVisible(true);
        break;
    }
}

void TitleBar::setIcon(const QString &iconpath)
{
    QPixmap pixmap(iconpath);
    m_logoLabel->setPixmap(QPixmap(iconpath).scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void TitleBar::setTitleName(const QString &titleName)
{
    m_titleLabel->setText(titleName);
}

void TitleBar::showTitleName(bool isShow)
{
    if (isShow) {
        m_logoWidget->show();
    }
    else {
        m_logoWidget->hide();
    }
}

void TitleBar::slotMaxBtnClicked()
{
    toggleMaxState();
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    toggleMaxState();
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    ReleaseCapture();
    SendMessage((HWND)window()->winId(), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
    event->ignore();
}

void TitleBar::toggleMaxState()
{
    if (window()->isMaximized()) {
        window()->showNormal();
    }
    else {
        window()->showMaximized();
    }
}

void TitleBar::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_logoWidget->setFixedWidth(this->width() / 5);
}
