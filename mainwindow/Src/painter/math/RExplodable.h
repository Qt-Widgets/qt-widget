

#ifndef REXPLODABLE_H
#define REXPLODABLE_H


#include <QSharedPointer>

#include "RShape.h"

#ifndef RDEFAULT_MIN1
#define RDEFAULT_MIN1 -1
#endif

/**
 * Interface for explodable shape classes.
 *
 * \ingroup math
 * \scriptable
 * \sharedPointerSupport
 */
class  RExplodable {
public:
    virtual ~RExplodable() {}
    virtual QList<QSharedPointer<RShape> > getExploded(int segments = RDEFAULT_MIN1) const = 0;
};

Q_DECLARE_METATYPE(RExplodable*)
Q_DECLARE_METATYPE(const RExplodable*)
Q_DECLARE_METATYPE(QSharedPointer<RExplodable>)
Q_DECLARE_METATYPE(QSharedPointer<RExplodable>*)

#endif
