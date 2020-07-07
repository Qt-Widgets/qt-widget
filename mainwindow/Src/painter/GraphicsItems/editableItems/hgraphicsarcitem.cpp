#include "hgraphicsarcitem.h"

#include <QtWidgets>



HGraphicsArcItem::HGraphicsArcItem( QGraphicsItem *parent):HGraphicsItem(parent)
{

}

HGraphicsArcItem::HGraphicsArcItem(const QPointF &pos, qreal r1, qreal r2, qreal angle1, qreal angle2, QGraphicsItem *parent)
    :HGraphicsItem(parent),ellipseRect(0,0,2*r1,2*r2),angle1(angle1),angle2(angle2)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
    setPos(pos);
    refreshPath();
}

HGraphicsArcItem::HGraphicsArcItem(qreal posX, qreal posY, qreal r1, qreal r2, qreal angle1, qreal angle2, QGraphicsItem *parent)
    :HGraphicsItem(parent),ellipseRect(0,0,2*r1,2*r2),angle1(angle1),angle2(angle2)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(false);
    setX(posX);
    setY(posY);
    refreshPath();
}

HGraphicsArcItem::~HGraphicsArcItem()
{

}

qint32 HGraphicsArcItem::type() const
{
     return Type;
}

QRectF HGraphicsArcItem::boundingRect() const
{
    return ellipseRect;
}

QPainterPath HGraphicsArcItem::shape() const
{
    return mpath;
}

QPainterPath HGraphicsArcItem::getScenePath() const
{
    QPainterPath path;
    return  path;
}

void HGraphicsArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = (item->state & QStyle::State_Selected) ? color.dark(150) : color;
    if (item->state & QStyle::State_MouseOver)
        fillColor = fillColor.light(125);

    const qreal lod = item->levelOfDetailFromTransform(painter->worldTransform());
    if (lod < 0.2) {
        if (lod < 0.125) {
            painter->fillRect(ellipseRect, fillColor);
            return;
        }

        QBrush b = painter->brush();
        painter->setBrush(fillColor);
        painter->drawRect(ellipseRect);
        painter->setBrush(b);
        return;
    }

    QPen pen = painter->pen();
    pen.setCosmetic(true);

    if (item->state & QStyle::State_Selected){
        pen.setColor(Qt::red);
    }else{
        pen.setColor(Qt::black);
    }

    painter->setPen(pen);
    painter->drawPath(mpath);
}

void HGraphicsArcItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void HGraphicsArcItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void HGraphicsArcItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void HGraphicsArcItem::refreshPath()
{
    mpath.arcMoveTo(ellipseRect,-angle1);
    mpath.arcTo(ellipseRect,(isCCW?-1.0:1.0)*angle1,(isCCW?-1.0:1.0)*qAbs((angle2<angle1?(angle2+360):angle2)-angle1));
}
