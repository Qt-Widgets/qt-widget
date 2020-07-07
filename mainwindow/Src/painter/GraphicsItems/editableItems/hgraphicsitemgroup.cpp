#include "hgraphicsitemgroup.h"

#include "hgraphicsarcitem.h"
#include "hgraphicsellipseitem.h"
#include "hgraphicslineitem.h"
#include "hgraphicsitem.h"
#include "hgraphicspathitem.h"
#include "hgraphicspixmapitem.h"
#include "hgraphicspolygonitem.h"
#include "hgraphicsrectitem.h"
#include "hgraphicssimpletextitem.h"



HGraphicsItemGroup::HGraphicsItemGroup(QGraphicsItem *parent) :HGraphicsItem(parent)
{

}

HGraphicsItemGroup::~HGraphicsItemGroup()
{

}

void HGraphicsItemGroup::HDataInputStream(QDataStream &input)
{
    qint32 number;
    input>>number;

    QPointF  mppos;
    input>>mppos;
    setPos(mppos);

    for(qint32 k=0;k<number;++k){
        qint32 type;
        input>>type;
        switch (type) {
        case HGraphicsPathItem::Type:{
            auto item=new HGraphicsPathItem();
            item->HDataInputStream(input);
        }
            break;
        default:
            break;
        }
    }
    bool isLocked,isOff;
    quint32 color;
    QStringList general,laser,cam,lpts;

    input>>isLocked>>isOff>>color>>general>>laser>>cam>>lpts;


    setEnabled(isLocked);
    setVisible(isOff);

    setData(HGraphicsItem::color,color);
    setData(HGraphicsItem::para_genaral,general);
    setData(HGraphicsItem::para_laser,laser);
    setData(HGraphicsItem::para_cam,cam);
    setData(HGraphicsItem::para_location,lpts);
}

void HGraphicsItemGroup::HDataOutputStream(QDataStream &output) const
{
    output<<m_groupItems.size();
    output<<pos();
    for(const auto &k:m_groupItems){
        k->HDataOutputStream(output);
    }

    output<<isEnabled()<<isVisible()<<data(HGraphicsItem::color).toUInt()<<data(HGraphicsItem::para_genaral).toStringList()<<
            data(HGraphicsItem::para_laser).toStringList()<<data(HGraphicsItem::para_cam).toStringList()<<data(HGraphicsItem::para_location).toStringList();
}

qint32 HGraphicsItemGroup::type() const
{
    return Type;
}

QRectF HGraphicsItemGroup::boundingRect() const
{
    return QRectF();
}

QPainterPath HGraphicsItemGroup::shape() const
{
    QPainterPath path;
    return path;
}

QPainterPath HGraphicsItemGroup::getScenePath() const
{
    QPainterPath path;
    return  path;
}

void HGraphicsItemGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{

}

void HGraphicsItemGroup::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void HGraphicsItemGroup::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void HGraphicsItemGroup::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}
