#include "hgraphicspathitem.h"

#include "Src/painter/math/RCircle.h"
#include "Src/painter/math/REllipse.h"
#include "Src/painter/math/RArc.h"
#include "Src/painter/math/RRay.h"
#include "Src/painter/math/RXLine.h"
#include "Src/painter/math/RLine.h"
#include "Src/painter/math/RPoint.h"

#include "Src/painter/math/RColor.h"
#include "Src/painter/math/RDxfServices.h"
#include "Src/painter/math/RLineweight.h"

#include "Src/painter/math/RVector.h"

#include "Src/painter/hgraphicsscene.h"


#include "Src/Gui/settings/hsystemadjustcontroller.h"

#include <QtWidgets>

#include <QCursor>

HGraphicsPathItem::HGraphicsPathItem(QGraphicsItem *parent) :HGraphicsItem(parent)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

HGraphicsPathItem::HGraphicsPathItem(QSharedPointer<RShape> sh, QGraphicsItem *parent,bool isEditable):HGraphicsItem(parent)
  ,m_shape(sh),m_path(sh)
{
    if(isEditable){
        setFlags(ItemIsSelectable | ItemIsMovable);
    }

    setAcceptHoverEvents(true);
}

HGraphicsPathItem::~HGraphicsPathItem()
{

}

void HGraphicsPathItem::HDataInputStream(QDataStream &input)
{

    qint32 type;
    input >>type;
    QPointF  mppos;
    input>>mppos;
    setPos(mppos);

    switch (type) {
    case RShape::Unknown:{

    }
        break;
    case RShape::Point:{
        m_shape.reset(new RPoint());
    }
        break;
    case RShape::Line:{
        m_shape.reset(new RLine());
    }
        break;
    case RShape::Arc:{
        m_shape.reset(new RArc());
    }
        break;
    case RShape::Circle:{
        m_shape.reset(new RCircle());
    }
        break;
    case RShape::Ellipse:{
        m_shape.reset(new REllipse());
    }
        break;
    case RShape::Polyline:{
        m_shape.reset(new RPolyline());
    }
        break;
    case RShape::Spline:{
        m_shape.reset(new RSpline());
    }
        break;
    case RShape::Triangle:{
        m_shape.reset(new RTriangle());
    }
        break;
    case RShape::XLine:{
        m_shape.reset(new RXLine());
    }
        break;
    case RShape::Ray:{
        m_shape.reset(new RRay());
    }
        break;
    default:
        break;
    }
    m_shape->HDataInputStream(input);
    m_path=m_shape;

    bool isLocked,isOff;
    quint32 color;
    QStringList general,laser,cam,lpts;

    input>>isLocked>>isOff>>color>>general>>laser>>cam>>lpts;


    setEnabled(isLocked);
    setVisible(isOff);

    setData(HGraphicsItem::color,color);
    setData(HGraphicsItem::para_genaral,general);
    setData(HGraphicsItem::para_laser,laser);
    setData(HGraphicsItem::para_cam,cam);
    setData(HGraphicsItem::para_location,lpts);
}

void HGraphicsPathItem::HDataOutputStream(QDataStream &output) const
{
    output<<m_shape->getShapeType();
    //m_shape->getTransformed(QTransform::fromTranslate(pos().x(),pos().y()))->HDataOutputStream(output);
    output<<pos();
    m_shape->HDataOutputStream(output);
    output<<isEnabled()<<isVisible()<<data(HGraphicsItem::color).toUInt()<<data(HGraphicsItem::para_genaral).toStringList()<<
            data(HGraphicsItem::para_laser).toStringList()<<data(HGraphicsItem::para_cam).toStringList()<<data(HGraphicsItem::para_location).toStringList();
}

bool HGraphicsPathItem::getA3200Gcode(QString &code, QStringList &paras, RLine &endPt, qint32 mode,  QTransform trans)
{

    if(!isVisible())
        return true;
    QStringList list,list2;
    for(qint32 k=HGraphicsItem::para_genaral;k<HGraphicsItem::para_cam+1;++k){
        for(const auto&m:data(k).toStringList()){
            list.append(m);
        }
    }


    qint32 size=list.size();
    for(qint32 k=0;k<size;++k){
        list2.append(list.at(k)==paras.value(k)?nullptr:list.at(k));
    }

    if(m_shape->getShapeType()==RShape::Spline){
        list2[HParaLibController::Cutter_Cam_speed]=list.at(HParaLibController::Cutter_Cam_speed);
        list2[HParaLibController::Cutter_Cam_acc]=list.at(HParaLibController::Cutter_Cam_acc);
    }
    paras=list;
    //         m_shape=m_shape->getTransformed(QTransform::fromTranslate(pos().x(),pos().y()));
    //          m_path.reset(new RPainterPath(m_shape));


    auto locations=data(HGraphicsItem::para_location).toStringList();

    if(!locations.isEmpty()){
        QVector<QPointF> ptss;

        for(qint32 k=0;k<locations.size()/2;++k){
            ptss<<QPointF(locations.at(2*k).toDouble(),locations.at(k*2+1).toDouble());
        }

        if(HSystemAdjustController::GetInstance()->getTransFromPainterPossUseCutter(trans,ptss))
            return false;
    }


    return  m_shape->HGetA3200Gcode(code,list2,trans.translate(pos().x(),pos().y()),endPt);
}

