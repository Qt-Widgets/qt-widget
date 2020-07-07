#include "htreemodel.h"
#include "htreeitemdata.h"

//HTreeModel::HTreeModel(const QStringList &colNames, QObject *parent) : QAbstractItemModel(parent)
//{
//    QVector<QVariant> rootData;

//    for(const auto &k:colNames){
//        rootData<<k;
//    }

//    m_rootItem = new HTreeItemData(rootData);
//}

//HTreeModel::~HTreeModel()
//{
//    delete m_rootItem;
//}

//int HTreeModel::columnCount(const QModelIndex &parent) const
//{
//    return m_rootItem->ColumnCount();
//}

//QVariant HTreeModel::data(const QModelIndex &index, int role) const
//{
//    if (!index.isValid())
//        return QVariant();

//    qint32 column=index.column();

//    HTreeItemData *item = getItem(index);
//    switch (role) {
//    case Qt::DisplayRole:
//    {


//    }
//    case Qt::EditRole:
//    {

//    }
//    case Qt::DecorationRole:{


//    }
//    case Qt::CheckStateRole:{
//        return QVariant();
//    }
//    case Qt::TextAlignmentRole:{


//    }
//    case Qt::TextColorRole:{
//        return QColor(Qt::black);
//    }
//    case Qt::SizeHintRole:{
//        return QVariant();
//    }
//    case Qt::FontRole:{


//    }
//    default:
//        return QVariant();
//    }
//}



//HTreeItemData *HTreeModel::getItem(const QModelIndex &index) const
//{
//    if (index.isValid()) {
//        HTreeItemData *item = static_cast<HTreeItemData*>(index.internalPointer());
//        if (item)
//            return item;
//    }
//    return m_rootItem;
//}
