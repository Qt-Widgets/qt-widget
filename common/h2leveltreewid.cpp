#include "h2leveltreewid.h"
#include "hcommon.h"
#include <QHeaderView>

H2LevelTreeWid::H2LevelTreeWid(QWidget *parent) : QTreeWidget(parent)
{
    setSortingEnabled(false);
    setDragEnabled(false);
    setAutoScroll(true);
    setColumnCount(colCount);
    header()->setHidden(1);
    setRootIsDecorated(true);

    header()->setSectionsClickable(false);
    header()->setFont(QFont("calibri", 12));
    header()->setDefaultAlignment(Qt::AlignCenter);
    header()->setSectionResizeMode(QHeaderView::Interactive);

    //header()->setMinimumSectionSize(120);
    header()->setStretchLastSection(true);
    header()->setHighlightSections(false);
    header()->setSectionsMovable(false);

    QSettings settings("AOC", "inno");
    header()->restoreState(settings.value("inory/treeViewHeaderStateFlow").toByteArray()/*header()->saveState()*/);

    setStyleSheet(HSheetStyle::H_TABLEVIEW2);


    connect(this,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));
    connect(this,&QTreeWidget::itemDoubleClicked,this,&H2LevelTreeWid::treeItemDoubleClick);
}

H2LevelTreeWid::~H2LevelTreeWid()
{
    QSettings settings("AOC", "inno");
    settings.setValue("inory/treeViewHeaderStateFlow", /*ptrTreeView->saveGeometry()*/header()->saveState());
}

void H2LevelTreeWid::AddAllItems(const HTextIcons &data)
{
    QTreeWidgetItem* parent=nullptr;
    for(const auto &k:data){
        switch (k.mm_Flag) {
        case -1:{
            parent=parent->parent();
        }
            break;
        case 0:{
            parent=creatListTreeItem(k,this,true);
        }
            break;
        case 1:{
            parent=creatListTreeItem(k,parent->parent(),false);
        }
            break;
        case 2:{
            parent=creatListTreeItem(k,parent,false);
        }
            break;
        default:
            break;
        }
    }
}

HTextIcons H2LevelTreeWid::GetAllItems()
{

    HTextIcons list;
    if(topLevelItemCount()){
        QTreeWidgetItemIterator it(this);
        auto parent=topLevelItem(0);
        while (*it) {
            HTextIcon item;

            item.type=(*it)->type();
            item.text=(*it)->text(0);
            item.text1=(*it)->text(1);
            item.icon=(*it)->icon(0);
            item.icon1=(*it)->icon(1);
            item.userData=(*it)->data(0,Qt::UserRole);
            item.checkable=(*it)->flags()&Qt::ItemIsUserCheckable;
            item.checkedState=(*it)->checkState(0);
            item.editable=(*it)->flags()&Qt::ItemIsEditable;
            item.enable=(*it)->flags()&Qt::ItemIsEnabled;
            item.selectable=(*it)->flags()&Qt::ItemIsSelectable;

            if((*it)->parent()){

                qint32 hghg=0;
                while(parent!=(*it)->parent()){
                    ++hghg;
                    parent=parent->parent();
                }
                for(qint32 k=0;k<hghg;++k){
                   item.mm_Flag=-1;
                   list.append(item);
                }
                item.mm_Flag=parent==(*it)?1:2;
                list.append(item);

            }else{

                item.mm_Flag=0;
                list.append(item);
            }
              parent=(*it);
            ++it;
        }
    }
    return list;
}



void H2LevelTreeWid::AddRootItem(const HTextIcon &data)
{
    m_cache.clear();
    m_parItem=nullptr;
    m_cache.append(creatListTreeItem(data,this,true));
    addTopLevelItem(m_cache.first());
}



void H2LevelTreeWid::AddChildItem(const HTextIcon &data)
{
    if(m_cache.isEmpty())
        return;
    auto item=creatListTreeItem(data, m_cache.last(),false);
    //m_cache.last()->addChild(item);
    m_cache.append(item);
}



void H2LevelTreeWid::AddChildItems(const HTextIcons &data)
{
    if(m_cache.isEmpty())
        return;
    auto last=m_cache.last();
    QList<QTreeWidgetItem*> items;
    for(const auto&k:data){
        items<<creatListTreeItem(k,last,false);
    }
    // last->addChildren(std::move(items));
    m_cache.append(items.last());
}

