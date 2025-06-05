#ifndef BOTTOMBAR_H
#define BOTTOMBAR_H

#include <QWidget>
#include <QPushButton>

class BottomBar : public QWidget
{
    Q_OBJECT
public:
    explicit BottomBar(QWidget *parent = nullptr);

    int layoutType() const { return m_layoutType; }

private:
    void initUI();
    void initConnection();

signals:
    void sigClickedLayoutBtn();
    void sigClickedDevListBtn();

private:
    QPushButton *m_layoutButton;
    QPushButton *m_devlistButton;

    int m_layoutType;
};

#endif // BOTTOMBAR_H
