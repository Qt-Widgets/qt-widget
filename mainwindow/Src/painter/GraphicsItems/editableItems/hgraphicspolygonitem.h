#ifndef HGRAPHICSPOLYGONITEM_H
#define HGRAPHICSPOLYGONITEM_H
#include "hgraphicsitem.h"

class HGraphicsPolygonItem : public HGraphicsItem
{
public:
    enum MType{ Type = UserType +108 };
    explicit HGraphicsPolygonItem(const QVariant &data,QGraphicsItem *parent = nullptr);
    ~HGraphicsPolygonItem() override;

    virtual void HDataInputStream(QDataStream & input) override{

    }
    virtual void HDataOutputStream(QDataStream & output) const override{

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
};

#endif // HGRAPHICSPOLYGONITEM_H
