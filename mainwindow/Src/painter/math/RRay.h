#ifndef RRAY_H
#define RRAY_H


#include "RXLine.h"
#include "RS.h"
#include "RShape.h"
#include "RVector.h"

class RBox;
class RPolyline;

/**
 * Low-level mathematical representation of a ray.
 *
 * \ingroup math
 * \scriptable
 * \sharedPointerSupport
 * \copyable
 * \hasStreamOperator
 */
class  RRay: public RXLine {
public:
    RRay();
    RRay(const RLine& line);
    RRay(const RVector& basePoint, const RVector& directionVector);
    RRay(const RVector& basePoint, double angle, double distance);
    virtual ~RRay();

    virtual RShape::Type getShapeType() const {
        return Ray;
    }

    void HDataInputStream(QDataStream & input){

    }

    void HDataOutputStream(QDataStream & output) const{

    }

    bool HGetA3200Gcode(QString &code,const QStringList&paras,const QTransform& trans,  RLine &endPt) const{
        return true;
    }

    virtual RRay* clone() const {
        return new RRay(*this);
    }

    virtual QList<RVector> getVectorProperties() const;

    virtual bool trimEndPoint(const RVector& trimPoint, const RVector& clickPoint = RVector::invalid, bool extend = false);
    virtual QList<RVector> getPointsWithDistanceToEnd(double distance, int from) const;

    virtual bool reverse();
    virtual RLine getClippedLine(const RBox& box) const;
    virtual RVector getVectorTo(const RVector& point, bool limited = true, double strictRange = RMAXDOUBLE) const;

    virtual bool stretch(const RPolyline& area, const RVector& offset);

    virtual QList<QSharedPointer<RShape> > splitAt(const QList<RVector>& points) const;

protected:
    virtual void print(QDebug dbg) const;
};

Q_DECLARE_METATYPE(const RRay*)
Q_DECLARE_METATYPE(RRay*)
Q_DECLARE_METATYPE(RRay)
Q_DECLARE_METATYPE(QSharedPointer<RRay>)
Q_DECLARE_METATYPE(QSharedPointer<RRay>*)

#endif
