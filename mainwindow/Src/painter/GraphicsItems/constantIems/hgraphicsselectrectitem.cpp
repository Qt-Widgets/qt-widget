#include "hgraphicsselectrectitem.h"
#include <QtWidgets>

HGraphicsSelectRectItem::HGraphicsSelectRectItem(QGraphicsItem *parent) :QGraphicsItem(parent)
{
    setFlags(/*ItemIsSelectable | */ItemIsMovable);
    setAcceptHoverEvents(false);
    //setZValue(99);
}

HGraphicsSelectRectItem::~HGraphicsSelectRectItem()
{

}

void HGraphicsSelectRectItem::setRect(const QRectF &r)
{
    setPos(r.topLeft());
    rect.setWidth(r.width());
    rect.setHeight(r.height());
}


qint32 HGraphicsSelectRectItem::type() const
{

    return Type;
}

QRectF HGraphicsSelectRectItem::boundingRect() const
{
    return rect;
}

QPainterPath HGraphicsSelectRectItem::shape() const
{
    QPainterPath path;
    path.addRect(rect);
    return path;
}


void HGraphicsSelectRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget);

//    const qreal scale = item->levelOfDetailFromTransform(painter->worldTransform());
//    if (scale < 0.2) {

//        QColor fillColor = (item->state & QStyle::State_Selected) ? color.dark(150) : color;
//        if (item->state & QStyle::State_MouseOver)
//            fillColor = fillColor.light(125);

//        if (scale < 0.125) {
//            painter->fillRect(rect, fillColor);
//            return;
//        }

//        QBrush b = painter->brush();
//        painter->setBrush(fillColor);
//        painter->drawRect(rect);
//        painter->setBrush(b);
//        return;
//    }

    QPen pen = painter->pen();
    pen.setCosmetic(true);
    pen.setColor(color);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->setBrush(QColor(196,233,242,150));
    painter->drawRect(rect);
    //painter->fillRect(rect,QColor(196,233,242,150));

}

void HGraphicsSelectRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void HGraphicsSelectRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void HGraphicsSelectRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}


QVariant HGraphicsSelectRectItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        //selectedRectItem->addRectAndRefresh(sceneBoundingRect(),value.toBool());
    }
    return QGraphicsItem::itemChange(change, value);
}
