#include "hcameracontroller.h"


#include "camera/baslercamera.h"
#include "camera/havtcamera.h"

#include "Src/data/hsettings.h"

HCameraController* HCameraController::m_pInstance = nullptr;

HCameraController *HCameraController::GetInstance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HCameraController;
        atexit(Release);
    }
    return m_pInstance;
}
void HCameraController::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

bool HCameraController::Init()
{
    if(InitAllCamera())
        return ConfigController();
    return false;
}

bool HCameraController::InitAllCamera()
{

    ReleaseDeviceRes();

    auto cameras=ptrSettings->getTableValuesAtCol(HSettings::Camera,HSettings::Camera_Type);
    auto cameraConfigs=ptrSettings->getTableValuesAtCol(HSettings::Camera,HSettings::Camera_ConfigFile);
    auto cameraName=ptrSettings->getTableValuesAtCol(HSettings::Camera,HSettings::Camera_Name);
    auto cameraChannels=ptrSettings->getTableValuesAtCol(HSettings::Camera,HSettings::Camera_Channel);

    auto availableCameras=ptrSettings->getAvailableCamera();

    bool isOk=true;



    for(qint32 k=0;k<cameras.size();++k){


        HCamera * camera=nullptr;

        auto cameraType=cameras.at(k).toString();
        qint32 type=availableCameras.indexOf(cameraType);
        if(type==0){
            camera=new HCamera(cameraChannels.at(k).toString().toInt());

        }else if(type==1){
            camera=new BaslerCamera(cameraChannels.at(k).toString().toInt());
        }
        else{
            camera=new HAVTCamera(cameraChannels.at(k).toString().toInt());
        }

        isOk&=(!camera->InitDevice());


        auto Name=cameraName.at(k).toString();

        m_CameraArray<<camera;
        m_CameraMap.insertMulti(Name,camera);
        m_CameraNames<<Name;

    }
    return isOk;
}

bool HCameraController::ConfigController()
{


    auto configFiles=ptrSettings->getTableValuesAtCol(HSettings::Camera,HSettings::Camera_ConfigFile);
    auto widths=ptrSettings->getTableValuesAtCol(HSettings::Camera,HSettings::Camera_ImgWidth);
    auto heights=ptrSettings->getTableValuesAtCol(HSettings::Camera,HSettings::Camera_ImgHeight);
    auto coords=ptrSettings->getTableValuesAtCol(HSettings::Camera,HSettings::Camera_Reverses);

    qint32 size=m_CameraArray.size();

    bool isOk=true;

    for(qint32 k=0;k<size;++k){
        HCamera * ptrCamera=m_CameraArray.at(k);
        CameraConfiguration config;
        config.imgWidth=widths.at(k).toString().toInt();
        config.imgHeight=heights.at(k).toString().toInt();

        auto flag=coords.at(k).toString();
        if(flag=="reverse X"||flag=="reverse XY"){
            config.reverseX=true;
        }
        if(flag=="reverse Y"||flag=="reverse XY"){
            config.reverseY=true;
        }
        isOk&=(!ptrCamera->SetConfigFile(configFiles.at(k).toString()));
        isOk&=(!ptrCamera->SetDeviceConfig(config));
    }
    return isOk;
}

HCamera *HCameraController::GetCamera(qint32 index) const
{

    return m_CameraArray.value(index);
}

HCamera *HCameraController::GetCamera(const QString &name) const
{
    return m_CameraMap.value(name);
}

qint32 HCameraController::GetCameraNum() const
{

    return m_CameraArray.size();
}

const QStringList &HCameraController::GetCameraNames() const
{
    return m_CameraNames;
}


void HCameraController::ReleaseDeviceRes()
{
    for(const auto &k:m_CameraArray){
        delete k;
    }
    m_CameraArray.clear();
    m_CameraMap.clear();
    m_CameraNames.clear();
}

HCameraController::HCameraController()
{
    ptrSettings=HSettings::GetInstance();
}

HCameraController::~HCameraController()
{
    ReleaseDeviceRes();
}


