#include "hgraphicsdrawcacheitem.h"


#include "hgraphicsselectrectitem.h"
#include <QtWidgets>

HGraphicsDrawCacheItem::HGraphicsDrawCacheItem(QGraphicsItem *parent) :QGraphicsItem(parent)
{
    setAcceptHoverEvents(false);
}

HGraphicsDrawCacheItem::~HGraphicsDrawCacheItem()
{

}

void HGraphicsDrawCacheItem::setPainterPath(QPainterPath &path){
    m_path.swap(path);
}

void HGraphicsDrawCacheItem::setColor(const QColor &c)
{
    color=c;
}

void HGraphicsDrawCacheItem::resetData()
{
    auto dfdf=m_path.controlPointRect();
    m_path=QPainterPath();
    update(dfdf);
}

QRectF HGraphicsDrawCacheItem::boundingRect() const
{
    return m_path.controlPointRect();
}

QPainterPath HGraphicsDrawCacheItem::shape() const
{
    return m_path;
}

void HGraphicsDrawCacheItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget)

    QPen pen;
    pen.setCosmetic(true);
    pen.setColor(color);
    painter->setPen(pen);
    painter->drawPath(m_path);
}

void HGraphicsDrawCacheItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void HGraphicsDrawCacheItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void HGraphicsDrawCacheItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}


