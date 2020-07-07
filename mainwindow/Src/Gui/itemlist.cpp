#include "itemlist.h"
#include "ui_itemlist.h"
#include <QDebug>
#include <QSettings>
#include "Src/window/treeitem.h"
#include "Src/window/treemodel.h"
#include "Src/window/delegate.h"
#include "Src/window/htreeheaderview.h"
#include "Src/window/programhandler.h"
#include "hcommon.h"
#include "Src/window/editdata.h"
#include "Src/window/itemlistmenu.h"

#include "Src/painter/paintermainwidget.h"
#include "htreeview/h_treeview.h"
#include "h_processwidget.h"



ItemList* ItemList::m_pInstance = nullptr;

ItemList *ItemList::GetInstance(QWidget *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new ItemList(parent);
    }
    return m_pInstance;
}


ItemList::ItemList(QWidget *parent) :
    QWidget(parent)
{

    pProgramData=ProgramHandler::GetInstance();
    ptrProcessWidget=H_ProcessWidget::GetInstance();
    ptrPainterWidget=PainterMainWidget::GetInstance();
    ptrTreeView=new H_TreeView (this);

    ptrTreeView->setStyleSheet(HSheetStyle::H_TABLEVIEW2);

    //HTreeHeaderView*m_headerView = new HTreeHeaderView(this);
    //ptrTreeView->setHeader(m_headerView);


    TreeModel* model=new TreeModel(ptrTreeView,this);
    ptrTreeView->setModel(model);
    ptrTreeView->setDragEnabled(false);
    setContextMenuPolicy(Qt::CustomContextMenu);
    menu=new ItemListMenu(ptrTreeView,this);



    ptrTreeView->setItemDelegateForColumn(2,new MyButtonDelegate(this));


    connect(pProgramData,&ProgramHandler::programActived,this,[=](){
        ptrTreeView->selectionModel()->clearSelection();
        ptrTreeView->update();
    });

    connect(pProgramData,&ProgramHandler::programSelectionChanged,this,[=](){ptrTreeView->update();});

    connect(this, &QTreeView::customContextMenuRequested, this,[=](const QPoint &pos){
        QPoint pt=pos-QPoint(0,ptrTreeView->header()->height());
        QModelIndex curIndex = ptrTreeView->indexAt(pt);
        if(curIndex.column()==2||curIndex==ptrTreeView->rootIndex())
            return;
        QModelIndex index = curIndex.sibling(curIndex.row(),1);
        if (index.isValid()&&index.data().toString()=="layer"){
            //if(index.data()==tr("Program"))
            menu->setLayerIndex(index.row());
            menu->exec(QCursor::pos());
           // qDebug()<<index.row();
        }
    });


    connect(ptrTreeView,&QTreeView::expanded,this,[=](const QModelIndex &index){
        ptrTreeView->resizeColumnToContents(index.column());
    });



    connect(ptrTreeView->selectionModel(),&QItemSelectionModel::selectionChanged,this,[=](const QItemSelection &selected, const QItemSelection &deselected){
        if(this->underMouse()){

            for(const auto&k: deselected.indexes()){
                if(k.column()==2){
                    EditData starRating = qvariant_cast<EditData>(k.data());

                    switch (starRating.m_type) {
                    case EditData::program:{
                        starRating.m_pProgram->setHighlight(false);
                    }
                        break;
                    case EditData::layer:{
                        starRating.m_pLayer->setHighlight(false);
                    }
                        break;
                    case EditData::item:{
                        starRating.m_pItem->setHighlight(false);
                    }
                        break;
                    default:
                        break;
                    }
                }
            }

            for(const auto&k: selected.indexes()){
                if(k.column()==2){
                    EditData starRating = qvariant_cast<EditData>(k.data());
                    switch (starRating.m_type) {
                    case EditData::program:{
                        starRating.m_pProgram->setHighlight(true);
                    }
                        break;
                    case EditData::layer:{
                        starRating.m_pLayer->setHighlight(true);
                    }
                        break;
                    case EditData::item:{
                        starRating.m_pItem->setHighlight(true);
                    }
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    });


}

ItemList::~ItemList()
{


}



void ItemList::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    ptrTreeView->resize(this->size());
}














