#include "hgraphicsbackground.h"
#include <QtWidgets>

HGraphicsBackground::HGraphicsBackground(const QRectF &content, QGraphicsItem *parent):QGraphicsItem(parent)
{
    //setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(false);

    setPos(content.topLeft());
    rect.setSize(content.size());

}

HGraphicsBackground::~HGraphicsBackground()
{

}

void HGraphicsBackground::setContentRect(const QRectF &r)
{
    setPos(r.topLeft());
    rect.setSize(r.size());


}

qint32 HGraphicsBackground::type() const
{
    return Type;
}

QRectF HGraphicsBackground::boundingRect() const
{
    return rect;
}

QPainterPath HGraphicsBackground::shape() const
{
    QPainterPath path;
    path.addRect(rect);
    return path;
}

void HGraphicsBackground::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget)

    const qreal scale = item->levelOfDetailFromTransform(painter->worldTransform());

    QPen pen;
    pen.setCosmetic(true);
    pen.setWidthF(2);
    pen.setColor(color);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->drawRect(rect);

    pen.setColor(Qt::darkGreen);
    pen.setWidthF(1.5);
    painter->setPen(pen);


    QPointF cpt=rect.center();
    qreal length=20/scale;
    painter->drawLine(QLineF(cpt.x()-length,cpt.y(),cpt.x()+length,cpt.y()));
    painter->drawLine(QLineF(cpt.x(),cpt.y()-length,cpt.x(),cpt.y()+length));

    pen.setWidthF(1);
    painter->setPen(pen);
}

void HGraphicsBackground::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void HGraphicsBackground::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void HGraphicsBackground::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

QVariant HGraphicsBackground::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    return QGraphicsItem::itemChange(change, value);
}
