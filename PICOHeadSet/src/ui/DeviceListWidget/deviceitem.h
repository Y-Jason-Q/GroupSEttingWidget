#ifndef DEVICEITEM_H
#define DEVICEITEM_H

#include <QWidget>
#include <QLabel>
#include "rotatingbutton.h"

class DeviceItem : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceItem(const QString &name, QWidget *parent = nullptr);

    void updateState(RotatingButton::State state);
    QString devName() const { return m_devName; }


private:
    void initUI();

signals:
    void connectRequested(const QString &deviceId);
    void disconnectRequested(const QString &name);

private:
    QLabel *m_devNameLabel;
    RotatingButton *m_linkButton;

    QString m_devName;
};

#endif // DEVICEITEM_H
