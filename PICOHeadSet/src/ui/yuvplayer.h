#ifndef YUVPLAYER_H
#define YUVPLAYER_H

#include <QObject>
#include <QWidget>
#include <QImage>
#include <QRect>
#include <QMutex>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QThread>
#include <QMutex>
#include <QVector>
#include <QTimer>
#include <QByteArray>
#include <QAtomicInt>
#include <QRunnable>

class YuvConvertTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    YuvConvertTask(const QByteArray &yuv, int width, int height, int dstW, int dstH, QAtomicInt *cancelFlag);
    void run() override;

signals:
    void frameConverted(const QImage &image);

private:
    QByteArray m_yuv;
    int m_width;
    int m_height;
    int m_dstW;
    int m_dstH;
    QAtomicInt *m_cancelFlag;
};

class YuvPlayer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit YuvPlayer(QWidget *parent = nullptr);
    ~YuvPlayer();

    void setFullValue(bool bFull) { m_isFull = bFull; }
    void setTitleName(const QString &name);
    void updateLinkStatus(bool isLink);
    void updateYuvFrame(const uint8_t *yuvData, int width, int height);
    void setActive(bool active);
    bool isLinked() const { return m_isLinked; }
    QString titleName() const { return m_titleText; }
    void stopConverter();

signals:
    void linkButtonClicked(const QString &title, bool linked);
    void switchButtonClicked();
    void maxButtonClicked(const QString &title, bool linked, bool isFull);
    void fullScreenRequested(int playerIndex);

    void requestConvert(const QByteArray &yuvData, int width, int height);

public slots:
    void handleConvertedFrame(const QImage & image);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void drawButtons(QPainter &painter);
    void drawTitle(QPainter &painter);
//    void drawLinkStatus(QPainter &painter);

    QImage m_blankImage;
    QString m_titleText;
    bool m_isLinked = false;
    bool m_isActive = false;
    bool m_hovered = false;

    QOpenGLTexture *m_texture = nullptr;
    QImage m_currentFrame;
    QMutex m_frameMutex;
    QAtomicInt m_cancelFlag;
    QAtomicInt m_processingFrame;

    QRect m_linkBtnRect;
    QRect m_switchBtnRect;
    QRect m_maxBtnRect;

    QPixmap m_maxBtnPixmap;

    QTimer *m_hideTimer = nullptr;

    bool m_isFull;
};

//class YuvPlayer : public QWidget
//{
//    Q_OBJECT
//public:
//    explicit YuvPlayer(QWidget *parent = nullptr);

//    QString titleName() const { return m_titleText; }
//    void setTitleName(const QString &name);
//    void updateLinkStatus(bool isLink = true);
//    void updateYuvFrame(const uint8_t *yuvData, int width, int height);

//signals:
//    void linkButtonClicked(const QString &devID, bool linked);
//    void switchButtonClicked();
//    void maxButtonClicked();

//protected:
//    void paintEvent(QPaintEvent* event) override;
//    void mousePressEvent(QMouseEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override;

//signals:

//private:
//    void drawButtons(QPainter &painter);

//    QString m_titleText;

//    QRect m_linkBtnRect;
//    QRect m_switchBtnRect;
//    QRect m_maxBtnRect;

//    QVector<uint8_t> m_rgbBuffer;
//    QImage m_image;
//    QImage m_blankImage;

//    bool m_isLinked = false;
//};

#endif // YUVPLAYER_H
