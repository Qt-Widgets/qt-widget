#ifndef HIODEVICE_H
#define HIODEVICE_H

#include "stable.h"

struct IOConfiguration
{
    IOConfiguration(qint32 cha=0,bool misReverse=0){
        channel=cha;
        isReverse=misReverse;

    }
    qint32 channel;
    bool isReverse;
};

class HIODevice
{
public:
    explicit HIODevice();
    virtual ~HIODevice();

    virtual qint32 InitDevice(const QString &configFile);

    virtual qint32 SetIOConfig(const IOConfiguration& data);

    virtual qint32 GetInputBit(ulong&data);
    virtual qint32 GetOutputBit(ulong&data);

    virtual qint32 SetOutputBit(ulong data);

protected:
    IOConfiguration m_IODeviceConfig;



private:



};

#endif // HIODEVICE_H
