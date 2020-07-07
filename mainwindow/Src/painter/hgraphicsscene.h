#ifndef HGRAPHICSSCENE_H
#define HGRAPHICSSCENE_H

#include <QGraphicsScene>

#include "GraphicsItems/editableItems/hgraphicsarcitem.h"
#include "GraphicsItems/editableItems/hgraphicsellipseitem.h"
#include "GraphicsItems/editableItems/hgraphicslineitem.h"
#include "GraphicsItems/editableItems/hgraphicsitem.h"
#include "GraphicsItems/editableItems/hgraphicsitemgroup.h"
#include "GraphicsItems/editableItems/hgraphicspathitem.h"
#include "GraphicsItems/editableItems/hgraphicspixmapitem.h"
#include "GraphicsItems/editableItems/hgraphicspolygonitem.h"
#include "GraphicsItems/editableItems/hgraphicsrectitem.h"
#include "GraphicsItems/editableItems/hgraphicssimpletextitem.h"

#include "GraphicsItems/constantIems/hgraphicsselectrectitem.h"

#include "GraphicsItems/constantIems/hgraphicsbackground.h"
#include "GraphicsItems/constantIems/hgraphicsdrawcacheitem.h"
#include "GraphicsItems/constantIems/hgraphicsvisionshow.h"
#include "GraphicsItems/constantIems/hgraphicsmouse.h"

#include "hmath.h"


class HString :public QString{

public:
    explicit HString(qint32 ind,const QString &str):QString(str),index(ind){

    }


    inline bool operator < (const HString& data) const{
        return index  < data.index;
    }

    inline bool operator > (const HString& data) const{
        return index > data.index;
    }

    inline bool operator == (const HString& data) const{
        return index == data.index;
    }


    qint32 index;
};


class HGraphicsScene;
class LayerInfo
{
public:
    explicit LayerInfo(){

    }

    explicit LayerInfo(HGraphicsScene *scene):ptrScene(scene){

    }

    explicit LayerInfo(const QString &str):name(str) {

    }

    qint32 getCheckedState() const{
        if(items.isEmpty()){
            return 0;
        }
        bool state=items.first()->isSelected();
        for(auto&k:items){
            if(state==k->isSelected()){
                continue;
            }else{
                return 1;
            }
        }
        return state?2:0;
    }


    bool isActiveLayer() const;

    const QColor& getColor() const{
        return color;
    }
    void setColor(const QColor& mcolor) {
        color=mcolor;
        for(auto&k:items){
            k->setColor(color);
        }
    }

    bool  getVisible() const{
        return isVisiable;
    }
    void setVisible(bool off){
        isVisiable=off;
        for(auto&k:items){
            k->setVisible(isVisiable);
        }
    }

    bool  getisEnabled() const{
        return isEnabled;
    }

    void setisEnabled(bool locked){
        isEnabled=locked;
        for(auto&k:items){
            k->setEnabled(isEnabled);
        }
    }

    bool getLayerHighlight() const{
        return islayerHighlight;
    }

    void setHighlight(bool highLight){
        for(auto&k:items){
            k->setHighlight(highLight);
        }
        islayerHighlight=highLight;
    }

    void setSelected(bool selected){
        for(auto&k:items){
            k->setSelected(selected);
        }
    }


    const QList<HGraphicsItem *> & getItems() const{
        return items;
    }

    QList<HGraphicsItem *> & getItems() {
        return items;
    }

    QList<HGraphicsItem *> * getItemsPtr() {
        return &items;
    }

    void deleteItem(QGraphicsScene *scene,HGraphicsItem *item){
        if(items.removeOne(item)){
            scene->removeItem(item);
            scene->update();
        }
    }

    void deleteItem(QGraphicsScene *scene,qint32 itemIndex){
        scene->removeItem(items.takeAt(itemIndex));
        scene->update();
    }

    void deleteItems(QGraphicsScene *scene,QVector<qint32> indexs){
        H_Math::ShellSort_R<qint32>(indexs);
        for(const auto&k:indexs){
            scene->removeItem(items.takeAt(k));
        }
        scene->update();
    }

