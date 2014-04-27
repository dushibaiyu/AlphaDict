#ifndef _DICTINDEXMODEL_H_
#define _DICTINDEXMODEL_H_

#include "dict/iDict.h"
#include "MutexLock.h"

#include <QAbstractListModel>

class DictIndexModel: public QAbstractListModel
{
     Q_OBJECT

public:
    DictIndexModel();
    virtual ~DictIndexModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    iIndexItem* item(int row);

private slots:
    void onUpdataList(int curitem);

private:
    IndexList  *m_indexList;
    int m_indexListStart;
    MutexCriticalSection m_cs;
};

#endif
