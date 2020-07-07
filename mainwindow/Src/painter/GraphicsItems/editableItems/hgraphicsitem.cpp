#include "hgraphicsitem.h"

#include "Src/Gui/settings/hparalibcontroller.h"
#include "Src/data/inisettings.h"

QRectF HGraphicsItem::m_PointRect=QRectF(0,0,5,5);


HGraphicsItem::HGraphicsItem(QGraphicsItem *parent) :QGraphicsItem(parent)
{
    auto para=HParaLibController::GetInstance();
    qint32 mode=IniSettings::GetInstance()->getValue(IniSettings::Edit_mode).toInt();

    setData(para_genaral,para->getGeneralDatas(mode));
    setData(para_laser,para->getLaserDatas(mode));
    setData(para_cam,para->getCamDatas(mode));

}

HGraphicsItem::~HGraphicsItem()
{

}

void HGraphicsItem::transLocationPts(const QTransform &trans)
{
    auto locations=data(HGraphicsItem::para_location).toStringList();
    QVector<QPointF> ptss;

    for(qint32 k=0;k<locations.size()/2;++k){
        ptss<<QPointF(locations.at(2*k).toDouble(),locations.at(k*2+1).toDouble());
    }
    QStringList list;
    for( auto&k:ptss){
        k=trans.map(k);
        list<<QString::number(k.x())<<QString::number(k.y());
    }
    setData(HGraphicsItem::para_location,list);
}

QVariantList HGraphicsItem::getPropertyParas() const
{
    return QVariantList();
}

const QGraphicsItem *HGraphicsItem::toQGrapicsItem() const
{
    return dynamic_cast<const QGraphicsItem *>(this);
}

bool HGraphicsItem::getA3200Gcode(QString &code, QStringList &paras, RLine &endPt, qint32 mode, QTransform trans)
{
    return true;
}

QPointF HGraphicsItem::getCenter()
{
    return QPointF();
}

double HGraphicsItem::getDistanceToShape(const QPointF &pt) const
{
    return 0;
}


qint32 HGraphicsItem::type() const
{
    return Type;
}

QRectF HGraphicsItem::boundingRect() const
{
    return QRectF();
}

QPainterPath HGraphicsItem::shape() const
{
    QPainterPath path;

    return path;
}

void HGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{

}

void HGraphicsItem::setHighlight(bool highlight)
{
    isHighlight=highlight;
    update();
}

bool HGraphicsItem::getIsHighlight() const
{
    return isHighlight;
}

void HGraphicsItem::setColor(const QColor &color)
{
    setData(-1,color.rgba());
    update();
}

QColor HGraphicsItem::getColor() const
{
    return QColor(data(-1).toUInt());
}


QPainterPath HGraphicsItem::getScenePath() const
{
    return QPainterPath();
}

void HGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void HGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void HGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}


