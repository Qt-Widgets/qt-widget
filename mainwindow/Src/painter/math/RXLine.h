

#ifndef RXLINE_H
#define RXLINE_H


#include "RLine.h"
#include "RS.h"
#include "RShape.h"
#include "RVector.h"

class RBox;
class RPolyline;

/**
 * Low-level mathematical representation of an infinite line.
 *
 * \ingroup math
 * \scriptable
 * \sharedPointerSupport
 * \copyable
 * \hasStreamOperator
 */
class  RXLine: public RShape {
public:
    RXLine();
    RXLine(const RLine& line);
    RXLine(const RVector& basePoint, const RVector& directionVector);
    RXLine(const RVector& basePoint, double angle, double distance);
    virtual ~RXLine();

    virtual RShape::Type getShapeType() const {
        return XLine;
    }
    void HDataInputStream(QDataStream & input){
        input>>basePoint>>directionVector;
    }

    void HDataOutputStream(QDataStream & output) const{
        output<<basePoint<<directionVector;
    }

   bool HGetA3200Gcode(QString &code,const QStringList&paras,const QTransform& trans,  RLine &endPt) const;

    RLine getLineShape() const {
        return RLine(basePoint, basePoint + directionVector);
    }

    virtual RXLine* clone() const {
        return new RXLine(*this);
    }

    virtual bool isDirected() const {
        return true;
    }

    RBox getBoundingBox() const;

    virtual void setZ(double z);

    virtual QList<RVector> getVectorProperties() const;

    virtual QList<RVector> getEndPoints() const;
    virtual QList<RVector> getMiddlePoints() const;
    virtual QList<RVector> getCenterPoints() const;
    virtual QList<RVector> getPointsWithDistanceToEnd(
            double distance, int from = RS::FromAny) const;
    virtual QList<RVector> getPointCloud(double segmentLength) const;

    virtual double getAngleAt(double distance, RS::From from = RS::FromStart) const;

    virtual RVector getVectorTo(const RVector& point, bool limited = true, double strictRange = RMAXDOUBLE) const;

    RVector getBasePoint() const;
    void setBasePoint(const RVector& vector);
    RVector getSecondPoint() const;
    void setSecondPoint(const RVector& vector);
    RVector getDirectionVector() const;
    void setDirectionVector(const RVector& vector);

    virtual RVector getMiddlePoint() const;
    
    double getLength() const;
    void setLength(double l);
    double getAngle() const;
    void setAngle(double a);

    virtual double getDirection1() const;
    virtual double getDirection2() const;

    virtual RS::Side getSideOfPoint(const RVector& point) const;

    virtual RVector getStartPoint() const;
    virtual RVector getEndPoint() const;

    virtual bool trimStartPoint(const RVector& trimPoint, const RVector& clickPoint = RVector::invalid, bool extend = false);
    virtual bool trimEndPoint(const RVector& trimPoint, const RVector& clickPoint = RVector::invalid, bool extend = false);
    virtual bool trimStartPoint(double trimDist) {
        return RShape::trimStartPoint(trimDist);
    }
    virtual bool trimEndPoint(double trimDist) {
        return RShape::trimEndPoint(trimDist);
    }
    virtual RS::Ending getTrimEnd(const RVector& trimPoint, const RVector& clickPoint);
    virtual double getDistanceFromStart(const RVector& p) const;

    virtual RLine getClippedLine(const RBox& box) const;

    virtual bool move(const RVector& offset);
    virtual bool rotate(double rotation, const RVector& center = RDEFAULT_RVECTOR);
    virtual bool scale(const RVector& scaleFactors, const RVector& center = RDEFAULT_RVECTOR);
    virtual bool mirror(const RLine& axis);
    virtual bool reverse();
    virtual bool stretch(const RPolyline& area, const RVector& offset);

    virtual QSharedPointer<RShape> getTransformed(const QTransform& transform) const;

    virtual QList<QSharedPointer<RShape> > getOffsetShapes(double distance, int number, RS::Side side, const RVector& position = RVector::invalid) {
        return RShape::getOffsetLines(*this, distance, number, side, position);
    }

    virtual QList<QSharedPointer<RShape> > splitAt(const QList<RVector>& points) const;

protected:
    virtual void print(QDebug dbg) const;

public:
    /**
     * \getter{getBasePoint}
     * \setter{setBasePoint}
     */
    RVector basePoint;
    /**
     * \getter{getDirectionVector}
     * \setter{setDirectionVector}
     */
    RVector directionVector;
};

Q_DECLARE_METATYPE(const RXLine*)
Q_DECLARE_METATYPE(RXLine*)
Q_DECLARE_METATYPE(RXLine)
Q_DECLARE_METATYPE(QSharedPointer<RXLine>)
Q_DECLARE_METATYPE(QSharedPointer<RXLine>*)

#endif
