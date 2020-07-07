#include "h_halcon_widget.h"
#include "himgprocessmwindow.h"

#include "Halcon.h"
#include "HalconCpp.h"
#include "hmathfunctions.h"

#include <QTimer>
//#include <QLabel>
//#include <QStackedLayout>

#include <QResizeEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <QPalette>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

using namespace OpencvCpp;


H_Halcon_Widget::H_Halcon_Widget(QWidget *parent) : QWidget(parent),parentWindow((HImgProcessMWindow*)parent)
{
    imgSizeRate=1.0;


    QGridLayout *layout=new  QGridLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);

    viewPort=new QFrame(this);
    viewPort->setMouseTracking(true);
    viewPort->setCursor(Qt::CrossCursor);
    viewPort->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //this->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    layout->addWidget(viewPort);


    SetSystem("use_window_thread","true");
    SetSystem("graphic_stack_size",0);
    SetWindowType("WIN32-Window");
    OpenWindow(0, 0, 100, 100, static_cast<Hlong>(viewPort->winId()), "visible", "", &h_WindowHandle);

    SetWindowParam(h_WindowHandle,"graphics_stack", "true");
    SetWindowParam(h_WindowHandle,"flush", "true");
    SetWindowParam(h_WindowHandle,"anti_aliasing", "true");
    SetWindowParam(h_WindowHandle,"graphics_stack_max_element_num", 1000);
    SetWindowParam(h_WindowHandle,"region_quality", "good");

    SetDraw(h_WindowHandle,"margin");
    SetLineWidth(h_WindowHandle,1);
    SetColored(h_WindowHandle,12);
    //    HTuple mds;
    //    QueryMshape(h_WindowHandle,&mds);

    //    for(qint32 k=0;k<mds.Length();++k){
    //            qDebug()<<mds[k].S();
    //    }

    //arrow,default,crosshair,text I-beam,Slashed circle,Size All,Size NESW,Size S,Size NWSE,Size WE,Vertical Arrow,Hourglass

    SetMshape(h_WindowHandle,"crosshair");

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::gray);
    setAutoFillBackground(true);
    setPalette(pal);

    viewPort->installEventFilter(this);
}



void H_Halcon_Widget::setImgSize(qint32 w, qint32 h)
{
    if(imgWidth==w&&imgHeight==h){
        return;
    }
    imgWidth=w;
    imgHeight=h;
    imgSizeRate=double(w)/double(h);

    qint32 winWindth=width();
    qint32 winHeight=height();
    double m_rate=(double)winWindth/(double)winHeight;


    if(imgSizeRate>m_rate){
        //label->setFixedWidth(winWindth);
        viewPort->setMaximumHeight(qint32(winWindth/imgSizeRate));
    }
    else{
        //label->setFixedHeight(winHeight);
        viewPort->setMaximumWidth(qint32(winHeight*imgSizeRate));
    }
    try{
        SetWindowExtents(h_WindowHandle,0,0,viewPort->width(),viewPort->height());
        SetPartFloat(0,0,h,w);
        FlushBuffer(h_WindowHandle);
    }
    catch (HOperatorException)
    {
        // this may happen, if the part is much too small or too big
    }

    //    QVector<QPointF>  src;
    //    QVector<QPointF>  dst;
    //    src<<imgShowRect.topLeft()<<imgShowRect.topRight()<<imgShowRect.bottomRight()<<imgShowRect.bottomLeft();
    //    dst<<QPointF(0,0)<<QPointF(imgShowRect.width(),0)<<QPointF(imgShowRect.width(),imgShowRect.height())<<QPointF(0,imgShowRect.height());
    //    viewToPortTrans=HMathFunc::GetPerspectiveTransform(src,dst);

}

