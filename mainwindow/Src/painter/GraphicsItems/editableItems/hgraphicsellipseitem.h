#ifndef HGRAPHICSELLIPSEITEM_H
#define HGRAPHICSELLIPSEITEM_H

#include "hgraphicsitem.h"

class HGraphicsEllipseItem : public HGraphicsItem
{
public:
    enum MType{ Type = UserType +102 };
    explicit HGraphicsEllipseItem(QGraphicsItem *parent = nullptr);
    explicit HGraphicsEllipseItem(const QPointF &pos,qreal r1,qreal r2, QGraphicsItem *parent = nullptr);
    explicit HGraphicsEllipseItem(qreal posX,qreal posY,qreal r1,qreal r2, QGraphicsItem *parent = nullptr);
    ~HGraphicsEllipseItem() override;

    virtual void HDataInputStream(QDataStream & input) override{
        input>>ellipseRect>>color>>stuff;
    }
    virtual void HDataOutputStream(QDataStream & output) const override{
         output<<ellipseRect<<color<<stuff;
    }


    qint32 type() const override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    QPainterPath getScenePath() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

     QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;

private:
    QRectF ellipseRect;
    QColor color;
    QVector<QPointF> stuff;

};

#endif // HGRAPHICSELLIPSEITEM_H
