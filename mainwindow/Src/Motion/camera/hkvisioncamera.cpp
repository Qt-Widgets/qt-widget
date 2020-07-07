//#include "hkvisioncamera.h"

//HKVisionCamera::HKVisionCamera(qint32 index):HCamera(index)
//{
//    m_hDevHandle        = NULL;
//    m_nTLayerType       = MV_UNKNOW_DEVICE;
//}

//HKVisionCamera::~HKVisionCamera()
//{
//    if (m_hDevHandle)
//    {
//        MV_CC_DestroyHandle(m_hDevHandle);
//        m_hDevHandle    = NULL;
//    }
//}


//qint32 HKVisionCamera::InitDevice()
//{
//    return 0;
//}

//void HKVisionCamera::UnInitDevice()
//{
//    return;
//}

//qint32 HKVisionCamera::OpenCamera()
//{
//    return 0;
//}

//qint32 HKVisionCamera::CloseCamera()
//{
//    return 0;
//}

//QString HKVisionCamera::GetDeviceName()
//{
//    return "default-"+QString::number(m_DeviceIndex);
//}

//QStringList HKVisionCamera::GetAllDeviceNames()
//{
//    return  QStringList(QString("default"));
//}

//qint32 HKVisionCamera::SetConfigFile( const QString &configFile)
//{
//    Q_UNUSED(configFile)
//    return 0;
//}


//qint32 HKVisionCamera::SetDeviceConfig( const CameraConfiguration &data)
//{
//    Q_UNUSED(data)
//    m_DevcieConfig=data;

//    return 0;
//}

//qint32 HKVisionCamera::GetImgWidthHeight( qint32 &width, qint32 &height)
//{
//    width=768;
//    height=768;
//    return 0;
//}

//qint32 HKVisionCamera::SetImgCaptureMode( qint32 mode)
//{
//    Q_UNUSED(mode)
//    return 0;
//}

//uchar *HKVisionCamera::GetImgBufferOnce()
//{
//  return 0;
//}

//uchar *HKVisionCamera::GetImgBufferContinues()
//{
//return 0;



//}

//HColoredImgBuffer HKVisionCamera::GetColoredImgBufferContinues()
//{

//return HColoredImgBuffer();

//}

//qint32 HKVisionCamera::GetGain(qint32 &value)
//{
//    Q_UNUSED(value)
//    return 0;
//}

//qint32 HKVisionCamera::GetExposure(qint32 &value)
//{
//    Q_UNUSED(value)
//    return 0;
//}

//qint32 HKVisionCamera::SetGain( qint32 value)
//{
//    Q_UNUSED(value)
//    return 0;
//}

//qint32 HKVisionCamera::SetExposure( qint32 value)
//{
//    Q_UNUSED(value)
//    return 0;
//}

//double HKVisionCamera::GetFPS()
//{
//    return 60;
//}






//int HKVisionCamera::EnumDevices(MV_CC_DEVICE_INFO_LIST* pstDevList)
//{
//    int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, pstDevList);
//    if (MV_OK != nRet)
//    {
//        return nRet;
//    }

//    return MV_OK;
//}

//// ch:打开设备 | en:Open Device
//int     HKVisionCamera::Open(MV_CC_DEVICE_INFO* pstDeviceInfo)
//{
//    if (NULL == pstDeviceInfo)
//    {
//        return MV_E_PARAMETER;
//    }

//    int nRet = MV_OK;
//    if(m_hDevHandle == NULL)
//    {
//        nRet  = MV_CC_CreateHandle(&m_hDevHandle, pstDeviceInfo);
//        if (MV_OK != nRet)
//        {
//            return nRet;
//        }
//    }

//    nRet = MV_CC_OpenDevice(m_hDevHandle);
//    if (MV_OK != nRet)
//    {
//        MV_CC_DestroyHandle(m_hDevHandle);
//        m_hDevHandle = NULL;

//        return nRet;
//    }

//    return MV_OK;
//}


//// ch:关闭设备 | en:Close Device
//int HKVisionCamera::Close()
//{
//    int nRet = MV_OK;

