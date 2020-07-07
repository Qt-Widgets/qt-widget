#include <QtWidgets>
#include "Src/Gui/htreeview/h_treeview.h"
#include "treeitem.h"
#include "treemodel.h"
#include "programhandler.h"
#include "editdata.h"






TreeModel::TreeModel(H_TreeView *view, QObject *parent): QAbstractItemModel(parent),ptrView(view)
{
    pDataHandler=ProgramHandler::GetInstance();
    QVector<QVariant> rootData;
    rootData<< tr("Name")<<tr("Type")<<tr("Information");





    ItemTypeNames.insert(EditData::layer,"layer");
    ItemTypeNames.insert(EditData::program,"program");
    ItemTypeNames.insert(HGraphicsArcItem::Type,"arc");
    ItemTypeNames.insert(HGraphicsItemGroup::Type,"group");
    ItemTypeNames.insert(HGraphicsLineItem::Type,"line");
    ItemTypeNames.insert(HGraphicsEllipseItem::Type,"ellipse");
    ItemTypeNames.insert(HGraphicsPolygonItem::Type,"polygon");
    ItemTypeNames.insert(HGraphicsPathItem::Type,"curve");
    ItemTypeNames.insert(HGraphicsRectItem::Type,"rect");

    ItemTypeIcons.insert("layer",QIcon(":/resource/icons/layers.svg"));
    ItemTypeIcons.insert("program",QIcon(":/resource/icons/science.svg"));
    ItemTypeIcons.insert("arc",QIcon(":/resource/icons/arc.svg"));
    ItemTypeIcons.insert("group",QIcon(":/resource/icons/group.svg"));
    ItemTypeIcons.insert("line",QIcon(":/resource/icons/line.svg"));
    ItemTypeIcons.insert("ellipse",QIcon(":/resource/icons/circle.svg"));
    ItemTypeIcons.insert("polygon",QIcon(":/resource/icons/point.svg"));
    ItemTypeIcons.insert("curve",QIcon(":/resource/icons/curve.svg"));
    ItemTypeIcons.insert("rect",QIcon(":/resource/icons/rectangle.svg"));
    

    rootItem = new TreeItem(rootData);
    // setupModelData(rootItem);

    //    m_publicIconMap.append(QIcon(QStringLiteral(":/resource/icons/eye.svg")));
    //    m_publicIconMap.append(QIcon(QStringLiteral(":/resource/icons/eye-disable.svg")));
    pDataHandler=ProgramHandler::GetInstance();


    connect(pDataHandler,&ProgramHandler::programAdded,this,&TreeModel::addProgram);
    connect(pDataHandler,&ProgramHandler::programClosed,this,&TreeModel::closeProgram);
    connect(pDataHandler,&ProgramHandler::programNewed,this,&TreeModel::newProgram);
    connect(pDataHandler,&ProgramHandler::programRefreshed,this,&TreeModel::refreshData);

    connect(this,&TreeModel::programChanged,pDataHandler,&ProgramHandler::setActiveProgram);

    
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    qint32 column=index.column();

    TreeItem *item = getItem(index);
    switch (role) {
    case Qt::DisplayRole:
    {
        return item->data(column);
    }
    case Qt::EditRole:
    {
        switch (column) {
        case 0:{
            return Qt::NoTextInteraction;
        }
        case 1:{
            return Qt::NoTextInteraction;
        }
        case 2:{
            return Qt::TextEditable;
        }
        }
    }
    case Qt::DecorationRole:{

        switch (column) {
        case 0:{

            return  QVariant();
        }
        case 1:{
            return ItemTypeIcons.value(item->data(column).toString());
        }
        case 2:{
            return  QVariant();
        }

        }
    }
    case Qt::CheckStateRole:{
        switch (column) {
        case 0:{
            auto inory=qvariant_cast<EditData>(index.siblingAtColumn(2).data());
            switch (inory.m_type) {
            case EditData::item:{
                return inory.m_pItem->isSelected()?2:0;
            }
            case EditData::layer:{
                return  inory.m_pLayer->getCheckedState();
            }
            case EditData::program:{
                return  inory.m_pProgram->getSelectedState();
            }
            default:{
                return 0;
            }
            }

        }
        case 1:{
            return  QVariant();
        }
        case 2:{
            return  QVariant();
        }

        }
    }
    case Qt::TextAlignmentRole:{

        switch (column) {
        case 0:{
            return  QVariant();
        }
        case 1:{
            return  QVariant();
        }
        case 2:{
            return  Qt::AlignCenter;
        }

        }
    }
    case Qt::TextColorRole:{
        EditData starRating = qvariant_cast<EditData>(index.siblingAtColumn(2).data());
        if(starRating.m_type==EditData::layer){
            return starRating.m_pLayer->isActiveLayer()?QColor(Qt::darkGreen):QColor(Qt::black);
        }
        return QColor(Qt::black);
    }
    case Qt::SizeHintRole:{
        return QVariant();
    }
    case Qt::FontRole:{
        switch (column) {
        case 0:{
            return QFont("Calibri", 11,QFont::Light);
        }
        case 1:{
            return QFont("Calibri", 12,QFont::Normal);
        }
        case 2:{
            return QFont("Calibri", 11,QFont::Light);
        }

        }

    }
    default:
        return QVariant();
    }
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return nullptr;

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    switch (index.column()) {
    case 0:{
        flags = flags | Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable;
    }
        break;
    case 1:{
        //flags = flags | Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled;
    }
        break;
    case 2:{
        flags |=Qt::ItemIsEditable;
    }
        break;
    default:
        break;
    }

    EditData data = qvariant_cast<EditData>(index.siblingAtColumn(2).data());
    if(data.GetProgramIndex(index)==pDataHandler->getActiveProgramIndex()){
        flags |=Qt::ItemIsEnabled;
    }else{
        flags^=Qt::ItemIsEnabled;
    }



    return flags;

}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}


