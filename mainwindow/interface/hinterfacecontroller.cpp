#include "hinterfacecontroller.h"
#include "Src/main-menu/hmainmenubar.h"

#include "Src/Motion/h_motioncontroller.h"
#include "Src/Motion/hcameracontroller.h"
#include "Src/Gui/h_cameracontrollerwidget.h"

HInterfaceController* HInterfaceController::m_pInstance = nullptr;

HInterfaceController *HInterfaceController::GetInstance(QObject *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HInterfaceController(parent);
        //atexit(Release);
    }
    return m_pInstance;
}
void HInterfaceController::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

bool HInterfaceController::Init()
{

    loadImgPlugin();
    return loadAxisAdjustPlugin();
}

bool HInterfaceController::getImage(const QString &name, uchar *&pData, qint32 &width, qint32 &height)
{
    auto cam=ptrCameraController->GetCamera(name);
    if(cam){
        ptrCameraShow->stopContinuesCapture();
        pData=cam->GetImgBufferOnce();
        if(!cam->GetImgWidthHeight(width,height)){
            return true;
        }
    }
    return false;
}

qint32 HInterfaceController::setCameraGain(const QString &name, qint32 gain)
{
    auto cam=ptrCameraController->GetCamera(name);
    if(cam){
        return cam->SetGain(gain);
    }
    return 1;
}

qint32 HInterfaceController::setCameraExposure(const QString &name, qint32 exposure)
{
    auto cam=ptrCameraController->GetCamera(name);
    if(cam){
        return cam->SetExposure(exposure);
    }
    return 1;
}

qint32 HInterfaceController::getCameraSettings(const QString &name, QStringList &data)
{
    auto cam=ptrCameraController->GetCamera(name);
    if(cam){
        qint32 gain,exposure;
        if(cam->GetGain(gain)){
            return 1;
        }
        if(cam->GetExposure(exposure)){
            return 2;
        }

        data<<QString::number(gain)<<QString::number(exposure);
        return 0;
    }
    return 1;
}

qint32 HInterfaceController::getAxisPosition(const QString &name, double &pos)
{
    auto axis=H_MotionController::GetInstance()->GetAxis(name);
    if(axis){
        pos=axis->GetFeedbackPos();
        return 0;
    }
    return 1;
}

qint32 HInterfaceController::getAxisPositionCount(const QString &name, qint32 &pos)
{
    auto axis=H_MotionController::GetInstance()->GetAxis(name);
    if(axis){
        pos=axis->GetFeedBackPosCounts();
        return 0;
    }
    return 1;
}

qint32 HInterfaceController::axisMoveToWaitStop(const QString &name, double pos)
{
    auto axis=H_MotionController::GetInstance()->GetAxis(name);
    if(axis){
        axis->MoveToWait(pos);
        return 0;
    }
    return 1;
}

qint32 HInterfaceController::axisMoveStepWaitStop(const QString &name, double pos)
{
    auto axis=H_MotionController::GetInstance()->GetAxis(name);
    if(axis){
        return axis->MoveStepWait(pos);
    }
    return 1;
}



qint32 HInterfaceController::processImage(const QString &func,QPointF &pt)
{
    if(ptrCameraShow->showOneImg(0)){
        return -1;
    }
    qint32 w,h;
    uchar *pData=ptrCameraShow->getCurrentImage(w,h);
    auto ptrImgProcess=HInterfaceController::GetInstance()->imgInterface;
    if(ptrImgProcess->showImage(pData,w,h)){
        return -1;
    }
    if(ptrImgProcess->processImage(func,pt)){
        return -1;
    }
    return 0;
}

qint32 HInterfaceController::processImageLine(const QString &func, QLineF &line)
{
    if(ptrCameraShow->showOneImg(0)){
        return -1;
    }
    qint32 w,h;
    uchar *pData=ptrCameraShow->getCurrentImage(w,h);
    auto ptrImgProcess=HInterfaceController::GetInstance()->imgInterface;
    if(ptrImgProcess->showImage(pData,w,h)){
        return -1;
    }
    if(ptrImgProcess->processImageLine(func,line)){
        return -1;
    }
    return 0;
}


HInterfaceController::HInterfaceController(QObject *parent):QObject(parent)
{
    ptrCameraShow=H_CameraControllerWidget::GetInstance();
    ptrCameraController=HCameraController::GetInstance();
}

