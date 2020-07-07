#include "hgraphicslineitem.h"
#include <QtWidgets>

HGraphicsLineItem::HGraphicsLineItem(HGraphicsItem *parent):HGraphicsItem(parent)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(false);
}

HGraphicsLineItem::HGraphicsLineItem(const QPointF &pos, qreal x2, qreal y2, QGraphicsItem *parent)
    :HGraphicsItem(parent),line(0,0,x2,y2)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(false);
    setPos(pos);
}

HGraphicsLineItem::HGraphicsLineItem(qreal posX, qreal posY, qreal x2, qreal y2, QGraphicsItem *parent) :HGraphicsItem(parent),line(0,0,x2,y2)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(false);
    setX(posX);
    setY(posY);
}

HGraphicsLineItem::HGraphicsLineItem(const QPointF &pos, const QPointF &p2, QGraphicsItem *parent) :HGraphicsItem(parent),line(0,0,p2.x(),p2.y())
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(false);
    setPos(pos);

}


HGraphicsLineItem::~HGraphicsLineItem()
{

}

qint32 HGraphicsLineItem::type() const
{
    return Type;
}

QRectF HGraphicsLineItem::boundingRect() const
{
    QPointF p1=line.p1(),p2=line.p2();

    QRectF rect(p1.y()<p2.y()?p1:p2,p1.y()<p2.y()?p2:p1);
    return rect+=QMarginsF(0.1,0.1,0.1,0.1);
}

QPainterPath HGraphicsLineItem::shape() const
{
    QPainterPath path;
    path.moveTo(line.p1());
    path.lineTo(line.p2());
    //path.translate(scenePos());
    return path;
}

QPainterPath HGraphicsLineItem::getScenePath() const
{
    QPainterPath path;
    path.moveTo(line.p1());
    path.lineTo(line.p2());
    path.translate(scenePos());
    return  path;
}

void HGraphicsLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget);

    const qreal scale = item->levelOfDetailFromTransform(painter->worldTransform());
    if (scale < 0.2) {

        QColor fillColor = (item->state & QStyle::State_Selected) ? color.dark(150) : color;
        if (item->state & QStyle::State_MouseOver)
            fillColor = fillColor.light(125);

        if (scale < 0.125) {
            painter->fillRect(boundingRect(), fillColor);
            return;
        }

        QBrush b = painter->brush();
        painter->setBrush(fillColor);
        painter->drawRect(boundingRect());
        painter->setBrush(b);
        return;
    }

    QPen pen = painter->pen();
    pen.setCosmetic(true);

    if(item->state & QStyle::State_Selected){

        //qDebug()<<HGraphicsItem::getPointRect(scale,line.p1());
        painter->fillRect(HGraphicsItem::getPointRect(scale,line.p1()),Qt::green);
        painter->fillRect(HGraphicsItem::getPointRect(scale,line.p2()),Qt::red);
        pen.setColor(selectedColor);



    }else{
        pen.setColor(Qt::black);
    }
    painter->setPen(pen);
    painter->drawLine(line);
}

void HGraphicsLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void HGraphicsLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void HGraphicsLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

QVariant HGraphicsLineItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) {
        //selectedRectItem->addRectAndRefresh(sceneBoundingRect(),value.toBool());
       // setParentItem(value.toBool()?selectedRectItem:nullptr);
    }
    return QGraphicsItem::itemChange(change, value);
}
