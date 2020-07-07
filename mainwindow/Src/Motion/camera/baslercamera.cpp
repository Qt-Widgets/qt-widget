#include "baslercamera.h"
#include <QDebug>
#include <QDateTime>

template <typename T>
constexpr inline const T &hMin(const T &a, const T &b) { return (a < b) ? a : b; }

uint32_t BaslerCamera::m_deviceRefrenceCount=0;


PCam::IGigETransportLayer* BaslerCamera::ptrPTL=nullptr;
PCam::CInstantCameraArray* BaslerCamera::ptrCameras=nullptr;
PCam::CGrabResultPtr* BaslerCamera::ptrGrabResult=nullptr;
PCam::DeviceInfoList_t* BaslerCamera::ptrDevices=nullptr;
PCam::CDeviceInfo* BaslerCamera::ptrInfo=nullptr;

QVector<Pylon::CBaslerGigEInstantCamera*> BaslerCamera::ptrCameraDevices=QVector<Pylon::CBaslerGigEInstantCamera*>();
QStringList BaslerCamera::CameraInformations=QStringList();



BaslerCamera::BaslerCamera(qint32 index):HCamera(index)
{
    ++m_deviceRefrenceCount;
}


BaslerCamera::~BaslerCamera()
{
    CloseCamera();
    if(m_deviceRefrenceCount){
        if(--m_deviceRefrenceCount)
            return;
    }
    if(!configFilePath.isEmpty()&&(m_DeviceIndex<ptrCameraDevices.size())){
        SaveParameterToFile(m_DeviceIndex,configFilePath);
    }
    FreeAllDevices();
}

qint32 BaslerCamera::InitDevice()
{

    if(m_deviceRefrenceCount>1){
        return 0;
    }else{
        return !InitAllDevices();
    }
}

void BaslerCamera::UnInitDevice()
{
    FreeAllDevices();
    m_deviceRefrenceCount=0;
}

qint32 BaslerCamera::OpenCamera()
{
    if(m_DeviceIndex>ptrCameraDevices.size()-1)
        return 2;
    return !openCamera(m_DeviceIndex);
}

qint32 BaslerCamera::CloseCamera()
{
    if(m_DeviceIndex>ptrCameraDevices.size()-1)
        return 2;
    return !closeCamera(m_DeviceIndex);
}

QString BaslerCamera::GetDeviceName()
{

    if(m_DeviceIndex>CameraInformations.size()-1)
        return  "No camera";
    return  CameraInformations.at(m_DeviceIndex);
}

QStringList BaslerCamera::GetAllDeviceNames()
{
    return CameraInformations.isEmpty()?QStringList("no camera"):CameraInformations;
}

qint32 BaslerCamera::SetConfigFile(const QString &configFile)
{
    configFilePath=configFile;
    if(m_DeviceIndex>ptrCameraDevices.size()-1)
        return 2;
    return !LoadParameterFile(m_DeviceIndex,configFile);
}


qint32 BaslerCamera::SetDeviceConfig(const CameraConfiguration &data)
{
    if(m_DeviceIndex>ptrCameraDevices.size()-1)
        return 2;

    m_DevcieConfig=data;

    AOIControl para;
    para.width=data.imgWidth;
    para.height=data.imgHeight;
    para.xOffset=0;
    para.yOffset=0;
    para.centerX=true;
    para.centerY=true;
    para.reverseX=data.reverseX;
    para.reverseY=data.reverseY;

    return !AOIControlParameters(m_DeviceIndex,para,false);
}

qint32 BaslerCamera::GetImgWidthHeight(qint32 &width, qint32 &height)
{
    if(m_DeviceIndex>ptrCameraDevices.size()-1)
        return 2;

    bool isOK=true;
    int64_t w,h;
    isOK&= Width(m_DeviceIndex,w,true);
    isOK&= Height(m_DeviceIndex,h,true);
    width=qint32(w);
    height=qint32(h);
    return !isOK;
}

qint32 BaslerCamera::SetImgCaptureMode(qint32 mode)
{
    if(m_DeviceIndex>ptrCameraDevices.size()-1)
        return 2;
    return !(mode?startContinuesShot(m_DeviceIndex):stopContinuesShot(m_DeviceIndex));
}

uchar *BaslerCamera::GetImgBufferOnce()
{
    if(m_DeviceIndex>ptrCameraDevices.size()-1)
        return nullptr;
    return GetOneImageBuffer(m_DeviceIndex);
}

uchar *BaslerCamera::GetImgBufferContinues()
{
    if(m_DeviceIndex>ptrCameraDevices.size()-1)
        return nullptr;
    return GetImageBuffer(m_DeviceIndex);
}

HColoredImgBuffer BaslerCamera::GetColoredImgBufferContinues()
{
    HColoredImgBuffer pbuffer;
    if(m_DeviceIndex>ptrCameraDevices.size()-1)
        return pbuffer;
    pbuffer.ptrBufferRed= GetImageBuffer(m_DeviceIndex);
    return pbuffer;
}

qint32 BaslerCamera::GetGain(qint32 &value)
{
    if(m_DeviceIndex>ptrCameraDevices.size()-1)
        return 1;
    qint64 dvv;
    if(Gain(m_DeviceIndex,dvv,true)){
        value=qint32(dvv);
        return 0;
    }else{
        return 2;
    }
}

qint32 BaslerCamera::GetExposure(qint32 &value)
{
    if(m_DeviceIndex>ptrCameraDevices.size()-1)
        return 1;
    qint64 dvv;
    if(ExposureTimeRaw(m_DeviceIndex,dvv,true)){
        value=qint32(dvv);
        return 0;
    }else{
        return 2;
    }
}



qint32 BaslerCamera::SetGain( qint32 value)
{
    if(m_DeviceIndex>ptrCameraDevices.size()-1)
        return 1;
    qint64 dvv=value;
    return !Gain(m_DeviceIndex,dvv,false);
}

qint32 BaslerCamera::SetExposure( qint32 value)
{

    if(m_DeviceIndex>ptrCameraDevices.size()-1)
        return 1;
    qint64 dvv=value;
    return !ExposureTimeRaw(m_DeviceIndex,dvv,false);
}

double BaslerCamera::GetFPS()
{
    if(m_DeviceIndex>ptrCameraDevices.size()-1)
        return false;
    double fps=0;
    return FramePerSecond(m_DeviceIndex,fps,true)?fps:-1;
}




bool BaslerCamera::InitAllDevices()
{

    try
    {
        FreeAllDevices();
        PCam::PylonInitialize();
        PCam::CTlFactory *ptrTlFactory = &PCam::CTlFactory::GetInstance();
        ptrPTL = dynamic_cast<PCam::IGigETransportLayer*>(ptrTlFactory->CreateTl(PCam::BaslerGigEDeviceClass));
        if (ptrPTL == nullptr)
        {
            throw RUNTIME_EXCEPTION("No GigE transport layer available.");
        }
        ptrDevices=new PCam::DeviceInfoList_t();

        if ( ptrTlFactory->EnumerateDevices(*ptrDevices) == 0 )
        {
            throw RUNTIME_EXCEPTION( "No camera present.");
        }


        ptrInfo=new PCam::CDeviceInfo();
        ptrInfo->SetDeviceClass(Camera_t::DeviceClass());

        ptrCameras=new PCam::CInstantCameraArray(qMin(ptrDevices->size(), devicesNumMax));

        for ( size_t i = 0; i < ptrCameras->GetSize(); ++i)
        {
            PCam::IPylonDevice *a=ptrTlFactory->CreateDevice( ptrDevices->operator[](i));

            ptrCameras->operator[](i).Attach(a);
            ptrDevices->operator[](i).SetDeviceClass( ptrCameras->operator[](i).GetDeviceInfo().GetDeviceClass());
            ptrDevices->operator[](i).SetSerialNumber( ptrCameras->operator[](i).GetDeviceInfo().GetSerialNumber());

            QString str=QString(ptrCameras->operator[](i).GetDeviceInfo().GetFriendlyName());
            CameraInformations.append(str);
            Camera_t*pptr=new Camera_t(a);

            //pptr->RegisterConfiguration( new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete);
            //pptr->RegisterConfiguration( new CConfigurationEventPrinter, RegistrationMode_Append, Cleanup_Delete);
            //pptr->RegisterImageEventHandler( new CImageEventPrinter, RegistrationMode_Append, Cleanup_Delete);
            pptr->Open();
            //pptr->MaxNumBuffer = 100;
            ptrCameraDevices<<pptr;
        }


        //ptrCameraDevices.at(dindex)->StartGrabbing(GrabStrategy_LatestImageOnly);

        //        PCam::GainSelectorEnums K;
        //        K=PCam::GainSelectorEnums::GainSelector_DigitalAll;
        //        GainSelector(K,false);

        ptrGrabResult=new PCam::CGrabResultPtr();



        return true;

    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }

}

void BaslerCamera::FreeAllDevices()
{



    if (ptrCameras!=nullptr)
    {
        if(ptrCameras->IsOpen())
        {
            if(ptrCameras->IsGrabbing()){
                ptrCameras->StopGrabbing();
                if(!configFilePath.isEmpty()){
                    if(m_DeviceIndex<(ptrCameraDevices.size())){
                        SaveParameterToFile(m_DeviceIndex,configFilePath);
                    }

                }
            }

            ptrCameras->Close();
        }

        if (!ptrCameras->IsCameraDeviceRemoved())
        {
            ptrCameras->DestroyDevice();
        }
        delete ptrCameras;

    }

    if (ptrInfo!=nullptr)
    {
        delete ptrInfo;
    }

    if (ptrDevices!=nullptr)
    {
        delete ptrDevices;
    }



    if (ptrGrabResult!=nullptr)
    {
        ptrGrabResult->Release();
        delete ptrGrabResult;
    }


    ptrCameras=nullptr;
    ptrGrabResult=nullptr;
    ptrPTL=nullptr;
    ptrInfo=nullptr;
    ptrDevices=nullptr;

    //    for(const auto &k:ptrCameraDevices){
    //        if(k){
    //            delete k;
    //        }
    //    }
    ptrCameraDevices.clear();
    CameraInformations.clear();

    PCam::PylonTerminate();
}

