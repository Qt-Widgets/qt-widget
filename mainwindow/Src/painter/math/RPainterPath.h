

#ifndef RPAINTERPATH_H
#define RPAINTERPATH_H


#include <QBrush>
#include <QFlags>
#include <QPainterPath>
#include <QPen>
#include <QSharedPointer>

#include "RBox.h"
#include "RVector.h"

class RArc;
class RShape;
class RSpline;
class RPoint;


/**
 * Extended painter path with a z-level and a pen.
 *
 * \ingroup core
 * \scriptable
 * \copyable
 */
class  RPainterPath : public QPainterPath {

public:
    enum Mode {
        NoModes = 0x000,
        Selected = 0x001,              //!< show as selected
        Highlighted = 0x002,           //!< show as highlighted
        Invalid = 0x004,
        FixedPenColor = 0x008,
        FixedBrushColor = 0x010,
        AutoRegen = 0x020,             //!< arcs (regen on zoom change)
        AlwaysRegen = 0x040,
        InheritPen = 0x080,            //!< inherit pen from entity / context
        PixelUnit = 0x100,             //!< path displayed in pixel (not drawing units)
        NoClipping = 0x200,            //!< disable clipping for this path
        PixelWidth = 0x400,            //!< interpret width in pixels
        NoColorMode = 0x800            //!< disable color mode
    };
    Q_DECLARE_FLAGS(Modes, Mode)

public:
    RPainterPath();
    RPainterPath(const QPainterPath& path);
    RPainterPath(const RPainterPath& other);
    RPainterPath(QSharedPointer<RShape> sshap);
    virtual ~RPainterPath();

    void setPath(const QPainterPath& path);

    QList<QSharedPointer<RShape> > getShapes() const;

    RVector getCurrentPosition() const {
        return RVector(currentPosition().x(), currentPosition().y());
    }

    bool isAtPosition(const RVector& p, double tolerance = RS::PointTolerance) const;

    void moveTo(const RVector& v) {
        QPainterPath::moveTo(v.x, v.y);
    }

    void moveToOrNop(const RVector& v) {
        if (!isAtPosition(v)) {
            moveTo(v);
        }
    }

    void moveTo(qreal x, qreal y) {
        QPainterPath::moveTo(x, y);
    }

    /**
     * \nonscriptable
     */
    void moveTo(const QPointF& p) {
        QPainterPath::moveTo(p);
    }

    void lineTo(const RVector& v) {
        QPainterPath::lineTo(v.x, v.y);
    }

    void lineTo(qreal x, qreal y) {
        QPainterPath::lineTo(x, y);
    }

    /**
     * \nonscriptable
     */
    void lineTo(const QPointF& p) {
        QPainterPath::lineTo(p);
    }

    void quadTo(const RVector& cp, const RVector& v) {
        QPainterPath::quadTo(cp.x, cp.y, v.x, v.y);
    }

    void quadTo(qreal ctrlPtx, qreal ctrlPty, qreal endPtx, qreal endPty) {
        QPainterPath::quadTo(ctrlPtx, ctrlPty, endPtx, endPty);
    }

    void cubicTo(const RVector& cp1, const RVector& cp2, const RVector& v) {
        QPainterPath::cubicTo(cp1.x, cp1.y, cp2.x, cp2.y, v.x, v.y);
    }

    void cubicTo(qreal ctrlPt1x, qreal ctrlPt1y, qreal ctrlPt2x, qreal ctrlPt2y, qreal endPtx, qreal endPty) {
        QPainterPath::cubicTo(ctrlPt1x, ctrlPt1y, ctrlPt2x, ctrlPt2y, endPtx, endPty);
    }

    void closeSubpath() {
        QPainterPath::closeSubpath();
    }

//    bool contains(const QPointF& point) const {
//        return QPainterPath::contains(point);
//    }

//    bool contains(const QRectF& rectangle) const {
//        return QPainterPath::contains(rectangle);
//    }

//    bool contains(const QPainterPath& p) const {
//        return QPainterPath::contains(p);
//    }

    bool containsPoint(const RVector& v) const {
        return QPainterPath::contains(QPointF(v.x, v.y));
    }

