#include "h_cameracontrollerwidget.h"
#include "Src/Camera/h_camerashow.h"

#include "hcommon.h"
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QTimer>
#include <QStatusBar>
#include <QFrame>
#include <QLabel>
#include <QFileDialog>
#include <QtConcurrent>

#include <opencv2/opencv.hpp>

#include "Src/data/mythreadworker.h"
//#include "Src/Camera/imgProcess/h_halconimgprocess.h"
//#include "Src/Gui/img-process/h_img_process_controller.h"

#include "Src/Motion/h_motioncontroller.h"
#include "Src/data/inisettings.h"

#include "Src/Motion/hcameracontroller.h"
#include "Src/data/hconfigwidget.h"

#include "Src/data/inisettings.h"

#include "interface/hinterfacecontroller.h"


//static bool getImage(cv::Mat *pCVImage)
//{
//    return pCVImage->data=CameraSet::GetInstance()->GetOneImageBuffer();
//}

//template <class T>
//static HalconCpp::HTuple getStringType( T data){
//    if(data<0.0001){
//        return HTuple("auto");
//    }
//    else{
//        return HTuple(data);
//    }
//}



H_CameraControllerWidget* H_CameraControllerWidget::m_pInstance = nullptr;
H_CameraControllerWidget *H_CameraControllerWidget::GetInstance(QWidget *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new H_CameraControllerWidget(parent);
    }
    return m_pInstance;
}

uchar *H_CameraControllerWidget::getCurrentImage(qint32 &width, qint32 &height)
{
    return tabList.at(m_currenCameraIndex)->GetImagePtr(width,height);
}

QPointF H_CameraControllerWidget::getCurrentImageCpt()
{
    //    QStringList list= IniSettings::GetInstance()->getValue(IniSettings::test4).toString().split(',');
    //    QStringList cameraTransList=IniSettings::GetInstance()->getValue(IniSettings::test2).toString().split(',');
    //    QTransform cameraTrans=QTransform(
    //                cameraTransList.at(0).toDouble(),cameraTransList.at(1).toDouble(),cameraTransList.at(2).toDouble()
    //                ,cameraTransList.at(3).toDouble(),cameraTransList.at(4).toDouble(),cameraTransList.at(5).toDouble()
    //                ,cameraTransList.at(6).toDouble(),cameraTransList.at(7).toDouble(),cameraTransList.at(8).toDouble()
    //                );
    //    QTransform transt=QTransform(
    //                list.at(0).toDouble(),list.at(1).toDouble(),list.at(2).toDouble()
    //                ,list.at(3).toDouble(),list.at(4).toDouble(),list.at(5).toDouble()
    //                ,list.at(6).toDouble(),list.at(7).toDouble(),list.at(8).toDouble()
    //                );

    //    auto sdsdsdsd=H_MotionController::GetInstance();
    //    return transt.map(QPointF(sdsdsdsd->GetAxis(0)->GetFeedbackPos(),sdsdsdsd->GetAxis(1)->GetFeedbackPos())-cameraTrans.map(QPointF(device_info.width*0.5,device_info.height*0.5)));
    return QPointF();
}

void H_CameraControllerWidget::setImage()
{
    qint32 w,h;
    auto ptr=tabList.at(m_currenCameraIndex)->GetImagePtr(w,h);
}


qint32 H_CameraControllerWidget::showOneImg(qint32 index)
{
    if(index<0||index>=ptrTabWid->count())
        return 1;
    ptrTabWid->setCurrentIndex(index);
    return showImgOne();
}

qint32 H_CameraControllerWidget::showOneImg(const QString &name)
{
    if(ptrCameraDevice->GetCameraNames().indexOf(name)<0){
        return -1;
    }
    ptrTabWid->setCurrentIndex(ptrCameraDevice->GetCameraNames().indexOf(name));
    return showImgOne();
}

void H_CameraControllerWidget::stopContinuesCapture()
{
    stopContinuousImg();
}

void H_CameraControllerWidget::addShowText(const QString &msg, const QColor &color, const QPointF &pos)
{
    tabList.at(m_currenCameraIndex)->AddShowText(msg,color,pos);
}

