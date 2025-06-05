#include "rotatingbutton.h"
#include <QPainter>
#include <QPixmap>
#include <QStyle>
#include <QFontMetrics>
#include <QDebug>

RotatingButton::RotatingButton(QWidget *parent)
    : QToolButton(parent)
    , m_animation(new QPropertyAnimation(this, "rotation", this))
{
    setAttribute(Qt::WA_StaticContents);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setCursor(Qt::PointingHandCursor);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setStyleSheet("border: none; background: transparent;");

    m_animation->setDuration(800);
    m_animation->setStartValue(0);
    m_animation->setEndValue(360);
    m_animation->setLoopCount(-1);

    m_state = Disconnected;
}

void RotatingButton::setState(State state)
{
    if (m_state != state) {
        m_state = state;
        updateDisplay();
    }
}

int RotatingButton::rotation() const
{
    return m_rotation;
}

void RotatingButton::setRotation(int angle)
{
    angle = angle % 360;
    if (m_rotation != angle) {
        m_rotation = angle;
        update();
        emit rotationChanged(angle);
    }
}

void RotatingButton::startConnectingAnim()
{
    setState(Connecting);
    m_animation->start();
}

void RotatingButton::stopConnectingAnim(bool success)
{
    m_animation->stop();
    setState(success ? Connected : Disconnected);
    setRotation(0);
}

QFont chooseChineseFont(int pointSize = 10) {
    QString testText = "未连接";
    QStringList candidates = { "Microsoft YaHei", "SimHei", "WenQuanYi Micro Hei", "Arial Unicode MS" };

    for (const QString &f : candidates) {
        QFont font(f, pointSize);
        QFontMetrics metrics(font);
        bool ok = true;
        for (const QChar &ch : testText) {
            if (!metrics.inFont(ch)) {
                ok = false;
                break;
            }
        }
        if (ok)
            return font;
    }

    return QFont(); // fallback
}


void RotatingButton::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);

    QFont font = chooseChineseFont(12);
    p.setFont(font);

//    qDebug() << "[RotatingButton] paintEvent - m_state : " << m_state;

    switch (m_state)
    {
    case Disconnected:
        p.setPen(QColor("#BBBBBB"));
        p.drawText(rect(), Qt::AlignCenter, QStringLiteral("未连接"));
        break;
    case Connecting:
    {
        p.save();
        p.translate(rect().center());
        p.rotate(m_rotation);
        QPixmap icon = QPixmap(":/images/waiting.png").scaled(20, 20);
        QPoint iconPos(-icon.width()/2, -icon.height()/2); // 图标中心对准旋转中心
        p.drawPixmap(iconPos, icon);
        p.restore();
        break;
    }
    case Connected:
    {
        p.setPen(QColor("##FF4D4F"));
        p.drawText(rect(), Qt::AlignCenter, QStringLiteral("断开连接"));
        break;
    }
    }
}

void RotatingButton::updateDisplay()
{
    switch(m_state)
    {
    case Disconnected:
        setCursor(Qt::PointingHandCursor);
        break;
    case Connecting:
        setCursor(Qt::WaitCursor);
        break;
    case Connected:
        setCursor(Qt::ArrowCursor);
        break;
    }
    update();
}

