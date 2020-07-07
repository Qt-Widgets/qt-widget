#ifndef HGRAPHICSLAYER_H
#define HGRAPHICSLAYER_H


#include "hgraphicsitem.h"


class HGraphicsLayer :  public HGraphicsItem
{
public:
    enum MType{ Type = UserType + 104 };
    explicit HGraphicsLayer(QGraphicsItem *parent = nullptr);
    ~HGraphicsLayer() override;

    void HDataInputStream(QDataStream & input) override;
    void HDataOutputStream(QDataStream & output) const override;

    qint32 type() const override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

     QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;

private:

};

#endif // HGRAPHICSLAYER_H
