#ifndef GROUPNAVMODEL_H
#define GROUPNAVMODEL_H

#include <QAbstractListModel>
#include "datamanager.h"

/*!
 * \brief 导航栏分组列表模型
 */
class GroupNavModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit GroupNavModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    int groupIdAt(int row) const;

public slots:
    void onDataChanged();

private:
    QVector<GroupInfo> m_groups;
};

#endif // GROUPNAVMODEL_H