bool H_Halcon_Widget::showImage(uchar *pData, qint32 width, qint32 height)
{
    if(!pData){
        return false;
    }
    try{
        ClearWindow(h_WindowHandle);
        setImgSize(width,height);
        GenImage1(&hImage,"byte",width,height,(Hlong)pData);
        //DispObj(hImage,h_WindowHandle);


        GenCrossContourXld(&hCross,height/2.0,width/2.0,qMax(width,height),0);
        //ConcatObj(cross1,cross2,&cross);
        DispObj(hImage,h_WindowHandle);
        DispObj(hCross,h_WindowHandle);
        for(const auto&k:hShowObjs){
            DispObj(k,h_WindowHandle);
        }


        return true;

    }
    catch (HException &HDevExpDefaultException)
    {
        auto err=HString(HDevExpDefaultException.ErrorMessage())+HString("\nfunction name: ")+HString(HDevExpDefaultException.ProcName());
        QMessageBox::critical(this,tr("show image failed!"),QString(err));
        return false;
    }
}


bool H_Halcon_Widget::showRect(double x1, double y1, double x2, double y2)
{
    try{
        HObject rect;
        GenRectangle1(&rect,y1,x1,y2,x2);
        DispObj(rect,h_WindowHandle);
        hShowObjs<<rect;
        return true;
    }
    catch (HException &HDevExpDefaultException)
    {
        auto err=HString(HDevExpDefaultException.ErrorMessage())+HString("\nfunction name: ")+HString(HDevExpDefaultException.ProcName());
        QMessageBox::critical(this,tr("show rect failed!"),QString(err));
        return false;
    }
}

bool H_Halcon_Widget::showCircle(double x, double y, double radius)
{
    try{
        HObject circle;
        GenCircle(&circle,y,x,radius);
        DispObj(circle,h_WindowHandle);
        hShowObjs<<circle;
        return true;
    }
    catch (HException &HDevExpDefaultException)
    {
        auto err=HString(HDevExpDefaultException.ErrorMessage())+HString("\nfunction name: ")+HString(HDevExpDefaultException.ProcName());
        QMessageBox::critical(this,tr("show circle failed!"),QString(err));
        return false;
    }
}

bool H_Halcon_Widget::drawCircle(QPointF &center, double &radius)
{
    windowMode=1;
    try{
        HTuple row,column,r;
        DrawCircle(h_WindowHandle,&row,&column,&r);
        center.rx()=column.D();
        center.ry()=row.D();
        radius=r.D();
        windowMode=0;
        return true;
    }
    catch (HException &HDevExpDefaultException)
    {
        auto err=HString(HDevExpDefaultException.ErrorMessage())+HString("\nfunction name: ")+HString(HDevExpDefaultException.ProcName());
        QMessageBox::critical(this,tr("draw circle failed!"),QString(err));
        windowMode=0;
        return false;
    }

}

bool H_Halcon_Widget::drawRect(QPointF &topLeft, QPointF &bottomRight)
{
    windowMode=1;
    try{
        HTuple row1,column1,row2,column2;
        DrawRectangle1(h_WindowHandle,&row1,&column1,&row2,&column2);
        topLeft.rx()=column1.D();
        topLeft.ry()=row1.D();
        bottomRight.rx()=column2.D();
        bottomRight.ry()=row2.D();
        //        CreateDrawingObjectRectangle1(100,100,200,200,&row1);
        //        AttachDrawingObjectToWindow(h_WindowHandle,row1);
        windowMode=0;
        return true;
    }
    catch (HException &HDevExpDefaultException)
    {
        auto err=HString(HDevExpDefaultException.ErrorMessage())+HString("\nfunction name: ")+HString(HDevExpDefaultException.ProcName());
        QMessageBox::critical(this,tr("draw rectangle failed!"),QString(err));
        windowMode=0;
        return false;
    }
}

