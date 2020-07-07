#ifndef HGRAPHICSITEMGROUP_H
#define HGRAPHICSITEMGROUP_H
#include "hgraphicsitem.h"

class HGraphicsItemGroup : public HGraphicsItem
{
public:
    enum MType{ Type = UserType + 103 };
    explicit HGraphicsItemGroup(QGraphicsItem *parent = nullptr);
    ~HGraphicsItemGroup() override;

    void HDataInputStream(QDataStream & input) override;
    void HDataOutputStream(QDataStream & output) const override;

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

    QList<HGraphicsItem*> m_groupItems;


};

#endif // HGRAPHICSITEMGROUP_H