    void deleteItems(QGraphicsScene *scene,const QList<HGraphicsItem *>& initems){
        for(const auto&k:initems){
            items.removeOne(k);
            scene->removeItem(k);
        }
        scene->update();
    }

    void addItem(QGraphicsScene *scene,HGraphicsItem *item){
        item->setVisible(isVisiable);
        item->setEnabled(isEnabled);
        item->setColor(color);

        items.append(item);
        scene->addItem(item);
        scene->update();
    }

    void addItems(QGraphicsScene *scene,const QList<HGraphicsItem *>& initems){

        for(const auto&k:initems){
            items.append(k);
            items.last()->setVisible(isVisiable);
            items.last()->setEnabled(isEnabled);
            items.last()->setColor(color);
            scene->addItem(k);
        }
        scene->update();
    }

    void insertItem(QGraphicsScene *scene,qint32 index,HGraphicsItem *item){
        item->setVisible(isVisiable);
        item->setEnabled(isEnabled);
        item->setColor(color);

        items.insert(index,item);
        scene->addItem(item);
        scene->update();
    }

    void insertItems(QGraphicsScene *scene,qint32 index,const QList<HGraphicsItem *>& initems){
        for(const auto&k:initems){
            items.insert(index,k);
            items.last()->setVisible(isVisiable);
            items.last()->setEnabled(isEnabled);
            items.last()->setColor(color);
            scene->addItem(k);
        }
        scene->update();
    }

    void insertItems(QGraphicsScene *scene,const QList<QPair<qint32,HGraphicsItem*>>&initems){
        for(const auto&k:initems){
            scene->addItem(k.second);

            items.insert(k.first,k.second);
            items.last()->setVisible(isVisiable);
            items.last()->setEnabled(isEnabled);
            items.last()->setColor(color);
        }
        scene->update();
    }
    enum ghghgh{
        dfdf,
    };

    const QVector<QPointF> & getLocationPts() const{
        return locationPts;
    }

    void setLocationPts(const QVector<QPointF> & pts){
        locationPts=pts;
    }




    friend QDataStream &operator>>(QDataStream & input, LayerInfo &obj){
        input >> obj.name>>obj.isVisiable>>obj.isEnabled>>obj.color>>obj.locationPts;
        qint32 size;
        input >>size;
        obj.items.clear();

        qint32 type;
        for(qint32 k=0;k<size;++k){
            input >>type;
            switch (type) {
            case HGraphicsPathItem::Type:{
                auto item=new HGraphicsPathItem();
                item->HDataInputStream(input);
                obj.items.append(item);
            }
                break;
            default:
                break;
            }
        }
        return input;
    }
    friend QDataStream &operator<<(QDataStream & output, const LayerInfo &obj){
        output << obj.name<<obj.isVisiable<<obj.isEnabled<<obj.color<<obj.locationPts;
        output<<obj.items.size();
        for(const auto&k:obj.items){
            output<<k->type();
            k->HDataOutputStream(output);
        }
        return output;
    }

    inline bool operator == (const LayerInfo& data) const{
        return name == data.name;
    }


    QString name;

    void setScenePtr( HGraphicsScene * scene){
        ptrScene=scene;
    }

    QVector<QPointF> locationPts;


private:

    HGraphicsScene *ptrScene=nullptr;

    QList<HGraphicsItem *> items;
    bool isVisiable=true;
    bool isEnabled=true;
    QColor color=Qt::black;
    bool islayerHighlight=false;



};


typedef QVector<LayerInfo> HLayerVector;