void H_CameraControllerWidget::addShowCross(const QPointF &pos)
{
    tabList.at(m_currenCameraIndex)->AddShowCross(pos);
}

void H_CameraControllerWidget::addShowEllipse(const QPointF &center, double radius1, double radius2)
{
    tabList.at(m_currenCameraIndex)->AddShowEllipse(center,radius1,radius2);
}


//bool H_CameraControllerWidget::eventFilter(QObject *target, QEvent *event)
//{
//    if(target==ptrTabWid)
//    {
//        switch(event->type()) {
//        case QEvent::Resize:{
//            QRect rect=ptrTabWid->rect();
//            qreal ratio_wh=qreal(device_info.width)/qreal(device_info.height);
//            if(ratio_wh*rect.height()>rect.width()){
//                int length=(rect.height()-static_cast<int>(rect.width()/ratio_wh+0.5))/2;
//                ptrTabWid->setContentsMargins(0,length,0,length);
//            }
//            else{
//                int length=(rect.width()-static_cast<int>(rect.height()*ratio_wh+0.5))/2;
//                ptrTabWid->setContentsMargins(length,0,length,0);
//            }

//        }
//            break;
//        default:
//            break;
//        }
//    }
//    return QWidget::eventFilter(target, event);
//}

H_CameraControllerWidget::H_CameraControllerWidget(QWidget *parent) : QWidget(parent)
{

    ptrCameraDevice=HCameraController::GetInstance();



    QVBoxLayout *horizontallyLayout=new QVBoxLayout(this);
    horizontallyLayout->setContentsMargins(0,0,0,0);
    horizontallyLayout->setSpacing(1);


    horizontallyLayout->addWidget(creatToolBar());
    horizontallyLayout->addWidget(creatTabWidget());
    horizontallyLayout->addWidget(creatStatusBar());

    openCameraShow();

    connect(HConfigWidget::GetInstance(this),&HConfigWidget::DataSaveAndChanged,this,&H_CameraControllerWidget::reCreatWindow);

    connect(&m_cameraShowThread,&CameraShowThread::cameraCaptureDone,this,&H_CameraControllerWidget::showImg,Qt::BlockingQueuedConnection);

    connect(ptrTabWid,&QTabWidget::currentChanged,this,&H_CameraControllerWidget::currentCameraChange);
}

H_CameraControllerWidget::~H_CameraControllerWidget()
{
    stopContinuousImg();
    m_cameraShowThread.quit();
}

void H_CameraControllerWidget::reCreatWindow()
{
    ptrTabWid->clear();
    for(const auto &k:tabList){
        delete k;
    }
    tabList.clear();

    openCameraShow();
}


void H_CameraControllerWidget::currentCameraChange(qint32 index)
{

    m_currenCameraIndex=index;

    if(m_currenCameraIndex==-1){

        ptrToolBar->setEnabled(false);

        actionList.at(ToolButton::Connect)->setChecked(false);
        actionList.at(ToolButton::Shot_Continues)->setChecked(false);
        actionList.at(ToolButton::show_cross)->setChecked(false);

        labelList.at(StatusLabel::Transport_info)->setStyleSheet("font-family:Microsoft YaHei UI;font-size:13;color:red;");
        labelList.at(StatusLabel::Transport_info)->setText("No camera");
        labelList.at(StatusLabel::ImgSize_info)->setText(QString("%1 X %2").arg(0).arg(0));
        labelList.at(StatusLabel::fps_info)->setText(QString("fps= %1").arg(0.0,0,'f',3));
    }

    if(!getCameraInfo(m_currenCameraIndex)){
        actionList.at(ToolButton::Connect)->setChecked(true);
        actionList.at(ToolButton::Shot_Continues)->setEnabled(true);
        actionList.at(ToolButton::Shot_Once)->setEnabled(true);

        labelList.at(StatusLabel::Transport_info)->setStyleSheet("font-family:Microsoft YaHei UI;font-size:13; color:green;");
        labelList.at(StatusLabel::Transport_info)->setText("Connected");
        labelList.at(StatusLabel::ImgSize_info)->setText(QString("%1 X %2").arg(device_info.width).arg(device_info.height));
        labelList.at(StatusLabel::fps_info)->setText(QString("fps= %1").arg(device_info.fps,0,'f',3));

    }
    else{
        actionList.at(ToolButton::Connect)->setChecked(false);
        actionList.at(ToolButton::Shot_Continues)->setChecked(false);

        actionList.at(ToolButton::Shot_Continues)->setEnabled(false);
        actionList.at(ToolButton::Shot_Once)->setEnabled(false);
        labelList.at(StatusLabel::Transport_info)->setStyleSheet("font-family:Microsoft YaHei UI;font-size:13;color:red;");
        labelList.at(StatusLabel::Transport_info)->setText("Disconnected");
    }
}

