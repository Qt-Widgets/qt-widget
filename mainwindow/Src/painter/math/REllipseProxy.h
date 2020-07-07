

#ifndef RELLIPSEPROXY_H
#define RELLIPSEPROXY_H

#include "core_global.h"

#include "RS.h"

class REllipse;
class RSpline;
class RPolyline;


/**
 * Proxy for advanced ellipse functionality. May be implemented
 * by a plugin.
 *
 * \ingroup math
 */
class  REllipseProxy {
public:
    virtual ~REllipseProxy() {}

    virtual QList<RSpline> approximateWithSplines(const REllipse& ellipse) const = 0;
    virtual RPolyline approximateWithArcs(const REllipse& ellipse, int segments) const = 0;
};

#endif
