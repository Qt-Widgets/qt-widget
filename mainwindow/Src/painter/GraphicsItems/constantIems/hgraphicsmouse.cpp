#include "hgraphicsmouse.h"

#include <QtWidgets>


HGraphicsMouse::HGraphicsMouse(QGraphicsItem *parent) :QGraphicsItem(parent)
{
    setAcceptHoverEvents(false);

    m_path.moveTo(-60,0);
    m_path.lineTo(60,0);
    m_path.moveTo(0,-60);
    m_path.lineTo(0,60);
    QVector<QPointF> points;
    double msize=m_msize*0.5;
    points<<QPointF(-msize,msize)<<QPointF(-msize,-msize)<<QPointF(msize,-msize)<<QPointF(msize,msize)<<QPointF(-msize,msize);
    m_path.addPolygon(points);

    m_pathScale.addPolygon(points);
    m_pathScale10=QTransform::fromScale(0.1,0.1).map(m_pathScale);
    m_pathContain=m_pathScale10;
}

HGraphicsMouse::~HGraphicsMouse()
{

}

double HGraphicsMouse::getMouseSize()
{
    return 3.0*m_msize/scale;
}

QPainterPath HGraphicsMouse::getContainSahpe() const
{
    return QTransform::fromTranslate(pos().x(),pos().y()).map(m_pathContain);
}



QRectF HGraphicsMouse::boundingRect() const
{
    return m_path.controlPointRect();
}

QPainterPath HGraphicsMouse::shape() const
{
    return m_path;
}

void HGraphicsMouse::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget)

    QPen pen;
    pen.setCosmetic(true);
    pen.setStyle(Qt::SolidLine);
    pen.setWidthF(1.0);
    pen.setColor(color);
    painter->setPen(pen);

    scale = item->levelOfDetailFromTransform(painter->worldTransform());

   QPainterPath m_drawPath=QTransform::fromScale(1/scale,1/scale).map(m_path);

    painter->drawPath(m_drawPath);
    m_pathContain=scale>10?(QTransform::fromScale(1/scale,1/scale).map(m_pathScale)):m_pathScale10;
}

void HGraphicsMouse::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void HGraphicsMouse::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void HGraphicsMouse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

bool HGraphicsMouse::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    return false;
}

//QVariant HGraphicsMouse::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
//{
//     if (change == ItemPositionChange && scene()) {
//         qDebug()<<value.toPointF();
//     }
//    return QGraphicsItem::itemChange(change, value);
//}
