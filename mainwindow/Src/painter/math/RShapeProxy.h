

#ifndef RSHAPEPROXY_H
#define RSHAPEPROXY_H


#include "RS.h"
#include "RMath.h"

class RShape;


/**
 * Proxy for advanced shape functionality. May be implemented
 * by a plugin.
 *
 * \ingroup math
 */
class  RShapeProxy {
public:
    virtual ~RShapeProxy() {}

    virtual QList<QSharedPointer<RShape> > roundAllCorners(const QList<QSharedPointer<RShape> >& shapes, double radius) = 0;
};

#endif
