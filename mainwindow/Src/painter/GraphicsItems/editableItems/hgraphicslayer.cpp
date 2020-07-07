#include "hgraphicslayer.h"

#include <QtWidgets>

HGraphicsLayer::HGraphicsLayer(QGraphicsItem *parent) :HGraphicsItem(parent)
{
    //setFlags(/*ItemIsSelectable | */ItemIsMovable);
    setAcceptHoverEvents(false);
    //setZValue(99);
}

HGraphicsLayer::~HGraphicsLayer()
{

}

void HGraphicsLayer::HDataInputStream(QDataStream &input)
{
    QString name;
    bool isOff;
    uint color;
    input>>name>>isOff>>color;
    setData(0,name);
    setData(1,isOff);
    setData(2,color);
}

void HGraphicsLayer::HDataOutputStream(QDataStream &output) const{
    output<<type()<<data(0).toString()<<data(0).toString()<<data(1).toBool()<<data(2).toUInt();
}


qint32 HGraphicsLayer::type() const
{
    return Type;
}

QRectF HGraphicsLayer::boundingRect() const
{
    return childrenBoundingRect();
}

QPainterPath HGraphicsLayer::shape() const
{
    QPainterPath path;
    return path;
}


void HGraphicsLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget);

}

void HGraphicsLayer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void HGraphicsLayer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void HGraphicsLayer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

QVariant HGraphicsLayer::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        //selectedRectItem->addRectAndRefresh(sceneBoundingRect(),value.toBool());
    }
    return QGraphicsItem::itemChange(change, value);
}
