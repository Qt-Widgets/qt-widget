#include "havtcamera.h"
#include "FGCamera.h"


#include <Windows.h>

#include <QMessageBox>

#define MAX_AVT_CNT (20)

CFGCamera* GetInstance(void* p)
{
    return (CFGCamera*)p;
}

HAVTCamera::HAVTCamera(qint32 index):HCamera(index)
{
    m_pImage = NULL;
    m_bLive = false;

    m_bRxFlag = false;
    m_nAOIWidth = 1292;
    m_nAOIHeight = 962;


    m_pBitmapInfo = NULL;

    m_pCamera = NULL;
}

HAVTCamera::~HAVTCamera()
{
    UnInitDevice();
}



qint32 HAVTCamera::InitDevice()
{

    if(NULL == m_pCamera)
    {
        m_pCamera = new CFGCamera;
    }

    UINT i = 0;
    QString str;
    UINT32 Result = FCE_NOERROR;
    UINT32 NodeCnt;
    FGNODEINFO NodeInfo[MAX_AVT_CNT];

    //(-1) 设置回调函数
    FGINIT Arg;
    memset(&Arg, 0, sizeof(FGINIT));
    Arg.pCallback = SnapCallback;
    Arg.Context = NULL;
    FGInitModule(&Arg);

    // (1) 获取摄像机列表
    Result = FGGetNodeList(NodeInfo,MAX_AVT_CNT,&NodeCnt);
    if(Result != FCE_NOERROR || 0 == NodeCnt)
    {

        m_strError=QStringLiteral("No camera!");

        return false;
    }


    if(i == NodeCnt)
    {

        m_strError=QStringLiteral("Fail to find camera,SN = ");

        return false;
    }
    Result= GetInstance(m_pCamera)->Connect( &NodeInfo[i].Guid, (void*)this );
    Sleep(100);
    if(Result != FCE_NOERROR)
    {

        m_strError="Fail to connect camera! SN = ";

        return false;
    }

    // (3)检测摄像机的型号是不是AVT彩色摄像机
    //char* deviceName = new char[100];
    //GetInstance(m_pCamera)->GetDeviceName(deviceName, 100);
    //CString strDevName;
    //strDevName.Format(_T("%s"),deviceName);
    //
    //if(strDevName.Find(_T("AVT")) == 0)
    //{
    //	if(strDevName.Find(_T("C")) == strDevName.GetLength() - 1)
    //	{
    //		m_IsBayerImage = true;
    //	}
    //	else
    //	{
    //		m_IsBayerImage = false;
    //	}
    //}
    //memset(deviceName, 0 , 100);
    //delete[] deviceName;

    m_IsBayerImage = false;

    //(4) 设置摄像机输出图像格式与类型
    if(m_IsBayerImage)
    {
        Result = GetInstance(m_pCamera)->SetParameter(FGP_IMAGEFORMAT, MAKEIMAGEFORMAT(RES_SCALABLE, CM_RAW8, 0));
    }
    else
    {
        Result = GetInstance(m_pCamera)->SetParameter(FGP_IMAGEFORMAT, MAKEIMAGEFORMAT(RES_SCALABLE, CM_Y8, 0));
    }
    if(Result != FCE_NOERROR)
    {

        m_strError = "Bad image format!";

        return false;
    }

    //(5) 提取当前摄像机分辨率信息



    //GetInstance(m_pCamera)->SetParameter(FGP_PHYSPEED,800);
    //GetInstance(m_pCamera)->SetParameter(FGP_TEMPERATURE,8);
    GetInstance(m_pCamera)->GetParameter(FGP_XSIZE, &m_nAOIWidth);
    GetInstance(m_pCamera)->GetParameter(FGP_YSIZE, &m_nAOIHeight);
    GetInstance(m_pCamera)->GetParameter(FGP_XPOSITION, &m_nAOIStartX);
    GetInstance(m_pCamera)->GetParameter(FGP_YPOSITION, &m_nAOIStartY);

    //unsigned long lTemp;
    //GetInstance(m_pCamera)->GetParameter(FGP_FRAMERATE,&lTemp);
    //GetInstance(m_pCamera)->GetParameter(FGP_PHYSPEED,&lTemp);
    //GetInstance(m_pCamera)->GetParameter(FGP_TEMPERATURE,&lTemp);

    //(6) 为采集分配DMA通道
    GetInstance(m_pCamera)->SetParameter(FGP_USEIRMFORCHN, 0);
    GetInstance(m_pCamera)->SetParameter(FGP_ISOCHANNEL, i);



    //(7) 分配图像缓冲区
    AllocBitmap();

    //(8) 设置包长
    GetInstance(m_pCamera)->SetParameter(FGP_PACKETSIZE, 4096);
    GetInstance(m_pCamera)->SetParameter(FGP_DMAMODE, DMA_REPLACE);

    //(9) 设置曝光增益


    GetInstance(m_pCamera)->WriteRegister(0xF0F00830,0x82E00000);


    isInit=true;
    OpenCamera();
    return true;
}

