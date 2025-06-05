#ifndef BLANKWIDGET_H
#define BLANKWIDGET_H

#include <QWidget>
#include <QLabel>

class BlankWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BlankWidget(QWidget *parent = nullptr);

    void setTitleName(const QString &titleName);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:

private:
    QLabel *m_titleLabel;
    QLabel *m_imageLabel;
    QLabel *m_textLabel;

    QString m_titleText;
    QString m_statusText;
    QImage m_image;
};

#endif // BLANKWIDGET_H
