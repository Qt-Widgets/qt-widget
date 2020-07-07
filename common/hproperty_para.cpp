#include "hproperty_para.h"
#include "hcommon.h"
#include "hmath.h"

#include "htableviewdelegate.h"

#include "htableviewmodel.h"
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QPushButton>





HProperty_para::HProperty_para(const QStringList &typesCol, qint32 userType, QWidget *parent, const QString &types, const QString & editables): QWidget(parent)
{
    setFont(MyFont1);

    ptrTableView =new QTableView(this);

    m_userType=userType;


    ptrTableModel=new HTableViewModel(this,typesCol);
    connect(ptrTableModel,&HTableViewModel::itemDataChanged,this,[=](qint32 row,qint32 column,const QVariant&data){
        emit dataChanged(row,column,data);
        emit dataChangedUseKeyName(ptrTableModel->getUniqueKey(row),column,data);
    });



    ptrTableView->setModel(ptrTableModel);


    ptrTableView->horizontalHeader()->setSectionsClickable(false);
    ptrTableView->horizontalHeader()->setFont(QFont("calibri", 12));
    ptrTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ptrTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);  // 定宽
    //ptrTableView->horizontalHeader()->resizeSections(QHeaderView::Interactive);

    //ptrTableView->horizontalHeader()->setMinimumSectionSize(120);
    ptrTableView->horizontalHeader()->setStretchLastSection(true);
    ptrTableView->horizontalHeader()->setHighlightSections(true);  //去选中黑体
    ptrTableView->horizontalHeader()->setSectionsMovable(false);
    ptrTableView->verticalHeader()->hide();


    //ptrTableView->setShowGrid(false);
    ptrTableView->setFocusPolicy(Qt::NoFocus);

    ptrTableView->setEditTriggers(QAbstractItemView::DoubleClicked/*| QAbstractItemView::SelectedClicked*/);
    ptrTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ptrTableView->setAlternatingRowColors(true);
    ptrTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ptrTableView->setStyleSheet(HSheetStyle::H_TABLEVIEW3);


    QVector<bool> editabledss;
    for(const auto&k:editables.split(',')){
        editabledss<<k.toInt();
    }
    if(editabledss.size()!=typesCol.size()){
        editabledss.clear();
        for(qint32 k=0;k<typesCol.size();++k){
            editabledss<<1;
        }
    }

    ptrTableDele=new HTableViewDelegate(this,editabledss);
    ptrTableView->setItemDelegate(ptrTableDele);

    QHBoxLayout *hLayout=new QHBoxLayout(this);
    hLayout->setMargin(0);

    hLayout->addWidget(ptrTableView,4);

    buttonLayout =new QVBoxLayout;
    buttonLayout->setContentsMargins(0,15,0,15);
    buttonLayout->setAlignment(Qt::AlignTop);
    hLayout->addLayout(buttonLayout);

    //ptrTableView->resizeColumnToContents(0);
    //ptrTableView->resizeColumnsToContents();


    connect(ptrTableView->selectionModel(),&QItemSelectionModel::selectionChanged,this,[=](){



        //if(this->underMouse()){

        selectedRows.clear();

        for (const QModelIndex& index:ptrTableView->selectionModel()->selectedRows()){
            selectedRows<<index.row();
        }
        H_Math::ShellSort(selectedRows);

        emit rowSelected(selectedRows);
        //}

    });


    connect(ptrTableView->selectionModel(),&QItemSelectionModel::currentRowChanged,this,[=](const QModelIndex &current, const QModelIndex &previous){
        emit currentRowChanged(current.row());
    });




    QVector<qint32> typesssf;
    for(const auto&k:types.split(',')){
        typesssf<<k.toInt();
    }

    if(typesssf.size()!=typesCol.size()){
        typesssf.clear();
        for(qint32 k=0;k<typesCol.size();++k){
            typesssf<<0;
        }
    }
    setColCtrTypes(typesssf);

}

HProperty_para::~HProperty_para()
{

}

void HProperty_para::addCtrlSpace(qint32 value)
{
    buttonLayout->addSpacing(value);
}

QPushButton *HProperty_para::addButton(const QString &str, const QIcon &icon)
{
    QPushButton *button=new QPushButton(str,this);
    button->setIcon(icon);
    button->setFocusPolicy(Qt::NoFocus);
    buttonLayout->addWidget(button);
    connect(button,&QPushButton::pressed,this,[=](){
        emit buttonHasPressed(str,m_userType);
    });
    return button;
}

