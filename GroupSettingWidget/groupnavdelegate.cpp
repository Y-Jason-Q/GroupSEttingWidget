#include "groupnavdelegate.h"
#include <QApplication>
#include <QMouseEvent>
#include <QPixmap>
#include <QPainter>

GroupNavDelegate::GroupNavDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

void GroupNavDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();

    // 1. 绘制背景，选中为圆角
    QColor bgColor = (option.state & QStyle::State_Selected) ? QColor("#393939") : QColor("#141414");
    QRect rect = option.rect.adjusted(24, 0, -24, 0);
    int radius = 16;

    if (option.state & QStyle::State_Selected) {
        QPainterPath path;
        path.addRoundedRect(rect, radius, radius);
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->fillPath(path, bgColor);
    } else {
        painter->fillRect(rect, bgColor);
    }

    // 2. 绘制文字
    QString text = index.data(Qt::DisplayRole).toString();
    painter->setPen(QColor("#FFFFFF"));
    QFont font = painter->font();
    font.setPointSize(18);
    painter->setFont(font);
    QRect textRect = rect.adjusted(24, 0, -64, 0);
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);

    // 3. 删除按钮（第0项不显示）
    if (index.row() > 0 && (option.state & QStyle::State_MouseOver)) {
        QRect delRect(rect.right() - 44, rect.top() + (rect.height() - 24) / 2, 24, 24);
        QPixmap delPixmap(":/images/delete_btn.png");
        if (!delPixmap.isNull()) {
            painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
            painter->drawPixmap(delRect, delPixmap.scaled(delRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }

    painter->restore();
}

bool GroupNavDelegate::editorEvent(QEvent* event, QAbstractItemModel*, const QStyleOptionViewItem& option, const QModelIndex& index)
{
//    if (event->type() == QEvent::MouseButtonRelease && index.row() > 0) {
//        auto* e = static_cast<QMouseEvent*>(event);
//        QRect delRect(option.rect.right() - 28, option.rect.top() + (option.rect.height() - 20) / 2, 20, 20);
//        if (delRect.contains(e->pos())) {
//            emit deleteClicked(index.row());
//            return true;
//        }
//    }
    if (event->type() == QEvent::MouseButtonRelease && index.row() > 0) {
        auto* e = static_cast<QMouseEvent*>(event);
        QRect rect = option.rect.adjusted(24, 0, -24, 0);
        QRect delRect(rect.right() - 44, rect.top() + (rect.height() - 24) / 2, 24, 24);
        if (delRect.contains(e->pos())) {
            emit deleteClicked(index.row());
            return true;
        }
    }
    return false;
}

QSize GroupNavDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(option.rect.width(), 88); // 例如高度88像素
}
