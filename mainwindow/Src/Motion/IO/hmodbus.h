#ifndef HMODBUS_H
#define HMODBUS_H

#include<QObject>
#include "hiodevice.h"


class QModbusClient;
class IniSettings;



class HModbus : public QObject ,public HIODevice
{
    Q_OBJECT
public:

    explicit HModbus(QObject *parent = nullptr);
    ~HModbus() Q_DECL_OVERRIDE;

     qint32 InitDevice(const QString& configFile) override;

     qint32 SetIOConfig(const IOConfiguration& data) override;


     qint32 GetInputBit(ulong&data) override;
     qint32 GetOutputBit(ulong&data) override;

     qint32 SetOutputBit(ulong data) override;


private slots:
    void readReady();

private:

    QModbusClient *modbusDevice=nullptr;
    IniSettings*  ptrIniSettings=nullptr;

    static quint32 m_handleRefrenceCount;

    bool isExit=false;
    bool isExitWait=false;

    bool readDone=false;
    bool writeDone=false;

    ulong readResult=0;
    bool writeResult=false;



    bool modbusConnect();
    bool readHoldingValue(quint16 addr,ulong &value);
    bool writeHoldingValue(quint16 addr,ulong value);
};

#endif // HMODBUS_H
