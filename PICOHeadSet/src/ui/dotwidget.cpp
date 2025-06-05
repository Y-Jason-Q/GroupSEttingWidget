#include "dotwidget.h"
#include <QPainter>

DotWidget::DotWidget(QWidget *parent)
    : QWidget{parent}
{
    currentIndex = 0;

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [this](){
        currentIndex = (currentIndex + 1) % 4;
        update();
    });

    setStyleSheet("background-color:transparent;");
}

void DotWidget::start(int intervalMs)
{
    if (!timer->isActive()) {
        timer->start(intervalMs);
    }
}

void DotWidget::stop()
{
    if (timer->isActive()) {
        timer->stop();
    }
}

void DotWidget::resetPosition(int index)
{
    currentIndex = 0;
    update();
}

void DotWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const int dotSize = 12;
    const int spacing = 12;
    const QColor darkColor("#393939");
    const QColor lightColor("#BBBBBB");

    // 计算总宽度和居中起始位置
    int totalWidth = 4 * dotSize + 3 * spacing; // 68px
    int startY = static_cast<int>((height() - dotSize) / 2);// 水平居中
    int startX = static_cast<int>((width() - totalWidth) / 2);// 垂直居中

    // 绘制 4 个点
    for (int i = 0; i < 4; ++i) {
        int x = startX + i * (dotSize + spacing);
        painter.setBrush(i == currentIndex ? lightColor : darkColor);
        painter.drawEllipse(x, startY, dotSize, dotSize);
    }
}
