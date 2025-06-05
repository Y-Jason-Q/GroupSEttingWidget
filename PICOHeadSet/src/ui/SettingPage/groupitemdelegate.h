#ifndef GROUPITEMDELEGATE_H
#define GROUPITEMDELEGATE_H

#include <QStyledItemDelegate>

class GroupItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit GroupItemDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // GROUPITEMDELEGATE_H
