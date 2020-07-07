#ifndef HCAMERA_H
#define HCAMERA_H

#include "stable.h"

struct CameraConfiguration
{
    CameraConfiguration(qint32 width=100,qint32 height=100,bool misReverseX=false,bool misReverseY=false,bool mexchangeXY=false){
        imgWidth=width;
        imgHeight=height;
        reverseX=misReverseX;
        reverseY=misReverseY;
        exchangeXY=mexchangeXY;
    }
    qint32 imgWidth;
    qint32 imgHeight;
    bool reverseX;
    bool reverseY;
    bool exchangeXY;
};


struct HColoredImgBuffer
{
    uchar * ptrBufferRed=nullptr;
    uchar * ptrBufferGreen=nullptr;
    uchar * ptrBufferBlue=nullptr;
};

class HCamera
{
public:
    explicit HCamera(qint32 index);
    virtual ~HCamera();

    virtual qint32 InitDevice();
    virtual void UnInitDevice();

    virtual qint32 OpenCamera();
    virtual qint32 CloseCamera();

    virtual QString GetDeviceName();

    virtual QStringList GetAllDeviceNames();

    virtual qint32 SetConfigFile(const QString &configFile);
    virtual qint32 SetDeviceConfig(const CameraConfiguration& data);

    virtual qint32 GetImgWidthHeight(qint32 &width,qint32 &height);
    virtual qint32 SetImgCaptureMode(qint32 mode=0);
    virtual uchar *GetImgBufferOnce();
    virtual uchar *GetImgBufferContinues(); //only used in continues mode
    virtual HColoredImgBuffer GetColoredImgBufferContinues(); //only used in continues mode

    virtual qint32 GetGain(qint32 &value);
    virtual qint32 GetExposure(qint32 &value);
    virtual qint32 SetGain(qint32 value);
    virtual qint32 SetExposure(qint32 value);

    virtual double GetFPS();

protected:
    CameraConfiguration m_DevcieConfig;
    qint32 m_DeviceIndex;

private:

    uchar m_pixelR [768][768];
    uchar m_pixelG [768][768];
    uchar m_pixelB [768][768];



};

#endif // HCAMERA_H