//    if (NULL == m_hDevHandle)
//    {
//        return MV_E_PARAMETER;
//    }

//    MV_CC_CloseDevice(m_hDevHandle);
//    nRet = MV_CC_DestroyHandle(m_hDevHandle);
//    m_hDevHandle = NULL;

//    return nRet;
//}


//// ch:开启抓图 | en:Start Grabbing
//int HKVisionCamera::StartGrabbing()
//{
//    return MV_CC_StartGrabbing(m_hDevHandle);
//}


//// ch:停止抓图 | en:Stop Grabbing
//int HKVisionCamera::StopGrabbing()
//{
//    return MV_CC_StopGrabbing(m_hDevHandle);
//}

//int HKVisionCamera::GetOneFrameTimeout(unsigned char* pData, unsigned int* pnDataLen, unsigned int nDataSize, MV_FRAME_OUT_INFO_EX* pFrameInfo, int nMsec)
//{
//    if (NULL == pnDataLen)
//    {
//        return MV_E_PARAMETER;
//    }

//    int nRet = MV_OK;

//    *pnDataLen  = 0;

//    nRet = MV_CC_GetOneFrameTimeout(m_hDevHandle, pData, nDataSize, pFrameInfo, nMsec);
//    if (MV_OK != nRet)
//    {
//        return nRet;
//    }

//    *pnDataLen = pFrameInfo->nFrameLen;

//    return nRet;
//}


//// ch:设置显示窗口句柄 | en:Set Display Window Handle
//int HKVisionCamera::Display(void* hWnd)
//{
//    return MV_CC_Display(m_hDevHandle, hWnd);
//}


//int HKVisionCamera::SaveImage(MV_SAVE_IMAGE_PARAM_EX* pstParam)
//{
//    if (NULL == pstParam)
//    {
//        return MV_E_PARAMETER;
//    }

//    return MV_CC_SaveImageEx2(m_hDevHandle, pstParam);
//}

//// ch:注册图像数据回调 | en:Register Image Data CallBack
//int HKVisionCamera::RegisterImageCallBack(void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo,
//                                                                    void* pUser),void* pUser)
//{
//    return MV_CC_RegisterImageCallBackEx(m_hDevHandle, cbOutput, pUser);
//}


//// ch:注册消息异常回调 | en:Register Message Exception CallBack
//int HKVisionCamera::RegisterExceptionCallBack(void(__stdcall* cbException)(unsigned int nMsgType, void* pUser),void* pUser)
//{
//    return MV_CC_RegisterExceptionCallBack(m_hDevHandle, cbException, pUser);
//}


//// ch:获取Int型参数，如 Width和Height，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
//// en:Get Int type parameters, such as Width and Height, for details please refer to MvCameraNode.xlsx file under SDK installation directory
//int HKVisionCamera::GetIntValue(IN const char* strKey, OUT unsigned int *pnValue)
//{
//    if (NULL == strKey || NULL == pnValue)
//    {
//        return MV_E_PARAMETER;
//    }

//    MVCC_INTVALUE stParam;
//    memset(&stParam, 0, sizeof(MVCC_INTVALUE));
//    int nRet = MV_CC_GetIntValue(m_hDevHandle, strKey, &stParam);
//    if (MV_OK != nRet)
//    {
//        return nRet;
//    }

//    *pnValue = stParam.nCurValue;

//    return MV_OK;
//}


//// ch:设置Int型参数，如 Width和Height，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
//// en:Set Int type parameters, such as Width and Height, for details please refer to MvCameraNode.xlsx file under SDK installation directory
//int HKVisionCamera::SetIntValue(IN const char* strKey, IN unsigned int nValue)
//{
//    if (NULL == strKey)
//    {
//        return MV_E_PARAMETER;
//    }

//    return MV_CC_SetIntValue(m_hDevHandle, strKey, nValue);
//}


