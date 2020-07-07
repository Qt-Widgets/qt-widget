#include "hfinddevice.h"
#include "motionStage/A3200Api.h"
#include "motionStage/GtMotion.h"
#include "camera/baslercamera.h"
#include "scanner/rtc4func.h"
#include "IO/hmodbus.h"



HFindDevice::HFindDevice(QObject *parent) : QObject(parent)
{

}

HDeviceNames HFindDevice::findAllDevice()
{


    // QRandomGenerator::global()->seed(GetTickCount());
    HDeviceNames devicessssss;
    int rateP=5;
    emit rateOfProcess(rateP);
    emit cacheOfProcess(devicessssss);
    A3200Api *a3200=new A3200Api(0);
    GtMotion *gtmotion=new GtMotion(0);
    bool isA3200=a3200->InitDevice(nullptr);
    bool isGts=gtmotion->IsReady();


    if(isA3200){
        devicessssss.append(HDeviceName(T2_MOTION_A3200,HDEVICE_MOTION.at(qint32(HDEVICE_MOTION_NAME::A3200))));
    }
    if(isGts){
        devicessssss.append(HDeviceName(T2_MOTION_GTS,HDEVICE_MOTION.at(qint32(HDEVICE_MOTION_NAME::GTS))));
    }

    if(isA3200){
        devicessssss.append(HDeviceName(T2_IO_A3200,HDEVICE_IO.at(qint32(HDEVICE_IO_NAME::A3200))));
    }
    if(isGts){
        devicessssss.append(HDeviceName(T2_IO_GTS,HDEVICE_IO.at(qint32(HDEVICE_IO_NAME::GTS))));
    }

    delete a3200;
    delete gtmotion;

    HModbus *modbus=new HModbus;

    if(modbus->InitDevice(nullptr))
        devicessssss.append(HDeviceName(T2_IO_MODBUS,HDEVICE_IO.at(qint32(HDEVICE_IO_NAME::MODBUS))));


    for(quint32 k=0;k<5;++k){
        QThread::msleep(100);
        rateP+=QRandomGenerator::global()->bounded(3);
        emit rateOfProcess(rateP);
    }
    rateP=20;
    emit rateOfProcess(rateP);
    emit cacheOfProcess(devicessssss);

    for(quint32 k=0;k<5;++k){
        QThread::msleep(100);
        rateP+=QRandomGenerator::global()->bounded(3);
        emit rateOfProcess(rateP);
    }
    rateP=35;
    emit rateOfProcess(rateP);



    rateP=50;
    emit rateOfProcess(rateP);
    emit cacheOfProcess(devicessssss);

    BaslerCamera *camera=new BaslerCamera(0);
    if(camera->InitDevice()){

        for(qint32 k=0;k<camera->GetCameraNames().size();++k){
            devicessssss.append(HDeviceName(T3_CAMERA_BASLER_INDEX+k,camera->GetCameraNames().at(k)));
        }
    }
    delete  camera;
    rateP=65;
    emit rateOfProcess(rateP);
    emit cacheOfProcess(devicessssss);

    for(quint32 k=0;k<5;++k){
        QThread::msleep(100);
        rateP+=QRandomGenerator::global()->bounded(3);
        emit rateOfProcess(rateP);
    }

    rateP=80;
    emit rateOfProcess(rateP);
    emit cacheOfProcess(devicessssss);

//    RTC4Func *scanner=new RTC4Func;
//    if(scanner->GetSerialNumber()){

//        devicessssss.append(HDeviceName(T3_SCANNER_RTC4_INDEX,"RTC4-"+QString::number(scanner->GetSerialNumber())));

//    }
//    delete  scanner;
    emit rateOfProcess(95);
    QThread::msleep(642);

    emit cacheOfProcess(devicessssss);
    emit rateOfProcess(100);
    return devicessssss;

}

bool HFindDevice::gettest()
{
    return false;
}
