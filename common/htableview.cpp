#include "htableview.h"
#include "hmath.h"
#include "hcommon.h"
#include "htableviewdelegate.h"
#include "htableviewmodel.h"

#include <QHeaderView>

HTableView::HTableView(QWidget *parent) : QTableView(parent)
{

    ptrTableModel=new HTableViewModel(this);
    setModel(ptrTableModel);
    ptrTableDele=new HTableViewDelegate(this);
    setItemDelegate(ptrTableDele);
    connect(ptrTableModel,&HTableViewModel::itemDataChanged,this,[=](qint32 row,qint32 column,const QVariant&data){


    });

    connect(selectionModel(),&QItemSelectionModel::selectionChanged,this,[=](){
        //if(this->underMouse()){
        selectedRows.clear();
        for (const QModelIndex& index:selectionModel()->selectedRows()){
            selectedRows<<index.row();
        }
        H_Math::ShellSort(selectedRows);
        //}
    });

    connect(selectionModel(),&QItemSelectionModel::currentRowChanged,this,[=](const QModelIndex &current, const QModelIndex &previous){
        emit currentRowChanged(current.row(),userIndex);
    });


    horizontalHeader()->setSectionsClickable(false);
    horizontalHeader()->setFont(QFont("calibri", 12));
    horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);  // 定宽
    //horizontalHeader()->resizeSections(QHeaderView::Interactive);

    //horizontalHeader()->setMinimumSectionSize(120);
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setHighlightSections(true);  //去选中黑体
    horizontalHeader()->setSectionsMovable(false);
    verticalHeader()->hide();


    //setShowGrid(false);
    setFocusPolicy(Qt::NoFocus);

    setEditTriggers(QAbstractItemView::DoubleClicked/*| QAbstractItemView::SelectedClicked*/);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setStyleSheet(HSheetStyle::H_TABLEVIEW3);
}

void HTableView::setHeaderNames(const HVariantList &names)
{
    ptrTableModel->setHeaderNames(names);
}

void HTableView::setColCtrTypes(const QVector<qint32> &types)
{
    ptrTableDele->SetAllColDelegateTypes(types);
    ptrTableModel->setAllColModelTypes(types);
}

void HTableView::setColCtrlEditables(const QString &editables)
{
    QVector<bool> editabledss;
    for(const auto&k:editables.split(',')){
        editabledss<<k.toInt();
    }
    if(editabledss.size()!=ptrTableModel->columnCount()){
        editabledss.clear();
        for(qint32 k=0;k<ptrTableModel->columnCount();++k){
            editabledss<<1;
        }
    }
    ptrTableDele->setAllEditables(editabledss);
}

void HTableView::setUniqueColIndex(qint32 index)
{
    ptrTableModel->setUniqueColumn(index);
}


void HTableView::setAutoColWidth(bool enable)
{
    if(enable){
        horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    }else{
        horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
}

void HTableView::setDefaultAddData(const HVariantList &data)
{
    ptrTableModel->setDefaultRowDatas(data);
}

void HTableView::setUserIndex(qint32 index)
{
    userIndex=index;
}

void HTableView::setHighlightRowsUseFirstColumn(const HVariantList &names)
{
    ptrTableDele->setHighlightRowsUseFirstColumn(names);
}



qint32 HTableView::getRowCount() const
{
    return ptrTableModel->rowCount();
}

HVariantList HTableView::getDataAtCol(qint32 col) const
{
    return ptrTableModel->getDatasAtCol(col);
}

HVariantList HTableView::getDataAtRow(qint32 row) const
{
    return ptrTableModel->getDatasAtRow(row);
}

HVariantList HTableView::getDataAtCurrentRow() const
{
    return ptrTableModel->getDatasAtRow(currentIndex().row());
}

HVariantList HTableView::getAllDataUseRow() const
{
    return ptrTableModel->getAllDatasUseRow();
}

QVector<QStringList> HTableView::getAllDataUseCol() const
{
    return ptrTableModel->getAllDatasUseCol();
}


void HTableView::insertRowData(const HVariantList &data)
{

    ptrTableModel->insertRowDatas(currentIndex().row(),data);
    //resizeColumnsToContents();
    scrollToBottom();

}

void HTableView::addRowData(const HVariantList &data)
{
    ptrTableModel->insertRowDatas(ptrTableModel->rowCount(),data);
    //resizeColumnsToContents();
    scrollToBottom();
}

bool HTableView::isUniqueDataExist(const QVariant &key)
{
    return ptrTableModel->isUniqueKeyExist(key);
}

QVariant HTableView::getCurrentUniqueKey() const
{
    return ptrTableModel->getUniqueKey(currentIndex().row());
}



QVariant HTableView::getDataAt(qint32 row, qint32 colum)
{
    return  ptrTableModel->getData(row,colum);
}

qint32 HTableView::getCurrentRow()
{
    return currentIndex().row();
}

void HTableView::setCurrentRow(qint32 row, qint32 column)
{
    setCurrentIndex(ptrTableModel->index(row,column));
}

void HTableView::setDatasAtCol(qint32 col, const HVariantList &data)
{
    ptrTableModel->setDatasAtCol(col,data);

}

void HTableView::setAllDatasUseRow(const HVariantList &data)
{
    ptrTableModel->setAllDatasUseRow(data);
}

void HTableView::setAllDatasUseCol(const HVariantList &data, qint32 rowCount)
{
    ptrTableModel->setAllDatasUseCol(data,rowCount);
    //resizeColumnsToContents();
}

void HTableView::setDatasAtCurrentRow(const HVariantList &data)
{
    ptrTableModel->setDatasAtRow(currentIndex().row(),data);
}



QVariant HTableView::deleteCurrentRow()
{
    return ptrTableModel->deleteRow(currentIndex().row());
}

HVariantList HTableView::deleteSelectedRows()
{
    HVariantList list;
    for(const auto &k:selectedRows){
        list<< ptrTableModel->deleteRow(k);
    }
    return list;
}


void HTableView::clearAllRows()
{
    ptrTableModel->clearAllRows();
}