bool BaslerCamera::openCamera(qint32 dindex)
{
    try
    {
        ptrCameraDevices.at(dindex)->Open();
        return true;
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}

bool BaslerCamera::closeCamera(qint32 dindex)
{
    try
    {
        if(ptrCameraDevices.size()>dindex){
            ptrCameraDevices.at(dindex)->Close();
            return true;
        }else{
           return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}



bool BaslerCamera::LoadParameterFile(qint32 dindex, const QString &FileNme)
{
    //QString file=QCoreApplication::applicationDirPath()+"/camera/acA4112-8gm.pfs";
    return LoadSettingFromFile(dindex,/*FileNme.toStdString().c_str()*/FileNme.toLocal8Bit().data());
}

bool BaslerCamera::SaveParameterToFile(qint32 dindex, const QString &FileNme)
{
    return  SaveSettingToFile(dindex,FileNme.toLocal8Bit().data());
}

unsigned char *BaslerCamera::GetImageBuffer(qint32 index, uint timeOut)
{

    //    if (ptrGrabResult!=nullptr)
    //    {
    //        ptrGrabResult->Release();
    //    }


    //qDebug()<< 0<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz");
    try
    {
        ///////////////////////////////////////////func 0///////////////////////////////////////////////////

        //        if(!ptrCameraDevices.at(dindex))
        //            throw RUNTIME_EXCEPTION("The camera is not existed.");

        Camera_t * ptrCam=ptrCameraDevices.at(index);


        //ptrCameraDevice.value(cameraName)->StartGrabbing(GrabStrategy_LatestImageOnly);
        if (ptrCam->CanWaitForFrameTriggerReady())
        {
            //if (ptrCameraDevice.value(cameraName)->WaitForFrameTriggerReady(200, TimeoutHandling_ThrowException))
            //{
            ptrCam->ExecuteSoftwareTrigger();
            //}
            //else{
            //    errorInfo=QObject::tr("wait for frameTrigger failed.");
            //    return nullptr;
            //}

            if(ptrCam->RetrieveResult(timeOut, *ptrGrabResult, PCam::TimeoutHandling_ThrowException)){

                //ptrCameraDevices.at(dindex)->StopGrabbing();
                //qDebug()<< 1<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss z");
                return static_cast<uint8_t *>((*ptrGrabResult)->GetBuffer());
            }
            else{
                ptrCam->StopGrabbing();
                //qDebug()<< 2<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss z");
                throw RUNTIME_EXCEPTION((*ptrGrabResult)->GetErrorDescription());
            }

        }
        else{
            ptrCam->StopGrabbing();
            //qDebug()<< 3<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss z");
            throw RUNTIME_EXCEPTION("camera are not ready to accept the next frame trigger.");
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////func 1///////////////////////////////////////////////////
        //        if(ptrCameraDevice.value(cameraName)->IsGrabbing())
        //        {
        //            // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
        //            ptrCameraDevice.value(cameraName)->RetrieveResult(timeOut,*ptrGrabResult, TimeoutHandling_ThrowException);

        //            // Image grabbed successfully?
        //            if ((*ptrGrabResult)->GrabSucceeded())
        //            {
        //                // Access the image data.
        //                //cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
        //                //cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
        //                //const uint8_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer();
        //                //cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0] << endl << endl;
        //                qDebug()<< 1<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss z");
        //                return static_cast<uint8_t *>((*ptrGrabResult)->GetBuffer());

        //            }
        //            else
        //            {
        //                errorInfo=(*ptrGrabResult)->GetErrorDescription();
        //                return nullptr;
        //            }
        //        }
        //        else{
        //            return nullptr;
        //        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////func 2///////////////////////////////////////////////////
        //        if(ptrCameraDevice.value(cameraName)->GrabOne(timeOut, *ptrGrabResult))
        //        {
        //            do
        //            {
        //                //cameras->operator[](0).RetrieveResult(timeOut, ptrGrabResult, TimeoutHandling_ThrowException);
        //                if (/*(*ptrGrabResult)->GrabSucceeded()&&*/ptrCameraDevice.value(cameraName)->IsGrabbing())
        //                {
        //                    continue;
        //                }
        //                else{
        //                    //Pylon::DisplayImage(0, *ptrGrabResult);
        //                    //uint8_t *pImageBuffer =(uint8_t *)(*ptrGrabResult)->GetBuffer();
        //                    qDebug()<< 1<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss z");
        //                    /*const uint8_t *pImageBuffer =*/return static_cast<uint8_t *>((*ptrGrabResult)->GetBuffer());
        //                }
        //            }
        //            while (true);
        //        }
        //        else{
        //            return nullptr;
        //        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return nullptr;
    }
}

unsigned char *BaslerCamera::GetOneImageBuffer(qint32 index, uint timeOut)
{
    try
    {

        Camera_t*  ptr=ptrCameraDevices.at(index);


        if(!ptr->IsGrabbing())
            ptr->StartGrabbing(PCam::GrabStrategy_LatestImageOnly);

        if (ptr->CanWaitForFrameTriggerReady())
        {
            ptr->ExecuteSoftwareTrigger();
            if(ptr->RetrieveResult(timeOut, *ptrGrabResult, PCam::TimeoutHandling_ThrowException)){
                ptr->StopGrabbing();
                //qDebug()<< 1<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss z");
                return static_cast<uint8_t *>((*ptrGrabResult)->GetBuffer());
            }
            else{
                ptr->StopGrabbing();
                //qDebug()<< 2<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss z");
                throw RUNTIME_EXCEPTION((*ptrGrabResult)->GetErrorDescription());
            }
        }
        else{
            ptr->StopGrabbing();
            //qDebug()<< 3<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss z");
            throw RUNTIME_EXCEPTION("camera are not ready to accept the next frame trigger.");
        }

    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return nullptr;
    }
}



bool BaslerCamera::startContinuesShot(qint32 dindex)
{
    try
    {
        if(!ptrCameraDevices.at(dindex))
            throw RUNTIME_EXCEPTION("The camera is not existed.");


        ptrCameraDevices.at(dindex)->StartGrabbing(PCam::GrabStrategy_LatestImageOnly);
        if(!ptrCameraDevices.at(dindex)->IsGrabbing()){
            throw RUNTIME_EXCEPTION("Started continues shot failed.");
        }
        else{
            return 1;
        }


    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return 0;
    }

}

bool BaslerCamera::stopContinuesShot(qint32 dindex)
{
    try
    {
        if(!ptrCameraDevices.at(dindex))
            throw RUNTIME_EXCEPTION("The camera is not existed.");

        ptrCameraDevices.at(dindex)->StopGrabbing();
        Sleep(100);
        if(ptrCameraDevices.at(dindex)->IsGrabbing()){
            throw RUNTIME_EXCEPTION("Stoped continues shot failed.");
        }
        else{
            return 1;
        }

    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return 0;
    }
}


QString BaslerCamera::GetLastErrorInfo()
{
    return errorInfo;
}




QStringList BaslerCamera::GetCameraNames()
{
    return CameraInformations;
}

bool BaslerCamera::LoadDefaultSetting(qint32 dindex)
{
    try
    {
        if(ptrCameraDevices.at(dindex)==nullptr)
            throw RUNTIME_EXCEPTION( "No camera present.");
        ptrCameraDevices.at(dindex)->Open();
        ptrCameraDevices.at(dindex)->UserSetSelector.SetValue(PCam::UserSetSelector_Default);
        ptrCameraDevices.at(dindex)->UserSetLoad.Execute();
        ptrCameraDevices.at(dindex)->Close();
        return true;

    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}

bool BaslerCamera::SaveAsDefaultSetting(qint32 dindex)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {
            ptrCameraDevices.at(dindex)->Open();
            ptrCameraDevices.at(dindex)->UserSetDefaultSelector.SetValue(PCam::UserSetDefaultSelector_UserSet1);
            ptrCameraDevices.at(dindex)->UserSetLoad.Execute();
            ptrCameraDevices.at(dindex)->Close();
            return false;
        }
        else
        {
            return true;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return true;
    }
}

bool BaslerCamera::LoadSettingFromFile(qint32 dindex,const PCam::gcstring& FileName)
{
    try
    {
        if(ptrCameraDevices.at(dindex)==nullptr)
            throw RUNTIME_EXCEPTION( "No camera present.");
        PCam::CFeaturePersistence::Load(FileName, &ptrCameraDevices.at(dindex)->GetNodeMap());
        return true;

    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}

bool BaslerCamera::SaveSettingToFile(qint32 dindex,char FileName[])
{
    try
    {
        if(ptrCameraDevices.at(dindex)==nullptr)
            throw RUNTIME_EXCEPTION( "No camera present.");
        PCam::CFeaturePersistence::Save( FileName, &ptrCameraDevices.at(dindex)->GetNodeMap() );
        return true;

    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}

bool BaslerCamera::FramePerSecond(qint32 dindex,double &value, bool mode)
{

    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {

                value = ptrCameraDevices.at(dindex)->ResultingFrameRateAbs.GetValue();
            }
            else
            {
                ptrCameraDevices.at(dindex)->ResultingFrameRateAbs.SetValue(value);
            }

            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}




bool BaslerCamera::ExposureAuto(qint32 dindex,int64_t& value,bool mode)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                value=PCam::ExposureAutoEnums(ptrCameraDevices.at(dindex)->ExposureAuto.GetValue());
            }
            else
            {

                ptrCameraDevices.at(dindex)->ExposureAuto.SetValue(PCam::ExposureAutoEnums(value));
            }

            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}
bool BaslerCamera::Gain(qint32 dindex,int64_t&value,bool mode)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                value=ptrCameraDevices.at(dindex)->GainRaw.GetValue();
            }
            else
            {
                ptrCameraDevices.at(dindex)->GainAuto.SetValue(PCam::GainAuto_Off);
                ptrCameraDevices.at(dindex)->GainRaw.SetValue(value);
            }

            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}
bool BaslerCamera::GainAuto(qint32 dindex,int64_t&value,bool mode)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                value=PCam::GainAutoEnums(ptrCameraDevices.at(dindex)->GainAuto.GetValue());
            }
            else
            {
                ptrCameraDevices.at(dindex)->GainAuto.SetValue(PCam::GainAutoEnums(value));
            }


            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}
