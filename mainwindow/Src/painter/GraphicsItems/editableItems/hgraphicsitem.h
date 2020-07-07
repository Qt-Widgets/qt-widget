#ifndef HGRAPHICSITEM_H
#define HGRAPHICSITEM_H

#include <QGraphicsItem>
#include "../constantIems/hgraphicsselectrectitem.h"

class RLine;
class HGraphicsView;

class HGraphicsItem : public QGraphicsItem
{
public:
    enum MType{ Type = UserType + 100 };

    enum DataType{
        none=-4,

        color=-1,
        para_genaral=0,
        para_laser,
        para_cam,
        para_location,

        end,
    };

    explicit HGraphicsItem(QGraphicsItem *parent = nullptr);
    ~HGraphicsItem() override;

    virtual void  transLocationPts(const QTransform & trans);

    virtual QVariantList getPropertyParas() const;

    const QGraphicsItem * toQGrapicsItem() const;

    virtual void HDataInputStream(QDataStream & input)=0;
    virtual void HDataOutputStream(QDataStream & output) const =0;

    virtual bool getA3200Gcode(QString &code,QStringList&paras,RLine&endPt, qint32 mode,QTransform trans);


    virtual QPointF getCenter() ;
    virtual double getDistanceToShape(const QPointF &pt) const;
    virtual QPainterPath getScenePath() const;


    qint32 type() const override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;


    void setHighlight(bool highlight);
    bool getIsHighlight() const;
    void setColor(const QColor &color);
    QColor getColor() const;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;


    static const QRectF& getPointRect(double scale,const QPointF& pt){
        m_PointRect.setWidth(4/scale);
        m_PointRect.setHeight(4/scale);
        m_PointRect.moveCenter(pt);
        return m_PointRect;
    }

    static QRectF m_PointRect;

    bool isHighlight=false;

    const QColor selectedColor=QColor(128,64,0);

};


#endif // HGRAPHICSITEM_H