bool H_Halcon_Widget::drawRect1(QPointF &rowColumn, QPointF &length1_2, double &phi)
{
    windowMode=1;
    try{
        HTuple row,column,Phi,length1,length2;
        DrawRectangle2(h_WindowHandle,&row,&column,&Phi,&length1,&length2);
        rowColumn.rx()=row.D();
        rowColumn.ry()=column.D();
        length1_2.rx()=length1.D();
        length1_2.ry()=length2.D();
        phi=Phi.D();
        //        CreateDrawingObjectRectangle1(100,100,200,200,&row1);
        //        AttachDrawingObjectToWindow(h_WindowHandle,row1);
        windowMode=0;
        return true;
    }
    catch (HException &HDevExpDefaultException)
    {
        auto err=HString(HDevExpDefaultException.ErrorMessage())+HString("\nfunction name: ")+HString(HDevExpDefaultException.ProcName());
        QMessageBox::critical(this,tr("draw rectangle failed!"),QString(err));
        windowMode=0;
        return false;
    }
}


bool H_Halcon_Widget::createCircleXLDTemplate(const QString &filePath, const QStringList &paras, double cx, double cy, double radius)
{
    try{

        HObject circleXLD;
        GenCircleContourXld(&circleXLD,cy,cx,radius,0, 6.28318, "positive", 1);

        HTuple modelHandle;
        CreateShapeModelXld(circleXLD,"auto",paras[0].toDouble(), paras[1].toDouble(),"auto","auto","ignore_color_polarity",
                paras[2].toInt(),&modelHandle);

        hTemplates[filePath]=modelHandle;
        WriteShapeModel(modelHandle,filePath.toLocal8Bit().data());

        return true;
    }
    catch(HException &HDevExpDefaultException){
        auto err=HString(HDevExpDefaultException.ErrorMessage())+HString("\nfunction name: ")+HString(HDevExpDefaultException.ProcName());
        QMessageBox::critical(this,tr("create template failed!"),QString(err));
        return false;
    }

}

bool H_Halcon_Widget::createRectRegion(double x1, double y1, double x2, double y2, bool isOn)
{

    try{

        if(!isOn){
            hImageRegion=hImage;
            return true;
        }

        HObject rect;
        GenRectangle1(&rect,y1,x1,y2,x2);
        Complement(rect,&rect);
        PaintRegion(rect,hImage,&hImageRegion,255,"fill");

        return true;
    }
    catch(HException &HDevExpDefaultException){
        auto err=HString(HDevExpDefaultException.ErrorMessage())+HString("\nfunction name: ")+HString(HDevExpDefaultException.ProcName());
        QMessageBox::critical(this,tr("create region failed!"),QString(err));
        return false;
    }
}

bool H_Halcon_Widget::templateProcess(const QString &filePath, const QStringList &paras, double &cx, double &cy, double &angle, double value, bool isShow)
{
    try{
        HTuple row_result,col_result,angle_result,scaleR_result,scaleC_result,score_result;
        if(!hTemplates.contains(filePath)){
            HTuple modelHandle;
            ReadShapeModel(filePath.toLocal8Bit().data(),&modelHandle);
            hTemplates[filePath]=modelHandle;
        }


        FindShapeModel(hImageRegion,hTemplates[filePath],paras[0].toDouble(),paras[1].toDouble(),paras[2].toDouble(),paras[3].toDouble(),paras[4].toDouble(),
                "least_squares",paras[5].toDouble(),paras[6].toDouble(),&row_result,&col_result,&angle_result,&score_result);

        if(score_result.Length()){

            if(isShow){
                HObject circle;
                GenCircleContourXld(&circle,row_result,col_result,value,0, 6.28318, "positive", 1);
                DispObj(circle,h_WindowHandle);
            }

            cx=col_result.D();
            cy=row_result.D();
            angle=angle_result.D();

            return true;
        }
        else{
            return false;
        }
    }
    catch(HException &HDevExpDefaultException){
        auto err=HString(HDevExpDefaultException.ErrorMessage())+HString("\nfunction name: ")+HString(HDevExpDefaultException.ProcName());
        QMessageBox::critical(this,tr("create template failed!"),QString(err));
        return false;
    }
}

