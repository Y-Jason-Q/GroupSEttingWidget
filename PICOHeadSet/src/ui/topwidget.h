#ifndef TOPWIDGET_H
#define TOPWIDGET_H

#include <QWidget>
#include <QLabel>

class DotWidget;

class TopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopWidget(QWidget *parent = nullptr);

    enum ConnectStatus
    {
        DISCONNECT = 0,
        CONNECTING,
        CONNECTED
    };
    Q_ENUM(ConnectStatus)

    void updateStatus(ConnectStatus status);

private:
    void initUI();

signals:

private:
    QLabel *m_leftLabel;
    DotWidget *m_midWidget;
    QLabel *m_rightLabel;

    QLabel *m_statusLabel;
    QLabel *m_infoLabel;

};

#endif // TOPWIDGET_H