void HAVTCamera::UnInitDevice()
{
    isInit=false;
    CloseCamera();

    FGExitModule();

    if(NULL != m_pBitmapInfo)
    {
        delete []m_pBitmapInfo;
        m_pBitmapInfo = NULL;
    }
    if(NULL != m_pImage)
    {
        delete []m_pImage;
        m_pImage = NULL;
    }

    if(NULL != m_pCamera)
    {
        delete m_pCamera;
        m_pCamera = NULL;
    }
}

qint32 HAVTCamera::OpenCamera()
{

    UINT32 Result = GetInstance(m_pCamera)->OpenCapture();
    if(Result != FCE_NOERROR)
    {
        m_strError ="OpenCapture Error";
        GetInstance(m_pCamera)->Disconnect();
        return false;
    }

    Result = GetInstance(m_pCamera)->StartDevice();
    if(Result != FCE_NOERROR )
    {
        m_strError = "StartDevice Error";
        GetInstance(m_pCamera)->CloseCapture();
        GetInstance(m_pCamera)->Disconnect();
        return false;
    }
    m_bLive = true;
    return true;
}

qint32 HAVTCamera::CloseCamera()
{
    GetInstance(m_pCamera)->CloseCapture();
    GetInstance(m_pCamera)->Disconnect();
    m_bLive = false;
    return true;
}

QString HAVTCamera::GetDeviceName()
{
    return "avt";
}

qint32 HAVTCamera::GetImgWidthHeight(qint32 &width, qint32 &height)
{
    width = m_nAOIWidth;
    height = m_nAOIHeight;
    return 0;
}

uchar *HAVTCamera::GetImgBufferOnce()
{
    SetRxFlag(false);
    GetInstance(m_pCamera)->WriteRegister(0xF0F0062C,0x80000000);
    Wait();
    return m_pImage;
}

qint32 HAVTCamera::GetGain(qint32 &value)
{
    ulong mk=0;
    GetInstance(m_pCamera)->GetParameter(FGP_GAIN, &mk);
    value=mk;
    return 0;
}

qint32 HAVTCamera::GetExposure(qint32 &value)
{
    ulong mk=0;
    GetInstance(m_pCamera)->GetParameter(FGP_SHUTTER, &mk);
    value=mk;
    return 0;

}

qint32 HAVTCamera::SetGain(qint32 value)
{
    GetInstance(m_pCamera)->SetParameter(FGP_GAIN, (unsigned long)value);
    return true;
}

qint32 HAVTCamera::SetExposure(qint32 value)
{

    GetInstance(m_pCamera)->SetParameter(FGP_SHUTTER, (unsigned long)value);
    return true;
}



