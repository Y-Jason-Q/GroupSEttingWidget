#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QFile>
#include <QToolButton>
#include <QMouseEvent>
#include "stdafx.h"

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    TitleBar(QWidget *parent);
    ~TitleBar();

    void setButtonType(TITLE_BUTTONTYPE type);

    void setIcon(const QString &iconpath);
    void setTitleName(const QString &titleName);
    void showTitleName(bool isShow);

public slots:
    void slotMaxBtnClicked();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);

private:
    void toggleMaxState();

    QToolButton *minBtn;
    QToolButton *maxBtn;
    QToolButton *clsBtn;
    QLabel *m_logoLabel;
    QLabel *m_titleLabel;
    QWidget *m_logoWidget;
};

#endif // TITLEBAR_H
