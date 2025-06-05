#ifndef ROTATINGBUTTON_H
#define ROTATINGBUTTON_H

#include <QWidget>
#include <QToolButton>
#include <QPropertyAnimation>

class RotatingButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(int rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
public:
    enum State {
        Disconnected = 0,   // 显示"未连接"
        Connecting = 1,     // 显示"旋转图标", 表示正在连接
        Connected = 2,      // 显示"已连接"/"断开连接", 表示已连接成功
        Disconnecting = 3,  // 显示断开连接中
    };
    Q_ENUM(State)

    explicit RotatingButton(QWidget *parent = nullptr);

    // 状态控制
    State state() const { return m_state; }
    void setState(State state);

    // 属性访问器
    int rotation() const;
    void setRotation(int angle);

    // 动画控制
    void startConnectingAnim();
    void stopConnectingAnim(bool success);

private:
    void updateDisplay();

signals:
    void rotationChanged(int angle);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    State m_state = Disconnected;
    int m_rotation = 0;
    QPropertyAnimation *m_animation = nullptr;
};

#endif // ROTATINGBUTTON_H
