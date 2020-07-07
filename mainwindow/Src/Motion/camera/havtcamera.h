#ifndef HAVTCAMERA_H
#define HAVTCAMERA_H
#include "hcamera.h"

#include "basetype.h"

class HAVTCamera : public HCamera
{
public:
    HAVTCamera(qint32 index);
    ~HAVTCamera();

    qint32 InitDevice();
    void UnInitDevice();

    qint32 OpenCamera();
    qint32 CloseCamera();

    QString GetDeviceName();


    qint32 GetImgWidthHeight(qint32 &width,qint32 &height);
    uchar *GetImgBufferOnce();


    qint32 GetGain(qint32 &value);
    qint32 GetExposure(qint32 &value);

    qint32 SetGain(qint32 value);
    qint32 SetExposure(qint32 value);


private:
   void AllocBitmap();
   static void WINAPI SnapCallback(void* Context, UINT32 wParam, void* lParam);


    void* m_pCamera;
    UINT32 m_nAOIWidth;
    UINT32 m_nAOIHeight;
    UINT32 m_nAOIStartX;
    UINT32 m_nAOIStartY;
    BOOL m_IsBayerImage;
    BITMAPINFO* m_pBitmapInfo;
    BYTE* m_pImage;   //存放数据的缓冲区

    bool isInit=false;

    bool m_bLive;
    QString m_strError;

     QMutex mutex1;
    bool m_bRxFlag;

    void SetRxFlag(bool bRx);
    bool GetRxFlag();
    bool Wait();

};

#endif // HAVTCAMERA_H
