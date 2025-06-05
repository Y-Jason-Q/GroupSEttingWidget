#ifndef NETWORKDISPLAYWIDGET_H
#define NETWORKDISPLAYWIDGET_H

#include <QWidget>
#include <QLabel>

class NetworkStatusThread;

class NetworkDisplayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkDisplayWidget(QWidget *parent = nullptr);

    void setNetworkName(const QString &name);

private:
    void initUI();

signals:

public slots:
    void onNetworkStatus(const QString &name, bool isConnected);

private:
    QLabel *m_iconLabel;
    QLabel *m_titleLabel;
    QLabel *m_networkNameLabel;

    NetworkStatusThread *m_networkThread;
};

#endif // NETWORKDISPLAYWIDGET_H
