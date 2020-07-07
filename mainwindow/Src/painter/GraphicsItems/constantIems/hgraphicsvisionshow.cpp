#include "hgraphicsvisionshow.h"


#include "hgraphicsselectrectitem.h"
#include <QtWidgets>

#include "Src/Gui/settings/hsystemadjustcontroller.h"

HGraphicsVisionShow::HGraphicsVisionShow(QGraphicsItem *parent) :QGraphicsItem(parent)
{
    setAcceptHoverEvents(false);
    ptrTransController=HSystemAdjustController::GetInstance();
   // setZValue(99);

}

HGraphicsVisionShow::~HGraphicsVisionShow()
{

}

QPointF HGraphicsVisionShow::getPos()
{
    return m_pos;
}

void HGraphicsVisionShow::setMPos(const QPointF &pos)
{
    m_pos=pos;
    update();
}



QRectF HGraphicsVisionShow::boundingRect() const
{
    return m_path.controlPointRect();
}

QPainterPath HGraphicsVisionShow::shape() const
{
    return m_path;
}

void HGraphicsVisionShow::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget)

    QPen pen;
    pen.setCosmetic(true);
    pen.setColor(color);
    painter->setPen(pen);


    const qreal scale = item->levelOfDetailFromTransform(painter->worldTransform());
    QPainterPath mpath;
    mpath.moveTo(m_pos.x()-30/scale,m_pos.y());
    mpath.lineTo(m_pos.x()+30/scale,m_pos.y());
    mpath.moveTo(m_pos.x(),m_pos.y()-30/scale);
    mpath.lineTo(m_pos.x(),m_pos.y()+30/scale);
    mpath.addEllipse(m_pos.x()-10/scale,m_pos.y()-10/scale,20/scale,20/scale);
    mpath.addEllipse(m_pos.x()-5/scale,m_pos.y()-5/scale,10/scale,10/scale);
    painter->drawPath(m_path);


    m_path.swap(mpath);
}

void HGraphicsVisionShow::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void HGraphicsVisionShow::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void HGraphicsVisionShow::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