void HAVTCamera::AllocBitmap()
{

    UINT i = 0;
    UINT32 Width, Height;

    if(NULL != m_pImage)
    {
        delete []m_pImage;
    }

    Width		= m_nAOIWidth;
    Height		= m_nAOIHeight;
    m_pImage	= new UINT8[Width * Height * 3];
    if(m_IsBayerImage)
    {
        m_pBitmapInfo=(BITMAPINFO*)(new char[sizeof(BITMAPINFOHEADER)]);
        m_pBitmapInfo->bmiHeader.biBitCount = (UINT16)(24);
    }
    else
    {
        m_pBitmapInfo=(BITMAPINFO*)(new char[sizeof(BITMAPINFOHEADER)+256*4]);
        m_pBitmapInfo->bmiHeader.biBitCount = (UINT16)(8);

        for(i = 0; i < 256; i++)
        {
            m_pBitmapInfo->bmiColors[i].rgbRed		= (UINT8)i;
            m_pBitmapInfo->bmiColors[i].rgbGreen	= (UINT8)i;
            m_pBitmapInfo->bmiColors[i].rgbBlue		= (UINT8)i;
            m_pBitmapInfo->bmiColors[i].rgbReserved	= 0;
        }
    }

    m_pBitmapInfo->bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
    m_pBitmapInfo->bmiHeader.biWidth			= Width;
    m_pBitmapInfo->bmiHeader.biHeight			= Height;
    m_pBitmapInfo->bmiHeader.biPlanes			= 1;

    m_pBitmapInfo->bmiHeader.biCompression		= BI_RGB;
    m_pBitmapInfo->bmiHeader.biSizeImage		= Width * Height * 3;
    m_pBitmapInfo->bmiHeader.biXPelsPerMeter	= 0;
    m_pBitmapInfo->bmiHeader.biYPelsPerMeter	= 0;
    m_pBitmapInfo->bmiHeader.biClrUsed			= 0;
    m_pBitmapInfo->bmiHeader.biClrImportant		= 0;
}


void HAVTCamera::SetRxFlag(bool bRx)
{
    QMutexLocker locker(&mutex1);
    m_bRxFlag = bRx;
}

bool HAVTCamera::GetRxFlag()
{
    QMutexLocker locker(&mutex1);
    return m_bRxFlag;
}

bool HAVTCamera::Wait()
{
    do
    {
        QEventLoop loop;
        QTimer::singleShot(100, &loop, SLOT(quit()));
        loop.exec();
    }
    while(!GetRxFlag());
    return true;
}



void WINAPI HAVTCamera::SnapCallback(void *Context, UINT32 wParam, void *lParam)
{

    //static DWORD dwStrt = 0 ;
    FGFRAME frame;
    UINT32 Result = 0;
    UINT32 FramesToDisplay;

    //dwStrt = GetTickCount() - dwStrt;
    //dwStrt = GetTickCount();
    HAVTCamera* pAVT = (HAVTCamera*)lParam;

    switch(wParam)
    {
    case WPARAM_FRAMESREADY:
        Result  = GetInstance(pAVT->m_pCamera)->GetParameter(FGP_FRAMEBUFFERCOUNT,&FramesToDisplay);
        FramesToDisplay = qMin(ulong(16),FramesToDisplay);
        do
        {
            // Try to read a frame
            Result = GetInstance(pAVT->m_pCamera)->GetFrame(&frame,0);

            // Any error?
            if(Result != FCE_NOERROR)
            {
                // Stop loop on error
                break;
            }

            // Only if <pData> is valid (ignore pure frame start events)
            if(frame.pData)
            {

                if(FramesToDisplay)
                {
                    int j;
                    int iWidth = pAVT->m_pBitmapInfo->bmiHeader.biWidth;
                    int iHeight = pAVT->m_pBitmapInfo->bmiHeader.biHeight;
                    BYTE *pData1 = NULL;
                    BYTE *pData2 = NULL;

                    /*	if(i == 12)
                        {
                            i = 12;
                        }*////???
                    // 目标地址
                    pData1 = pAVT->m_pImage;
                    // 起始地址 = 源 + 宽度*(高度-1)
                    pData2 = frame.pData + iWidth * ( iHeight - 1 );

                    for(j = 0; j < iHeight; j++)
                    {
                        memcpy(pData1 + iWidth * j, pData2 - iWidth * j, iWidth);
                    }

                    FramesToDisplay--;
                }
                GetInstance(pAVT->m_pCamera)->PutFrame(&frame);
                pAVT->SetRxFlag(true);

            }
        }while(!(frame.Flags&FGF_LAST));



        break;
    default:
        break;

    }
}

