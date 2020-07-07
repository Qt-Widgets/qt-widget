#include "tablemodel.h"
#include <QtWidgets>
#include <QMessageBox>
#include "Src/Motion/hiodevicecontroller.h"


TableModel::TableModel(QObject *parent): QAbstractTableModel(parent)
{
    headData<<tr("Index")<< tr("Input-Name")<<tr("Input-State")<<tr("Output-Name")<<tr("Output")<<tr("Output-State");

    ptrIODevice=HIODeviceController::GetInstance();
}

TableModel::~TableModel()
{

}

int TableModel::columnCount(const QModelIndex & /* parent */) const
{
    return headData.size();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int column=index.column();
    int row=index.row();

    switch (role) {
    case Qt::DisplayRole:
    {
        switch (column) {
        case ColumnType::index:{
            return row;
        }
        case ColumnType::inputName:{

            return ptrIODevice->GetInputNames().value(row);
        }
        case ColumnType::input:{

            if(row<ptrIODevice->GetInputNum()){
                ulong gf=0;
                return ptrIODevice->GetInputPort(row)->GetInputBit(gf)?-1:qint32(gf);
            }
            //return ptrGtMotion->GetInput(4,row);
             return -1;
        }

        case ColumnType::outputName:{
            return ptrIODevice->GetOutputNames().value(row);
        }
        case ColumnType::output:{

        }
            break;
        case ColumnType::outState:{

            if(row<ptrIODevice->GetOutputNum()){
                ulong gf=0;
                return ptrIODevice->GetOutputPort(row)->GetOutputBit(gf)?-1:qint32(gf);
            }
             return -1;
        }
        }
        return QVariant();

    }
    case Qt::EditRole:
    {

    }
        break;
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

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return nullptr;
    if(index.column()==4){
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    }
    return  QAbstractItemModel::flags(index);
}



QVariant TableModel::headerData(int section, Qt::Orientation orientation,int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return headData.value(section);
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



int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return   qMax(ptrIODevice->GetInputNum(),ptrIODevice->GetOutputNum());
}


bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;
    emit dataChanged(index, index);
    return true;
}