bool BaslerCamera::Height(qint32 dindex, int64_t &value , bool mode)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                value=ptrCameraDevices.at(dindex)->Height.GetValue();
            }
            else
            {
                ptrCameraDevices.at(dindex)->Height.SetValue(value);
            }

            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}
bool BaslerCamera::Width(qint32 dindex,int64_t & value ,bool mode)
{

    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                value=ptrCameraDevices.at(dindex)->Width.GetValue();
            }
            else
            {
                ptrCameraDevices.at(dindex)->Width.SetValue(value);
            }

            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}
bool BaslerCamera::XOffset(qint32 dindex,int64_t & value ,bool mode)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                value=ptrCameraDevices.at(dindex)->OffsetX.GetValue();
            }
            else
            {
                if (ptrCameraDevices.at(dindex)->CenterX.GetValue()==false)
                {
                    ptrCameraDevices.at(dindex)->OffsetX.SetValue(value);
                }
            }


            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}
bool BaslerCamera::YOffset(qint32 dindex,int64_t & value ,bool mode)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                value=ptrCameraDevices.at(dindex)->OffsetY.GetValue();
            }
            else
            {
                if (ptrCameraDevices.at(dindex)->CenterY.GetValue()==false)
                {
                    ptrCameraDevices.at(dindex)->OffsetY.SetValue(value);
                }
            }


            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}
bool BaslerCamera::CenterX(qint32 dindex,bool &value,bool mode)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                value=ptrCameraDevices.at(dindex)->CenterX.GetValue();
            }
            else
            {
                ptrCameraDevices.at(dindex)->CenterX.SetValue(value);
            }


            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}
bool BaslerCamera::CenterY(qint32 dindex,bool &value,bool mode)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                value=ptrCameraDevices.at(dindex)->CenterY.GetValue();
            }
            else
            {
                ptrCameraDevices.at(dindex)->CenterY.SetValue(value);
            }

            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}
bool BaslerCamera::GetGainRawMin(qint32 dindex,int64_t&value)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {


            value=ptrCameraDevices.at(dindex)->GainRaw.GetMin();


            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}
bool BaslerCamera::GetGainRawMax(qint32 dindex,int64_t&value)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {


            value=ptrCameraDevices.at(dindex)->GainRaw.GetMax();


            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}

bool BaslerCamera::GetExposureTimeRawMin(qint32 dindex,int64_t&value)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {


            value=ptrCameraDevices.at(dindex)->ExposureTimeRaw.GetMin();



            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}

bool BaslerCamera::GetExposureTimeRawMax(qint32 dindex,int64_t&value)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {


            value=ptrCameraDevices.at(dindex)->ExposureTimeRaw.GetMax()/35;

            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}

bool BaslerCamera::ExposureTimeRaw(qint32 dindex,int64_t &value,bool mode)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                value=ptrCameraDevices.at(dindex)->ExposureTimeRaw.GetValue();
                //value/=35;
            }
            else
            {
                ptrCameraDevices.at(dindex)->ExposureAuto.SetValue(PCam::ExposureAuto_Off);
                if(value<137)
                    value=137;
                ptrCameraDevices.at(dindex)->ExposureTimeRaw.SetValue(value);
            }

            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {

        errorInfo=e.GetDescription();
        return false;
    }
}
bool BaslerCamera::PixelFormat(qint32 dindex,int64_t &value,bool mode)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                value=PCam::PixelFormatEnums(ptrCameraDevices.at(dindex)->PixelFormat.GetValue());
            }
            else
            {
                ptrCameraDevices.at(dindex)->PixelFormat.SetValue(PCam::PixelFormatEnums(value));
            }


            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }
}




bool BaslerCamera::NetWorkParameters(qint32 dindex,CameraIP &NetWorkPara,bool mode)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                NetWorkPara.m_ipaddress=ptrCameraDevices.at(dindex)->GevCurrentIPAddress.ToString();
                NetWorkPara.m_subnet=ptrCameraDevices.at(dindex)->GevCurrentSubnetMask.ToString();
                NetWorkPara.m_gataway=ptrCameraDevices.at(dindex)->GevCurrentDefaultGateway.ToString();
                NetWorkPara.m_macaddress=ptrCameraDevices.at(dindex)->GevMACAddress.ToString();
                NetWorkPara.dhcp_support=ptrCameraDevices.at(dindex)->GevSupportedIPConfigurationDHCP.GetValue();
                NetWorkPara.ipfix_support=ptrCameraDevices.at(dindex)->GevSupportedIPConfigurationPersistentIP.GetValue();
                NetWorkPara.autoip_support=ptrCameraDevices.at(dindex)->GevSupportedIPConfigurationLLA.GetValue();
                NetWorkPara.netWorkMode=ptrCameraDevices.at(dindex)->GevCurrentIPConfiguration.GetValue()-4;
            }
            else
            {
                switch (NetWorkPara.netWorkMode)
                {
                case 0:
                {
                    ptrCameraDevices.at(dindex)->ChangeIpConfiguration(0,0);
                }
                    break;
                case 1:
                {
                    ptrCameraDevices.at(dindex)->ChangeIpConfiguration(1,0);
                    ptrCameraDevices.at(dindex)->SetPersistentIpAddress(NetWorkPara.m_ipaddress,NetWorkPara.m_subnet,NetWorkPara.m_gataway);
                }
                    break;
                case 2:
                {
                    ptrCameraDevices.at(dindex)->ChangeIpConfiguration(0,1);
                }
                    break;
                default:
                {
                    ptrCameraDevices.at(dindex)->Close();
                    ptrPTL->RestartIpConfiguration(NetWorkPara.m_macaddress);
                    return true;
                }
                }
            }

            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {

        errorInfo=e.GetDescription();
        return false;
    }



}

bool BaslerCamera::AnalogControlParameters(qint32 dindex,AnalogControl &analogControl,bool mode)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                analogControl.autoGain=PCam::GainAutoEnums(ptrCameraDevices.at(dindex)->GainAuto.GetValue());
                analogControl.gainMode=PCam::GainSelectorEnums(ptrCameraDevices.at(dindex)->GainSelector.GetValue());
                analogControl.gainRaw=ptrCameraDevices.at(dindex)->GainRaw.GetValue();
                analogControl.gainRawMin=ptrCameraDevices.at(dindex)->GainRaw.GetMin();
                analogControl.gainRawMax=ptrCameraDevices.at(dindex)->GainRaw.GetMax();
                analogControl.blackLevelSelector=PCam::BlackLevelSelectorEnums(ptrCameraDevices.at(dindex)->BlackLevelSelector.GetValue());
                analogControl.blackLevelRaw=ptrCameraDevices.at(dindex)->BlackLevelRaw.GetValue();
                analogControl.blackLevelRawMin=ptrCameraDevices.at(dindex)->BlackLevelRaw.GetMin();
                analogControl.blackLevelRawMax=ptrCameraDevices.at(dindex)->BlackLevelRaw.GetMax();
                analogControl.gammaEnable=ptrCameraDevices.at(dindex)->GammaEnable.GetValue();
                analogControl.gammmaSelector=PCam::GammaSelectorEnums(ptrCameraDevices.at(dindex)->GammaSelector.GetValue());
                analogControl.gamma=ptrCameraDevices.at(dindex)->Gamma.GetValue();
                analogControl.digitalShift=ptrCameraDevices.at(dindex)->DigitalShift.GetValue();
            }
            else
            {
                ptrCameraDevices.at(dindex)->GainAuto.SetValue(PCam::GainAutoEnums(analogControl.autoGain));
                ptrCameraDevices.at(dindex)->GainSelector.SetValue(PCam::GainSelectorEnums(analogControl.gainMode));
                if (PCam::GainAutoEnums(analogControl.autoGain)== PCam::GainAuto_Off)
                {
                    ptrCameraDevices.at(dindex)->GainRaw.SetValue(analogControl.gainRaw);
                }
                ptrCameraDevices.at(dindex)->BlackLevelSelector.SetValue(PCam::BlackLevelSelectorEnums(analogControl.blackLevelSelector));
                ptrCameraDevices.at(dindex)->BlackLevelRaw.SetValue(analogControl.blackLevelRaw);
                ptrCameraDevices.at(dindex)->GammaEnable.SetValue(analogControl.gammaEnable);
                ptrCameraDevices.at(dindex)->GammaSelector.SetValue(PCam::GammaSelectorEnums(analogControl.gammmaSelector));
                if (analogControl.gammmaSelector==0)
                {
                    ptrCameraDevices.at(dindex)->Gamma.SetValue(analogControl.gamma);
                }

                ptrCameraDevices.at(dindex)->DigitalShift.SetValue(analogControl.digitalShift);
            }

            return true;

        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {
        errorInfo=e.GetDescription();
        return false;
    }


}