bool H_Halcon_Widget::callipersCircleProcess(const QStringList &paras, double &cx, double &cy, double &radius)
{

    double dfCx1	     = paras.value(0).toDouble();
    double dfCy1		 = paras.value(1).toDouble();
    double dfRadius   =  paras.value(2).toDouble();

    double dfLength =  paras.value(3).toDouble();
    double dfAngleStart	 =  paras.value(4).toDouble()/180*PI;
    double dfAngleEnd	 =  paras.value(5).toDouble()/180*PI;
    double sigma=paras.value(6).toDouble();
    int threshold=paras.value(7).toInt();
    int nCount = paras.value(8).toInt();
    bool translation=paras.value(9).toInt();
    bool select=paras.value(10).toInt();
    bool polarity=paras.value(11).toInt();


    try{

        HObject Rect,Cross,Circle,objShow;
        HTuple  width,height,MeasureHandle, RowEdge, ColumnEdge, Amplitude;
        HTuple  Distance;


        double dfA = 0.0;
        double Column=0.0;
        double Row=0.0;

        double dfInnerRadius=dfRadius-dfLength/2;
        double dfOuterRadius=dfRadius+dfLength/2;

        bool isCW=dfAngleStart<dfAngleEnd?true:false;

        double dfLength1=dfLength/2;
        double dfRa=fabs(dfAngleEnd-(isCW?dfAngleStart:dfAngleStart-2*PI));
        double dfLength2=sin(dfRa/nCount*0.5)*dfRadius;




        GetImageSize(hImage,&width,&height);

        GenEmptyObj(&objShow);
        GenCircleContourXld(&Circle,dfCy1,dfCx1,dfInnerRadius,isCW?dfAngleStart:dfAngleEnd,isCW?dfAngleEnd:dfAngleStart,isCW?"positive":"negative", 1);
        ConcatObj(Circle,objShow,&objShow);
        GenCircleContourXld(&Circle,dfCy1,dfCx1,dfOuterRadius,isCW?dfAngleStart:dfAngleEnd,isCW?dfAngleEnd:dfAngleStart,isCW?"positive":"negative", 1);
        ConcatObj(Circle,objShow,&objShow);

        int i = 0,j=0;
        HTuple ptRows,ptCols;
        for( i = 0, j = 0; i < nCount; i++)
        {

            dfA=(isCW?dfAngleStart:dfAngleEnd)+(isCW?(i*dfRa/nCount):(-i*dfRa/nCount));
            Row=-dfRadius*sin(dfA)+dfCy1;
            Column=dfRadius*cos(dfA)+dfCx1;
            dfA+=(!translation)*PI;


            GenRectangle2(&Rect,Row,Column,dfA,dfLength1,dfLength2);
            //set_color(hWnd,"yellow");


            GenMeasureRectangle2(Row, Column, dfA, dfLength1, dfLength2, width, height, "nearest_neighbor",
                                 &MeasureHandle);
            MeasurePos(hImage, MeasureHandle, sigma, threshold,
                       polarity?"positive":"negative", select?"first":"last", &RowEdge, &ColumnEdge, &Amplitude, &Distance);

            if(RowEdge.Length() == 1)
            {
                GenCrossContourXld(&Cross,RowEdge[0].D(),ColumnEdge[0].D(),dfLength2<10?10:dfLength2,dfA-PI/4);
                ConcatObj(Cross,objShow,&objShow);
                ptRows.Append(RowEdge);
                ptCols.Append(ColumnEdge);
                j++;
            }
            CloseMeasure(MeasureHandle);

        }

        if(ptRows.Length()<3){
            return false;
        }

        HObject points;
        GenContourPolygonXld(&points,ptRows,ptCols);



        HTuple Row1,Column1,Phi, Radius1, Radius2, StartPhi, EndPhi, PointOrder;
        FitEllipseContourXld(points, "fitzgibbon", -1, 2, 0, 200, 3, 2.0, &Row1, &Column1, &Phi, &Radius1, &Radius2, &StartPhi, &EndPhi, &PointOrder);

        if(Row1.Length()==1){

            GenEllipseContourXld(&Circle, Row1, Column1, Phi,Radius1,Radius2,StartPhi,EndPhi,PointOrder,1);
            GenCrossContourXld (&Cross, Row1,Column1, 120,HTuple(0));

            DispObj(objShow,h_WindowHandle);
            DispObj(Circle,h_WindowHandle);
            DispObj(Cross,h_WindowHandle);

            cx=Column1.D()-width.D()*0.5;
            cy=-Row1.D()+height.D()*0.5;
            radius=(Radius1.D()+Radius2.D())*0.5;

            DispText(h_WindowHandle,QString("X=%1").arg(cx,0,'f',3).toLocal8Bit().data(),"window",10,5,"red",HTuple(),HTuple());
            DispText(h_WindowHandle,QString("Y=%1").arg(cy,0,'f',3).toLocal8Bit().data(),"window",30,5,"red",HTuple(),HTuple());
            DispText(h_WindowHandle,QString("R=%1").arg(radius,0,'f',3).toLocal8Bit().data(),"window",50,5,"red",HTuple(),HTuple());

            return true;

        }else{
            return false;
        }
    }
    catch(HException &HDevExpDefaultException){
        auto err=HString(HDevExpDefaultException.ErrorMessage())+HString("\nfunction name: ")+HString(HDevExpDefaultException.ProcName());
        QMessageBox::critical(this,tr("callipers circle failed!"),QString(err));
        return false;
    }
}

