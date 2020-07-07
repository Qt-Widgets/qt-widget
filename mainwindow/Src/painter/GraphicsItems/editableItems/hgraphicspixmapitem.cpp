#include "hgraphicspixmapitem.h"



HGraphicsPixmapItem::HGraphicsPixmapItem(const QVariant &data,QGraphicsItem *parent) :HGraphicsItem(parent)
{
    setData(0,data);
}

HGraphicsPixmapItem::~HGraphicsPixmapItem()
{

}

qint32 HGraphicsPixmapItem::type() const
{
    return Type;
}

QRectF HGraphicsPixmapItem::boundingRect() const
{
    return QRectF();
}

QPainterPath HGraphicsPixmapItem::shape() const
{
    QPainterPath path;
    return path;
}

QPainterPath HGraphicsPixmapItem::getScenePath() const
{
    QPainterPath path;
    return  path;
}

void HGraphicsPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{

}

void HGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void HGraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void HGraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
