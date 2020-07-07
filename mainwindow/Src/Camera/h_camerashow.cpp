#include "h_camerashow.h"

#include "hmathfunctions.h"
#include "hmath.h"



H_CameraShow::H_CameraShow(QWidget *parent) : QFrame(parent)
{
    imgWidth=0;
    imgHeight=0;
    ratio_wh=1;

    setMouseTracking(true);

    this->setCursor(Qt::CrossCursor);
}

H_CameraShow::~H_CameraShow()
{

}


void H_CameraShow::ShowGreyImg(uchar *pImg)
{
    m_showPaths.clear();
    m_showMsgs.clear();
    uchar * greyData=m_Greyimage.bits();
    for(qint32 h=0;h<imgHeight;++h){
        for(qint32 w=0;w<imgWidth;++w){
            m_image.setPixelColor(w,h,qRgba(*pImg,*pImg,*pImg,0));
            *greyData=*pImg;
            greyData++;
            pImg++;
        }
    }
    update();
}

void H_CameraShow::ShowColorImg(uchar *pRed, uchar *pGreen, uchar *pBlue)
{
    m_showPaths.clear();
    m_showMsgs.clear();
    for(qint32 h=0;h<imgHeight;++h){
        for(qint32 w=0;w<imgWidth;++w){
            m_image.setPixelColor(w,h,qRgba(*pRed,*pGreen,*pBlue,255));
            pRed++;
            pGreen++;
            pBlue++;
        }
    }
    update();
}

void H_CameraShow::ShowImage(const QImage &img)
{
    m_showPaths.clear();
    m_showMsgs.clear();
    imgWidth=img.width();
    imgHeight=img.height();
    ratio_wh=static_cast<double>(imgWidth)/static_cast<double>(imgHeight);
    m_image=img;
    m_Greyimage=img.convertToFormat(QImage::Format_Indexed8);


    imageShowRect=QRect(0,0,imgWidth,imgHeight);

    resizeWindowToImage();
}


void H_CameraShow::SaveImage(const QString &filePath)
{
    m_Greyimage.save(filePath);
}

void H_CameraShow::setImgInfo(qint32 width, qint32 height)
{
    m_showPaths.clear();
    m_showMsgs.clear();
    imgWidth=width;
    imgHeight=height;
    ratio_wh=static_cast<double>(imgWidth)/static_cast<double>(imgHeight);
    m_image=QImage(width,height,QImage::Format_ARGB32);
    m_Greyimage=QImage(width,height,QImage::Format_Indexed8);

    imageShowRect=QRect(0,0,imgWidth,imgHeight);
     resizeWindowToImage();
}

void H_CameraShow::ShowCross(bool isShow)
{
    showCross=isShow;
    update();
}

void H_CameraShow::ShowDrawCircle(bool isShow)
{
    showDrawCircle=isShow;
    update();
}

void H_CameraShow::ShowImgNormal()
{
    inory.reset();
    inory1.reset();
    offset=QPoint(0,0);
    update();
}

uchar *H_CameraShow::GetImagePtr(qint32 &width, qint32 &height)
{
    width=m_Greyimage.width();
    height=m_Greyimage.height();
    return m_Greyimage.bits();
}

void H_CameraShow::AddShowCross(const QPointF &pos)
{
    QPainterPath path;
    path.moveTo(pos.x()-100+imgWidth*0.5,pos.y()+imgHeight*0.5);
    path.lineTo(pos.x()+100+imgWidth*0.5,pos.y()+imgHeight*0.5);
    path.moveTo(pos.x()+imgWidth*0.5,pos.y()-100+imgHeight*0.5);
    path.lineTo(pos.x()+imgWidth*0.5,pos.y()+100+imgHeight*0.5);
    m_showPaths.append(path);
    update();
}

void H_CameraShow::AddShowEllipse(const QPointF &center, double r1, double r2)
{
    QRectF rect=QRectF(center.x()-r1,center.y()-r2,r1*2,r2*2);
    QPainterPath path;
    path.addEllipse(rect);

    QPointF ccpt=rect.center();
    path.moveTo(ccpt.x()-100+imgWidth*0.5,ccpt.y()+imgHeight*0.5);
    path.lineTo(ccpt.x()+100+imgWidth*0.5,ccpt.y()+imgHeight*0.5);
    path.moveTo(ccpt.x()+imgWidth*0.5,ccpt.y()-100+imgHeight*0.5);
    path.lineTo(ccpt.x()+imgWidth*0.5,ccpt.y()+100+imgHeight*0.5);
    m_showPaths.append(path);
    update();
}