bool H_Halcon_Widget::callipersLineProcess(const QStringList &paras, QLineF &line)
{
    try
    {
        auto para_row=paras.value(0).toDouble();
        auto para_col=paras.value(1).toDouble();
        auto para_length1=paras.value(2).toDouble();
        auto para_length2=paras.value(3).toDouble();
        auto para_phi=paras.value(4).toDouble();
        auto para_sigma=paras.value(5).toDouble();
        auto para_thresold=paras.value(6).toDouble();
        auto para_num=paras.value(7).toDouble();
        auto para_select=paras.value(9).toInt();
        auto para_polarity=paras.value(10).toInt();

        double dfX,dfY,Row2,Column2,Row1,Column1,Row3,Column3;

        QPointF pt1,pt2;


        Row1=para_row*cos(-para_phi)-para_col*sin(-para_phi);
        Column1=para_col*cos(-para_phi)+para_row*sin(-para_phi);

        Row2=Row1-para_length2+para_length2/para_num;
        Column2=Column1;

        pt1.rx()=Column2*cos(para_phi)+(Row1-para_length2)*sin(para_phi);
        pt1.ry()=(Row1-para_length2)*cos(para_phi)-Column2*sin(para_phi);

        pt2.rx()=Column2*cos(para_phi)+(Row1+para_length2)*sin(para_phi);
        pt2.ry()=(Row1+para_length2)*cos(para_phi)-Column2*sin(para_phi);


        HTuple Width ,Height,MeasureHandle,RowEdge,ColumnEdge,Amplitude,Distance;

        GetImageSize(hImage,&Width,&Height);

        HObject Cross,Rect;

        QVector<QPointF> pts;

        for (int i = 0; i < para_num; i++)
        {
            Row3=Row2+2*i*para_length2/para_num;

            Column3=Column2*cos(para_phi)+Row3*sin(para_phi);
            Row3=Row3*cos(para_phi)-Column2*sin(para_phi);

            GenRectangle2(&Rect,Row3,Column3,para_phi,para_length1,para_length2/para_num);

            DispObj(Rect,h_WindowHandle);
            //pMainDlg->pic.SendMessage(FromThreadMessage, CPictrure::myMsg::msg_MvsImageShowObj, (LPARAM)(&Rect));



            GenMeasureRectangle2 (Row3, Column3, para_phi, para_length1,para_length2/para_num, Width, Height, "nearest_neighbor", &MeasureHandle);
            MeasurePos (hImage, MeasureHandle, para_sigma, para_thresold, para_polarity==true?"positive":"negative",
                        para_select==true?"first":"last", &RowEdge, &ColumnEdge, &Amplitude, &Distance);

            if (RowEdge.Length()!=0)
            {
                dfX = ColumnEdge[0].D();
                dfY = RowEdge[0].D();
                GenCrossContourXld (&Cross, RowEdge[0].D(), ColumnEdge[0].D(), 30,45);
                DispObj(Cross,h_WindowHandle);

                pts<<QPointF(dfX,dfY);
            }


            CloseMeasure(MeasureHandle);



        }

        double angle,A,B,C;

        if(HMathFunc::LineFitting(pts,angle,A,B,C)){
            line.setLine((-(-(pt1.y()-tan(para_phi)*pt1.x())*B-C)/(A+B*tan(para_phi))*A-C)/B,(-(pt1.y()-tan(para_phi)*pt1.x())*B-C)/(A+B*tan(para_phi)),
                         (-(-(pt2.y()-tan(para_phi)*pt2.x())*B-C)/(A+B*tan(para_phi))*A-C)/B,(-(pt2.y()-tan(para_phi)*pt2.x())*B-C)/(A+B*tan(para_phi)));
            DispLine(h_WindowHandle,line.p1().x(),line.p1().y(),line.p2().x(),line.p2().y());
            line=QLineF(line.p1().y()- Height[0].D()/2,line.p1().x()-Width[0].D()/2,line.p2().y()- Height[0].D()/2,line.p2().x()-Width[0].D()/2);
            line.setP1(QPointF(line.p1().x()-Width.D()*0.5,-line.p1().y()+Height.D()*0.5));
            line.setP2(QPointF(line.p2().x()-Width.D()*0.5,-line.p2().y()+Height.D()*0.5));
            return true;

        }



    }
    catch(HException &HDevExpDefaultException){
        auto err=HString(HDevExpDefaultException.ErrorMessage())+HString("\nfunction name: ")+HString(HDevExpDefaultException.ProcName());
        QMessageBox::critical(this,tr("callipers circle failed!"),QString(err));
        return false;
    }
    return false;

}

