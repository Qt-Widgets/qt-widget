#ifndef HGRAPHICSDRAWCACHEITEM_H
#define HGRAPHICSDRAWCACHEITEM_H


#include <QGraphicsItem>

class HGraphicsDrawCacheItem : public QGraphicsItem
{
public:
    enum MType{ Type = UserType + 11 };
    explicit HGraphicsDrawCacheItem(QGraphicsItem *parent = nullptr);
    ~HGraphicsDrawCacheItem() override;

   void setPainterPath(QPainterPath &path);
   void setColor(const QColor& c);
   void resetData();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QColor color=Qt::blue;
    QPainterPath m_path;
};

#endif // HGRAPHICSDRAWCACHEITEM_H
