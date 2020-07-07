#include "hgraphicsellipseitem.h"

#include <QtWidgets>

HGraphicsEllipseItem::HGraphicsEllipseItem(QGraphicsItem *parent) :HGraphicsItem(parent)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

HGraphicsEllipseItem::HGraphicsEllipseItem(const QPointF &pos, qreal r1, qreal r2, QGraphicsItem *parent):HGraphicsItem(parent),ellipseRect(0,0,2*r1,2*r2)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
    setPos(pos);
}

HGraphicsEllipseItem::HGraphicsEllipseItem(qreal posX, qreal posY, qreal r1, qreal r2, QGraphicsItem *parent):HGraphicsItem(parent),ellipseRect(0,0,2*r1,2*r2)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
    setX(posX);
    setY(posY);
}




HGraphicsEllipseItem::~HGraphicsEllipseItem()
{

}

qint32 HGraphicsEllipseItem::type() const
{
    return Type;
}

QRectF HGraphicsEllipseItem::boundingRect() const
{
    return ellipseRect;
}

QPainterPath HGraphicsEllipseItem::shape() const
{
    QPainterPath path;
    path.addEllipse(ellipseRect);
     //path.translate(scenePos());
    return path;
}

QPainterPath HGraphicsEllipseItem::getScenePath() const
{
    QPainterPath path;
    path.addEllipse(ellipseRect);
    path.translate(scenePos());
    return path;
}

void HGraphicsEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
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
        pen.setColor(selectedColor);
    }else{
        pen.setColor(Qt::black);
    }

    painter->setPen(pen);

    painter->drawEllipse(ellipseRect);

}

void HGraphicsEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void HGraphicsEllipseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void HGraphicsEllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

QVariant HGraphicsEllipseItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) {
        //selectedRectItem->addRectAndRefresh(sceneBoundingRect(),value.toBool());
        //setParentItem(value.toBool()?selectedRectItem:nullptr);
    }
    return QGraphicsItem::itemChange(change, value);
}
