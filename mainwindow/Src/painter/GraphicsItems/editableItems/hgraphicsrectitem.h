#ifndef HGRAPHICSRECTITEM_H
#define HGRAPHICSRECTITEM_H
#include "hgraphicsitem.h"

class HGraphicsRectItem : public HGraphicsItem
{
public:
    enum MType{ Type = UserType + 109 };
    explicit HGraphicsRectItem(QGraphicsItem *parent = nullptr);
    explicit HGraphicsRectItem(const QRectF &r,QGraphicsItem *parent = nullptr,bool isEditable=true);
    ~HGraphicsRectItem() override;

    virtual void HDataInputStream(QDataStream & input) override;
    virtual void HDataOutputStream(QDataStream & output) const override;


    bool getA3200Gcode(QString &code, QStringList &paras, RLine&endPt, qint32 mode,  QTransform trans) override;
    QPointF getCenter()  override;
    QPainterPath getScenePath() const override;
    QVariantList getPropertyParas() const override;

    void setRect(const QRectF &r);

    qint32 type() const override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

private:

    QList<QPointF> getVectorProperties() const;
    QRectF rect=QRectF(0,0,0,0);
    qreal scale=1.0;
};

#endif // HGRAPHICSRECTITEM_H