QPointF HGraphicsPathItem::getCenter()
{
    return m_shape->getCenterPoints().first().get2DPoint();
}

double HGraphicsPathItem::getDistanceToShape(const QPointF &pt) const
{
    return m_shape->getDistanceTo(pt);
}

QPainterPath HGraphicsPathItem::getScenePath() const
{

    return  m_path;
}


QVariantList HGraphicsPathItem::getPropertyParas() const
{
    QVariantList list;
    list<<m_path.getStartPoint().x+pos().x();
    list<<m_path.getStartPoint().y+pos().y();
    list<<m_path.getEndPoint().x+pos().x();
    list<<m_path.getEndPoint().y+pos().y();
    list<<m_path.length();
    return list;
}

qint32 HGraphicsPathItem::type() const
{
    return Type;
}

QRectF HGraphicsPathItem::boundingRect() const
{
    return m_path.controlPointRect();
}

QPainterPath HGraphicsPathItem::shape() const
{
    return m_path;
}



void HGraphicsPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget)

    qreal scale = item->levelOfDetailFromTransform(painter->worldTransform());

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
        for(const auto&k:m_shape->getVectorProperties()){
            painter->fillRect(HGraphicsItem::getPointRect(scale,k.get2DPoint()),change?Qt::blue:Qt::red);
            change=!change;
        }

        painter->fillRect(HGraphicsItem::getPointRect(scale,m_shape->getStartPoint().get2DPoint()),Qt::blue);
        painter->fillRect(HGraphicsItem::getPointRect(scale,m_shape->getEndPoint().get2DPoint()),Qt::red);

        //        for(const auto&k:m_shape->getMiddlePoints()){
        //            painter->fillRect(HGraphicsItem::getPointRect(scale,k.get2DPoint()),Qt::darkCyan);
        //        }

        pen.setWidthF(2);
    }else{
        pen.setColor(isEnabled()?(isHighlight?Qt::red:QColor(data(HGraphicsItem::color).toUInt())):Qt::gray);
    }



    pen.setStyle(isHighlight?Qt::DashDotDotLine:Qt::SolidLine);

    painter->setPen(pen);
    painter->drawPath(m_path);

    pen.setWidthF(0);
    painter->setPen(pen);
}

void HGraphicsPathItem::setShape(QSharedPointer<RShape> sh)
{
    m_shape.clear();
    m_path.clear();
    m_shape=sh;
    m_path=m_shape;

}

void HGraphicsPathItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    auto mkg=((HGraphicsScene*)scene());
    double size=mkg->getMMouseSize()*0.5;
    auto mpos=event->scenePos();
    RVector mPt=mpos;
    double mDis=LLONG_MAX;
    for(const auto&k:m_shape->getVectorProperties()){
        auto mmmmk=k+RVector(pos().x(),pos().y());
        if(mmmmk.x>(mpos.x()-size)&&mmmmk.x<(mpos.x()+size)&&mmmmk.y>(mpos.y()-size)&&mmmmk.y<(mpos.y()+size)){
            if(mmmmk.getDistanceTo2D(mpos)<mDis){
                mPt=mmmmk;
            }
        }
    }
    mkg->setMMousePos(mPt.get2DPoint());
    QGraphicsItem::mousePressEvent(event);
}

void HGraphicsPathItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsItem::mouseMoveEvent(event);
}

void HGraphicsPathItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void HGraphicsPathItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
}

void HGraphicsPathItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
}

void HGraphicsPathItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{

    auto mkg=((HGraphicsScene*)scene());
    double size=mkg->getMMouseSize()*0.5;
    auto mpos=event->scenePos();
    RVector mPt=mpos;
    double mDis=LLONG_MAX;
    for(const auto&k:m_shape->getVectorProperties()){
        auto mmmmk=k+RVector(pos().x(),pos().y());
        if(mmmmk.x>(mpos.x()-size)&&mmmmk.x<(mpos.x()+size)&&mmmmk.y>(mpos.y()-size)&&mmmmk.y<(mpos.y()+size)){
            if(mmmmk.getDistanceTo2D(mpos)<mDis){
                mPt=mmmmk;
            }
        }
    }
    mkg->setMMousePos(mPt.get2DPoint());

    QGraphicsItem::hoverMoveEvent(event);
}

QVariant HGraphicsPathItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange) {
        // value is the new position.
        QPointF newPos = value.toPointF();
        QPointF dpt=newPos-pos();
        m_shape=m_shape->getTransformed(QTransform::fromTranslate(dpt.x(),dpt.y()));
        //          QRectF rect = scene()->sceneRect();
        //          if (!rect.contains(newPos)) {
        //              // Keep the item inside the scene rect.
        //              newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
        //              newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
        //              return newPos;
        //          }
    }
    return QGraphicsItem::itemChange(change, value);
}

bool HGraphicsPathItem::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    return true;
}