//// ch:获取Float型参数，如 ExposureTime和Gain，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
//// en:Get Float type parameters, such as ExposureTime and Gain, for details please refer to MvCameraNode.xlsx file under SDK installation directory
//int HKVisionCamera::GetFloatValue(IN const char* strKey, OUT float *pfValue)
//{
//    if (NULL == strKey || NULL == pfValue)
//    {
//        return MV_E_PARAMETER;
//    }

//    MVCC_FLOATVALUE stParam;
//    memset(&stParam, 0, sizeof(MVCC_FLOATVALUE));
//    int nRet = MV_CC_GetFloatValue(m_hDevHandle, strKey, &stParam);
//    if (MV_OK != nRet)
//    {
//        return nRet;
//    }

//    *pfValue = stParam.fCurValue;

//    return MV_OK;
//}


//// ch:设置Float型参数，如 ExposureTime和Gain，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
//// en:Set Float type parameters, such as ExposureTime and Gain, for details please refer to MvCameraNode.xlsx file under SDK installation directory
//int HKVisionCamera::SetFloatValue(IN const char* strKey, IN float fValue)
//{
//    if (NULL == strKey)
//    {
//        return MV_E_PARAMETER;
//    }

//    return MV_CC_SetFloatValue(m_hDevHandle, strKey, fValue);
//}


//// ch:获取Enum型参数，如 PixelFormat，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
//// en:Get Enum type parameters, such as PixelFormat, for details please refer to MvCameraNode.xlsx file under SDK installation directory
//int HKVisionCamera::GetEnumValue(IN const char* strKey, OUT unsigned int *pnValue)
//{
//    if (NULL == strKey || NULL == pnValue)
//    {
//        return MV_E_PARAMETER;
//    }

//    MVCC_ENUMVALUE stParam;
//    memset(&stParam, 0, sizeof(MVCC_ENUMVALUE));
//    int nRet = MV_CC_GetEnumValue(m_hDevHandle, strKey, &stParam);
//    if (MV_OK != nRet)
//    {
//        return nRet;
//    }

//    *pnValue = stParam.nCurValue;

//    return MV_OK;
//}


//// ch:设置Enum型参数，如 PixelFormat，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
//// en:Set Enum type parameters, such as PixelFormat, for details please refer to MvCameraNode.xlsx file under SDK installation directory
//int HKVisionCamera::SetEnumValue(IN const char* strKey, IN unsigned int nValue)
//{
//    if (NULL == strKey)
//    {
//        return MV_E_PARAMETER;
//    }

//    return MV_CC_SetEnumValue(m_hDevHandle, strKey, nValue);
//}


//// ch:获取Bool型参数，如 ReverseX，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
//// en:Get Bool type parameters, such as ReverseX, for details please refer to MvCameraNode.xlsx file under SDK installation directory
//int HKVisionCamera::GetBoolValue(IN const char* strKey, OUT bool *pbValue)
//{
//    if (NULL == strKey || NULL == pbValue)
//    {
//        return MV_E_PARAMETER;
//    }

//    return MV_CC_GetBoolValue(m_hDevHandle, strKey, pbValue);
//}


//// ch:设置Bool型参数，如 ReverseX，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
//// en:Set Bool type parameters, such as ReverseX, for details please refer to MvCameraNode.xlsx file under SDK installation directory
//int HKVisionCamera::SetBoolValue(IN const char* strKey, IN bool bValue)
//{
//    if (NULL == strKey)
//    {
//        return MV_E_PARAMETER;
//    }

//    return MV_CC_SetBoolValue(m_hDevHandle, strKey, bValue);
//}


//// ch:获取String型参数，如 DeviceUserID，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件UserSetSave
//// en:Get String type parameters, such as DeviceUserID, for details please refer to MvCameraNode.xlsx file under SDK installation directory
//int HKVisionCamera::GetStringValue(IN const char* strKey, IN OUT char* strValue, IN unsigned int nSize)
//{
//    if (NULL == strKey || NULL == strValue)
//    {
//        return MV_E_PARAMETER;
//    }

//    MVCC_STRINGVALUE stParam;
//    memset(&stParam, 0, sizeof(MVCC_STRINGVALUE));
//    int nRet = MV_CC_GetStringValue(m_hDevHandle, strKey, &stParam);
//    if (MV_OK != nRet)
//    {
//        return nRet;
//    }

