#ifndef HGRAPHICSMOUSE_H
#define HGRAPHICSMOUSE_H




class HGraphicsMouse : public QGraphicsItem
{
public:
    enum MType{ Type = UserType + 12 };
    explicit HGraphicsMouse(QGraphicsItem *parent = nullptr);
    ~HGraphicsMouse() override;

    double getMouseSize();

     QPainterPath getContainSahpe() const;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override;

    //QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;



private:
    QPainterPath m_path;
    QPainterPath m_pathScale;
    QPainterPath m_pathScale10;
    QPainterPath m_pathContain;
    double m_msize=10.0;
    double scale=1.0;
    QColor color=Qt::black;
};

#endif // HGRAPHICSMOUSE_H