bool BaslerCamera::AcquisitionControlParameters(qint32 dindex,AcquisitionControl &acquisitioncontrol,bool mode)
{
    try
    {

        if (ptrCameraDevices.at(dindex)!=nullptr)
        {
            if (mode)
            {
                acquisitioncontrol.exposureAuto=PCam::ExposureAutoEnums(ptrCameraDevices.at(dindex)->ExposureAuto.GetValue());
                acquisitioncontrol.exposureTimeRaw=ptrCameraDevices.at(dindex)->ExposureTimeRaw.GetValue();
                acquisitioncontrol.exposureTimeRawMin=ptrCameraDevices.at(dindex)->ExposureTimeRaw.GetMin();
                acquisitioncontrol.exposureTimeRawMax=ptrCameraDevices.at(dindex)->ExposureTimeRaw.GetMax();
                acquisitioncontrol.globalResetReleaseModeEnable=ptrCameraDevices.at(dindex)->GlobalResetReleaseModeEnable.GetValue();
                acquisitioncontrol.acquisitionFrameRateEnable=ptrCameraDevices.at(dindex)->AcquisitionFrameRateEnable.GetValue();
                acquisitioncontrol.acquisitionFrameRateAbs=ptrCameraDevices.at(dindex)->AcquisitionFrameRateAbs.GetValue();
                acquisitioncontrol.resultingFrameRateAbs=ptrCameraDevices.at(dindex)->ResultingFrameRateAbs.GetValue();
            }
            else
            {
                ptrCameraDevices.at(dindex)->ExposureAuto.SetValue(PCam::ExposureAutoEnums(acquisitioncontrol.exposureAuto));
                if (acquisitioncontrol.exposureAuto==0)
                {
                    ptrCameraDevices.at(dindex)->ExposureTimeRaw.SetValue(acquisitioncontrol.exposureTimeRaw);
                }
                ptrCameraDevices.at(dindex)->GlobalResetReleaseModeEnable.SetValue(acquisitioncontrol.globalResetReleaseModeEnable);
                ptrCameraDevices.at(dindex)->AcquisitionFrameRateEnable.SetValue(acquisitioncontrol.acquisitionFrameRateEnable);
                ptrCameraDevices.at(dindex)->AcquisitionFrameRateAbs.SetValue(acquisitioncontrol.acquisitionFrameRateAbs);
            }
            return true;

        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {


        errorInfo=e.GetDescription();
        return false;
    }



}

bool BaslerCamera::ImageFormatControlParameters(qint32 dindex,ImageFormatControl &imageformatcontrol,bool mode)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                imageformatcontrol.pixelFormat=PCam::PixelFormatEnums(ptrCameraDevices.at(dindex)->PixelFormat.GetValue());
                imageformatcontrol.reverseX=ptrCameraDevices.at(dindex)->ReverseX.GetValue();
            }
            else
            {
                ptrCameraDevices.at(dindex)->PixelFormat.SetValue(PCam::PixelFormatEnums(imageformatcontrol.pixelFormat));
                ptrCameraDevices.at(dindex)->ReverseX.SetValue(imageformatcontrol.reverseX);
            }

            return true;

        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {


        errorInfo=e.GetDescription();
        return false;
    }


}

bool BaslerCamera::AOIControlParameters(qint32 dindex,AOIControl &aoicontrol,bool mode)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                aoicontrol.width=ptrCameraDevices.at(dindex)->Width.GetValue();
                aoicontrol.height=ptrCameraDevices.at(dindex)->Height.GetValue();
                aoicontrol.xOffset=ptrCameraDevices.at(dindex)->OffsetX.GetValue();
                aoicontrol.yOffset=ptrCameraDevices.at(dindex)->OffsetY.GetValue();
                aoicontrol.centerX=ptrCameraDevices.at(dindex)->CenterX.GetValue();
                aoicontrol.centerY=ptrCameraDevices.at(dindex)->CenterY.GetValue();
                aoicontrol.reverseX=ptrCameraDevices.at(dindex)->ReverseX.GetValue();
                aoicontrol.reverseY=ptrCameraDevices.at(dindex)->ReverseY.GetValue();

            }
            else
            {
                ptrCameraDevices.at(dindex)->Width.SetValue(aoicontrol.width);
                ptrCameraDevices.at(dindex)->Height.SetValue(aoicontrol.height);
                ptrCameraDevices.at(dindex)->CenterX.SetValue(aoicontrol.centerX);
                ptrCameraDevices.at(dindex)->CenterY.SetValue(aoicontrol.centerY);
                if (aoicontrol.centerX==false)
                {
                    ptrCameraDevices.at(dindex)->OffsetX.SetValue(aoicontrol.xOffset);
                }
                if (aoicontrol.centerY==false)
                {
                    ptrCameraDevices.at(dindex)->OffsetY.SetValue(aoicontrol.yOffset);
                }
                ptrCameraDevices.at(dindex)->ReverseX.SetValue(aoicontrol.reverseX);
                ptrCameraDevices.at(dindex)->ReverseY.SetValue(aoicontrol.reverseY);
            }

            return true;

        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {


        errorInfo=e.GetDescription();
        return false;
    }

}

bool BaslerCamera::TransportLayerParameters(qint32 dindex,TransportLayer &transportlayer,bool mode)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            if (mode)
            {
                transportlayer.heartbeatTimeout=ptrCameraDevices.at(dindex)->GevHeartbeatTimeout.GetValue();

            }
            else
            {
                ptrCameraDevices.at(dindex)->GevHeartbeatTimeout.SetValue(transportlayer.heartbeatTimeout);

            }

            return true;

        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {


        errorInfo=e.GetDescription();
        return false;
    }
}

bool BaslerCamera::GetDeviceInfo(qint32 dindex,DeviceInfo&deviceInfo)
{
    try
    {
        if (ptrCameraDevices.at(dindex)!=nullptr)
        {

            deviceInfo.deviceVendorName=ptrCameraDevices.at(dindex)->DeviceVendorName.GetValue();
            deviceInfo.deviceModelName=ptrCameraDevices.at(dindex)->DeviceModelName.GetValue();
            deviceInfo.deviceManufacturerInfo=ptrCameraDevices.at(dindex)->DeviceManufacturerInfo.GetValue();
            deviceInfo.deviceVersion=ptrCameraDevices.at(dindex)->DeviceVersion.GetValue();
            deviceInfo.deviceFirmwareVersion=ptrCameraDevices.at(dindex)->DeviceFirmwareVersion.GetValue();
            deviceInfo.deviceID=ptrCameraDevices.at(dindex)->DeviceID.GetValue();
            deviceInfo.deviceUserID=ptrCameraDevices.at(dindex)->DeviceUserID.GetValue();
            deviceInfo.deviceScanType=PCam::DeviceScanTypeEnums(ptrCameraDevices.at(dindex)->DeviceScanType.GetValue());
            deviceInfo.sensorWidth=ptrCameraDevices.at(dindex)->SensorWidth.GetValue();
            deviceInfo.deviceVendorName=ptrCameraDevices.at(dindex)->DeviceVendorName.GetValue();
            deviceInfo.deviceVendorName=ptrCameraDevices.at(dindex)->DeviceVendorName.GetValue();
            deviceInfo.deviceVendorName=ptrCameraDevices.at(dindex)->DeviceVendorName.GetValue();
            deviceInfo.deviceVendorName=ptrCameraDevices.at(dindex)->DeviceVendorName.GetValue();

            return true;

        }
        else
        {
            return false;
        }
    }
    catch (const PCam::GenericException &e)
    {


        errorInfo=e.GetDescription();
        return false;
    }

}

void BaslerCamera::GainAuto(qint32 dindex,PCam::GainAutoEnums&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GainAuto.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GainAuto.GetValue();
        }
    }
}

void BaslerCamera::GainSelector(qint32 dindex,PCam::GainSelectorEnums&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GainSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GainSelector.GetValue();
        }
    }
}

void BaslerCamera::Gain1(qint32 dindex,int64_t&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GainRaw.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GainRaw.GetValue();
        }
    }
}

void BaslerCamera::BlackLevelSelector(qint32 dindex,PCam::BlackLevelSelectorEnums&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->BlackLevelSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->BlackLevelSelector.GetValue();
        }
    }
}

void BaslerCamera::BlackLevel(qint32 dindex,int64_t&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->BlackLevelRaw.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->BlackLevelRaw.GetValue();
        }
    }
}

void BaslerCamera::GammaEnable(qint32 dindex,bool&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GammaEnable.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GammaEnable.GetValue();
        }
    }
}

void BaslerCamera::GammaSelector(qint32 dindex,PCam::GammaSelectorEnums&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GammaSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GammaSelector.GetValue();
        }
    }
}

void BaslerCamera::Gamma(qint32 dindex,double &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->Gamma.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->Gamma.GetValue();
        }
    }
}

void BaslerCamera::DigitalShift(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->DigitalShift.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->DigitalShift.GetValue();
        }
    }
}

void BaslerCamera::PixelFormat(qint32 dindex,PCam::PixelFormatEnums&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->PixelFormat.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->PixelFormat.GetValue();
        }
    }
}

void BaslerCamera::GetPixelSize(qint32 dindex,PCam::PixelSizeEnums &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        value = ptrCameraDevices.at(dindex)->PixelSize.GetValue();
    }
}

void BaslerCamera::GetPixelColorFilter(qint32 dindex,PCam::PixelColorFilterEnums& value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        value = ptrCameraDevices.at(dindex)->PixelColorFilter.GetValue();
    }
}

