#include "hgraphicsscene.h"

#include "hgraphicsview.h"


HGraphicsScene::HGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(x,y,width,height,parent),ptrView((HGraphicsView*)parent)
{
    layer.reserve(999);
    updateScene();
    //setMinimumRenderSize(1.0);
    connect(this,&HGraphicsScene::selectionChanged,this,&HGraphicsScene::handlerSelectionChange);
}

QPointF HGraphicsScene::getItemCenterPt(qint32 index)
{
    return  layer.at(activeLayer).getItems().at(index)->getCenter();
}

qint32 HGraphicsScene::getFirstHighlightItemIndex() const
{
    if(layer.at(activeLayer).getLayerHighlight()){
        return 0;
    }
    auto hghg=layer.at(activeLayer).getItems();
    qint32 size=hghg.size();
    for(qint32 k=0;k<size;++k){
        if(hghg.at(k)->getIsHighlight()){
            return ++k;
        }
    }
    return size;
}

void HGraphicsScene::transAllItems(const QTransform &trans)
{
    for(const auto&k:layer){
        for(const auto&m:k.getItems()){
            m->setPos(trans.map(m->pos()));
            m->transLocationPts(trans);
        }
    }
    update();
}

void HGraphicsScene::setMMouseVisible(bool visible)
{
    mouseItem->setVisible(visible);
}


void HGraphicsScene::setMMousePos(const QPointF &pos)
{
    mouseItem->setPos(pos);
}

double HGraphicsScene::getMMouseSize()
{
    return mouseItem->getMouseSize();
}

void HGraphicsScene::updateScene()
{

    clear();

    selectedRectItem=new HGraphicsSelectRectItem;
    backgrounpRect=new HGraphicsBackground(QRectF(-300,-300,600,600));

    cameraPosItem=new HGraphicsVisionShow;
    paintCacheItem=new HGraphicsDrawCacheItem;
    mouseItem=new HGraphicsMouse;

    addItem(selectedRectItem);
    addItem(backgrounpRect);
    addItem(cameraPosItem);
    addItem(paintCacheItem);
    addItem(mouseItem);

    for(const auto&k:layer){
        for(const auto&m:k.getItems()){
            addItem(m);
            m->installSceneEventFilter(mouseItem);
        }
    }
    update();
}

void HGraphicsScene::deleteSelectedItems()
{
    for(const auto&k:selectedItems()){
        removeItem(k);
        for(auto&m:layer){
            m.getItems().removeOne((HGraphicsItem *)k);
        }
    }
    update();
}

void HGraphicsScene::clearItems()
{
    paintCacheItem->resetData();
    for(const auto&k:layer){
        for(const auto&m:k.getItems()){
            removeItem(m);
        }
    }
    layer.clear();
}

void HGraphicsScene::clearSelectedItems()
{
    clearSelection();
    handlerSelectionChange();
}

void HGraphicsScene::deleteItemInActive(HGraphicsItem *item)
{
    layer[activeLayer].deleteItem(this,item);
}

void HGraphicsScene::deleteItem(qint32 layerIndex, HGraphicsItem *item)
{
    layer[layerIndex].deleteItem(this,item);
}

void HGraphicsScene::deleteItem(const QString &layerName, HGraphicsItem *item)
{
    qint32 index=layer.indexOf(LayerInfo(layerName));
    layer[index].deleteItem(this,item);
}

void HGraphicsScene::deleteItem(qint32 layerIndex, qint32 itemIndex)
{
    layer[layerIndex].deleteItem(this,itemIndex);
}

void HGraphicsScene::deleteItem(const QString &layerName, qint32 itemIndex)
{
    qint32 index=layer.indexOf(LayerInfo(layerName));
    layer[index].deleteItem(this,itemIndex);
}


qint32 HGraphicsScene::deleteItem(HGraphicsItem *item)
{
    qint32 size=layer.size();
    for(qint32 k=0;k<size;++k){
        if(layer[k].getItems().contains(item)){
            layer[k].deleteItem(this,item);
            return k;
        }

    }
    return -1;
}

qint32 HGraphicsScene::deleteItem(QGraphicsItem *item)
{
    return deleteItem((HGraphicsItem *)item);
}

void HGraphicsScene::deleteItemsInActive(const QList<QPair<qint32, HGraphicsItem *> > &items)
{
    for(const auto&k:items){
        if(layer[activeLayer].getItems().removeOne(k.second)){
            this->removeItem(k.second);
            //layer[activeLayer].getItems().removeAt(k.first);
        }
    }
}

void HGraphicsScene::deleteItems(qint32 layerIndex, const QMap<qint32, HGraphicsItem *> &items)
{
    for(const auto&k:items){
        if(layer[layerIndex].getItems().removeOne(k)){
            this->removeItem(k);
        }
    }
}

void HGraphicsScene::addHItem(HGraphicsItem *item)
{

    layer[activeLayer].addItem(this,item);
    item->installSceneEventFilter(mouseItem);
}


void HGraphicsScene::addHItem(qint32 layerIndex, HGraphicsItem *item)
{

    layer[layerIndex].addItem(this,item);
    item->installSceneEventFilter(mouseItem);
}

void HGraphicsScene::addHItem(const QString &layerName, HGraphicsItem *item)
{

    qint32 index=layer.indexOf(LayerInfo(layerName));
    layer[index].addItem(this,item);
    item->installSceneEventFilter(mouseItem);
}

void HGraphicsScene::addHItems(const QList<HGraphicsItem *> &items)
{

    layer[activeLayer].addItems(this,items);
    for(const auto &k:items){
        k->installSceneEventFilter(mouseItem);
    }
}