//    strcpy_s(strValue, nSize, stParam.chCurValue);

//    return MV_OK;
//}


//// ch:设置String型参数，如 DeviceUserID，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件UserSetSave
//// en:Set String type parameters, such as DeviceUserID, for details please refer to MvCameraNode.xlsx file under SDK installation directory
//int HKVisionCamera::SetStringValue(IN const char* strKey, IN const char* strValue)
//{
//    if (NULL == strKey)
//    {
//        return MV_E_PARAMETER;
//    }

//    return MV_CC_SetStringValue(m_hDevHandle, strKey, strValue);
//}


//// ch:执行一次Command型命令，如 UserSetSave，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
//// en:Execute Command once, such as UserSetSave, for details please refer to MvCameraNode.xlsx file under SDK installation directory
//int HKVisionCamera::CommandExecute(IN const char* strKey)
//{
//    if (NULL == strKey)
//    {
//        return MV_E_PARAMETER;
//    }

//    return MV_CC_SetCommandValue(m_hDevHandle, strKey);
//}

//int HKVisionCamera::GetOptimalPacketSize()
//{
//    return MV_CC_GetOptimalPacketSize(m_hDevHandle);
//}

//int HKVisionCamera::GetAllMatchInfo(IN void* hDevHandle, IN unsigned int nTLayerType, OUT unsigned int *nLostFrame, OUT unsigned int *nFrameCount )
//{
//    int nRet = MV_OK;
//    m_hDevHandle = hDevHandle;
//    if (MV_GIGE_DEVICE == nTLayerType)
//    {
//        MV_ALL_MATCH_INFO struMatchInfo = {0};
//        MV_MATCH_INFO_NET_DETECT stMatchInfoNetDetect;
//        struMatchInfo.pInfo = &stMatchInfoNetDetect;

//        struMatchInfo.nType = MV_MATCH_TYPE_NET_DETECT; // ch:网络流量和丢包信息 | en:Net flow and lsot packet information
//        memset(struMatchInfo.pInfo, 0, sizeof(MV_MATCH_INFO_NET_DETECT));
//        struMatchInfo.nInfoSize = sizeof(MV_MATCH_INFO_NET_DETECT);

//        nRet = MV_CC_GetAllMatchInfo(m_hDevHandle, &struMatchInfo);
//        if (MV_OK != nRet)
//        {
//            return nRet;
//        }

//        MV_MATCH_INFO_NET_DETECT *pInfo = (MV_MATCH_INFO_NET_DETECT*)struMatchInfo.pInfo;
//        *nFrameCount = pInfo->nNetRecvFrameCount;
//        *nLostFrame = stMatchInfoNetDetect.nLostFrameCount;
//    }
//    else if (MV_USB_DEVICE == nTLayerType)
//    {
//        MV_ALL_MATCH_INFO struMatchInfo = {0};
//        MV_MATCH_INFO_USB_DETECT stMatchInfoNetDetect;
//        struMatchInfo.pInfo = &stMatchInfoNetDetect;

//        struMatchInfo.nType = MV_MATCH_TYPE_USB_DETECT; // ch:网络流量和丢包信息 | en:Net flow and lsot packet information
//        memset(struMatchInfo.pInfo, 0, sizeof(MV_MATCH_INFO_USB_DETECT));
//        struMatchInfo.nInfoSize = sizeof(MV_MATCH_INFO_USB_DETECT);

//        nRet = MV_CC_GetAllMatchInfo(m_hDevHandle, &struMatchInfo);
//        if (MV_OK != nRet)
//        {
//            return nRet;
//        }

//        MV_MATCH_INFO_USB_DETECT *pInfo = (MV_MATCH_INFO_USB_DETECT*)struMatchInfo.pInfo;
//        *nFrameCount = pInfo->nRevicedFrameCount;
//        *nLostFrame = pInfo->nErrorFrameCount;
//    }

//    return MV_OK;
//}
