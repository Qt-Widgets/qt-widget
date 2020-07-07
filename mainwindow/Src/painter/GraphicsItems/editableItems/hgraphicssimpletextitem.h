#ifndef HGRAPHICSSIMPLETEXTITEM_H
#define HGRAPHICSSIMPLETEXTITEM_H
#include "hgraphicsitem.h"

class HGraphicsSimpleTextItem : public HGraphicsItem
{
public:
    enum MType{ Type = UserType + 110 };
    explicit HGraphicsSimpleTextItem(const QVariant &data,QGraphicsItem *parent = nullptr);
    ~HGraphicsSimpleTextItem() override;

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

#endif // HGRAPHICSSIMPLETEXTITEM_H
