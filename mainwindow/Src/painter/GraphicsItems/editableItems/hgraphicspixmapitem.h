#ifndef HGRAPHICSPIXMAPITEM_H
#define HGRAPHICSPIXMAPITEM_H
#include "hgraphicsitem.h"

class HGraphicsPixmapItem : public HGraphicsItem
{
public:
    enum MType{ Type = UserType + 107 };
    explicit HGraphicsPixmapItem(const QVariant &data,QGraphicsItem *parent = nullptr);
    ~HGraphicsPixmapItem() override;

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

#endif // HGRAPHICSPIXMAPITEM_H
