#ifndef LAYOUTCHOOSEWIDGET_H
#define LAYOUTCHOOSEWIDGET_H

#include <QWidget>
#include <QToolButton>

class LayoutChooseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LayoutChooseWidget(QWidget *parent = nullptr);

    void showBelowWidget(QWidget *widget);

private:
    void initUI();
    void initUI2();

signals:
    void clickedButtonIndex(int layoutType);

private slots:
    void onButtonClicked(int id);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QToolButton *m_lay1Btn;
    QToolButton *m_lay2Btn;
    QToolButton *m_lay4Btn;
    QToolButton *m_lay6Btn;
    QToolButton *m_lay8Btn;
    QToolButton *m_lay9Btn;

    QButtonGroup *buttonGroup;
};

#endif // LAYOUTCHOOSEWIDGET_H