void H2LevelTreeWid::EndAddChildItem()
{
    if(m_cache.isEmpty())
        return;
    m_cache.removeLast();
}

void H2LevelTreeWid::InsertItemAtSelection(const HTextIcon &data)
{
    auto sitems=selectedItems();
    if(sitems.isEmpty()||sitems.first()->parent()==nullptr)
        return;
    sitems.first()->parent()->insertChild(GetSelectedItemIndex(),creatListTreeItem(data,nullptr,false));

}

void H2LevelTreeWid::InsertItemsAtSelection(const HTextIcons &data)
{
    auto sitems=selectedItems();
    if(sitems.isEmpty()||sitems.first()->parent()==nullptr)
        return;
    QList<QTreeWidgetItem*> items;
    for(const auto &k:data){
        items<<creatListTreeItem(k,nullptr,false);
    }
    sitems.first()->parent()->insertChildren(GetSelectedItemIndex(),items);
}

void H2LevelTreeWid::InsertChildItemAtSelection(const HTextIcon &data)
{
    auto sitems=selectedItems();
    if(sitems.isEmpty())
        return;
    creatListTreeItem(data,sitems.first(),false);

}

void H2LevelTreeWid::InsertChildItemsAtSelection(const HTextIcons &data)
{
    auto sitems=selectedItems();
    if(sitems.isEmpty())
        return;

    QList<QTreeWidgetItem*> items;
    for(const auto &k:data){
        items<<creatListTreeItem(k,sitems.first(),false);
    }
    //sitems.first()->insertChildren(GetSelectedItemIndex(),items);
}



void H2LevelTreeWid::RemoveSelectedItems()
{
    for(const auto &k:selectedItems()){
        if(k->parent())
            k->parent()->removeChild(k);
        else{
            delete  k;
        }
    }
}

void H2LevelTreeWid::RemovechildItems(QTreeWidgetItem *parent, qint32 start, qint32 count)
{
    QList<QTreeWidgetItem *> childs;

    for(qint32 k=start;k<count;++k){
        childs<<parent->child(k);
    }

    for(const auto &k:childs){
        parent->removeChild(k);
    }
}



void H2LevelTreeWid::SetColNames(const QStringList &names)
{
    if(names.isEmpty())
        return;
    colCount=names.size();
    header()->setHidden(colCount==1);
    setColumnCount(colCount);
    setHeaderLabels(names);
}


HTextIcons H2LevelTreeWid::GetSelectedItems()
{
    HTextIcons items;
    HTextIcon item;
    for(const auto &k:selectedItems()){
        item.icon=k->icon(0);
        item.text=k->text(0);
        item.type=k->type();
        items.append(item);
    }
    return items;
}



HTreeItem H2LevelTreeWid::GetAllCheckedData()
{
    HTreeItem list;
    QTreeWidgetItemIterator it(this);
    while (*it) {
        if ((*it)->checkState(0)>Qt::Unchecked){
            list<<QPair<int,QString>((*it)->type(),(*it)->text(0));
        }
        ++it;


    }
    return list;
}

HTreeItem H2LevelTreeWid::GetAll2ChildCheckedData()
{
    HTreeItem list;
    QTreeWidgetItemIterator it(this);
    while (*it) {
        if ((*it)->checkState(0)==Qt::Checked&&(*it)->childCount()==0){
            list<<QPair<int,QString>((*it)->type(),(*it)->text(0));
        }
        ++it;
    }
    return list;
}

qint32 H2LevelTreeWid::GetSelectedItemIndex()
{
    auto sitems=selectedItems();
    if(sitems.isEmpty()||sitems.first()->parent()==nullptr)
        return -1;
    return sitems.first()->parent()->indexOfChild(sitems.first());
}

//HTreeItemIndex H2LevelTreeWid::GetSelectedItemIndexs(QTreeWidgetItem *itemm)
//{
//    HTreeItemIndex list;

//    auto mitem=itemm;


//    while (mitem->parent()){
//        list.prepend(mitem->parent()->indexOfChild(mitem));
//        mitem=mitem->parent();
//    }

