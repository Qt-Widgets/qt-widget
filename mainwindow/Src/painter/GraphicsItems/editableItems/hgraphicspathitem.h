#ifndef HGRAPHICSPATHITEM_H
#define HGRAPHICSPATHITEM_H
#include "hgraphicsitem.h"

#include "Src/painter/math/RShape.h"
#include "Src/painter/math/RPainterPath.h"
#include <QSharedPointer>





class HGraphicsPathItem : public HGraphicsItem
{
public:
    enum MType{ Type = UserType + 106 };
    explicit HGraphicsPathItem(QGraphicsItem *parent = nullptr);
    explicit HGraphicsPathItem(QSharedPointer<RShape> sh,QGraphicsItem *parent = nullptr,bool isEditable=true);
    ~HGraphicsPathItem() override;

    void HDataInputStream(QDataStream & input) override;
    void HDataOutputStream(QDataStream & output) const override;

    bool getA3200Gcode(QString &code, QStringList &paras, RLine&endPt, qint32 mode,  QTransform trans) override;
    QPointF getCenter()  override;
    double getDistanceToShape(const QPointF &pt) const override;
    QPainterPath getScenePath() const override;
    QVariantList getPropertyParas() const override;

    qint32 type() const override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;


    void setShape(QSharedPointer<RShape> sh) ;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;

    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override;


private:
    QSharedPointer<RShape> m_shape;
    RPainterPath m_path;

};

#endif // HGRAPHICSPATHITEM_H
