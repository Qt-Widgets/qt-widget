#include "hiodevice.h"

HIODevice::HIODevice()
{


}

HIODevice::~HIODevice()
{

}

qint32 HIODevice::InitDevice(const QString &configFile)
{
   return 0;
}

qint32 HIODevice::SetIOConfig(const IOConfiguration &data)
{
    m_IODeviceConfig=data;
    return 0;
}

qint32 HIODevice::GetInputBit( ulong &data)
{
    data=0;
    return 0;
}

qint32 HIODevice::GetOutputBit(ulong &data)
{
    data=1;
    return 0;
}

qint32 HIODevice::SetOutputBit(ulong data)
{
    return 0;
}
