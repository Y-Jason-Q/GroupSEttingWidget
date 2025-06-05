#include "layoutchoosewidget.h"
#include <QGridLayout>
#include <QButtonGroup>
#include <QFrame>
#include <QPainter>
#include <QScreen>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QTimer>
#include <QDebug>

LayoutChooseWidget::LayoutChooseWidget(QWidget *parent)
    : QWidget(parent, Qt::Popup)
{
    initUI();
}

void LayoutChooseWidget::onButtonClicked(int id)
{
    int type = 1;
    switch (id) {
    case 0: type = 1; break;
    case 1: type = 2; break;
    case 2: type = 4; break;
    case 3: type = 6; break;
    case 4: type = 8; break;
    case 5: type = 9; break;
    }

    qDebug() << "[LayoutChooseWidget::onButtonClicked] clicked button id: " << id << ", type : " << type;
    emit clickedButtonIndex(type);
}

void LayoutChooseWidget::initUI2()
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::FramelessWindowHint, true);
    setStyleSheet("QWidget{background-color: #282828; border-radius: 8px;}");

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 120));
    setGraphicsEffect(shadow);

    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(15, 15, 15, 15);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);

    QList<QString> imagelist({":/images/layout_single.png",
                           ":/images/layout_double.png",
                           ":/images/layout_four.png",
                           ":/images/layout_six.png",
                           ":/images/layout_eghit.png",
                            ":/images/layout_nine.png"});

    QList<QString> namelist({QStringLiteral("单画面"), QStringLiteral("双画面"),
                            QStringLiteral("四画面"), QStringLiteral("六画面"),
                            QStringLiteral("八画面"), QStringLiteral("九画面")});

    for (int i = 0; i < 6; i++) {
        QToolButton *btn = new QToolButton(this);
        btn->setText(namelist.at(i));
        btn->setIcon(QIcon(imagelist.at(i)));
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btn->setIconSize(QSize(24, 24));
        btn->setCheckable(true);
        btn->setMinimumSize(180, 60);
        btn->setStyleSheet("QToolButton{"
                           "    background-color: #393939;"
                           "    qproperty-iconSize: 24px 24px;"
                           "    qproperty-toolButtonStyle: ToolButtonTextBesideIcon;"
//                           "    text-align: center;"
                           "    font-size: 18px;"
                           "    color: white;"
                           "}"
                           "QToolButton:hover {"
                           "     background-color: #393939;"
                           "}"
                           "QToolButton:checked {"
                           "    background-color: #3D8BFF;"
                           "}");

        buttonGroup->addButton(btn, i);
        layout->addWidget(btn, i / 2, i % 2, Qt::AlignHCenter);
    }

//    this->setStyleSheet(R"(
//        QToolButton {
//            background-color: #393939;
//            border: none;
//            border-radius: 4px;
//            padding: 8px;
//            color: white;
//            font-size: 18px;
//            spacing: 6px;
//            text-align: center;
//        }
//        QToolButton:hover {
//            background-color: #424242;
//            border: 1px solid #505050;
//        }
//        QToolButton:checked {
//            background-color: #3D8BFF;
//            color: white;
//            border: none;
//        }
//    )");

    connect(buttonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &LayoutChooseWidget::onButtonClicked);

    buttonGroup->button(0)->setChecked(true);
}