void H_CameraShow::AddShowText(const QString &msg, const QColor &color, const QPointF &pos)
{
    MsgShow mText;
    mText.pos=pos;
    mText.msg=msg;
    mText.color=color;
    m_showMsgs.append(mText);
    update();
}

void H_CameraShow::ClearShowPath()
{
    m_showMsgs.clear();
    m_showPaths.clear();
    update();
}

void H_CameraShow::paintEvent(QPaintEvent *event)
{
       QPainter painter(this);
       //painter.setRenderHint(QPainter::HighQualityAntialiasing);

       painter.fillRect(event->rect(),QColor(206,187,179,120));

       QPen pennn;
       pennn.setCosmetic(true);
       painter.setPen(pennn);

       int w=width();
       int h=height();

       painter.setWindow(displayRect);
       painter.setViewport(qint32((w-displayRect.width())/2+offset.x()+pressMove.x()+0.5),qint32((h-displayRect.height())/2+offset.y()+pressMove.y()),
                            displayRect.width(),displayRect.height());

       painter.setTransform(inory);
       inory=painter.transform();
       inory1=painter.combinedTransform();
       painter.drawPixmap(displayRect,QPixmap::fromImage(m_image),imageShowRect);



       pennn.setColor(QColor(255,69,0));
       painter.setPen(pennn);
       for(const auto&k:m_showPaths){
           painter.drawPath(tragetSourceTrans.inverted().map(k));
       }

       QLinearGradient myGradient;
       painter.setBrush(myGradient);
       for(const auto&k:m_showMsgs){
           pennn.setColor(k.color);
           painter.setPen(pennn);
           painter.setFont(k.font);
           painter.drawText(k.pos,k.msg);
       }

       painter.setBrush(QBrush());



       if(showCross){

           pennn.setColor(Qt::red);
           painter.setPen(pennn);

           QPointF center=displayRect.center();
           painter.drawLine(QLineF(displayRect.left(),center.y(),displayRect.right()+1,center.y()));
           painter.drawLine(QLineF(center.x(),displayRect.top(),center.x(),displayRect.bottom()+1));
       }

       if(showDrawCircle){


           if(m_circleDrawPts.size()==3){
               pennn.setColor(Qt::green);
               painter.setPen(pennn);
               painter.drawEllipse(m_circleDrawCenter,m_circleDrawRadius,m_circleDrawRadius);

               pennn.setColor(Qt::cyan);
               painter.setPen(pennn);

               double lg=m_circleDrawRadius/5;
               painter.drawLine(QLineF(m_circleDrawCenter.x()-lg,m_circleDrawCenter.y(),m_circleDrawCenter.x()+lg,m_circleDrawCenter.y()));
               painter.drawLine(QLineF(m_circleDrawCenter.x(),m_circleDrawCenter.y()-lg,m_circleDrawCenter.x(),m_circleDrawCenter.y()+lg));

           }

           pennn.setColor(Qt::blue);
           pennn.setWidth(6);
           painter.setPen(pennn);
           for(const auto&k:m_circleDrawPts){
               painter.drawPoint(k);
           }
       }

       QPoint pppsss=QPoint(qint32(mouseRealPos.x()),qint32(mouseRealPos.y()));
       if(underMouse()&&imageShowRect.contains(pppsss)){
           emit MousePosInfo(mouseRealPos,m_image.pixelColor(pppsss).toRgb());
       }
}



void H_CameraShow::mousePressEvent(QMouseEvent *e)
{
    this->setCursor(Qt::ClosedHandCursor);
    pressed=true;
    pressPos=e->pos();
    if(showDrawCircle){
        if(m_circleDrawPts.size()<3){
            m_circleDrawPts<<inory1.inverted().map(pressPos);
            if(m_circleDrawPts.size()==3){
                getCircleCenter();
            }
        }else{
            m_circleDrawPts.clear();
        }
        update();
    }
}