void BaslerCamera::GetPixelDynamicRangeMin(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        value = ptrCameraDevices.at(dindex)->PixelDynamicRangeMin.GetValue();
    }
}

void BaslerCamera::GetPixelDynamicRangeMax(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        value = ptrCameraDevices.at(dindex)->PixelDynamicRangeMax.GetValue();
    }
}

void BaslerCamera::ReverseX(qint32 dindex,bool&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ReverseX.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ReverseX.GetValue();
        }
    }
}

void BaslerCamera::TestImageSelector(qint32 dindex,PCam::TestImageSelectorEnums& value, bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->TestImageSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->TestImageSelector.GetValue();
        }
    }
}

void BaslerCamera::Width1(qint32 dindex,int64_t & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->Width.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->Width.GetValue();
        }
    }
}

void BaslerCamera::Height1(qint32 dindex,int64_t & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->Height.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->Height.GetValue();
        }
    }
}

void BaslerCamera::XOffset1(qint32 dindex,int64_t & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->OffsetX.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->OffsetX.GetValue();
        }
    }
}

void BaslerCamera::YOffset1(qint32 dindex,int64_t & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->OffsetY.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->OffsetY.GetValue();
        }
    }
}

void BaslerCamera::CenterX1(qint32 dindex,bool &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->CenterX.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->CenterX.GetValue();
        }
    }
}

void BaslerCamera::CenterY1(qint32 dindex,bool &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->CenterY.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->CenterY.GetValue();
        }
    }
}

void BaslerCamera::BinningModeHorizontal(qint32 dindex,PCam::BinningModeHorizontalEnums& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->BinningModeHorizontal.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->BinningModeHorizontal.GetValue();
        }
    }
}

void BaslerCamera::BinningHorizontal(qint32 dindex,int64_t & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->BinningHorizontal.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->BinningHorizontal.GetValue();
        }
    }
}

void BaslerCamera::BinningModeVertical(qint32 dindex,PCam::BinningModeVerticalEnums&value, bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->BinningModeVertical.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->BinningModeVertical.GetValue();
        }
    }
}

void BaslerCamera::BinningVertical(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->BinningVertical.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->BinningVertical.GetValue();
        }
    }
}

void BaslerCamera::DecimationHorizontal(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->DecimationHorizontal.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->DecimationHorizontal.GetValue();
        }
    }
}

void BaslerCamera::DecimationVertical(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->DecimationVertical.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->DecimationVertical.GetValue();
        }
    }
}

void BaslerCamera::EnableBurstAcquisition(qint32 dindex,bool &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->EnableBurstAcquisition.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->EnableBurstAcquisition.GetValue();
        }
    }
}

void BaslerCamera::AcquisitionFrameCount(qint32 dindex,int64_t & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AcquisitionFrameCount.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AcquisitionFrameCount.GetValue();
        }
    }
}

void BaslerCamera::TriggerSelector(qint32 dindex,PCam::TriggerSelectorEnums & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->TriggerSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->TriggerSelector.GetValue();
        }
    }
}

void BaslerCamera::TriggerMode(qint32 dindex,PCam::TriggerModeEnums & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->TriggerMode.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->TriggerMode.GetValue();
        }
    }
}

void BaslerCamera::TriggerSoftware(qint32 dindex)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        ptrCameraDevices.at(dindex)->TriggerSoftware.Execute();
    }
}

void BaslerCamera::TriggerSource(qint32 dindex,PCam::TriggerSourceEnums& value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->TriggerSource.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->TriggerSource.GetValue();
        }
    }
}

void BaslerCamera::TriggerActivation(qint32 dindex,PCam::TriggerActivationEnums& value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->TriggerActivation.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->TriggerActivation.GetValue();
        }
    }
}

void BaslerCamera::TriggerDelayAbs(qint32 dindex,double& value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->TriggerDelayAbs.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->TriggerDelayAbs.GetValue();
        }
    }
}

void BaslerCamera::ExposureMode(qint32 dindex,PCam::ExposureModeEnums& value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ExposureMode.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ExposureMode.GetValue();
        }
    }
}

void BaslerCamera::ExposureAuto(qint32 dindex,PCam::ExposureAutoEnums& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ExposureAuto.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ExposureAuto.GetValue();
        }
    }
}

void BaslerCamera::ExposureTimeAbs(qint32 dindex,double& value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ExposureTimeAbs.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ExposureTimeAbs.GetValue();
        }
    }
}

void BaslerCamera::ExposureTimeRaw1(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ExposureTimeRaw.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ExposureTimeRaw.GetValue();
        }
    }
}

void BaslerCamera::ReadoutTimeAbs(qint32 dindex,double &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ReadoutTimeAbs.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ReadoutTimeAbs.GetValue();
        }
    }
}

void BaslerCamera::ShutterMode(qint32 dindex,PCam::ShutterModeEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ShutterMode.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ShutterMode.GetValue();
        }
    }
}

void BaslerCamera::AcquisitionFrameRateEnable(qint32 dindex,bool &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AcquisitionFrameRateEnable.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AcquisitionFrameRateEnable.GetValue();
        }
    }
}

void BaslerCamera::AcquisitionFrameRateAbs(qint32 dindex,double &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AcquisitionFrameRateAbs.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AcquisitionFrameRateAbs.GetValue();
        }
    }
}


void BaslerCamera::ResultingFramePeriodAbs(qint32 dindex,double &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ResultingFramePeriodAbs.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ResultingFramePeriodAbs.GetValue();
        }
    }
}

void BaslerCamera::ResultingFrameRateAbs(qint32 dindex,double &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ResultingFrameRateAbs.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ResultingFrameRateAbs.GetValue();
        }
    }
}



void BaslerCamera::AcquisitionStatusSelector(qint32 dindex,PCam::AcquisitionStatusSelectorEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AcquisitionStatusSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AcquisitionStatusSelector.GetValue();
        }
    }
}

void BaslerCamera::AcquisitionStatus(qint32 dindex,bool &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AcquisitionStatus.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AcquisitionStatus.GetValue();
        }
    }
}



void BaslerCamera::LineSelector(qint32 dindex,PCam::LineSelectorEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->LineSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->LineSelector.GetValue();
        }
    }
}

void BaslerCamera::LineMode(qint32 dindex,PCam::LineModeEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->LineMode.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->LineMode.GetValue();
        }
    }
}

void BaslerCamera::LineLogic(qint32 dindex,PCam::LineLogicEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->LineLogic.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->LineLogic.GetValue();
        }
    }
}

void BaslerCamera::LineFormat(qint32 dindex,PCam::LineFormatEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->LineFormat.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->LineFormat.GetValue();
        }
    }
}

void BaslerCamera::LineSource(qint32 dindex,PCam::LineSourceEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->LineSource.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->LineSource.GetValue();
        }
    }
}

void BaslerCamera::LineInverter(qint32 dindex,bool&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->LineInverter.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->LineInverter.GetValue();
        }
    }
}

void BaslerCamera::LineDebouncerTimeAbs(qint32 dindex,double&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->LineDebouncerTimeAbs.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->LineDebouncerTimeAbs.GetValue();
        }
    }
}

void BaslerCamera::MinOutPulseWidthAbs(qint32 dindex,double&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->MinOutPulseWidthAbs.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->MinOutPulseWidthAbs.GetValue();
        }
    }
}

void BaslerCamera::LineStatus(qint32 dindex,bool&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->LineStatus.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->LineStatus.GetValue();
        }
    }
}

void BaslerCamera::LineStatusAll(qint32 dindex,int64_t&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->LineStatusAll.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->LineStatusAll.GetValue();
        }
    }
}

void BaslerCamera::UserOutputSelector(qint32 dindex,PCam::UserOutputSelectorEnums&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->UserOutputSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->UserOutputSelector.GetValue();
        }
    }
}

void BaslerCamera::UserOutputValue(qint32 dindex,bool&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->UserOutputValue.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->UserOutputValue.GetValue();
        }
    }
}

void BaslerCamera::UserOutputValueAll(qint32 dindex,int64_t&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->UserOutputValueAll.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->UserOutputValueAll.GetValue();
        }
    }
}

void BaslerCamera::SyncUserOutputSelector(qint32 dindex,PCam::SyncUserOutputSelectorEnums&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->SyncUserOutputSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->SyncUserOutputSelector.GetValue();
        }
    }
}

void BaslerCamera::SyncUserOutputValue(qint32 dindex,bool&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->SyncUserOutputValue.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->SyncUserOutputValue.GetValue();
        }
    }
}

void BaslerCamera::SyncUserOutputValueAll(qint32 dindex,int64_t&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->SyncUserOutputValueAll.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->SyncUserOutputValueAll.GetValue();
        }
    }
}


void BaslerCamera::ActionCommandCount(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ActionCommandCount.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ActionCommandCount.GetValue();
        }
    }
}

void BaslerCamera::ActionDeviceKey(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ActionDeviceKey.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ActionDeviceKey.GetValue();
        }
    }
}

void BaslerCamera::ActionSelector(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ActionSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ActionSelector.GetValue();
        }
    }
}

void BaslerCamera::ActionGroupKey(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ActionGroupKey.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ActionGroupKey.GetValue();
        }
    }
}

void BaslerCamera::ActionGroupMask(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ActionGroupMask.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ActionGroupMask.GetValue();
        }
    }
}

void BaslerCamera::SequenceEnable(qint32 dindex,bool&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->SequenceEnable.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->SequenceEnable.GetValue();
        }
    }
}

void BaslerCamera::SequenceCurrentSet(qint32 dindex,int64_t&value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->SequenceCurrentSet.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->SequenceCurrentSet.GetValue();
        }
    }
}

