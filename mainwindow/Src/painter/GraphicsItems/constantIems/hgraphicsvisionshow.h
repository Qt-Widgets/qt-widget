#ifndef HGRAPHICSVISIONSHOW_H
#define HGRAPHICSVISIONSHOW_H

#include <QGraphicsItem>
class HSystemAdjustController;

class HGraphicsVisionShow : public QGraphicsItem
{
public:
    enum MType{ Type = UserType + 14 };
    explicit HGraphicsVisionShow(QGraphicsItem *parent = nullptr);
    ~HGraphicsVisionShow() override;

    QPointF getPos();
    void setMPos(const QPointF &pos);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPainterPath m_path;
    QColor color=Qt::darkCyan;
    QPointF m_pos;
    HSystemAdjustController *ptrTransController=nullptr;
};

#endif // HGRAPHICSVISIONSHOW_H