class HGraphicsView;
class HGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit HGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = nullptr);


    HLayerVector *getLayerPtr() {return &layer;}
    HLayerVector &getLayer(){return layer;}
    const HLayerVector &getLayer() const{return layer;}

    QPointF getItemCenterPt(qint32 index);

    qint32 getFirstHighlightItemIndex() const;

    void transAllItems(const QTransform & trans);

    void setMMouseVisible(bool visible);
    void setMMousePos(const QPointF&pos);
    double getMMouseSize();

    void updateScene();

    void deleteSelectedItems();
    void clearItems();

    void clearSelectedItems();

    HGraphicsDrawCacheItem * getDrawCacheItem() const{return paintCacheItem;}

    void deleteItemInActive(HGraphicsItem *item);
    void deleteItem(qint32 layerIndex ,HGraphicsItem *item);
    void deleteItem(const QString& layerName,HGraphicsItem *item);
    void deleteItem(qint32 layerIndex ,qint32 itemIndex);
    void deleteItem(const QString& layerName ,qint32 itemIndex);
    qint32 deleteItem(HGraphicsItem *item);
    qint32 deleteItem(QGraphicsItem *item);

    void deleteItemsInActive(const QList<QPair<qint32,HGraphicsItem*>>&items);
    void deleteItems(qint32 layerIndex ,const QMap<qint32,HGraphicsItem*>&items);


    void addHItem(HGraphicsItem *item);
    void addHItem(qint32 layerIndex ,HGraphicsItem *item);
    void addHItem(const QString& layerName,HGraphicsItem *item);
    void addHItems(const QList<HGraphicsItem*> &items);
    void addHItems(qint32 layerIndex ,const QList<HGraphicsItem*> &items);
    void addHItems(const QString& layerName,const QList<HGraphicsItem*> &items);

    void insertItem(qint32 itemIndex,HGraphicsItem *item);
    void insertItem(qint32 layerIndex ,qint32 itemIndex,HGraphicsItem *item);
    void insertItem(const QString& layerName ,qint32 itemIndex,HGraphicsItem *item);
    void insertItems(qint32 itemIndex,const QList<HGraphicsItem *>&items);
    void insertItems(qint32 layerIndex ,qint32 itemIndex,const QList<HGraphicsItem *>&items);
    void insertItems(qint32 layerIndex  ,const QMap<qint32,HGraphicsItem*>&items);
    void insertItems(const QString& layerName ,qint32 itemIndex,const QList<HGraphicsItem *>&items);

    void addLayer(const LayerInfo &info);
    void setActiveLayer(qint32 index){if(index<layer.size()&&index>-1){activeLayer=index;}}
    qint32 getActiveLayer()const {return activeLayer;}
    const LayerInfo& getActiveLayerData() const{return layer.at(activeLayer);}
    const LayerInfo * getActiveLayerDataPtr() const {return &(layer[activeLayer]);}


    void groupItems(const QList<QGraphicsItem *>& items);
    QList<QGraphicsItem *> getSelectedItems() const;

    QMap<qint32, QMap<qint32,HGraphicsItem*> > getSelectedItemsIndexs() const;


    friend QDataStream &operator>>(QDataStream & input, HGraphicsScene &obj){
        input >>obj.activeLayer>> obj.layer;

        for(auto&k:obj.layer){
            k.setScenePtr(&obj);
        }
        return input;
    }

    friend QDataStream &operator<<(QDataStream & output, const HGraphicsScene &obj){
        output <<obj.activeLayer<< obj.layer;
        return output;
    }

    HGraphicsSelectRectItem *selectedRectItem;
    HGraphicsBackground *backgrounpRect;
    HGraphicsVisionShow *cameraPosItem;
    HGraphicsMouse *mouseItem;
    HGraphicsDrawCacheItem *paintCacheItem;
    qint32 m_mousePressMode=1;


signals:

    void sceneMouseMoved(QGraphicsSceneMouseEvent *mouseEvent,const QPointF& mmpos);
    void sceneMousePressed(QGraphicsSceneMouseEvent *mouseEvent,const QPointF& mmpos);
    void sceneMouseReleased(QGraphicsSceneMouseEvent *mouseEvent,const QPointF& mmpos);


public slots:
    void handlerSelectionChange();

protected:

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) override;



private:



    qint32 activeLayer=0;
    HLayerVector layer;



    HGraphicsView * ptrView=nullptr;

};

#endif // HGRAPHICSSCENE_H
