#ifndef VIDEOITEMWRAPPER_H
#define VIDEOITEMWRAPPER_H

#include <QGraphicsWidget>
#include <QUuid>

class YuvPlayer;
class QGraphicsProxyWidget;

class VideoItemWrapper : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit VideoItemWrapper(QWidget *parent = nullptr);

    YuvPlayer *player() const { return m_player; }
    QUuid id() const { return m_id; }

    // 交换视频数据接口
    void swapVideoWith(VideoItemWrapper *other);

    void setGeometry(const QRectF &rect);
    void setGeometry(qreal x, qreal y, qreal w, qreal h);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void fullscreenRequested(VideoItemWrapper *item);
    void switchRequested();
    void linkRequested(bool linked, VideoItemWrapper *item);

private:
    void createProxyWidget();
    void setupConnections();

    YuvPlayer *m_player;
    QGraphicsProxyWidget *m_proxy;

    // 拖拽
    bool m_dragging = false;
    QPointF m_dragStartPos;

    QUuid m_id;
};

#endif // VIDEOITEMWRAPPER_H
