

#ifndef RPOINT_H
#define RPOINT_H


#include "RShape.h"
#include "RVector.h"
#include "RBox.h"

/**
 * Low-level mathematical representation of a point.
 *
 * \ingroup math
 * \scriptable
 * \sharedPointerSupport
 * \copyable
 * \hasStreamOperator
 */
class  RPoint: public RShape {
public:
    RPoint();
    RPoint(double x, double y);
    RPoint(const RVector& position);
    virtual ~RPoint();

    virtual RShape::Type getShapeType() const {
        return Point;
    }

    void HDataInputStream(QDataStream & input){
         input>>position;
    }

   void HDataOutputStream(QDataStream & output) const{
          output<<position;
    }

    bool HGetA3200Gcode(QString &code,const QStringList&paras,const QTransform& trans, RLine &endPt) const;

    virtual RPoint* clone() const {
        return new RPoint(*this);
    }

    virtual void setZ(double z);

    virtual QList<RVector> getVectorProperties() const;

    RVector getPosition() const {
       return position;
    }

    void setPosition(const RVector& p) {
       position = p;
    }

    virtual RBox getBoundingBox() const;
    virtual double getLength() const;

    virtual QList<RVector> getEndPoints() const;
    virtual QList<RVector> getMiddlePoints() const;
    virtual QList<RVector> getCenterPoints() const;
    virtual QList<RVector> getPointsWithDistanceToEnd(
        double distance, int from = RS::FromAny) const;
    virtual QList<RVector> getPointCloud(double segmentLength) const;

    virtual double getAngleAt(double distance, RS::From from = RS::FromStart) const;

    virtual RVector getVectorTo(const RVector& point,
            bool limited = true, double strictRange = RMAXDOUBLE) const;

    virtual bool move(const RVector& offset);
    virtual bool rotate(double rotation, const RVector& center = RDEFAULT_RVECTOR);
    virtual bool scale(const RVector& scaleFactors, const RVector& center = RDEFAULT_RVECTOR);
    virtual bool mirror(const RLine& axis);
    virtual bool flipHorizontal();
    virtual bool flipVertical();

    virtual QSharedPointer<RShape> getTransformed(const QTransform& transform) const;

protected:
    virtual void print(QDebug dbg) const;

public:
    /**
     * \getter{getPosition}
     * \setter{setPosition}
     */
    RVector position;
};

Q_DECLARE_METATYPE(RPoint)
Q_DECLARE_METATYPE(RPoint*)
Q_DECLARE_METATYPE(QSharedPointer<RPoint>)
Q_DECLARE_METATYPE(QSharedPointer<RPoint>*)

#endif
