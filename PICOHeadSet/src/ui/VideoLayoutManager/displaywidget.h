#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "../DeviceListWidget/rotatingbutton.h"

class QGridLayout;
class BlankWidget;
class YuvPlayer;

struct WidgetInfo
{
    YuvPlayer *widget;
    QString devID;
    bool isBlank;
};

class DisplayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayWidget(QWidget *parent = nullptr);
    void initialize();

    void setWidgetList(QList<YuvPlayer*> widgetlist);

    void addWidget(const QString &id, bool isBlank = false);

    void updateFrame(const QString &devID, const uint8_t *yuvData, int width, int height);

    YuvPlayer *yuvPlayer(const QString &devID);

    void clickedLinkButton(const QString &devID, RotatingButton::State state);

signals:
    void noConnected();
    void connectRequest(const QString &id);
    void disconnectRequest(const QString &id);
    void sigFullScreen(const QString &id, bool isLinked, bool isFull);

public slots:
    void updateLayout(int layoutType);
    void onUpdateLinkStatus(const QString &devID, bool isLinked);
    void onFullScreen(const QString &id, bool isLinked, bool isFull);

private:
    void layout1(int n);
    void layout2(int n);
    void layout4(int n);
    void layout6(int n);
    void layout8(int n);
    void layout9(int n);
    void clearAll();
    void fillBlankWidget(int layoutType);

    bool containsID(const QString& targetID);

private:
    QList<WidgetInfo> m_stWidgetInfoList;
    QGridLayout *m_layout;
    int m_currentLayoutType;
};

#endif // DISPLAYWIDGET_H
