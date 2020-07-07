#include "hgraphicssimpletextitem.h"



HGraphicsSimpleTextItem::HGraphicsSimpleTextItem(const QVariant &data,  QGraphicsItem *parent) :HGraphicsItem(parent)
{
    setData(0,data);
}

HGraphicsSimpleTextItem::~HGraphicsSimpleTextItem()
{

}

qint32 HGraphicsSimpleTextItem::type() const
{
    return Type;
}

QRectF HGraphicsSimpleTextItem::boundingRect() const
{
    return QRectF();
}

QPainterPath HGraphicsSimpleTextItem::shape() const
{
    QPainterPath path;
    return path;
}

QPainterPath HGraphicsSimpleTextItem::getScenePath() const
{
    QPainterPath path;
    return  path;
}

void HGraphicsSimpleTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{

}

void HGraphicsSimpleTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void HGraphicsSimpleTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void HGraphicsSimpleTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}
