#include "hgraphicsrectitem.h"

#include <QtWidgets>

HGraphicsRectItem::HGraphicsRectItem(QGraphicsItem *parent) :HGraphicsItem(parent)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

HGraphicsRectItem::HGraphicsRectItem(const QRectF &r, QGraphicsItem *parent, bool isEditable):HGraphicsItem(parent),rect(r)
{
        if(isEditable){
            setFlags(ItemIsSelectable | ItemIsMovable);
        }
    setAcceptHoverEvents(true);
}

HGraphicsRectItem::~HGraphicsRectItem()
{

}

void HGraphicsRectItem::HDataInputStream(QDataStream &input)
{
    bool isLocked,isOff;
    quint32 color;
    QStringList general,laser,cam,lpts;

    input>>rect>>isLocked>>isOff>>color>>general>>laser>>cam>>lpts;

    setEnabled(isLocked);
    setVisible(isOff);

    setData(HGraphicsItem::color,color);
    setData(HGraphicsItem::para_genaral,general);
    setData(HGraphicsItem::para_laser,laser);
    setData(HGraphicsItem::para_cam,cam);
    setData(HGraphicsItem::para_location,lpts);
}

void HGraphicsRectItem::HDataOutputStream(QDataStream &output) const
{
    output<<type()<<rect<<isEnabled()<<isVisible()<<data(HGraphicsItem::color).toUInt()<<data(HGraphicsItem::para_genaral).toStringList()<<
            data(HGraphicsItem::para_laser).toStringList()<<data(HGraphicsItem::para_cam).toStringList()<<data(HGraphicsItem::para_location).toStringList();
}

bool HGraphicsRectItem::getA3200Gcode(QString &code, QStringList &paras, RLine &endPt, qint32 mode, QTransform trans)
{
    return true;
}

QPointF HGraphicsRectItem::getCenter()
{
    return rect.center();
}

QPainterPath HGraphicsRectItem::getScenePath() const
{
    QPainterPath path;
    path.addRect(rect);
    return  path;
}

QVariantList HGraphicsRectItem::getPropertyParas() const
{
    QVariantList list;
    list<<rect.topLeft().x()+pos().x();
    list<<rect.topLeft().y()+pos().y();
    list<<rect.topRight().x()+pos().x();
    list<<rect.topRight().y()+pos().y();
    list<<rect.bottomRight().x()+pos().x();
    list<<rect.bottomRight().y()+pos().y();
    list<<rect.bottomLeft().x()+pos().x();
    list<<rect.bottomLeft().y()+pos().y();
    list<<rect.center().x()+pos().x();
    list<<rect.center().y()+pos().y();
    return list;
}

void HGraphicsRectItem::setRect(const QRectF &r)
{
    setX(r.x());
    setY(r.y());
    rect.setWidth(r.width());
    rect.setHeight(r.height());
}

qint32 HGraphicsRectItem::type() const
{
    return Type;
}

QRectF HGraphicsRectItem::boundingRect() const
{
    QRectF gfg;
    gfg.setWidth(rect.width());
    gfg.setHeight(rect.height());
    gfg.moveCenter(rect.center());
    return gfg;
}

QPainterPath HGraphicsRectItem::shape() const
{
    QPainterPath path;
    path.addRect(rect);
    return path;
}


void HGraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget);

    scale = item->levelOfDetailFromTransform(painter->worldTransform());

    auto rectfl=boundingRect();
    if (scale*rectfl.width() < 2.0&&scale*rectfl.height() < 2.0) {
        painter->fillRect(rectfl, Qt::gray);
        return;
    }

    QPen pen = painter->pen();
    pen.setCosmetic(true);

    if(item->state & QStyle::State_Selected){

        //qDebug()<<HGraphicsItem::getPointRect(scale,line.p1());
        pen.setColor(selectedColor);

        bool change=1;
        for(const auto&k:getVectorProperties()){
            painter->fillRect(HGraphicsItem::getPointRect(scale,k),change?Qt::blue:Qt::red);
            change=!change;
        }
        pen.setWidthF(2);
    }else{
        pen.setColor(isEnabled()?(isHighlight?Qt::red:QColor(data(HGraphicsItem::color).toUInt())):Qt::gray);
    }

    pen.setStyle(isHighlight?Qt::DashDotDotLine:Qt::SolidLine);

    painter->setPen(pen);
    painter->drawRect(rect);

    pen.setWidthF(0);
    painter->setPen(pen);
}


void HGraphicsRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void HGraphicsRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void HGraphicsRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void HGraphicsRectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
}

void HGraphicsRectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
}

void HGraphicsRectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    //    for(const auto&k:m_shape->getVectorProperties()){
    //        qreal d1=k.getDistanceTo2D(event->scenePos());
    //        if(d1<(5.0/scale)){
    //            if(d1<k.getDistanceTo2D(event->lastScenePos())){
    //                ((HGraphicsView*)(scene()->parent()))->setMousePos(k.get2DPoint());
    //                break;
    //            }
    //        }
    //    }

    QGraphicsItem::hoverMoveEvent(event);
}



QList<QPointF> HGraphicsRectItem::getVectorProperties() const
{
    QList<QPointF> pts;
    pts<<rect.topLeft()<<rect.topRight()<<rect.bottomRight()<<rect.bottomLeft();
    return pts;
}
