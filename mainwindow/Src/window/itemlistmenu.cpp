#include "itemlistmenu.h"
#include "Src/Gui/itemlist.h"
#include "Src/window/programhandler.h"
#include "Src/Gui/htreeview/h_treeview.h"
#include "dialoglayerlopts.h"

ItemListMenu::ItemListMenu(H_TreeView *view, QWidget *parent) : QMenu(parent),ptrView(view)
{
    ptrProgramHandler=ProgramHandler::GetInstance();
//    addAction(tr("optimize"));
//    addSeparator();
//    QAction* actionParent_Operate = addAction(tr("operate"));
//    QMenu* subMenu_Operate = new QMenu(this);
//    subMenu_Operate->addAction(tr("group"));
//    subMenu_Operate->addSeparator();
//    subMenu_Operate->addAction(tr("new"));
//    subMenu_Operate->addAction(tr("copy"));
//    subMenu_Operate->addAction(tr("paste"));
//    subMenu_Operate->addSeparator();
//    subMenu_Operate->addAction(tr("cut"));
//    subMenu_Operate->addAction(tr("delete"),this,[=](){
//        for(const auto&k:ptrView->selectionModel()->selectedRows()){
//            qDebug()<<k.data().toString();
//        }
//    });
    //subMenu_Operate->setEnabled(false);
 //   actionParent_Operate->setMenu(subMenu_Operate);
 //   addSeparator();
    ptrDialogs=new DialogLayerLoPts(this);
    addAction(tr("set location pts"),this,[=](){
         ptrDialogs->show();
    });
    addSeparator();
//    addAction(tr("close"),[&](){

//    });
    connect(this,&QMenu::aboutToShow,this,[&](){

    });

}

void ItemListMenu::setLayerIndex(qint32 index)
{
    ptrDialogs->setLayerIndex(index);
}