void BaslerCamera::SequenceAsyncRestart(qint32 dindex)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        ptrCameraDevices.at(dindex)->SequenceAsyncRestart();
    }
}

void BaslerCamera::SequenceAsyncAdvance(qint32 dindex)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        ptrCameraDevices.at(dindex)->SequenceAsyncAdvance();
    }
}

void BaslerCamera::SequenceAdvanceMode(qint32 dindex,PCam::SequenceAdvanceModeEnums& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->SequenceAdvanceMode.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->SequenceAdvanceMode.GetValue();
        }
    }
}

void BaslerCamera::SequenceControlSelector(qint32 dindex,PCam::SequenceControlSelectorEnums& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->SequenceControlSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->SequenceControlSelector.GetValue();
        }
    }
}

void BaslerCamera::SequenceControlSource(qint32 dindex,PCam::SequenceControlSourceEnums& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->SequenceControlSource.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->SequenceControlSource.GetValue();
        }
    }
}

void BaslerCamera::SequenceAddressBitSelector(qint32 dindex,PCam::SequenceAddressBitSelectorEnums& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->SequenceAddressBitSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->SequenceAddressBitSelector.GetValue();
        }
    }
}

void BaslerCamera::SequenceAddressBitSource(qint32 dindex,PCam::SequenceAddressBitSourceEnums& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->SequenceAddressBitSource.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->SequenceAddressBitSource.GetValue();
        }
    }
}

void BaslerCamera::SequenceSetTotalNumber(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->SequenceSetTotalNumber.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->SequenceSetTotalNumber.GetValue();
        }
    }
}

void BaslerCamera::SequenceSetIndex(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->SequenceSetIndex.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->SequenceSetIndex.GetValue();
        }
    }
}

void BaslerCamera::SequenceSetStore(qint32 dindex)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        ptrCameraDevices.at(dindex)->SequenceSetStore();
    }
}

void BaslerCamera::SequenceSetLoad(qint32 dindex)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        ptrCameraDevices.at(dindex)->SequenceSetLoad();
    }
}

void BaslerCamera::SequenceSetExecutions(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->SequenceSetExecutions.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->SequenceSetExecutions.GetValue();
        }
    }
}

void BaslerCamera::TimerDelayTimebaseAbs(qint32 dindex,double &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->TimerDelayTimebaseAbs.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->TimerDelayTimebaseAbs.GetValue();
        }
    }
}

void BaslerCamera::TimerDurationTimebaseAbs(qint32 dindex,double &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->TimerDurationTimebaseAbs.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->TimerDurationTimebaseAbs.GetValue();
        }
    }
}

void BaslerCamera::TimerSelector(qint32 dindex,PCam::TimerSelectorEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->TimerSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->TimerSelector.GetValue();
        }
    }
}

void BaslerCamera::TimerDelayAbs(qint32 dindex,double &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->TimerDelayAbs.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->TimerDelayAbs.GetValue();
        }
    }
}

void BaslerCamera::TimerDelayRaw(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->TimerDelayRaw.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->TimerDelayRaw.GetValue();
        }
    }
}

void BaslerCamera::TimerDurationAbs(qint32 dindex,double &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->TimerDurationAbs.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->TimerDurationAbs.GetValue();
        }
    }
}

void BaslerCamera::TimerTriggerSource(qint32 dindex,PCam::TimerTriggerSourceEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->TimerTriggerSource.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->TimerTriggerSource.GetValue();
        }
    }
}

void BaslerCamera::TimerTriggerActivation(qint32 dindex,PCam::TimerTriggerActivationEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->TimerTriggerActivation.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->TimerTriggerActivation.GetValue();
        }
    }
}

void BaslerCamera::CounterSelector(qint32 dindex,PCam::CounterSelectorEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->CounterSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->CounterSelector.GetValue();
        }
    }
}

void BaslerCamera::CounterEventSource(qint32 dindex,PCam::CounterEventSourceEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->CounterEventSource.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->CounterEventSource.GetValue();
        }
    }
}

void BaslerCamera::CounterResetSource(qint32 dindex,PCam::CounterResetSourceEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->CounterResetSource.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->CounterResetSource.GetValue();
        }
    }
}

void BaslerCamera::CounterReset(qint32 dindex)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        ptrCameraDevices.at(dindex)->CounterReset();
    }
}

void BaslerCamera::LUTSelector(qint32 dindex,PCam::LUTSelectorEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->LUTSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->LUTSelector.GetValue();
        }
    }
}

void BaslerCamera::LUTEnable(qint32 dindex,bool &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->LUTEnable.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->LUTEnable.GetValue();
        }
    }
}

void BaslerCamera::LUTIndex(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->LUTIndex.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->LUTIndex.GetValue();
        }
    }
}

void BaslerCamera::LUTValue(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->LUTValue.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->LUTValue.GetValue();
        }
    }
}

void BaslerCamera::UserDefinedValueSelector(qint32 dindex,PCam::UserDefinedValueSelectorEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->UserDefinedValueSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->UserDefinedValueSelector.GetValue();
        }
    }
}

void BaslerCamera::UserDefinedValue(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->UserDefinedValue.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->UserDefinedValue.GetValue();
        }
    }
}

void BaslerCamera::FeatureSet(qint32 dindex,PCam::FeatureSetEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->FeatureSet.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->FeatureSet.GetValue();
        }
    }
}

void BaslerCamera::GetDeviceVendorName(qint32 dindex,PCam::gcstring &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        value = ptrCameraDevices.at(dindex)->DeviceVendorName.GetValue();
    }
}

void BaslerCamera::GetDeviceModelName(qint32 dindex,PCam::gcstring &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        value = ptrCameraDevices.at(dindex)->DeviceModelName.GetValue();
    }
}

void BaslerCamera::GetDeviceManufacturerInfo(qint32 dindex,PCam::gcstring &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        value = ptrCameraDevices.at(dindex)->DeviceManufacturerInfo.GetValue();
    }
}

void BaslerCamera::GetDeviceVersion(qint32 dindex,PCam::gcstring &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        value = ptrCameraDevices.at(dindex)->DeviceVersion.GetValue();
    }
}

void BaslerCamera::GetDeviceFirmwareVersion(qint32 dindex,PCam::gcstring &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        value = ptrCameraDevices.at(dindex)->DeviceFirmwareVersion.GetValue();
    }
}

void BaslerCamera::GetDeviceID(qint32 dindex,PCam::gcstring &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        value = ptrCameraDevices.at(dindex)->DeviceID.GetValue();
    }
}

void BaslerCamera::DeviceUserID(qint32 dindex,PCam::gcstring &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->DeviceUserID.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->DeviceUserID.GetValue();
        }
    }
}

void BaslerCamera::GetSensorWidth(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        value = ptrCameraDevices.at(dindex)->SensorWidth.GetValue();
    }
}

void BaslerCamera::GetSensorHeight(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        value = ptrCameraDevices.at(dindex)->SensorHeight.GetValue();
    }
}

void BaslerCamera::GetWidthMax(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        value = ptrCameraDevices.at(dindex)->WidthMax.GetValue();
    }
}

void BaslerCamera::GetHeightMax(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        value = ptrCameraDevices.at(dindex)->HeightMax.GetValue();
    }
}

void BaslerCamera::GetLastError(qint32 dindex,PCam::LastErrorEnums&value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        value = ptrCameraDevices.at(dindex)->LastError.GetValue();
    }
}

void BaslerCamera::ClearLastError(qint32 dindex)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        ptrCameraDevices.at(dindex)->ClearLastError();
    }
}

void BaslerCamera::ChunkModeActive(qint32 dindex,bool &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ChunkModeActive.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ChunkModeActive.GetValue();
        }
    }
}

void BaslerCamera::ChunkSelector(qint32 dindex,PCam::ChunkSelectorEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ChunkSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ChunkSelector.GetValue();
        }
    }
}

void BaslerCamera::ChunkEnable(qint32 dindex,bool &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ChunkEnable.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ChunkEnable.GetValue();
        }
    }
}

void BaslerCamera::DeviceRegistersStreamingStart(qint32 dindex)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        ptrCameraDevices.at(dindex)->DeviceRegistersStreamingStart();
    }
}

void BaslerCamera::DeviceRegistersStreamingEnd(qint32 dindex)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        ptrCameraDevices.at(dindex)->DeviceRegistersStreamingEnd();
    }
}

void BaslerCamera::EventSelector(qint32 dindex,PCam::EventSelectorEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->EventSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->EventSelector.GetValue();
        }
    }
}

void BaslerCamera::EventNotification(qint32 dindex,PCam::EventNotificationEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->EventNotification.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->EventNotification.GetValue();
        }
    }
}

void BaslerCamera::UserSetSelector(qint32 dindex,PCam::UserSetSelectorEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->UserSetSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->UserSetSelector.GetValue();
        }
    }
}

void BaslerCamera::UserSetLoad(qint32 dindex)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        ptrCameraDevices.at(dindex)->UserSetLoad();
    }
}

void BaslerCamera::UserSetSave(qint32 dindex)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        ptrCameraDevices.at(dindex)->UserSetSave();
    }
}

void BaslerCamera::UserSetDefaultSelector(qint32 dindex,PCam::UserSetDefaultSelectorEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->UserSetDefaultSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->UserSetDefaultSelector.GetValue();
        }
    }
}

void BaslerCamera::PayloadSize(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->PayloadSize.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->PayloadSize.GetValue();
        }
    }
}

void BaslerCamera::GevSCPSPacketSize(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevSCPSPacketSize.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevSCPSPacketSize.GetValue();
        }
    }
}