    void addPolyline(const RPolyline& pl);
    void addPath(const RPainterPath& path);
    void appendPath(const RPainterPath& path);
    void addLine(const RLine& line);
    void addArc(const RArc& arc);
    void addSpline(const RSpline& spline);
    void addPt(const RPoint& pt);

    void addRect(double x1, double y1, double x2, double y2);
    void addRect(const QRectF& rect);
    void addBox(const RBox& box);

    RBox getBoundingBox() const;

    RVector getStartPoint() const;
    RVector getEndPoint() const;

    bool isValid() const;
    void setValid(bool on);

    bool isSane() const;

    int getZLevel() const;
    void setZLevel(int zl);

    //bool hasPen() const;
    QPen getPen() const;
    void setPen(const QPen& p);

    QBrush getBrush() const;
    void setBrush(const QBrush& b);

    void setMode(RPainterPath::Mode mode, bool on = true);
    bool getMode(RPainterPath::Mode mode) const;

    void setHighlighted(bool on);
    bool isHighlighted() const;

    void setSelected(bool on);
    bool isSelected() const;

    void setFixedPenColor(bool on);
    bool isFixedPenColor() const;

    void setFixedBrushColor(bool on);
    bool isFixedBrushColor() const;

    void setAutoRegen(bool on);
    bool getAutoRegen() const;

    void setAlwaysRegen(bool on);
    bool getAlwaysRegen() const;

    void setInheritPen(bool on);
    bool getInheritPen() const;

    void setPixelUnit(bool on);
    bool getPixelUnit() const;

    void setNoClipping(bool on);
    bool getNoClipping() const;

    void setNoColorMode(bool on);
    bool getNoColorMode() const;

    void setPixelWidth(bool on);
    bool getPixelWidth() const;

    void setFeatureSize(double s);
    double getFeatureSize() const;

    void setPixelSizeHint(double s);
    double getPixelSizeHint() const;

    double getDistanceTo(const RVector& point) const;

    void addPoint(const RVector& position);
    bool hasPoints();
    void setPoints(const QList<RVector>& p);
    QList<RVector> getPoints() const;

    void transform(const QTransform& t);
    void move(const RVector& offset);
    void rotate(double angle);
    void scale(double fx, double fy);

    int getElementCount() const;
    double getXAt(int i) const;
    double getYAt(int i) const;
    QPainterPath::ElementType getTypeAt(int i) const;
    bool isEmpty() const;

    void addShape(QSharedPointer<RShape> shape);

    void addOriginalShape(QSharedPointer<RShape> shape);
    bool hasOriginalShapes() const;
    int countOriginalShapes() const;
    QSharedPointer<RShape> getOriginalShape(int i) const;

    static void rotateList(QList<RPainterPath>& pps, double angle);
    static void translateList(QList<RPainterPath>& pps, const RVector& offset);
    static void scaleList(QList<RPainterPath>& pps, double fx, double fy);
    static RVector getMinList(QList<RPainterPath>& pps);
    static RVector getMaxList(QList<RPainterPath>& pps);


private:
    int zLevel;
    QPen pen;
    QBrush brush;
    Modes modes;
    QList<RVector> points;
    // < 0 for secondary path (e.g. bounding box of text)
    double featureSize;
    double pixelSizeHint;
    QList<QSharedPointer<RShape> > originalShapes;
};

 QDebug operator<<(QDebug dbg, RPainterPath& p);

//QDataStream& operator<< (QDataStream& stream, const RPainterPath& path);
//QDataStream& operator>> (QDataStream& stream, RPainterPath& path);
 bool operator< (const RPainterPath& p1, const RPainterPath& p2);

Q_DECLARE_OPERATORS_FOR_FLAGS(RPainterPath::Modes)
Q_DECLARE_METATYPE(RPainterPath)
Q_DECLARE_METATYPE(RPainterPath*)
Q_DECLARE_METATYPE(QList<RPainterPath>)
Q_DECLARE_METATYPE(QList<RPainterPath>*)
Q_DECLARE_METATYPE(RPainterPath::Mode)
Q_DECLARE_METATYPE(RPainterPath::Mode*)

#endif
