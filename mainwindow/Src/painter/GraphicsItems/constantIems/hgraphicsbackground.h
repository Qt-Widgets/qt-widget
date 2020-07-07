#ifndef HGRAPHICSBACKGROUND_H
#define HGRAPHICSBACKGROUND_H

#include <QGraphicsItem>

class HGraphicsBackground: public QGraphicsItem
{

public:
    enum MType{ Type = UserType + 10 };
    explicit HGraphicsBackground(const QRectF&content,QGraphicsItem *parent = nullptr);
    ~HGraphicsBackground() override;


    void setContentRect(const QRectF&r);

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
    QPointF pos;
    QRectF rect=QRectF(0,0,0,0);
};

#endif // HGRAPHICSBACKGROUND_H
