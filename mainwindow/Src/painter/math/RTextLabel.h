

#ifndef RTEXTLABEL_H
#define RTEXTLABEL_H

#include "core_global.h"

#include "RPoint.h"
#include "RVector.h"
#include "RBox.h"

#ifndef RDEFAULT_QVARIANT
#define RDEFAULT_QVARIANT QVariant()
#endif

class  RTextLabel: public RPoint {
public:
    RTextLabel();
    RTextLabel(const RVector& position, const QString& text, const QVariant& userData = RDEFAULT_QVARIANT);
    virtual ~RTextLabel();

    virtual RTextLabel* clone() const {
        return new RTextLabel(*this);
    }

    QString getText() const {
        return text;
    }
    QVariant getUserData() const {
        return userData;
    }

protected:
    virtual void print(QDebug dbg) const;

public:
    QString text;
    QVariant userData;
};

Q_DECLARE_METATYPE(RTextLabel)
Q_DECLARE_METATYPE(RTextLabel*)
Q_DECLARE_METATYPE(QSharedPointer<RTextLabel>)
Q_DECLARE_METATYPE(QSharedPointer<RTextLabel>*)
Q_DECLARE_METATYPE(QList<RTextLabel>)

#endif