bool H_Halcon_Widget::noneReProcess(double &cx, double &cy, double &radius)
{
    try{
        HTuple  width,height;
        HObject Cross,circle;
        GetImageSize(hImage,&width,&height);
        GenCrossContourXld (&Cross, cy,cx, 120,HTuple(0));
        GenCircleContourXld(&circle,cy,cx,radius,0, 6.28318, "positive", 1);
        DispObj(Cross,h_WindowHandle);
        DispObj(circle,h_WindowHandle);
        cx=cx-width.D()*0.5;
        cy=cy-height.D()*0.5;
        return true;
    }
    catch(HException &HDevExpDefaultException){
        auto err=HString(HDevExpDefaultException.ErrorMessage())+HString("\nfunction name: ")+HString(HDevExpDefaultException.ProcName());
        QMessageBox::critical(this,tr("callipers circle failed!"),QString(err));
        return false;
    }
}



void H_Halcon_Widget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    if(event->size()!=QSize(0,0)){
        qint32 winWindth=width();
        qint32 winHeight=height();
        double m_rate=(double)winWindth/(double)winHeight;


        if(imgSizeRate>m_rate){
            //label->setFixedWidth(winWindth);
            viewPort->setMaximumHeight(qint32(winWindth/imgSizeRate));
        }
        else{
            //label->setFixedHeight(winHeight);
            viewPort->setMaximumWidth(qint32(winHeight*imgSizeRate));
        }


        try{
            SetWindowExtents(h_WindowHandle,0,0,viewPort->width(),viewPort->height());
            FlushBuffer(h_WindowHandle);
        }
        catch (HOperatorException)
        {
            // this may happen, if the part is much too small or too big
        }
    }
}


