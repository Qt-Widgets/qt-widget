#ifndef HGRAPHICSARCITEM_H
#define HGRAPHICSARCITEM_H

#include "hgraphicsitem.h"

class HGraphicsArcItem : public HGraphicsItem
{
public:
    enum MType{ Type = UserType + 101 };
    explicit HGraphicsArcItem(QGraphicsItem *parent = nullptr);
    explicit HGraphicsArcItem(const QPointF &pos,qreal r1,qreal r2,qreal angle1 ,qreal angle2,QGraphicsItem *parent = nullptr);
    explicit HGraphicsArcItem(qreal posX,qreal posY,qreal r1,qreal r2,qreal angle1 ,qreal angle2, QGraphicsItem *parent = nullptr);

    ~HGraphicsArcItem() override;

    virtual void HDataInputStream(QDataStream & input) override{
        input>>ellipseRect>>mpath>>angle1>>angle2>>isCCW>>color;
    }
    virtual void HDataOutputStream(QDataStream & output) const override{
         output<<ellipseRect<<mpath<<angle1<<angle2<<isCCW<<color;
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

private:

    void refreshPath();

    QRectF ellipseRect;
    QPainterPath mpath;
    qreal angle1=0;
    qreal angle2=90;
    bool isCCW=1;
    QColor color;

};

#endif // HGRAPHICSARCITEM_H
