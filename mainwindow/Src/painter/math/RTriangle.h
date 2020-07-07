

#ifndef RTRIANGLE_H
#define RTRIANGLE_H



#include "RShape.h"
#include "RVector.h"
#include "RLine.h"
#include "RExplodable.h"

class RBox;

#ifndef RDEFAULT_MIN1
#define RDEFAULT_MIN1 -1
#endif



class  RTriangle: public RShape {
public:
    RTriangle();
    RTriangle(const RVector& p1, const RVector& p2, const RVector& p3);
    virtual ~RTriangle();

    virtual RShape::Type getShapeType() const {
        return Triangle;
    }

    void HDataInputStream(QDataStream & input){
         input>>corner[0]>>corner[1]>>corner[2];
    }

    void HDataOutputStream(QDataStream & output) const{
        output<<corner[0]<<corner[1]<<corner[2];
    }

    bool HGetA3200Gcode(QString &code,const QStringList&paras,const QTransform& trans, RLine &endPt) const;

    virtual RTriangle* clone() const {
        return new RTriangle(*this);
    }

    virtual void setZ(double z);

    virtual QList<RVector> getVectorProperties() const;

    static RTriangle createArrow(const RVector& position, double direction, double size);

    virtual RBox getBoundingBox() const;
    virtual double getLength() const;
    double getArea() const;
    RVector getCorner(int i) const;
    void setCorner(int i, const RVector& p);

    virtual QList<RVector> getEndPoints() const;
    virtual QList<RVector> getMiddlePoints() const;
    virtual QList<RVector> getCenterPoints() const;
    virtual QList<RVector> getPointsWithDistanceToEnd(
        double distance, int from = RS::FromAny) const;
    virtual QList<RVector> getPointCloud(double segmentLength) const;

    virtual double getDistanceTo(const RVector& point, bool limited = true, double strictRange = RMAXDOUBLE) const;
    virtual RVector getVectorTo(const RVector& point, bool limited = true, double strictRange = RMAXDOUBLE) const;
    //virtual RVector getVectorTo(const RLine& line, bool limited = true) const;

    virtual RVector getNormal() const;

//  virtual bool intersectsWith(const RShape& other, bool limited) const;

//  virtual QList<RVector> getIntersectionPoints(const RShape& other,
//        bool limited) const;

    bool isPointInTriangle(const RVector& p, bool treatAsQuadrant = false) const;
    bool isPointInQuadrant(const RVector& p) const;

    double getD() const;

    virtual QList<QSharedPointer<RShape> > getExploded(int segments = RDEFAULT_MIN1) const;

    virtual bool move(const RVector& offset) {
        corner[0].move(offset);
        corner[1].move(offset);
        corner[2].move(offset);
        return true;
    }
    virtual bool rotate(double rotation, const RVector& center = RDEFAULT_RVECTOR) {
        corner[0].rotate(rotation, center);
        corner[1].rotate(rotation, center);
        corner[2].rotate(rotation, center);
        return true;
    }
    virtual bool scale(const RVector& scaleFactors, const RVector& center =
            RVector()) {
        corner[0].scale(scaleFactors, center);
        corner[1].scale(scaleFactors, center);
        corner[2].scale(scaleFactors, center);
        return true;
    }
    virtual bool mirror(const RLine& axis) {
        corner[0].mirror(axis);
        corner[1].mirror(axis);
        corner[2].mirror(axis);
        return true;
    }
    virtual bool flipHorizontal() {
        corner[0].flipHorizontal();
        corner[1].flipHorizontal();
        corner[2].flipHorizontal();
        return true;
    }
    virtual bool flipVertical() {
        corner[0].flipVertical();
        corner[1].flipVertical();
        corner[2].flipVertical();
        return true;
    }

    QSharedPointer<RShape> getTransformed(const QTransform& transform) const {
        return QSharedPointer<RShape>(
            new RTriangle(
                corner[0].getTransformed2D(transform),
                corner[1].getTransformed2D(transform),
                corner[2].getTransformed2D(transform)
            )
        );
    }

protected:
    virtual void print(QDebug dbg) const;

public:
    RVector corner[3];
};

Q_DECLARE_METATYPE(QList<RTriangle>)
Q_DECLARE_METATYPE(RTriangle*)
Q_DECLARE_METATYPE(const RTriangle*)
Q_DECLARE_METATYPE(RTriangle)
Q_DECLARE_METATYPE(QSharedPointer<RTriangle>)
Q_DECLARE_METATYPE(QSharedPointer<RTriangle>*)

#endif