void H_Halcon_Widget::GetPartFloat(double *row1, double *col1, double *row2, double *col2)
{
    // to get float values from get_part, use HTuple parameters
    HTuple trow1, tcol1, trow2, tcol2;
    GetPart(h_WindowHandle,&trow1, &tcol1, &trow2, &tcol2);
    *row1 = trow1.D();
    *col1 = tcol1.D();
    *row2 = trow2.D();
    *col2 = tcol2.D();
}

void H_Halcon_Widget::SetPartFloat(double row1, double col1, double row2, double col2)
{
    // convert the double values to HTuple. Otherwise the int variant of SetPart is used
    // this enables smooth movement and zooming even when zoomed in
    SetPart(h_WindowHandle,HTuple(row1), HTuple(col1),
            HTuple(row2), HTuple(col2));
}

bool H_Halcon_Widget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == viewPort)
    {
        switch (event->type()) {
        case QEvent::Wheel:{
            QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);

            QPointF mpos=wheelEvent->pos();

            //    mpos.rx()-=imgShowRect.left();
            //    mpos.ry()-=imgShowRect.top();

            int num_notch = std::abs(wheelEvent->delta()) / 120;
            //   mpos=mpos-imgDisplay.topLeft();

            double factor = (wheelEvent->delta() < 0) ? std::sqrt(2.0) : 1.0 / std::sqrt(2.0);
            while (num_notch > 1)
            {
                factor = factor * ((wheelEvent->delta() < 0) ? std::sqrt(2.0) : 1.0 / std::sqrt(2.0));
                num_notch--;
            }

            try
            {
                // get zooming center
                HTuple centerRow, centerCol;
                ConvertCoordinatesWindowToImage(h_WindowHandle,mpos.y(), mpos.x(), &centerRow, &centerCol);
                // get current image part
                double row1, col1, row2, col2;
                GetPartFloat(&row1, &col1, &row2, &col2);
                // zoom around center
                double left = centerRow - row1;
                double right = row2 - centerRow;
                double top = centerCol - col1;
                double buttom = col2 - centerCol;
                double newRow1 = centerRow - left * factor;
                double newRow2 = centerRow + right * factor;
                double newCol1 = centerCol - top * factor;
                double newCol2 = centerCol + buttom * factor;

                SetPartFloat(newRow1, newCol1, newRow2, newCol2);
                FlushBuffer(h_WindowHandle);
            }
            catch (HOperatorException)
            {
                // this may happen, if the part is much too small or too big
            }

        }
            break;
        case QEvent::MouseButtonDblClick:{
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->buttons() == Qt::LeftButton)
            {
                // reset image part
                try
                {
                    SetPart(h_WindowHandle,0, 0, -1, -1);
                    FlushBuffer(h_WindowHandle);
                }
                catch (HOperatorException)
                {
                    // this may happen, if the part is much too small or too big
                }
            }

        }
            break;
        case QEvent::MouseButtonPress:{
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            try
            {
                GetPartFloat(&lastRow1, &lastCol1, &lastRow2, &lastCol2);

                HTuple x,y,button,grey;
                GetMposition(h_WindowHandle,&x,&y,&button);
                GetGrayval(hImage,x,y,&grey);

                parentWindow->showStatusMessage(QString("X: %1    Y: %2    Grey: %3").arg(y.I()).arg(x.I()).arg(grey.I()));

            }
            catch (HOperatorException)
            {
                // this may happen, if the part is much too small or too big
            }
            lastMousePos = mouseEvent->pos();

        }
            break;
        case QEvent::MouseMove:{
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);


            if ((mouseEvent->buttons() == Qt::LeftButton) && lastMousePos.x() != -1)
            {

                QPoint delta = lastMousePos - mouseEvent->pos();

                // scale delta to image zooming factor
                double scalex = (lastCol2 - lastCol1 + 1) / (double)width();
                double scaley = (lastRow2 - lastRow1 + 1) / (double)height();

                try
                {
                    // set new visible part
                    if(!windowMode){
                        SetPartFloat(lastRow1 + (delta.y() * scaley),
                                     lastCol1 + (delta.x() * scalex),
                                     lastRow2 + (delta.y() * scaley),
                                     lastCol2 + (delta.x() * scalex));
                    }
                    // initiate redraw ()

                    FlushBuffer(h_WindowHandle);
                }
                catch (HOperatorException)
                {
                    // this may happen, if the part image is moved outside the window
                }
            }


        }
            break;
        case QEvent::MouseButtonRelease:{
            lastMousePos = QPoint(-1, -1);
        }
            break;
        default:
            return false;
        }
        return true;

    }else{
        return H_Halcon_Widget::eventFilter(obj, event);
    }
}

