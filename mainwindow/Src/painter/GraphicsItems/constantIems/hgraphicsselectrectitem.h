#ifndef HGRAPHICSSELECTRECTITEM_H
#define HGRAPHICSSELECTRECTITEM_H

#include <QGraphicsItem>

class HGraphicsSelectRectItem : public QGraphicsItem
{
public:
    enum MType{ Type = UserType + 13 };
    explicit HGraphicsSelectRectItem(QGraphicsItem *parent = nullptr);
    ~HGraphicsSelectRectItem() override;

    void setRect(const QRectF &r);

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
    QColor color=Qt::blue;
    QRectF rect=QRectF(0,0,0,0);
};

#endif // HGRAPHICSSELECTRECTITEM_H
