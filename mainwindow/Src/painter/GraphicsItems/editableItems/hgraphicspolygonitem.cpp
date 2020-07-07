#include "hgraphicspolygonitem.h"


HGraphicsPolygonItem::HGraphicsPolygonItem(const QVariant &data,QGraphicsItem *parent) :HGraphicsItem(parent)
{
    setData(0,data);
}

HGraphicsPolygonItem::~HGraphicsPolygonItem()
{

}

qint32 HGraphicsPolygonItem::type() const
{
    return Type;
}

QRectF HGraphicsPolygonItem::boundingRect() const
{
    return QRectF();
}

QPainterPath HGraphicsPolygonItem::shape() const
{
    QPainterPath path;
    return path;
}

QPainterPath HGraphicsPolygonItem::getScenePath() const
{
    QPainterPath path;
    return  path;
}

void HGraphicsPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{

}

void HGraphicsPolygonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void HGraphicsPolygonItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void HGraphicsPolygonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}