//    auto rootItem=mitem;
//    while (rootItem->parent()){
//        rootItem=mitem->parent();
//    }

//    list.prepend(indexOfTopLevelItem(rootItem));

//    return list;

//}



void H2LevelTreeWid::treeItemChanged(QTreeWidgetItem *item, int col)
{
    Q_UNUSED(col);
    if(Qt::Checked == item->checkState(0)){
        int count = item->childCount();
        if (count > 0){
            for (int i = 0; i < count; i++){
                if(item->child(i)->flags()&Qt::ItemIsUserCheckable)
                    item->child(i)->setCheckState(0, Qt::Checked) ;
            }
            if(item->parent()){
                if(item->parent()->childCount()==1){
                    updateParentItem(item);
                }
            }
        }else{
            updateParentItem(item);
        }
    }else if(Qt::Unchecked == item->checkState(0)){
        int count = item->childCount();
        if (count > 0){
            for (int i = 0; i < count; i++){
                if(item->child(i)->flags()&Qt::ItemIsUserCheckable)
                    item->child(i)->setCheckState(0, Qt::Unchecked);
            }
            if(item->parent()){
                if(item->parent()->childCount()==1){
                    updateParentItem(item);
                }
            }
        }else{
            updateParentItem(item);
        }
    }else { // Qt::PartiallyChecked == item->checkState(0)

    }
}

void H2LevelTreeWid::treeItemDoubleClick(QTreeWidgetItem *item, int column)
{
    item->setData(0,Qt::UserRole,emit HTreeItemDoubleClicked(item->type(),item->data(0,Qt::UserRole)));
}

void H2LevelTreeWid::updateParentItem(QTreeWidgetItem* item){


    QTreeWidgetItem *parent = item->parent();
    if (parent == nullptr){
        return;
    }

    int selectedCount = 0;
    int partSelectedCount = 0;
    int childCount = parent->childCount();
    for (int i = 0; i < childCount; i++){
        QTreeWidgetItem *childItem = parent->child(i);
        if (childItem->checkState(0) == Qt::Checked||childItem->checkState(0) == Qt::PartiallyChecked){
            selectedCount++;
        }
        if(childItem->checkState(0) == Qt::PartiallyChecked){
            partSelectedCount++;
        }
    }
    if (selectedCount <= 0){
        if(parent->flags()&Qt::ItemIsUserCheckable)
            parent->setCheckState(0, Qt::Unchecked);

    }else if(selectedCount > 0 && selectedCount < childCount){
        if(parent->flags()&Qt::ItemIsUserCheckable)
            parent->setCheckState(0, Qt::PartiallyChecked);
    }else if(selectedCount == childCount){
        if(parent->flags()&Qt::ItemIsUserCheckable){
            if(partSelectedCount==0){
                parent->setCheckState(0, Qt::Checked);
            }else{
                parent->setCheckState(0, Qt::PartiallyChecked);
            }
        }
    }
    updateParentItem(parent);

    //    changeFromUser = true;
}

QTreeWidgetItem *H2LevelTreeWid::creatListTreeItem(const HTextIcon &data, void *parent, bool isRoot)
{
    QTreeWidgetItem * item1;
    if(isRoot){
        item1=new QTreeWidgetItem(static_cast<QTreeWidget*>(parent),data.type);
    }else{
        item1=new QTreeWidgetItem(static_cast<QTreeWidgetItem*>(parent),data.type);
    }
    item1->setText(0,data.text);
    if(colCount==2){
        item1->setText(1,data.text1);
    }
    item1->setFlags((data.editable?Qt::ItemIsEditable:Qt::NoItemFlags) | (data.checkable?Qt::ItemIsUserCheckable:Qt::NoItemFlags)
                    |(data.enable?Qt::ItemIsEnabled:Qt::NoItemFlags)  | (data.selectable?Qt::ItemIsSelectable:Qt::NoItemFlags));
    if(data.checkable){
        item1->setCheckState(0,Qt::CheckState(data.checkedState));
    }
    item1->setIcon(0, data.icon);

    if(colCount==2){
        item1->setIcon(1,data.icon1);
    }
    item1->setData(0,Qt::UserRole,data.userData);

    return item1;
}
