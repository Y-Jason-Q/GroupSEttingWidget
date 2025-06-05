#ifndef DOTWIDGET_H
#define DOTWIDGET_H

#include <QWidget>
#include <QTimer>

class DotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DotWidget(QWidget *parent = nullptr);

    void start(int intervalMs = 500);
    void stop();

    void resetPosition(int index = 0);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int currentIndex;
    QTimer *timer;
};

#endif // DOTWIDGET_H