void BaslerCamera::GevSCPD(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevSCPD.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevSCPD.GetValue();
        }
    }
}

void BaslerCamera::GevSCFTD(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevSCFTD.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevSCFTD.GetValue();
        }
    }
}

void BaslerCamera::GevSCBWR(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevSCBWR.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevSCBWR.GetValue();
        }
    }
}

void BaslerCamera::GevSCBWRA(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevSCBWRA.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevSCBWRA.GetValue();
        }
    }
}

void BaslerCamera::GevSCBWA(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevSCBWA.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevSCBWA.GetValue();
        }
    }
}

void BaslerCamera::GevSCDMT(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevSCDMT.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevSCDMT.GetValue();
        }
    }
}

void BaslerCamera::GevSCDCT(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevSCDCT.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevSCDCT.GetValue();
        }
    }
}

void BaslerCamera::GevSCFJM(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevSCFJM.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevSCFJM.GetValue();
        }
    }
}

void BaslerCamera::GetGevVersionMajor(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevVersionMajor.GetValue();

    }
}

void BaslerCamera::GetGevVersionMinor(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevVersionMinor.GetValue();

    }
}

void BaslerCamera::GetGevDeviceModeIsBigEndian(qint32 dindex,bool &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevDeviceModeIsBigEndian.GetValue();

    }
}

void BaslerCamera::GetGevDeviceModeCharacterSet(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevDeviceModeCharacterSet.GetValue();

    }
}

void BaslerCamera::GevInterfaceSelector(qint32 dindex,PCam::GevInterfaceSelectorEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevInterfaceSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevInterfaceSelector.GetValue();
        }
    }
}

void BaslerCamera::GetGevMACAddress(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevMACAddress.GetValue();

    }
}

void BaslerCamera::GetGevSupportedIPConfigurationLLA(qint32 dindex,bool &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevSupportedIPConfigurationLLA.GetValue();

    }
}

void BaslerCamera::GetGevSupportedIPConfigurationDHCP(qint32 dindex,bool &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevSupportedIPConfigurationDHCP.GetValue();

    }
}

void BaslerCamera::GetGevSupportedIPConfigurationPersistentIP(qint32 dindex,bool &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevSupportedIPConfigurationPersistentIP.GetValue();

    }
}

void BaslerCamera::GevCurrentIPConfiguration(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevCurrentIPConfiguration.GetValue();

    }
}

void BaslerCamera::GetGevCurrentIPAddress(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevCurrentIPAddress.GetValue();

    }
}

void BaslerCamera::GetGevCurrentSubnetMask(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevCurrentSubnetMask.GetValue();

    }
}

void BaslerCamera::GetGevCurrentDefaultGateway(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevCurrentDefaultGateway.GetValue();

    }
}

void BaslerCamera::GevPersistentIPAddress(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevPersistentIPAddress.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevPersistentIPAddress.GetValue();
        }
    }
}

void BaslerCamera::GevPersistentSubnetMask(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevPersistentSubnetMask.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevPersistentSubnetMask.GetValue();
        }
    }
}

void BaslerCamera::GevPersistentDefaultGateway(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevPersistentDefaultGateway.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevPersistentDefaultGateway.GetValue();
        }
    }
}

void BaslerCamera::GetGevLinkSpeed(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevLinkSpeed.GetValue();

    }
}

void BaslerCamera::GetGevLinkMaster(qint32 dindex,bool &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevLinkMaster.GetValue();

    }
}

void BaslerCamera::GetGevLinkFullDuplex(qint32 dindex,bool &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevLinkFullDuplex.GetValue();

    }
}

void BaslerCamera::GetGevLinkCrossover(qint32 dindex,bool &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevLinkCrossover.GetValue();

    }
}

void BaslerCamera::GetGevFirstURL(qint32 dindex,PCam::gcstring &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevFirstURL.GetValue();

    }
}

void BaslerCamera::GetGevSecondURL(qint32 dindex,PCam::gcstring &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevSecondURL.GetValue();

    }
}

void BaslerCamera::GetGevNumberOfInterfaces(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevNumberOfInterfaces.GetValue();

    }
}

void BaslerCamera::GetGevMessageChannelCount(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevMessageChannelCount.GetValue();

    }
}

void BaslerCamera::GetGevSupportedIEEE1588(qint32 dindex,bool &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevSupportedIEEE1588.GetValue();

    }
}

void BaslerCamera::GetGevSupportedOptionalCommandsEVENTDATA(qint32 dindex,bool &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevSupportedOptionalCommandsEVENTDATA.GetValue();

    }
}

void BaslerCamera::GetGevSupportedOptionalCommandsEVENT(qint32 dindex,bool &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevSupportedOptionalCommandsEVENT.GetValue();

    }
}

void BaslerCamera::GetGevSupportedOptionalCommandsPACKETRESEND(qint32 dindex,bool &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevSupportedOptionalCommandsPACKETRESEND.GetValue();

    }
}

void BaslerCamera::GetGevSupportedOptionalCommandsWRITEMEM(qint32 dindex,bool &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevSupportedOptionalCommandsWRITEMEM.GetValue();

    }
}

void BaslerCamera::GevHeartbeatTimeout(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevHeartbeatTimeout.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevHeartbeatTimeout.GetValue();
        }
    }
}

void BaslerCamera::GetGevTimestampTickFrequency(qint32 dindex,int64_t &value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        value = ptrCameraDevices.at(dindex)->GevTimestampTickFrequency.GetValue();

    }
}

void BaslerCamera::GevTimestampControlLatchReset(qint32 dindex)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        ptrCameraDevices.at(dindex)->GevTimestampControlLatchReset();

    }
}

void BaslerCamera::GevCCP(qint32 dindex,PCam::GevCCPEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevCCP.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevCCP.GetValue();
        }
    }
}

void BaslerCamera::GevSCPInterfaceIndex(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevSCPInterfaceIndex.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevSCPInterfaceIndex.GetValue();
        }
    }
}

void BaslerCamera::GevSCPSDoNotFragment(qint32 dindex,bool &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GevSCPSDoNotFragment.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GevSCPSDoNotFragment.GetValue();
        }
    }
}

void BaslerCamera::FileSelector(qint32 dindex,PCam::FileSelectorEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->FileSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->FileSelector.GetValue();
        }
    }
}

void BaslerCamera::FileOperationSelector(qint32 dindex,PCam::FileOperationSelectorEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->FileOperationSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->FileOperationSelector.GetValue();
        }
    }
}

void BaslerCamera::FileOpenMode(qint32 dindex,PCam::FileOpenModeEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->FileOpenMode.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->FileOpenMode.GetValue();
        }
    }
}

void BaslerCamera::GetFileAccessBuffer(qint32 dindex,uint8_t *value)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        ptrCameraDevices.at(dindex)->FileAccessBuffer.Get(value,ptrCameraDevices.at(dindex)->FileAccessBuffer.GetLength());
    }
}

void BaslerCamera::FileAccessOffset(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->FileAccessOffset.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->FileAccessOffset.GetValue();
        }
    }
}

void BaslerCamera::FileAccessLength(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->FileAccessLength.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->FileAccessLength.GetValue();
        }
    }
}

void BaslerCamera::FileOperationStatus(qint32 dindex,PCam::FileOperationStatusEnums &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->FileOperationStatus.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->FileOperationStatus.GetValue();
        }
    }
}

void BaslerCamera::FileOperationResult(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->FileOperationResult.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->FileOperationResult.GetValue();
        }
    }
}

void BaslerCamera::FileSize(qint32 dindex,int64_t &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->FileSize.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->FileSize.GetValue();
        }
    }
}

void BaslerCamera::FileOperationExecute(qint32 dindex)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {

        ptrCameraDevices.at(dindex)->FileOperationExecute();

    }
}

void BaslerCamera::AutoTargetValue(qint32 dindex,int64_t & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AutoTargetValue.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AutoTargetValue.GetValue();
        }
    }
}

void BaslerCamera::GrayValueAdjustmentDampingAbs(qint32 dindex,double & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GrayValueAdjustmentDampingAbs.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GrayValueAdjustmentDampingAbs.GetValue();
        }
    }
}

void BaslerCamera::GrayValueAdjustmentDampingRaw(qint32 dindex,int64_t & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GrayValueAdjustmentDampingRaw.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GrayValueAdjustmentDampingRaw.GetValue();
        }
    }
}

void BaslerCamera::BalanceWhiteAdjustmentDampingAbs(qint32 dindex,double & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->BalanceWhiteAdjustmentDampingAbs.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->BalanceWhiteAdjustmentDampingAbs.GetValue();
        }
    }
}

void BaslerCamera::BalanceWhiteAdjustmentDampingRaw(qint32 dindex,int64_t & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->BalanceWhiteAdjustmentDampingRaw.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->BalanceWhiteAdjustmentDampingRaw.GetValue();
        }
    }
}

void BaslerCamera::AutoGainRawLowerLimit(qint32 dindex,int64_t & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AutoGainRawLowerLimit.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AutoGainRawLowerLimit.GetValue();
        }
    }
}

void BaslerCamera::AutoGainRawUpperLimit(qint32 dindex,int64_t & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AutoGainRawUpperLimit.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AutoGainRawUpperLimit.GetValue();
        }
    }
}

void BaslerCamera::AutoExposureTimeAbsLowerLimit(qint32 dindex,double & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AutoExposureTimeAbsLowerLimit.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AutoExposureTimeAbsLowerLimit.GetValue();
        }
    }
}

void BaslerCamera::AutoExposureTimeAbsUpperLimit(qint32 dindex,double & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AutoExposureTimeAbsUpperLimit.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AutoExposureTimeAbsUpperLimit.GetValue();
        }
    }
}