void H_CameraControllerWidget::refreshStatusPosInfo(const QPointF &pos, const QColor &color)
{
    labelList.at(StatusLabel::Pixel_info)->setText(QString("X=%1  Y=%2").arg(pos.x(),0,'f',2).arg(pos.y(),0,'f',2));
    labelList.at(StatusLabel::Scale_info)->setText(QString("R:%1  G:%2  B:%3  A:%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
}

void H_CameraControllerWidget::connectCamera(bool ope)
{
    qint32 ccc=m_currenCameraIndex;
    if(ope){
        ptrCameraDevice->GetCamera(ccc)->OpenCamera();
    }
    else{
        stopContinuousImg();
        ptrCameraDevice->GetCamera(ccc)->CloseCamera();
    }
    currentCameraChange(m_currenCameraIndex);
}

void H_CameraControllerWidget::showImgContinues(bool ope)
{
    if(ope){
        if(showContinuousImg()){
            actionList.at(ToolButton::Shot_Continues)->setChecked(false);
        }
    }
    else{
        stopContinuousImg();
    }
}

void H_CameraControllerWidget::showCrossLine(bool ope)
{
    tabList.at(m_currenCameraIndex)->ShowCross(ope);
}

void H_CameraControllerWidget::showDrawCircle(bool ope)
{
    tabList.at(m_currenCameraIndex)->ShowDrawCircle(ope);
}

void H_CameraControllerWidget::showImg(uchar *ptrRed,uchar *ptrGreen,uchar *ptrBlue)
{
    if(ptrGreen==nullptr){
        tabList.at(m_currenCameraIndex)->ShowGreyImg(ptrRed);
        return;
    }
    tabList.at(m_currenCameraIndex)->ShowColorImg(ptrRed,ptrGreen,ptrBlue);
}

qint32 H_CameraControllerWidget::showImgOne()
{

    if(m_cameraShowThread.isRunning()){
        actionList.at(ToolButton::Shot_Continues)->setChecked(false);
        stopContinuousImg();
    }
    auto camera=ptrCameraDevice->GetCamera(m_currenCameraIndex);
    if(!camera){
        return 1;
    }
    uchar *ptrImgBuffer=camera->GetImgBufferOnce();
    if(ptrImgBuffer!=nullptr){
        if(getCameraInfo(m_currenCameraIndex)){
            return 2;
        }
        tabList.at(m_currenCameraIndex)->setImgInfo(device_info.width,device_info.height);

        tabList.at(m_currenCameraIndex)->ShowGreyImg(ptrImgBuffer);
        return 0;
    }
    else{
        return 1;
    }
}

void H_CameraControllerWidget::saveImg()
{
    QString file=QFileDialog::getSaveFileName(this,tr("picture file save"),nullptr,tr("bmp file(*.bmp);;"
                                                                                      "png file(*.png);;"
                                                                                      "jpg file(*.jpg);;"
                                                                                      "jpeg file(*.jpeg);;"
                                                                                      "xpm file(*.xpm);;"
                                                                                      "xbm file(*.xbm);;"
                                                                                      "ppm file(*.ppm)"));
    if(file.isEmpty())
        return;
    tabList.at(m_currenCameraIndex)->SaveImage(file);
}

void H_CameraControllerWidget::openImg()
{
    QString file=QFileDialog::getOpenFileName(this,tr("picture file choose"),nullptr,tr("image file(*.png *.jpg *jpeg *.bmp *.tiff *.gif *.xpm *.xbm *.ppm *.pbm *pgm)"));
    if(file.isEmpty())
        return;

    QImage image(file);
    tabList.at(m_currenCameraIndex)->ShowImage(image);

    labelList.at(StatusLabel::ImgSize_info)->setText(QString("%1 X %2").arg(image.width()).arg(image.height()));

}

void H_CameraControllerWidget::showWindowNormal()
{
    tabList.at(m_currenCameraIndex)->ShowImgNormal();
}

void H_CameraControllerWidget::clearWindow()
{
    tabList.at(m_currenCameraIndex)->ClearShowPath();
}

void H_CameraControllerWidget::openSettingDialog()
{

}




QWidget *H_CameraControllerWidget::creatStatusBar()
{
    ptrStatusBar=new QStatusBar(this);

    for(int k=0;k!=5;++k){
        QLabel *label =new QLabel(ptrStatusBar);
        label->setFrameStyle(QFrame::Box | QFrame::Raised);
        label->setLineWidth(2);
        ptrStatusBar->addWidget(label,k);
        labelList.append(label);
    }
    return ptrStatusBar;
}

QWidget *H_CameraControllerWidget::creatTabWidget()
{
    ptrTabWid = new QTabWidget(this);
    //ptrTabWidget->setStyleSheet(HSheetStyle::H_TABWIDGET2);

    //ptrTabWidget->installEventFilter(this);

    return ptrTabWid;
}

QWidget *H_CameraControllerWidget::creatToolBar()
{
    ptrToolBar=new QToolBar(this);
    ptrToolBar->setStyleSheet("background-color:lightgrey");

    ptrToolBar->setIconSize(QSize(25,25));
    actionList.append(ptrToolBar->addAction(QIcon(":/resource/icons/connect.svg"),tr("connect"),this,&H_CameraControllerWidget::connectCamera));
    actionList.last()->setCheckable(true);
    ptrToolBar->addSeparator();
    actionList.append(ptrToolBar->addAction(QIcon(":/resource/icons/rectangles.svg"),tr("capture continues"),this,&H_CameraControllerWidget::showImgContinues));
    actionList.last()->setCheckable(true);

    connect(actionList.last(),&QAction::toggled,this,[=](bool checked){

        ptrTabWid->tabBar()->setEnabled(!checked);

    });
    actionList.append(ptrToolBar->addAction(QIcon(":/resource/icons/cross.svg"),tr("show cross"),this,&H_CameraControllerWidget::showCrossLine));
    actionList.last()->setCheckable(true);


    actionList.append(ptrToolBar->addAction(QIcon(":/resource/icons/rectangle.svg"),tr("capture"),this,&H_CameraControllerWidget::showImgOne));
    actionList.append(ptrToolBar->addAction(QIcon(":/resource/icons/save.svg"),tr("save image"),this,&H_CameraControllerWidget::saveImg));
    actionList.append(ptrToolBar->addAction(QIcon(":/resource/icons/folder.svg"),tr("open iamge"),this,&H_CameraControllerWidget::openImg));

    ptrToolBar->addSeparator();
    actionList.append(ptrToolBar->addAction(QIcon(":/resource/icons/maximize.svg"),tr("show normal"),this,&H_CameraControllerWidget::showWindowNormal));
    actionList.append(ptrToolBar->addAction(QIcon(":/resource/icons/sweep.svg"),tr("clear"),this,&H_CameraControllerWidget::clearWindow));
    ptrToolBar->addSeparator();
    actionList.append(ptrToolBar->addAction(QIcon(":/resource/icons/settings.svg"),tr("setting"),this,&H_CameraControllerWidget::openSettingDialog));

    ptrToolBar->addSeparator();
    ptrToolBar->addSeparator();
    actionList.append(ptrToolBar->addAction(QIcon(":/resource/icons/circle.svg"),tr("show draw circle"),this,&H_CameraControllerWidget::showDrawCircle));
    actionList.last()->setCheckable(true);
    ptrToolBar->addSeparator();
    actionList.append(ptrToolBar->addAction(QIcon(":/resource/icons/paper-plane.svg"),tr("move to focus"),this,[=](){HInterfaceController::GetInstance()->imgInterface->moveToFocus();}));

    return ptrToolBar;
}







qint32 H_CameraControllerWidget::showContinuousImg()
{
    if(ptrCameraDevice->GetCamera(m_currenCameraIndex)->SetImgCaptureMode(1)){
        return 1;
    }
    if(getCameraInfo(m_currenCameraIndex)){
        return 2;
    }
    tabList.at(m_currenCameraIndex)->setImgInfo(device_info.width,device_info.height);


    m_cameraShowThread.ptrCamera=ptrCameraDevice;
    m_cameraShowThread.cameraIndex=m_currenCameraIndex;
    m_cameraShowThread.start(QThread::TimeCriticalPriority);

    actionList.at(ToolButton::Shot_Once)->setEnabled(false);


    return 0;

}

void H_CameraControllerWidget::stopContinuousImg()
{
    if(m_cameraShowThread.isFinished()){
        return;
    }
    m_cameraShowThread.requestInterruption();

    while(m_cameraShowThread.isRunning()){
        QEventLoop loop;
        QTimer::singleShot(100, &loop, SLOT(quit()));
        loop.exec();
    }



    ptrCameraDevice->GetCamera(m_currenCameraIndex)->SetImgCaptureMode(0);
    actionList.at(ToolButton::Shot_Continues)->setChecked(false);
    actionList.at(ToolButton::Shot_Once)->setEnabled(true);

}


void H_CameraControllerWidget::openCameraShow()
{

    for(const auto&k:ptrCameraDevice->GetCameraNames()){

        H_CameraShow * camShow=new H_CameraShow(ptrTabWid);
//        QSurfaceFormat format;
//        format.setDepthBufferSize(64);
//        format.setStencilBufferSize(128);
//        format.setVersion(3,2);
//        format.setSamples(16);
//        format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
//        format.setProfile(QSurfaceFormat::CoreProfile);
//        format.setSwapInterval(60);
//        camShow->setFormat(format);

        connect(camShow,&H_CameraShow::MousePosInfo,this,&H_CameraControllerWidget::refreshStatusPosInfo);
        tabList.append(camShow);


        ptrTabWid->addTab(camShow,k);

        //getCameraInfo(k);

    }
    if(!tabList.isEmpty()){
        currentCameraChange(0);
        ptrTabWid->setCurrentIndex(m_currenCameraIndex);
        ptrToolBar->setEnabled(true);

    }else{
        currentCameraChange(-1);
    }
}

qint32 H_CameraControllerWidget::getCameraInfo(qint32 index)
{
    if(index <0)
        return 1;
    auto camera=ptrCameraDevice->GetCamera(index);
    if(!camera){
        return 2;
    }

    if(camera->GetImgWidthHeight(device_info.width,device_info.height)){
        return 3;
    }
    device_info.fps=camera->GetFPS();

    if(device_info.fps<0)
        return 4;
    m_cameraShowThread.waitTime=quint32(1000/device_info.fps);

    labelList.at(StatusLabel::ImgSize_info)->setText(QString("%1 X %2").arg(device_info.width).arg(device_info.height));
    labelList.at(StatusLabel::fps_info)->setText(QString("fps= %1").arg(device_info.fps,0,'f',2));

    return 0;


}






CameraShowThread::CameraShowThread(QObject *parent)
{

}

CameraShowThread::~CameraShowThread()
{

}

void CameraShowThread::run()
{
    while (!isInterruptionRequested()){
        auto camera=ptrCamera->GetCamera(cameraIndex);
        if(!camera){
            break;
        }
        HColoredImgBuffer ptrBuffer=camera->GetColoredImgBufferContinues();
        //ptrImgBuffer=ptrCameraDevice->GetCamera(ccc)->GetImgBufferContinues(ccc);
        if(ptrBuffer.ptrBufferRed!=nullptr)
            emit cameraCaptureDone(ptrBuffer.ptrBufferRed,ptrBuffer.ptrBufferGreen,ptrBuffer.ptrBufferBlue);
        QThread::msleep(waitTime);
    }

}
