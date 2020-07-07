

#ifndef RDIRECTED_H
#define RDIRECTED_H

#include "core_global.h"

#include <QSharedPointer>

#include "RShape.h"

/**
 * Interface for directed shape classes.
 *
 * \ingroup math
 * \scriptable
 * \sharedPointerSupport
 */
class  RDirected {
public:
    virtual ~RDirected() {}

//    virtual double getDirection1() const = 0;
//    virtual double getDirection2() const = 0;

//    virtual RS::Side getSideOfPoint(const RVector& point) const = 0;

//    virtual RVector getStartPoint() const = 0;
//    virtual RVector getEndPoint() const = 0;
//    virtual RVector getMiddlePoint() const = 0;

//    virtual bool reverse() = 0;

//    /**
//     * \param trimPoint Trim start point or end point to this coordinate.
//     * \param clickPoint Point that was clicked by user.
//     * \param extend True if the shape is to be extended.
//     * Used for polylines to determine that the first segment has to be trimmed even if another segment was clicked.
//     */
//    virtual bool trimStartPoint(const RVector& trimPoint, const RVector& clickPoint = RVector::invalid, bool extend = false) = 0;
//    virtual bool trimStartPoint(double trimDist) {
//        return trimStartPoint();
//    }

//    /**
//     * \param trimPoint Trim start point or end point to this coordinate.
//     * \param clickPoint Point that was clicked by user.
//     * \param extend True if the shape is to be extended.
//     * Used for polylines to determine that the last segment has to be trimmed even if another segment was clicked.
//     */
//    virtual bool trimEndPoint(const RVector& trimPoint, const RVector& clickPoint = RVector::invalid, bool extend = false) = 0;

//    /**
//     * \param trimPoint Trim start point or end point to this coordinate.
//     * \param clickPoint Point that was clicked by user.
//     */
//    virtual RS::Ending getTrimEnd(const RVector& trimPoint, const RVector& clickPoint) = 0;

//    /**
//     * \return Distance of given point to start point of shape along shape.
//     * \param p Point (assumed to be on shape).
//     */
//    virtual double getDistanceFromStart(const RVector& p) const {
//        Q_UNUSED(p)
//        return RMAXDOUBLE;
//    }

//    /**
//     * \return All possible distances of given point to start point of shape along shape.
//     * \param p Point (assumed to be on shape).
//     */
//    virtual QList<double> getDistancesFromStart(const RVector& p) const {
//        return QList<double>() << getDistanceFromStart(p);
//    }
};

Q_DECLARE_METATYPE(RDirected*)
Q_DECLARE_METATYPE(const RDirected*)
Q_DECLARE_METATYPE(QSharedPointer<RDirected>)
Q_DECLARE_METATYPE(QSharedPointer<RDirected>*)

#endif
