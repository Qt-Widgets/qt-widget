#include "hmodbus.h"

#include "Src/data/inisettings.h"

#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QModbusDataUnit>


quint32 HModbus::m_handleRefrenceCount=0;


HModbus::HModbus(QObject *parent)  : QObject(parent)
{
    ++m_handleRefrenceCount;
    ptrIniSettings=IniSettings::GetInstance();
}

HModbus::~HModbus()
{
    if(m_handleRefrenceCount){
        if(--m_handleRefrenceCount)
            return;
    }

    modbusDevice->disconnectDevice();
}

qint32 HModbus::InitDevice(const QString &configFile)
{
    if(m_handleRefrenceCount>1){
        return 0;
    }else{
        return  !modbusConnect();
    }
}

qint32 HModbus::SetIOConfig(const IOConfiguration &data)
{
    m_IODeviceConfig=data;
    return 0;
}



qint32 HModbus::GetInputBit( ulong &data)
{
    return !readHoldingValue(quint16(m_IODeviceConfig.channel),data);
}

qint32 HModbus::GetOutputBit(ulong &data)
{
    return !readHoldingValue(quint16(m_IODeviceConfig.channel),data);
}

qint32 HModbus::SetOutputBit( ulong data)
{
    return !writeHoldingValue(quint16(m_IODeviceConfig.channel),data);
}


void HModbus::readReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        for (uint i = 0; i < unit.valueCount(); i++) {
            const QString entry = tr("Address: %1, Value:   %2").arg(unit.startAddress() + i)
                    .arg(QString::number(unit.value(i),
                                         unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16));
            readResult=unit.value(i);
        }
    } else if (reply->error() == QModbusDevice::ProtocolError) {
        readResult=0;
    } else {
        readResult=0;
    }

    reply->deleteLater();
    readDone=true;
}



bool HModbus::modbusConnect()
{
    if (modbusDevice) {
        modbusDevice->disconnectDevice();
        delete modbusDevice;
        modbusDevice = nullptr;
    }

    modbusDevice = new QModbusTcpClient();

    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
        const QUrl url = QUrl::fromUserInput(ptrIniSettings->getValue(IniSettings::ModBus_Client_IP).toString().remove(' '));
        modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
        modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());
        return modbusDevice->connectDevice();
    }
    return false;

}

bool HModbus::readHoldingValue(quint16 addr,ulong &value)
{
    readDone=false;

    if (!modbusDevice)
        return -1;

    if (auto *reply = modbusDevice->sendReadRequest(QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, addr, 1), 1)) {


        if (!reply->isFinished()){

            connect(reply, &QModbusReply::finished, this, &HModbus::readReady);

            qint32 ct=0;
            while (!readDone) {

                if(isExit)
                    return false;

                if(ct==11){
                    return false;
                }
                QEventLoop loop;
                QTimer::singleShot(100, &loop, SLOT(quit()));
                loop.exec();
                ++ct;
            }
            value=readResult;
            return true;
        }
        else{
            delete reply; // broadcast replies return immediately
            return false;
        }

    } else {
        return false;
    }
}

bool HModbus::writeHoldingValue(quint16 addr, ulong value)
{
    if (!modbusDevice){
        return false;
    }

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, addr, 1);
    writeUnit.setValue(0,quint16(value));

    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit,1)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    writeDone=true;
                    writeResult=false;
                } else if (reply->error() != QModbusDevice::NoError) {
                    writeDone=true;
                    writeResult=false;
                }
                reply->deleteLater();
                writeDone=true;
                writeResult=true;
            });

            qint32 ct=0;
            while (!writeDone) {

                if(isExit){
                    return false;
                }

                if(ct==11){
                    return false;
                }
                QEventLoop loop;
                QTimer::singleShot(100, &loop, SLOT(quit()));
                loop.exec();
                ++ct;
            }
            return writeResult;


        } else {
            // broadcast replies return immediately
            reply->deleteLater();
            return false;
        }
    } else {
        return false;
    }
}
