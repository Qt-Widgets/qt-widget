#ifndef H_CAMERASHOW_H
#define H_CAMERASHOW_H

#include "hopenglwidget.h"

#include <QImage>
#include <QTransform>


class H_CameraShow : public QFrame
{
    Q_OBJECT

public:
    H_CameraShow(QWidget *parent = nullptr);
    ~H_CameraShow() Q_DECL_OVERRIDE;

    void ShowGreyImg(uchar *pImg);

    void ShowColorImg(uchar *pRed,uchar *pGreen,uchar *pBlue);

    void ShowImage(const QImage &img);

    void SaveImage(const QString &filePath);

    void setImgInfo(qint32 width,qint32 height);

    void ShowCross(bool isShow);

    void ShowDrawCircle(bool isShow);

    void ShowImgNormal();

    uchar *GetImagePtr(qint32 &width,qint32 &height);

    void AddShowCross(const QPointF&pos);
    void AddShowEllipse(const QPointF& center,double r1,double r2);

    void AddShowText(const QString &msg,const QColor& color,const QPointF&pos);

    void ClearShowPath();


signals:

    void MousePosInfo(const QPointF &pos, const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *wheelEvent) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
     void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:

     void resizeWindowToImage();



    qint32 imgWidth;
    qint32 imgHeight;
    qreal ratio_wh;

    QRect displayRect;
    QRect imageShowRect;

    QTransform tragetSourceTrans;

    bool showCross=false;
    bool showDrawCircle=false;


    QVector<QPointF> m_circleDrawPts;
    QPointF m_circleDrawCenter;
    double m_circleDrawRadius=0;
    void getCircleCenter();

    mutable QTransform inory;
    mutable QTransform inory1;
    mutable QPointF offset=QPoint(0,0);
    inline void Zoom(const QPointF& mpos,double scale);

    mutable bool pressed=false;
    mutable QPointF mouseRealPos;
    mutable QPointF pressPos;
    mutable QPointF pressMove;

    double m_scale=1.0;

    QImage m_image;

    QImage m_Greyimage;

    struct MsgShow
    {
        QPointF pos=QPointF(10,40);
        QString msg;
        QColor color=Qt::darkGreen;
        QFont font=MyFont_Image1;
    };
    QVector<MsgShow> m_showMsgs;

    QVector<QPainterPath> m_showPaths;


signals:

public slots:
};

#endif // H_CAMERASHOW_H