void HProperty_para::setColCtrTypes(const QVector<qint32> &types)
{
    ptrTableDele->SetAllColDelegateTypes(types);
    ptrTableModel->setAllColModelTypes(types);
}

void HProperty_para::setUniqueColIndex(qint32 index)
{
    ptrTableModel->setUniqueColumn(index);
}

void HProperty_para::setAutoColWidth(bool enable)
{
    if(enable){
        ptrTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    }else{
        ptrTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
}


void HProperty_para::setHeaderColWidthFixed(bool enable)
{
    ptrTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode(enable));
}

void HProperty_para::setDefaultAddData(const HVariantList &data)
{
    ptrTableModel->setDefaultRowDatas(data);
}

void HProperty_para::resizeDatas(qint32 rowCount)
{
    ptrTableModel->resizeDatas(rowCount);
}

qint32 HProperty_para::getRowCount() const
{
    return ptrTableModel->rowCount();
}

HVariantList HProperty_para::getDataAtCol(qint32 col) const
{
    return ptrTableModel->getDatasAtCol(col);
}

HVariantList HProperty_para::getAllDataUseRow() const
{
    return ptrTableModel->getAllDatasUseRow();
}


void HProperty_para::insertRowData(const HVariantList &data)
{

    ptrTableModel->insertRowDatas(ptrTableView->currentIndex().row(),data);
    //ptrTableView->resizeColumnsToContents();
    ptrTableView->scrollToBottom();

}


bool HProperty_para::isUniqueDataExist(const QVariant &key)
{
    return ptrTableModel->isUniqueKeyExist(key);
}

QVariant HProperty_para::getCurrentUniqueKey() const
{
    return ptrTableModel->getUniqueKey(ptrTableView->currentIndex().row());
}



QVariant HProperty_para::getDataAt(qint32 row, qint32 colum)
{
    return  ptrTableModel->getData(row,colum);
}

qint32 HProperty_para::getCurrentRow()
{
    return ptrTableView->currentIndex().row();
}

void HProperty_para::addEmptyRow()
{
    ptrTableModel->insertRowDatas(ptrTableModel->rowCount()-1,QVariantList());
    //ptrTableView->resizeColumnsToContents();
    ptrTableView->scrollToBottom();
}

void HProperty_para::addRowData(const HVariantList &data)
{
    ptrTableModel->insertRowDatas(ptrTableModel->rowCount(),data);
    //ptrTableView->resizeColumnsToContents();
    ptrTableView->scrollToBottom();
}

void HProperty_para::addRowPointData(const QPointF &pt)
{
    QVariantList list;
    list<<QString::number(pt.x(),'f',5)<<QString::number(pt.y(),'f',5);
    ptrTableModel->insertRowDatas(ptrTableModel->rowCount(),list);
    //ptrTableView->resizeColumnsToContents();
    ptrTableView->scrollToBottom();
}

void HProperty_para::setDatasAtCol(qint32 col, const HVariantList &data)
{
    ptrTableModel->setDatasAtCol(col,data);
    //ptrTableView->resizeColumnsToContents();
}


void HProperty_para::setAllDatasUseRow(const HVariantList &data)
{
    ptrTableModel->setAllDatasUseRow(data);
    //ptrTableView->resizeColumnsToContents();
}

void HProperty_para::setAllDatasUseCol(const HVariantList &data, qint32 rowCount)
{
    ptrTableModel->setAllDatasUseCol(data,rowCount);
    //ptrTableView->resizeColumnsToContents();
}

void HProperty_para::setDatasAtCurrentRow(const HVariantList &data)
{
    ptrTableModel->setDatasAtRow(ptrTableView->currentIndex().row(),data);
}



QVariant HProperty_para::deleteCurrentRow()
{
    return ptrTableModel->deleteRow(ptrTableView->currentIndex().row());
}

HVariantList HProperty_para::deleteSelectedRows()
{
    HVariantList list;
    for(const auto &k:selectedRows){
        list<< ptrTableModel->deleteRow(k);
    }
    return list;
}


void HProperty_para::clearAllRows()
{
    ptrTableModel->clearAllRows();
}







