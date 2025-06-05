#ifndef GROUPNAVMODEL_H
#define GROUPNAVMODEL_H

#include <QAbstractListModel>
#include "datamanager.h"

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
    QVector<int> m_memberCounts;
};

#endif // GROUPNAVMODEL_H
