#ifndef GROUPNAVDELEGATE_H
#define GROUPNAVDELEGATE_H

#include <QStyledItemDelegate>

class GroupNavDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit GroupNavDelegate(QObject *parent = nullptr);
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

signals:
    void deleteClicked(int row);
};

#endif // GROUPNAVDELEGATE_H