void H_CameraShow::wheelEvent(QWheelEvent *wheelEvent)
{
    Q_UNUSED(wheelEvent)
    qreal ss=inory.m11();
    if(wheelEvent->delta()>0){
        if(ss<1000000){
            Zoom(wheelEvent->pos(),1.2);
        }
    }else{
        if(ss>0.0000001){
            Zoom(wheelEvent->pos(),1.0/1.2);
        }
    }
    update();

}

void H_CameraShow::mouseMoveEvent(QMouseEvent *e)
{
    QPointF mousePosF=e->pos();
    if(pressed){

        pressMove=mousePosF-pressPos;
        update();

    }

    mouseRealPos=tragetSourceTrans.map(inory1.inverted().map(mousePosF));
    QPoint mpos=QPoint(qint32(mouseRealPos.x()),qint32(mouseRealPos.y()));
    if(imageShowRect.contains(mpos)){
        emit MousePosInfo(mouseRealPos,m_image.pixelColor(mpos).toRgb());
    }


}

void H_CameraShow::mouseReleaseEvent(QMouseEvent *e)
{
    if(pressed){

        offset+=pressMove;
        pressMove=QPoint(0,0);

        pressed=false;
        this->setCursor(Qt::CrossCursor);
    }
}

void H_CameraShow::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void H_CameraShow::resizeEvent(QResizeEvent *event)
{
   resizeWindowToImage();
}

void H_CameraShow::resizeWindowToImage()
{
    QRect rect=this->rect();
    if(ratio_wh*rect.height()>rect.width()){
        displayRect.setWidth(rect.width());
        displayRect.setHeight(qint32(rect.width()/ratio_wh+0.5));
    }
    else{
        displayRect.setWidth(qint32(rect.height()*ratio_wh+0.5));
        displayRect.setHeight(rect.height());
    }
    QPoint pt=rect.center();
    displayRect.moveCenter(pt);

    tragetSourceTrans=HMathFunc::GetPerspectiveTransform(displayRect,QRectF(0,0,imgWidth,imgHeight));

    update();
}


void H_CameraShow::getCircleCenter()
{
    double a, b, c, d, e, f;

    a = 2*(m_circleDrawPts.at(1).x()-m_circleDrawPts.at(0).x());
    b = 2*(m_circleDrawPts.at(1).y()-m_circleDrawPts.at(0).y());
    c = m_circleDrawPts.at(1).x()*m_circleDrawPts.at(1).x()+m_circleDrawPts.at(1).y()*m_circleDrawPts.at(1).y()-m_circleDrawPts.at(0).x()*m_circleDrawPts.at(0).x()-m_circleDrawPts.at(0).y()*m_circleDrawPts.at(0).y();
    d = 2*(m_circleDrawPts.at(2).x()-m_circleDrawPts.at(1).x());
    e = 2*(m_circleDrawPts.at(2).y()-m_circleDrawPts.at(1).y());
    f = m_circleDrawPts.at(2).x()*m_circleDrawPts.at(2).x()+m_circleDrawPts.at(2).y()*m_circleDrawPts.at(2).y()-m_circleDrawPts.at(1).x()*m_circleDrawPts.at(1).x()-m_circleDrawPts.at(1).y()*m_circleDrawPts.at(1).y();
    m_circleDrawCenter.setX( (b*f-e*c)/(b*d-e*a));
    m_circleDrawCenter.setY((d*c-a*f)/(b*d-e*a));
    m_circleDrawRadius = sqrt((m_circleDrawCenter.x()-m_circleDrawPts.at(0).x())*(m_circleDrawCenter.x()-m_circleDrawPts.at(0).x())+(m_circleDrawCenter.y()-m_circleDrawPts.at(0).y())*(m_circleDrawCenter.y()-m_circleDrawPts.at(0).y()));//radius
}

void H_CameraShow::Zoom(const QPointF &mpos, double scale)
{
    QPointF tt=inory.map(inory1.inverted().map(mpos));
    inory*=QTransform::fromTranslate(-tt.x(),-tt.y())*QTransform::fromScale(scale,scale)*QTransform::fromTranslate(tt.x(),tt.y());
    update();
}