void BaslerCamera::AutoFunctionProfile(qint32 dindex,PCam::AutoFunctionProfileEnums& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AutoFunctionProfile.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AutoFunctionProfile.GetValue();
        }
    }
}

void BaslerCamera::AutoFunctionAOISelector(qint32 dindex,PCam::AutoFunctionAOISelectorEnums & value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AutoFunctionAOISelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AutoFunctionAOISelector.GetValue();
        }
    }
}

void BaslerCamera::AutoFunctionAOIWidth(qint32 dindex,int64_t & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AutoFunctionAOIWidth.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AutoFunctionAOIWidth.GetValue();
        }
    }
}

void BaslerCamera::AutoFunctionAOIHeight(qint32 dindex,int64_t & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AutoFunctionAOIHeight.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AutoFunctionAOIHeight.GetValue();
        }
    }
}

void BaslerCamera::AutoFunctionAOIOffsetX(qint32 dindex,int64_t & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AutoFunctionAOIOffsetX.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AutoFunctionAOIOffsetX.GetValue();
        }
    }
}

void BaslerCamera::AutoFunctionAOIOffsetY(qint32 dindex,int64_t & value ,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AutoFunctionAOIOffsetY.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AutoFunctionAOIOffsetY.GetValue();
        }
    }
}

void BaslerCamera::AutoFunctionAOIUsageIntensity(qint32 dindex,bool &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AutoFunctionAOIUsageIntensity.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AutoFunctionAOIUsageIntensity.GetValue();
        }
    }
}

void BaslerCamera::AutoFunctionAOIUsageWhiteBalance(qint32 dindex,bool &value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->AutoFunctionAOIUsageWhiteBalance.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->AutoFunctionAOIUsageWhiteBalance.GetValue();
        }
    }
}

void BaslerCamera::MedianFilter(qint32 dindex,bool& value,bool mode)
{

}

void BaslerCamera::ParameterSelector(qint32 dindex,PCam::ParameterSelectorEnums& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ParameterSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ParameterSelector.GetValue();
        }
    }
}

void BaslerCamera::TypeofStreamGrabber(qint32 dindex,PCam::TypeEnums& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Type.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Type.GetValue();
        }
    }
}

void BaslerCamera::RemoveLimits(qint32 dindex,bool& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->RemoveLimits.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->RemoveLimits.GetValue();
        }
    }
}



void BaslerCamera::ExpertFeatureAccessSelector(qint32 dindex,PCam::ExpertFeatureAccessSelectorEnums& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ExpertFeatureAccessSelector.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ExpertFeatureAccessSelector.GetValue();
        }
    }
}

void BaslerCamera::ExpertFeatureAccessKey(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ExpertFeatureAccessKey.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ExpertFeatureAccessKey.GetValue();
        }
    }
}

void BaslerCamera::ExpertFeatureEnable(qint32 dindex,bool& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->ExpertFeatureEnable.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->ExpertFeatureEnable.GetValue();
        }
    }
}


void BaslerCamera::ReadTimeout(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetTLParams().ReadTimeout.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetTLParams().ReadTimeout.GetValue();
        }
    }
}

void BaslerCamera::WriteTimeout(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetTLParams().WriteTimeout.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetTLParams().WriteTimeout.GetValue();
        }
    }
}

void BaslerCamera::MaxRetryCountRead(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetTLParams().MaxRetryCountRead.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetTLParams().MaxRetryCountRead.GetValue();
        }
    }
}

void BaslerCamera::MaxRetryCountWrite(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetTLParams().MaxRetryCountWrite.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetTLParams().MaxRetryCountWrite.GetValue();
        }
    }
}

void BaslerCamera::HeartbeatTimeout(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetTLParams().HeartbeatTimeout.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetTLParams().HeartbeatTimeout.GetValue();
        }
    }
}

void BaslerCamera::CommandDuplicationEnable(qint32 dindex,bool& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetTLParams().CommandDuplicationEnable.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetTLParams().CommandDuplicationEnable.GetValue();
        }
    }
}

void BaslerCamera::Type(qint32 dindex,PCam::TypeEnums& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Type.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Type.GetValue();
        }
    }
}

void BaslerCamera::AccessMode(qint32 dindex,PCam::AccessModeEnums& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().AccessMode.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().AccessMode.GetValue();
        }
    }
}

void BaslerCamera::TypeIsWindowsIntelPerformanceDriverAvailable(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().TypeIsWindowsIntelPerformanceDriverAvailable.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().TypeIsWindowsIntelPerformanceDriverAvailable.GetValue();
        }
    }
}


void BaslerCamera::TypeIsSocketDriverAvailable(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().TypeIsSocketDriverAvailable.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().TypeIsSocketDriverAvailable.GetValue();
        }
    }
}

void BaslerCamera::Statistic_Total_Buffer_Count(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Statistic_Total_Buffer_Count.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Statistic_Total_Buffer_Count.GetValue();
        }
    }
}

void BaslerCamera::Statistic_Failed_Buffer_Count(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Statistic_Failed_Buffer_Count.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Statistic_Failed_Buffer_Count.GetValue();
        }
    }
}

void BaslerCamera::Statistic_Buffer_Underrun_Count(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Statistic_Buffer_Underrun_Count.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Statistic_Buffer_Underrun_Count.GetValue();
        }
    }
}

void BaslerCamera::Statistic_Total_Packet_Count(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Statistic_Total_Packet_Count.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Statistic_Total_Packet_Count.GetValue();
        }
    }
}

void BaslerCamera::Statistic_Failed_Packet_Count(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Statistic_Failed_Packet_Count.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Statistic_Failed_Packet_Count.GetValue();
        }
    }
}

void BaslerCamera::Statistic_Resend_Request_Count(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Statistic_Resend_Request_Count.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Statistic_Resend_Request_Count.GetValue();
        }
    }
}

void BaslerCamera::Statistic_Resend_Packet_Count(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Statistic_Resend_Packet_Count.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().Statistic_Resend_Packet_Count.GetValue();
        }
    }
}

void BaslerCamera::TransmissionType(qint32 dindex,PCam::TransmissionTypeEnums& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().TransmissionType.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().TransmissionType.GetValue();
        }
    }
}

void BaslerCamera::DestinationAddr(qint32 dindex,PCam::gcstring& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().DestinationAddr.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().DestinationAddr.GetValue();
        }
    }
}

void BaslerCamera::DestinationPort(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().DestinationPort.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().DestinationPort.GetValue();
        }
    }
}

void BaslerCamera::MaxNumBuffer(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().MaxNumBuffer.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().MaxNumBuffer.GetValue();
        }
    }
}

void BaslerCamera::MaxBufferSize(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().MaxBufferSize.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().MaxBufferSize.GetValue();
        }
    }
}

void BaslerCamera::EnableResend(qint32 dindex,bool& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().EnableResend.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().EnableResend.GetValue();
        }
    }
}

void BaslerCamera::PacketTimeout(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().PacketTimeout.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().PacketTimeout.GetValue();
        }
    }
}

void BaslerCamera::AutoPacketSize(qint32 dindex,bool& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().AutoPacketSize.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().AutoPacketSize.GetValue();
        }
    }
}

void BaslerCamera::FrameRetention(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().FrameRetention.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().FrameRetention.GetValue();
        }
    }
}

void BaslerCamera::ReceiveThreadPriorityOverride(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().ReceiveThreadPriorityOverride.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().ReceiveThreadPriorityOverride.GetValue();
        }
    }
}

void BaslerCamera::ReceiveThreadPriority(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().ReceiveThreadPriority.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().ReceiveThreadPriority.GetValue();
        }
    }
}

void BaslerCamera::SocketBufferSize(qint32 dindex,int64_t& value,bool mode)
{
    if (ptrCameraDevices.at(dindex)!=nullptr)
    {
        if (mode)
        {
            ptrCameraDevices.at(dindex)->GetStreamGrabberParams().SocketBufferSize.SetValue(value);
        }
        else
        {
            value = ptrCameraDevices.at(dindex)->GetStreamGrabberParams().SocketBufferSize.GetValue();
        }
    }
}






void BaslerCamera::OutputBitAlignment(qint32 dindex,PCam::OutputBitAlignmentEnums& value,bool mode)
{
    PCam::CImageFormatConverter converter;
    if (mode)
    {
        converter.OutputBitAlignment.SetValue(value);
    }
    else
    {
        value = converter.OutputBitAlignment.GetValue();
    }
}

void BaslerCamera::OutputOrientation(qint32 dindex,PCam::OutputOrientationEnums& value,bool mode)
{
    PCam::CImageFormatConverter converter;
    if (mode)
    {
        converter.OutputOrientation.SetValue(value);
    }
    else
    {
        value = converter.OutputOrientation.GetValue();
    }
}

void BaslerCamera::MonoConversionMethod(qint32 dindex,PCam::MonoConversionMethodEnums& value,bool mode)
{
    PCam::CImageFormatConverter converter;
    if (mode)
    {
        converter.MonoConversionMethod.SetValue(value);
    }
    else
    {
        value = converter.MonoConversionMethod.GetValue();
    }
}

void BaslerCamera::Gamma1(qint32 dindex,double & value ,bool mode)
{
    PCam::CImageFormatConverter converter;
    if (mode)
    {
        converter.Gamma.SetValue(value);
    }
    else
    {
        value = converter.Gamma.GetValue();
    }
}

void BaslerCamera::AdditionalLeftShift(qint32 dindex,int64_t & value ,bool mode)
{
    PCam::CImageFormatConverter converter;
    if (mode)
    {
        converter.AdditionalLeftShift.SetValue(value);
    }
    else
    {
        value = converter.AdditionalLeftShift.GetValue();
    }
}
