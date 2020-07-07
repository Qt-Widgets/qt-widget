#include "htableviewmodel.h"

#include <QMessageBox>



HTableViewModel::HTableViewModel(QObject *parent, const QStringList &headNames): QAbstractTableModel(parent),headData(headNames)
{
    m_defaultAddData.resize(headNames.size());
}

HTableViewModel::~HTableViewModel()
{

}

void HTableViewModel::setHeaderNames(const HVariantList &headNames)
{
    beginResetModel();
    clearAllRows();
    headData=headNames.toStringList_h();
    m_defaultAddData.resize(headNames.size());
    endResetModel();
}

QVariant HTableViewModel::getUniqueKey(qint32 row) const
{
    return getData(row,uniqueCol);
}

bool HTableViewModel::isUniqueKeyExist(const QVariant &key) const
{
    for(const auto&k:m_data){
        if(k.at(uniqueCol)==key)
            return true;
    }
    return false;
}

void HTableViewModel::clearAllRows()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

QVariant HTableViewModel::deleteRow(qint32 row)
{
    QVariant key;
    if(row<0||row>m_data.size()-1){
        return key;
    }
    if(uniqueCol!=-2)
        key=m_data.at(row).at(uniqueCol);
    removeRow(row);
    return key;
}

void HTableViewModel::resizeDatas(qint32 rowCount)
{
    m_data.clear();
    for(qint32 k=0;k<rowCount;++k){
      m_data.append(m_defaultAddData);
    }
}

void HTableViewModel::setData(qint32 row,qint32 column,const QVariant &data)
{
    beginResetModel();
    setData(index(row,column),data);
    endResetModel();
}

void HTableViewModel::setDatasAtRow(qint32 row, const HVariantList &datas)
{
    if(row<0||row>m_data.size()-1){
        return;
    }
    beginResetModel();
    m_data[row]=datas.toVariantVector_h();
    m_data[row].resize(headData.size());
    endResetModel();
}

void HTableViewModel::setDatasAtCol(qint32 column, const HVariantList &datas)
{
    beginResetModel();
    for(qint32 k=0;k<m_data.size();++k){
        m_data[k][column]=datas.value(k);
    }
    endResetModel();
}


void HTableViewModel::setAllDatasUseRow(const HVariantList &datas)
{
    if(datas.isEmpty()){
        beginResetModel();
        m_data.clear();
        endResetModel();
        return;
    }
    beginResetModel();
    m_data.clear();
    qint32 colcount=headData.size();
    m_data.resize(datas.size()/colcount);
    qint32 kh=0;
    for( auto &k:m_data){
        for(qint32 m=0;m<colcount;++m){
            k<<datas.at(kh);
            ++kh;
        }
    }
    endResetModel();
}

void HTableViewModel::setAllDatasUseCol(const HVariantList &data, qint32 rowCount)
{
    beginResetModel();
    qint32 colcount=headData.size();
    m_data.clear();
    m_data.resize(rowCount==-1?(data.size()/colcount):rowCount);

    qint32 mk=0;

    for(qint32 k=0;k<colcount;++k){
        for(auto &k:m_data){
            k<<data.value(mk);
            ++mk;
        }

    }

    endResetModel();
}

QVariant HTableViewModel::getData(qint32 row, qint32 column) const
{
    return m_data.value(row).value(column);
}

HVariantList HTableViewModel::getDatasAtCol(qint32 col) const
{
    HVariantList list;

    for(const auto &k:m_data){
        if(k.at(col).canConvert<QStringList>()){
            auto dddddd=k.at(col).toStringList();
            if(dddddd.isEmpty()){
                list<<QVariant();
            }else{
                list<<dddddd.first();
            }
        }else{
            list<<k.at(col);
        }
    }
    return list;
}

HVariantList HTableViewModel::getDatasAtRow(qint32 row) const
{
    HVariantList list;

    for(const auto &k:m_data.value(row)){
        list<<k;
    }
    return list;
}

HVariantList HTableViewModel::getAllDatasUseRow() const
{
    HVariantList list;

    for(const auto&k:m_data){
        for(const auto&m:k){
            list<<m;
        }
    }
    return list;
}

QVector<QStringList> HTableViewModel::getAllDatasUseCol() const
{
    QVector<QStringList> datasss;
    datasss.resize(columnCount());

   for(qint32 k=0;k<m_data.size();++k){

        for(qint32 m=0;m<datasss.size();++m){
            datasss[m]<<m_data[k].value(m).toString();
        }
    }
    return datasss;
}