void LayoutChooseWidget::initUI()
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::FramelessWindowHint, true);
    setStyleSheet("background-color: #282828; border-radius: 8px;");

    m_lay1Btn = new QToolButton;
    m_lay2Btn = new QToolButton;
    m_lay4Btn = new QToolButton;
    m_lay6Btn = new QToolButton;
    m_lay8Btn = new QToolButton;
    m_lay9Btn = new QToolButton;

    m_lay1Btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_lay2Btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_lay4Btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_lay6Btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_lay8Btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_lay9Btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    m_lay1Btn->setText(QStringLiteral("单画面"));
    m_lay2Btn->setText(QStringLiteral("双画面"));
    m_lay4Btn->setText(QStringLiteral("四画面"));
    m_lay6Btn->setText(QStringLiteral("六画面"));
    m_lay8Btn->setText(QStringLiteral("八画面"));
    m_lay9Btn->setText(QStringLiteral("九画面"));

    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->setContentsMargins(20, 20, 20, 20);
    gridLayout->setSpacing(10);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);

    m_lay1Btn->setIcon(QIcon(":/images/layout_single.png"));
    m_lay2Btn->setIcon(QIcon(":/images/layout_double.png"));
    m_lay4Btn->setIcon(QIcon(":/images/layout_four.png"));
    m_lay6Btn->setIcon(QIcon(":/images/layout_six.png"));
    m_lay8Btn->setIcon(QIcon(":/images/layout_eghit.png"));
    m_lay9Btn->setIcon(QIcon(":/images/layout_nine.png"));

    int i = 0;
    for (auto btn : {m_lay1Btn, m_lay2Btn, m_lay4Btn, m_lay6Btn, m_lay8Btn, m_lay9Btn})
    {
        btn->setCheckable(true);
        btn->setMinimumSize(180, 60);
//        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        buttonGroup->addButton(btn, i);
        gridLayout->addWidget(btn, i / 2, i % 2);
        i++;

        connect(btn, &QToolButton::clicked, this, [this]() {
            QTimer::singleShot(150, this, [this]{close();});
        });
    }

    m_lay1Btn->setStyleSheet(
        "QToolButton { background-color: #393939;  color: white; border: none;  padding: 8px 12px; border-radius: 4px; spacing: 6px; font-size: 18px; }"
        "QToolButton:hover { background-color: #393939; }"
        "QToolButton:checked { background-color: #3D8BFF; }"
        );
    m_lay2Btn->setStyleSheet(
        "QToolButton { background-color: #393939;  color: white; border: none;  padding: 8px 12px; border-radius: 4px; spacing: 6px; font-size: 18px;}"
        "QToolButton:hover { background-color: #393939; }"
        "QToolButton:checked { background-color: #3D8BFF; }"
        );
    m_lay4Btn->setStyleSheet(
        "QToolButton { background-color: #393939;  color: white; border: none;  padding: 8px 12px; border-radius: 4px; spacing: 6px; font-size: 18px;}"
        "QToolButton:hover { background-color: #393939; }"
        "QToolButton:checked { background-color: #3D8BFF; }"
        );
    m_lay6Btn->setStyleSheet(
        "QToolButton { background-color: #393939;  color: white; border: none;  padding: 8px 12px; border-radius: 4px; spacing: 6px; font-size: 18px;}"
        "QToolButton:hover { background-color: #393939; }"
        "QToolButton:checked { background-color: #3D8BFF; }"
        );
    m_lay8Btn->setStyleSheet(
        "QToolButton { background-color: #393939;  color: white; border: none;  padding: 8px 12px; border-radius: 4px; spacing: 6px; font-size: 18px;}"
        "QToolButton:hover { background-color: #393939; }"
        "QToolButton:checked { background-color: #3D8BFF; }"
        );
    m_lay9Btn->setStyleSheet(
        "QToolButton { background-color: #393939;  color: white; border: none;  padding: 8px 12px; border-radius: 4px; spacing: 6px; font-size: 18px;}"
        "QToolButton:hover { background-color: #393939; }"
        "QToolButton:checked { background-color: #3D8BFF; }"
        );

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 100));
    setGraphicsEffect(shadow);

    connect(buttonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &LayoutChooseWidget::onButtonClicked);

    buttonGroup->button(0)->setChecked(true);
}

void LayoutChooseWidget::showBelowWidget(QWidget *widget)
{
    if (!widget)
        return;

    QPoint globalPos = widget->mapToGlobal(QPoint(0, 0));

    int x = globalPos.x() + (widget->width() - width()) / 2;
    int y = globalPos.y() + (widget->height()) + 10;

    QScreen *screen = QGuiApplication::screenAt(globalPos);
    if (screen) {
        QRect screenRect = screen->availableGeometry();
        if (y + height() > screenRect.bottom()) {
            y = globalPos.y() - height() - 10;
        }
    }

    move(x, y);

    // 显示窗口并添加动画效果
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(150);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start();

    show();
}

void LayoutChooseWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(45, 45, 45));
    painter.drawRoundedRect(rect().adjusted(0, 0, 0, 0), 8, 8);
}
