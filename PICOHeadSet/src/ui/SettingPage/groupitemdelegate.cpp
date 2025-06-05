#include "groupitemdelegate.h"
#include <QPainter>
#include <QApplication>
#include <QStyleOptionViewItem>
#include <QStyleOptionButton>
#include <QMouseEvent>
#include <QPainterPath>

GroupItemDelegate::GroupItemDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

void GroupItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
           const QModelIndex &index) const
{
    painter->save();

    // 背景
    QPainterPath path;
    path.addRoundedRect(option.rect, 12, 12);
    QBrush bgBrush = (option.state & QStyle::State_Selected) ? option.palette.highlight() : QColor("#393939");
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->fillPath(path, bgBrush);

    // 组名
    QString text = index.data(Qt::DisplayRole).toString();
    QFont font("Microsoft YaHei", 18);
    painter->setFont(font);
    painter->setPen(QColor("#FFFFFF"));
    QRect textRect = option.rect.adjusted(32, 0, -64, 0);
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);

    // 自定义复选框 以及 选中和未选中的图片
    QRect checkRect(option.rect.right() - 56, option.rect.top() + (option.rect.height() - 24) / 2, 24, 24);
    QPixmap checkedPixmap(":/images/checkbtn_checked.png");
    QPixmap uncheckedPixmap(":/images/checkbtn_uncheck.png");
    bool checked = index.data(Qt::CheckStateRole).toInt() == Qt::Checked;
    painter->drawPixmap(checkRect, checked ? checkedPixmap : uncheckedPixmap);

    painter->restore();
}

bool GroupItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                 const QStyleOptionViewItem &option,
                 const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        auto *mouseEvent = static_cast<QMouseEvent*>(event);
        // 判断点击是否在checkbox上
        //        QRect checkRect(option.rect.right() - 56, option.rect.top() + (option.rect.height()-24)/2, 24, 24);
        //        if (checkRect.contains(mouseEvent->pos())) {
        //            bool checked = index.data(Qt::CheckStateRole).toInt() == Qt::Checked;
        //            model->setData(index, checked ? Qt::Unchecked : Qt::Checked, Qt::CheckStateRole);
        //            return true;
        //        }
        // 判断点击是否在整行范围内
        if (option.rect.contains(mouseEvent->pos())) {
            bool checked = index.data(Qt::CheckStateRole).toInt() == Qt::Checked;
            model->setData(index, checked ? Qt::Unchecked : Qt::Checked, Qt::CheckStateRole);
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QSize GroupItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(option.rect.width(), 80); // 例如高度80像素
}