void HTableViewModel::insertRowDatas(qint32 row, const HVariantList &data)
{
    qint32 size=m_defaultAddData.size();
    if(row==-1){
        row=m_data.size();
    }
    for(qint32 k=0;k<size;++k){
        m_defaultAddData[k]=data.value(k);
    }
    insertRow(row);

}


int HTableViewModel::columnCount(const QModelIndex & /* parent */) const
{
    return headData.size();
}

bool HTableViewModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(parent, position, position + rows - 1);
    m_data.remove(position,rows);
    endRemoveRows();
    return true;
}

bool HTableViewModel::insertRows(int row, int count, const QModelIndex &parent)
{

    if(count<=0||row>m_data.size())
        return false;
    beginInsertRows(parent, row, row + count - 1);

    for(qint32 k=0;k<count;++k){
        m_data.insert(row,m_defaultAddData);
    }
    endInsertRows();
    return true;
}

QVariant HTableViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    switch (role) {
    case Qt::DisplayRole:
    {
        switch (m_colTypes.value(index.column())) {
        case ColType::LineEdit:{
            return m_data.at(index.row())[index.column()];
        }
        case ColType::Combbox:{
            return m_data.at(index.row())[index.column()].toList().first();
        }
            break;
        case ColType::FileDialog:
            return m_data.at(index.row())[index.column()];
            break;
        default:
            return QVariant();
        }

    }
    case Qt::EditRole:
    {
        return m_data.at(index.row())[index.column()];
    }
    case Qt::DecorationRole:{

    }
        break;
    case Qt::CheckStateRole:{


    }
        break;
    case Qt::TextAlignmentRole:{
        return Qt::AlignCenter;
    }
    case Qt::TextColorRole:{
        return QColor(Qt::black);
    }
    case Qt::SizeHintRole:
        //return QSize(100,30);
        break;
    case Qt::FontRole:
        //        if(column==1)
        //            return QFont("Calibri", 12,QFont::Normal);
        //        else
        //            return QFont("Calibri", 11,QFont::Light);

        break;
    default:
        break;
    }

    return QVariant();

}

Qt::ItemFlags HTableViewModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return nullptr;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}



QVariant HTableViewModel::headerData(int section, Qt::Orientation orientation,int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return headData.at(section);
    return QVariant();
}




//void TableModel::setupModelData(TreeItem *rootItem)
//{
//    beginResetModel();
//    QSqlQuery query(db);
//    query.exec("SELECT COUNT(*) AS num FROM data_io");
//    query.next();
//    int num=query.value("num").toInt();
//    for(int k=0;k<num;++k){
//        rootItem->insertChildren(rootItem->childCount(), 1, rootItem->columnCount());
//        query.exec(QString("SELECT indexNum,inputName,input,type,typeValue,outputName,output,outputState FROM data_io WHERE indexNum='%1'").arg(k));
//        query.next();

//        rootItem->child(rootItem->childCount() - 1)->setData(ColumnType::index, query.value(0).toString());
//        rootItem->child(rootItem->childCount() - 1)->setData(ColumnType::inputName, query.value(1).toString());
//        rootItem->child(rootItem->childCount() - 1)->setData(ColumnType::input, query.value(2).toString());
//        rootItem->child(rootItem->childCount() - 1)->setData(ColumnType::type, query.value(3).toString());
//        rootItem->child(rootItem->childCount() - 1)->setData(ColumnType::typeValue, query.value(4).toString());
//        rootItem->child(rootItem->childCount() - 1)->setData(ColumnType::outputName, query.value(5).toString());
//        rootItem->child(rootItem->childCount() - 1)->setData(ColumnType::output, query.value(6).toString());
//        rootItem->child(rootItem->childCount() - 1)->setData(ColumnType::outState, query.value(7).toString());
//    }


//    endResetModel();
//}



int HTableViewModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return  m_data.size();
}


bool HTableViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    if(uniqueCol==index.column()){
        QVariantList list;
        for(const auto&k:m_data){
            list.push_back(k.at(uniqueCol));
        }
        list.removeAt(index.row());
        if(list.contains(value)){
            QMessageBox::warning(nullptr,tr("table error"),tr("use same unique key name is illegal!"));
            return false;
        }
    }
    m_data[index.row()][index.column()]=value;
    emit itemDataChanged(index.row(),index.column(),value);
    return true;
}