bool H_Halcon_Widget::clearWindow()
{
    try{
        hShowObjs.clear();
        ClearWindow(h_WindowHandle);
        DispObj(hImage,h_WindowHandle);
        DispObj(hCross,h_WindowHandle);
        return true;
    }
    catch (HException &HDevExpDefaultException)
    {
        auto err=HString(HDevExpDefaultException.ErrorMessage())+HString("\nfunction name: ")+HString(HDevExpDefaultException.ProcName());
        //qDebug()<<err;
        return false;
    }
}

bool H_Halcon_Widget::resetWindow()
{
    try{
        SetPart(h_WindowHandle,0, 0, -1, -1);
        FlushBuffer(h_WindowHandle);
        return true;
    }
    catch (HException &HDevExpDefaultException)
    {
        auto err=HString(HDevExpDefaultException.ErrorMessage())+HString("\nfunction name: ")+HString(HDevExpDefaultException.ProcName());
        //qDebug()<<err;
        return false;
    }

}

bool H_Halcon_Widget::showImageFromFile()
{
    QString file=QFileDialog::getOpenFileName(this,tr("picture file choose"),nullptr,tr("image file(*.png *.jpg *jpeg *.bmp *.tiff *.gif)"));
    if(file.isEmpty())
        return false;

    try{
        ReadImage(&hImage,file.toLocal8Bit().data());

        HTuple width,height;
        GetImageSize(hImage,&width,&height);

        ClearWindow(h_WindowHandle);
        setImgSize(width,height);
        //DispObj(hImage,h_WindowHandle);


        GenCrossContourXld(&hCross,height/2.0,width/2.0,qMax(width,height),0);
        //ConcatObj(cross1,cross2,&cross);
        DispObj(hImage,h_WindowHandle);
        DispObj(hCross,h_WindowHandle);
        for(const auto&k:hShowObjs){
            DispObj(k,h_WindowHandle);
        }
        return true;
    }
    catch (HException &HDevExpDefaultException)
    {
        auto err=HString(HDevExpDefaultException.ErrorMessage())+HString("\nfunction name: ")+HString(HDevExpDefaultException.ProcName());
        // qDebug()<<ErrorMsg.S();
        return false;
    }
}


//void H_Halcon_Widget::paintEvent(QPaintEvent *event)
//{
//    QPainter painter(this);
//    const QRect rect = event->rect();

//    painter.setPen(QPen(QColor(255,0,0,150),1,Qt::SolidLine));
//    painter.drawLine(QPoint(rect.left(),rect.center().y()),QPoint(rect.right(),rect.center().y()));
//    painter.drawLine(QPoint(rect.center().x(),rect.top()),QPoint(rect.center().x(),rect.bottom()));
//}



