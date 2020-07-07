#include "hcamera.h"

#include <QBitmap>
#include <QRandomGenerator>




HCamera::HCamera(qint32 index):m_DeviceIndex(index)
{


}

HCamera::~HCamera()
{

}

qint32 HCamera::InitDevice()
{

    return 0;
}

void HCamera::UnInitDevice()
{
    return;
}

qint32 HCamera::OpenCamera()
{
    return 0;
}

qint32 HCamera::CloseCamera()
{
    return 0;
}

QString HCamera::GetDeviceName()
{
    return "default-"+QString::number(m_DeviceIndex);
}

QStringList HCamera::GetAllDeviceNames()
{
    return  QStringList(QString("default"));
}

qint32 HCamera::SetConfigFile( const QString &configFile)
{
    Q_UNUSED(configFile)
    return 0;
}


qint32 HCamera::SetDeviceConfig( const CameraConfiguration &data)
{
    Q_UNUSED(data)
    m_DevcieConfig=data;

    return 0;
}

qint32 HCamera::GetImgWidthHeight( qint32 &width, qint32 &height)
{
    width=768;
    height=768;
    return 0;
}

qint32 HCamera::SetImgCaptureMode( qint32 mode)
{
    Q_UNUSED(mode)
    return 0;
}

uchar *HCamera::GetImgBufferOnce()
{
    GetColoredImgBufferContinues();
    return &m_pixelR[0][0];
}

uchar *HCamera::GetImgBufferContinues()
{


    return &m_pixelG[0][0];

}

HColoredImgBuffer HCamera::GetColoredImgBufferContinues()
{

    static quint8 color1=255;
    static quint8 color2=0;
    static quint8 color3=0;
    static qint8 flag=0;


    for(qint32 h=0;h<768;++h)
    {

        for(qint32 w=0;w<768;++w)
        {
            m_pixelR[h][w]=quint8(color1);
            m_pixelG[h][w]=quint8(color2);
            m_pixelB[h][w]=quint8(color3);

        }

        switch (flag) {
        case 0:{
            color1--;
            color2++;
            if(color2==255){
                flag=1;
            }
        }
        break;
        case 1:{
            color2--;
            color3++;
            if(color3==255){
                flag=2;
            }
        }
        break;
        case 2:{
            color3--;
            color1++;
            if(color1==255){
                flag=0;
            }
        }
        break;
        default:
            break;
        }
    }


    HColoredImgBuffer  ptrBuffer;
    ptrBuffer.ptrBufferRed=&m_pixelR[0][0];
    ptrBuffer.ptrBufferGreen=&m_pixelG[0][0];
    ptrBuffer.ptrBufferBlue=&m_pixelB[0][0];
    return ptrBuffer;

}

qint32 HCamera::GetGain(qint32 &value)
{
    Q_UNUSED(value)
    return 0;
}

qint32 HCamera::GetExposure(qint32 &value)
{
    Q_UNUSED(value)
    return 0;
}

qint32 HCamera::SetGain( qint32 value)
{
    Q_UNUSED(value)
    return 0;
}

qint32 HCamera::SetExposure( qint32 value)
{
    Q_UNUSED(value)
    return 0;
}

double HCamera::GetFPS()
{
    return 60;
}