void HGraphicsScene::addHItems(qint32 layerIndex, const QList<HGraphicsItem *> &items)
{

    layer[layerIndex].addItems(this,items);
    for(const auto &k:items){
        k->installSceneEventFilter(mouseItem);
    }
}

void HGraphicsScene::addHItems(const QString &layerName, const QList<HGraphicsItem *> &items)
{

    qint32 index=layer.indexOf(LayerInfo(layerName));
    layer[index].addItems(this,items);
    for(const auto &k:items){
        k->installSceneEventFilter(mouseItem);
    }
}


void HGraphicsScene::insertItem(qint32 itemIndex, HGraphicsItem *item)
{

    layer[activeLayer].insertItem(this,itemIndex,item);
    item->installSceneEventFilter(mouseItem);
}

void HGraphicsScene::insertItem(qint32 layerIndex, qint32 itemIndex,HGraphicsItem *item)
{

    layer[layerIndex].insertItem(this,itemIndex,item);
    item->installSceneEventFilter(mouseItem);
}

void HGraphicsScene::insertItem(const QString &layerName, qint32 itemIndex,HGraphicsItem *item)
{

    qint32 index=layer.indexOf(LayerInfo(layerName));
    layer[index].insertItem(this,itemIndex,item);
    item->installSceneEventFilter(mouseItem);
}

void HGraphicsScene::insertItems(qint32 itemIndex, const QList<HGraphicsItem *> &items)
{

    layer[activeLayer].insertItems(this,itemIndex,items);
    for(const auto &k:items){
        k->installSceneEventFilter(mouseItem);
    }
}

void HGraphicsScene::insertItems(qint32 layerIndex,qint32 itemIndex, const QList<HGraphicsItem *>&items)
{

    layer[layerIndex].insertItems(this,itemIndex,items);
    for(const auto &k:items){
        k->installSceneEventFilter(mouseItem);
    }
}

void HGraphicsScene::insertItems(qint32 layerIndex, const QMap<qint32, HGraphicsItem *> &items)
{
    auto i = items.constBegin();
    while (i != items.constEnd()) {

        layer[layerIndex].insertItem(this,i.key(),i.value());
        i.value()->installSceneEventFilter(mouseItem);
        ++i;
    }
}

void HGraphicsScene::insertItems(const QString &layerName, qint32 itemIndex,const QList<HGraphicsItem *>&items)
{

    qint32 index=layer.indexOf(LayerInfo(layerName));
    layer[index].insertItems(this,itemIndex,items);
    for(const auto &k:items){
        k->installSceneEventFilter(mouseItem);
    }
}


void HGraphicsScene::addLayer(const LayerInfo &info)
{
    layer.append(info);
}

void HGraphicsScene::groupItems(const QList<QGraphicsItem *> &items)
{

}

QList<QGraphicsItem *> HGraphicsScene::getSelectedItems() const
{
    return this->selectedItems();
}

QMap<qint32, QMap<qint32,HGraphicsItem*> > HGraphicsScene::getSelectedItemsIndexs() const
{
    QMap<qint32, QMap<qint32,HGraphicsItem*> > indexs;
    for(const auto&k:this->selectedItems()){
        for(qint32 m=0;m<layer.size();++m){
            auto index=layer.at(m).getItems().indexOf((HGraphicsItem*)k);
            if(index!=-1){
                indexs[m][index]=(HGraphicsItem*)k;
                break;
            }
        }
    }
    return indexs;
}

void HGraphicsScene::handlerSelectionChange()
{

    //    for(const auto&k:selectedItems()){
    //        k->setParentItem(selectedRectItem);
    //    }
    //    selectedRectItem->setRect(selectedRectItem->childrenBoundingRect());
    //    for(const auto&k:selectedRectItem->childItems()){
    //        k->setParentItem(nullptr);
    //    }
    //    ptrView->update();

    emit ptrView->selectionChanged();
}

void HGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void HGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    auto mousePos=mouseEvent->scenePos();
    mouseItem->setPos(mousePos);
    QGraphicsScene::mouseMoveEvent(mouseEvent);
    emit ptrView->mouseMoved(mouseItem->pos());
    emit sceneMouseMoved(mouseEvent,mouseItem->pos());
}

void HGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

    //QPointF mousePressedPos=mouseEvent->scenePos();
    //mouseItem->setPos(mousePressedPos);


    //    setSelectionArea(mouseItem->shape(),Qt::IntersectsItemShape);
    //    QGraphicsItem *item=nullptr;
    //    double dis=LLONG_MAX;
    //    for(const auto&k:selectedItems()){
    //        k->setSelected(false);
    //        auto dddd=((HGraphicsItem*)k)->getDistanceToShape(mouseItem->pos());
    //        if(dddd<dis){
    //          dis= dddd;
    //          item=k;
    //        }
    //    }
    //    if(item){
    //      item->setSelected(true);
    //    }

    //    auto mk=items(mouseItem->shape(),Qt::IntersectsItemShape);
    //    if(!mk.isEmpty()){
    //        mk.first()->setSelected(true);
    //    }
    //    qDebug()<<mk;

    emit ptrView->mousePressed(mouseItem->pos());
    if(m_mousePressMode){
        emit sceneMousePressed(mouseEvent,mouseItem->pos());
    }
    //itemAt()

}

void HGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
    emit sceneMouseReleased(mouseEvent,mouseItem->pos());

}

void HGraphicsScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent)
{
    QGraphicsScene::wheelEvent(wheelEvent);
}



bool LayerInfo::isActiveLayer() const
{
    return ptrScene->getActiveLayerDataPtr()==this;
}
