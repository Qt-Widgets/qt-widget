#ifndef HGRAPHICSLINEITEM_H
#define HGRAPHICSLINEITEM_H

#include "hgraphicsitem.h"

class HGraphicsLineItem : public HGraphicsItem
{

public:
    enum MType{ Type = UserType + 105 };
    explicit HGraphicsLineItem(HGraphicsItem *parent = nullptr);
    explicit HGraphicsLineItem(const QPointF &pos, qreal x2, qreal y2, QGraphicsItem *parent = nullptr);
    explicit HGraphicsLineItem(qreal posX,qreal posY, qreal x2, qreal y2, QGraphicsItem *parent = nullptr);
    explicit HGraphicsLineItem(const QPointF &pos,const QPointF &p2, QGraphicsItem *parent = nullptr);
    ~HGraphicsLineItem() override;

    virtual void HDataInputStream(QDataStream & input) override{
        input>>line>>color>>stuff;
    }
    virtual void HDataOutputStream(QDataStream & output) const override{
         output<<line<<color<<stuff;
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

    QLineF line;
    QColor color;
    QVector<QPointF> stuff;

};

#endif // HGRAPHICSLINEITEM_H