HInterfaceController::~HInterfaceController()
{
    if(imgInterface){
        imgInterface->unInit();
    }
}

bool HInterfaceController::loadImgPlugin()
{
    QPluginLoader pluginLoader(qApp->applicationDirPath()+"/plugins/imgplugin.dll");
    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        imgInterface = qobject_cast<HImgInterface *>(plugin);
        imgInterface->Init((QWidget*)(this->parent()));

        //        if(imgInterface->getWindow()){
        //            imgInterface->getWindow()->setParent((QWidget*)(this->parent()));
        //        }


        connect(HMainMenuBar::GetInstance()->getAction(HMainMenuBar::Setting_ImgProcess),&QAction::triggered,this,[=](){
            imgInterface->resetAxisNames(H_MotionController::GetInstance()->GetAxisNames());
            imgInterface->resetCameraNames(HCameraController::GetInstance()->GetCameraNames());
            imgInterface->showWindow();
        });

        connect(dynamic_cast<QObject *>(imgInterface),SIGNAL(imgShowed(const QString&,uchar *& ,qint32 &,qint32 &))
                ,this,SLOT(getImage(const QString &, uchar *&, qint32 &, qint32 &)));

        connect(dynamic_cast<QObject *>(imgInterface),SIGNAL(cameraGainChanged(const QString&,qint32))
                ,this,SLOT(setCameraGain(const QString&,qint32)));

        connect(dynamic_cast<QObject *>(imgInterface),SIGNAL(cameraExposureChanged(const QString&,qint32))
                ,this,SLOT(setCameraExposure(const QString&,qint32)));

        connect(dynamic_cast<QObject *>(imgInterface),SIGNAL(cameraSettingsRefreshed(const QString&,QStringList&))
                ,this,SLOT(getCameraSettings(const QString&,QStringList&)));

        connect(dynamic_cast<QObject *>(imgInterface),SIGNAL(getAxisPos(const QString&,double&))
                ,this,SLOT(getAxisPosition(const QString&,double&)));

        connect(dynamic_cast<QObject *>(imgInterface),SIGNAL(axisMoveToWait(const QString&,double))
                ,this,SLOT(axisMoveToWaitStop(const QString&,double)));

        return imgInterface;
    }
    return false;
}

bool HInterfaceController::loadAxisAdjustPlugin()
{
    QPluginLoader pluginLoader(qApp->applicationDirPath()+"/plugins/axisAdjustPlugin.dll");
    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        axisAdjustInterface = qobject_cast<HAxisAdjustInterface *>(plugin);
        axisAdjustInterface->Init((QWidget*)(this->parent()));

        //        if(imgInterface->getWindow()){
        //            imgInterface->getWindow()->setParent((QWidget*)(this->parent()));
        //        }


        connect(HMainMenuBar::GetInstance()->getAction(HMainMenuBar::Setting_AxisAdjust),&QAction::triggered,this,[=](){
            axisAdjustInterface->refreshDeviceNames(H_MotionController::GetInstance()->GetAxisNames(),imgInterface->getProcessFuncNames());
            axisAdjustInterface->showWindow();
        });

        connect(dynamic_cast<QObject *>(axisAdjustInterface),SIGNAL(processImg(const QString&,QPointF &)),this,SLOT(processImage(const QString&,QPointF &)));
        connect(dynamic_cast<QObject *>(axisAdjustInterface),SIGNAL(processImgLine(const QString&,QLineF &)),this,SLOT(processImageLine(const QString&,QLineF &)));
        connect(dynamic_cast<QObject *>(axisAdjustInterface),SIGNAL(getAxisPos(const QString&,double &)),this,SLOT(getAxisPosition(const QString&,double &)));
        connect(dynamic_cast<QObject *>(axisAdjustInterface),SIGNAL(getAxisPosCount(const QString&,qint32 &)),this,SLOT(getAxisPositionCount(const QString&,qint32 &)));
        connect(dynamic_cast<QObject *>(axisAdjustInterface),SIGNAL(axisMoveToWait(const QString&,double)),this,SLOT(axisMoveToWaitStop(const QString&,double)));
        connect(dynamic_cast<QObject *>(axisAdjustInterface),SIGNAL(axisMoveStepWait(const QString&,double)),this,SLOT(axisMoveStepWaitStop(const QString&,double)));

        return axisAdjustInterface;
    }
    return false;
}
