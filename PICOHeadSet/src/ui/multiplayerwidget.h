#ifndef MULTIPLAYERWIDGET_H
#define MULTIPLAYERWIDGET_H

#include <QObject>
#include <QMainWindow>
#include <QWidget>
#include <QList>
#include <QHBoxLayout>
#include <QMdiArea>

#include "../../common.h"
#include "customvideoplayer.h"

class MultiPlayerWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit MultiPlayerWidget(QWidget *parent = nullptr);

    void setViewWindowLayout(bw::ScreenLayout lyt);

private:
    void setupUI();

    void singleLayout();
    void doubleLayout();
    void tripleLayout();
    void fourLayout();
    void sixLayout();
    void eightLayout();
    void nineLayout();

signals:

private:
    QList<CustomVideoPlayer*> m_playerList;
    QWidget *widget_1;
    QWidget *widget_2;
    QWidget *widget_3;
    QWidget *widget_4;
    QWidget *widget_5;
    QWidget *widget_6;
    QWidget *widget_7;
    QWidget *widget_8;
    QWidget *widget_9;
    QWidget *widget_10;
    QWidget *widget_11;
    QWidget *widget_12;
    QWidget *widget_13;
    QWidget *widget_14;
    QWidget *widget_15;
    QWidget *widget_16;

    QList<QWidget*> m_widgetList;

    QHBoxLayout *m_hLayout;

    QMdiArea m_mdiArea;
};

#endif // MULTIPLAYERWIDGET_H
