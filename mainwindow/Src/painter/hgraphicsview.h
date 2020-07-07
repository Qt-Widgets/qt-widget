#ifndef HGRAPHICSVIEW_H
#define HGRAPHICSVIEW_H

#include <QGraphicsView>

#include "hgraphicsscene.h"


class HGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit HGraphicsView(QWidget *parent=nullptr);
    ~HGraphicsView() override;

    void setModifyMode(bool isModify);
    void setMousePressMode(qint32 mode);

     HGraphicsScene* getScene() const{return scene;}

    friend QDataStream &operator>>(QDataStream & input, HGraphicsView &obj){
        input >>*obj.scene;
        return input;
    }
    friend QDataStream &operator<<(QDataStream & output, const HGraphicsView &obj){
        output <<*obj.scene;
        return output;
    }
    
    QPointF getCameraPos();

signals:

   void selectionChanged();
   void mouseMoved(const QPointF& scenePos);

   void mousePressed(const QPointF& scenePos);

public slots:
    void resetView();
    void setupMatrix();
    void togglePointerMode(qint32 mode);
    void toggleOpenGL(bool useOpengl);
    void toggleAntialiasing(bool useAntia);
    void print();


protected:
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *) override;
#endif
    void scrollContentsBy(int dx, int dy) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;


private:


    HGraphicsScene *scene;

    qint32 dragMode=1;


    double zoomValue=1.0;   // 0~500
    double rotateValue=0;   //-360~360
};

#endif // HGRAPHICSVIEW_H
