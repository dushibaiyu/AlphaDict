#include "DictIndexModel.h"
#include "DictManager.h"
#include "MessageQueue.h"

#include <stdlib.h>
#define INDEXLIST_SIZE_MAX  10000

DictIndexModel::DictIndexModel():m_indexListStart(-1)
{
    //QStringList list;
    //setStringList(list);
    m_indexList = new IndexList(); /* avoid to check NULL pointer */
}

DictIndexModel::~DictIndexModel()
{
}

int DictIndexModel::rowCount(const QModelIndex &parent) const
{
    //MutexLock lock(m_cs);
    return m_indexList->size();
}

QVariant DictIndexModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_indexList->size())
        return QVariant();

    if (role == Qt::DisplayRole) {       
        //printf("data:%d\n", index.row())
        //MutexLock lock(m_cs);
        iIndexItem* item = (*m_indexList)[index.row()];
        QString strIndex = QString::fromWCharArray(item->index, item->inxlen);
        return strIndex;
     } else {
        return QVariant();
    }
}

iIndexItem* DictIndexModel::item(int row)
{
    //MutexLock lock(m_cs);
    return (*m_indexList)[row];
}

QVariant DictIndexModel::headerData(int section, Qt::Orientation orientation, int role) const
{
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal)
         return QString("Column %1").arg(section);
     else
         return QString("Row %1").arg(section);
}

void DictIndexModel::onUpdataList(int curitem)
{
    if (m_indexListStart == -1) {
        m_indexListStart = 0;

        beginResetModel();
        DictManager::getReference().getIndexList(*m_indexList, 0, INDEXLIST_SIZE_MAX);
        endResetModel();
        return;
    }
    printf("onUpdataList: %d, %d\n", curitem, m_indexList->size());
    if (curitem == 0) {
        if (m_indexListStart != 0) {
            IndexList* index = new IndexList();
            int start = m_indexListStart - INDEXLIST_SIZE_MAX/2;
            DictManager::getReference().getIndexList(*index, start, start + INDEXLIST_SIZE_MAX/2);
            for (int i=0; i<INDEXLIST_SIZE_MAX/2; i++)
                index->push_back((*m_indexList)[i]);
            for (int i=INDEXLIST_SIZE_MAX/2; i<index->size(); i++)
                delete (*m_indexList)[i];
            
            beginResetModel();
            m_indexList->clear();
            delete m_indexList;
            m_indexList = index;
            m_indexListStart = start;
            endResetModel();
            return;
        }
    } else if (m_indexList->size()- curitem <= 50) { /* when scroll to bottom, (9977 -> 9999)*/
        if (m_indexList->size() == INDEXLIST_SIZE_MAX) {
            IndexList* index = new IndexList();
            int start = m_indexListStart + INDEXLIST_SIZE_MAX;
            int ret = DictManager::getReference().getIndexList(*index, start, start + INDEXLIST_SIZE_MAX/2);
            if (ret == INDEXLIST_SIZE_MAX/2) {
                beginResetModel();
                for (int i=0; i<INDEXLIST_SIZE_MAX/2; i++)
                    delete (*m_indexList)[i];
                m_indexList->erase(m_indexList->begin(), m_indexList->begin()+INDEXLIST_SIZE_MAX/2);
                for (int i=0; i<index->size(); i++)
                    m_indexList->push_back((*index)[i]);
                m_indexListStart += INDEXLIST_SIZE_MAX/2;
                endResetModel();
                delete index;
            }
            return;
        }
    }
}