QVariant TreeModel::headerData(int section, Qt::Orientation orientation,int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);
    return QVariant();
}


QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{

    if (parent.isValid() && parent.column() != 0){
        return QModelIndex();
    }

    TreeItem *parentItem = getItem(parent);
    if(parentItem->childCount()==0)
        return QModelIndex();
    TreeItem *childItem = parentItem->child(row);
    return createIndex(row, column, childItem);

}


bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());

    endInsertRows();

    return success;
}


QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}


bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

//bool TreeModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
//{
//    qDebug()<<sourceParent.data();
//    return false;
//}


int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

//QMimeData *TreeModel::mimeData(const QModelIndexList &indexes) const
//{
//    if (indexes.count() <= 0)
//    {//If the list of indexes is empty, or there are no supported MIME types,
//        //0 is returned rather than a serialized empty list.
//        return nullptr;
//    }


//    QByteArray itemData;
//    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

//    dataStream<<indexes.size();

//    for(const QModelIndex &index:indexes)
//    {
//        auto dk=index.siblingAtColumn(2).data().value<EditData>();
//        dataStream<<index.row()<<index.siblingAtColumn(0).data()<<index.siblingAtColumn(1).data()<<dk.m_type<<dk.m_isCheck<<dk.m_color;
//    }

//    QMimeData *data = new QMimeData;
//    data->setData("items", itemData);


//    return data;
//}

//bool TreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
//{
//    //    if (data->hasFormat("items"))
//    //    {
//    //        if(row!=-1&&column!=-1){
//    //            QByteArray itemData = data->data("items");
//    //            QDataStream dataStream(&itemData, QIODevice::ReadOnly);
//    //            int a,index,type;
//    //            bool isSub,Checked;
//    //            QColor color;
//    //            QVariant k,m;
//    //            dataStream >> a>> index>> k>>m>>type>>isSub>>Checked>>color;


//    //            insertRow(row,parent);


//    //            getItem(parent)->insertChildren(row,1,rootItem->columnCount());
//    //            getItem(parent)->child(row)->setData(0,k);
//    //            getItem(parent)->child(row)->setData(1,m);
//    //            getItem(parent)->child(row)->setData(2,QVariant::fromValue(EditData(type,isSub,Checked,color)));


//    //            removeRow(index,parent);
//    //            getItem(parent)->removeChildren(index,1);

//    //        }

//    //    }
//    return 1;
//}

//QStringList TreeModel::mimeTypes() const
//{
//    QStringList types;

//    types << "items";

//    return types;
//}




bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    switch (role) {
    case Qt::CheckStateRole:{
        auto inory=qvariant_cast<EditData>(index.siblingAtColumn(2).data());
        switch (inory.m_type) {
        case EditData::item:{
            inory.m_pItem->setSelected(value.toInt());
        }
            break;
        case EditData::layer:{
            inory.m_pLayer->setSelected(value.toInt());
        }
            break;
        case EditData::program:{
            inory.m_pProgram->setSelected(value.toInt());
        }
            break;
        default:{
        }
        }
        ptrView->update();
        pDataHandler->getActiveProgram()->getViewPtr()->getScene()->handlerSelectionChange();
        return true;
    }
    case Qt::EditRole:{
        return getItem(index)->setData(index.column(), value);
    }
    default:{
        return false;
    }
    }

    //    if (result){
    //        emit dataChanged(index, index);
    //        QVariant col2= index.siblingAtColumn(2).data();
    //        if (col2.canConvert<EditData>()) {
    //            emit programChanged(qvariant_cast<EditData>(col2).getIndex(index).programIndex);
    //        }
    //    }
    //     qDebug()<<QDateTime::currentDateTime().msecsTo(time);
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}



void TreeModel::newProgram(QString Programname,ProgramData *pprogram)
{
    beginResetModel();

    QList<TreeItem*> parents;
    parents << rootItem;

    TreeItem *parent = parents.last();
    parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());


    parent->child(parent->childCount() - 1)->setData(0, Programname);
    parent->child(parent->childCount() - 1)->setData(1, ItemTypeNames.value(EditData::program));

    parent->child(parent->childCount() - 1)->setData(2, QVariant::fromValue(EditData(EditData::program,pprogram)));


    parents << parents.last()->child(parents.last()->childCount()-1);
    parent = parents.last();

    qint32 rootItemNum= rootItem->columnCount();

    parent->insertChildren(parent->childCount(), 1, rootItemNum);
    parent->child(parent->childCount() - 1)->setData(0, "0");
    parent->child(parent->childCount() - 1)->setData(1, ItemTypeNames.value(EditData::layer));
    parent->child(parent->childCount() - 1)->setData(2, QVariant::fromValue(EditData(EditData::layer,pprogram,&pprogram->getViewPtr()->getScene()->getLayer().first())));


    endResetModel();
}

void TreeModel::addProgram(QString Programname, ProgramData *pprogram)
{
    beginResetModel();
    QList<TreeItem*> parents;
    parents << rootItem;


    TreeItem *parent = parents.last();
    parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());


    parent->child(parent->childCount() - 1)->setData(0, Programname);
    parent->child(parent->childCount() - 1)->setData(1, ItemTypeNames.value(EditData::program));

    parent->child(parent->childCount() - 1)->setData(2, QVariant::fromValue(EditData(EditData::program,pprogram)));

    parents << parents.last()->child(parents.last()->childCount()-1);
    parent = parents.last();

    addProgramData(parent,pprogram);

    endResetModel();

}



void TreeModel::closeProgram(int index)
{
    beginResetModel();
    TreeItem*lolita=rootItem->child(index);
    lolita->removeChildren(0,rootItem->child(index)->childCount());
    rootItem->removeChildren(index,1);
    endResetModel();
}

void TreeModel::refreshData(int index)
{
    beginResetModel();
    TreeItem*lolita=rootItem->child(index);
    lolita->removeChildren(0,rootItem->child(index)->childCount());
    addProgramData(lolita,pDataHandler->getActiveProgram());
    endResetModel();
}

void TreeModel::DeleteLayer(qint32 index)
{

}

void TreeModel::DeleteItem(qint32 index)
{

}

void TreeModel::addProgramData(TreeItem *inory,ProgramData*pprogram)
{
    QList<TreeItem*> parents;
    parents<<inory;
    TreeItem* parent=parents.last();


    qint32 rootItemNum= rootItem->columnCount();


    for(auto &k:pprogram->getData()){

        //add layer data

        parent->insertChildren(parent->childCount(), 1, rootItemNum);
        parent->child(parent->childCount() - 1)->setData(0, k.name);
        parent->child(parent->childCount() - 1)->setData(1, ItemTypeNames.value(EditData::layer));
        parent->child(parent->childCount() - 1)->setData(2, QVariant::fromValue(EditData(EditData::layer,pprogram,&k)));
        parents << parents.last()->child(parents.last()->childCount()-1);
        parent = parents.last();

        auto sizei=k.getItems().size();
        for(qint32 i=0;i<sizei;++i){

            parent->insertChildren(parent->childCount(), 1, rootItemNum);
            parent->child(parent->childCount() - 1)->setData(0, tr("object-")+QString::number(i));

            auto dfgdg= k.getItems().at(i);
            parent->child(parent->childCount() - 1)->setData(1, ItemTypeNames.value(dfgdg->type()));
            parent->child(parent->childCount() - 1)->setData(2, QVariant::fromValue(EditData(EditData::item,pprogram,&k,dfgdg)));

        }
        parents.pop_back();
        parent = parents.last();

    }
    parents.pop_back();
}
