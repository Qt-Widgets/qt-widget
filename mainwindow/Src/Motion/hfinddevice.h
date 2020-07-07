#ifndef HFINDDEVICE_H
#define HFINDDEVICE_H

#include <QObject>
#include <QMap>



struct HDeviceName
{

    HDeviceName(qint32 m_type=0,QString m_text=nullptr){
      type=  m_type;
      text= m_text;
    }

    qint32 type;
    QString text;

    bool operator < (const HDeviceName &o) const
    {
        return type < o.type;
    }
    bool operator > (const HDeviceName &o) const
    {
        return type > o.type;
    }

    bool operator >= (const HDeviceName &o) const
    {
        return type >= o.type;
    }

    bool operator <= (const HDeviceName &o) const
    {
        return type <= o.type;
    }


//    void operator = (const HDeviceName &o)
//    {
//        type=o.type;
//        text=o.text;
//    }
};

typedef  QVector<HDeviceName> HDeviceNames;

enum  HDEVICE_TYPE{

    T1_START=0x0100000,
    T2_START=0x0001000,
    T3_START=0x0000001,

    T1_MOTION=0x0200000,
    T2_MOTION_A3200=0x0201000,
    T2_MOTION_GTS=0x0202000,
    T2_MOTION_ACS=0x0203000,

    T1_IO=0x0300000,
    T2_IO_A3200=0x0301000,
    T2_IO_GTS=0x0302000,
    T2_IO_ACS=0x0303000,
    T2_IO_ADLINK=0x0304000,
    T2_IO_MODBUS=0x0305000,

    T1_CAMERA=0x0400000,
    T2_CAMERA_BASLER=0x0401000,
    T3_CAMERA_BASLER_INDEX=0x0401001,

    T1_SCANNER=0x0500000,
    T2_SCANNER_RTC4=0x0501000,
    T3_SCANNER_RTC4_INDEX=0x0501001,



    T1_OTHER=0x0600000,
    T2_OTHER_SERIAL=0x0601000,
    T2_OTHER_TCP=0x0602000,

};


enum class HDEVICE_TYPE_NAME:qint32{motion,io,camera,scanner,other,};
const QStringList HDEVICE_TYPE={"Motion","IntputOutpt","Camera","Scanner","Other"};

enum class HDEVICE_MOTION_NAME:qint32{simulation,A3200,GTS,ACS,};
const QStringList HDEVICE_MOTION={"simulation-motion","A3200-motion","GTS-motion","ACS-motion"};

enum class HDEVICE_IO_NAME:qint32{simulation,A3200,GTS,ACS,ADLINK,MODBUS};
const QStringList HDEVICE_IO={"simulation-io","A3200-io","GTS-io","ACS-io","ADLINK-io","modbus-io"};

enum class HDEVICE_CAMERA_NAME:qint32{simulation,Basler,Hikvision,};
const QStringList HDEVICE_CAMERA={"simulation-camera","basler","hikvision"};

enum class HDEVICE_SCANNER_NAME:qint32{simulation,RTC4,};
const QStringList HDEVICE_SCANNER={"simulation-scanner","scanlab"};

enum class HDEVICE_OTHER_NAME:qint32{Modbus,serialport,TCP};
const QStringList HDEVICE_OTHER={"serial port","TCP/IP"};

const QVector<QStringList> ALLDEVICENAMES={HDEVICE_MOTION,HDEVICE_IO,HDEVICE_CAMERA,HDEVICE_SCANNER,HDEVICE_OTHER};


class HFindDevice  : public QObject
{
    Q_OBJECT
public:
    explicit HFindDevice(QObject *parent = nullptr);

    HDeviceNames findAllDevice();

    bool gettest();

    static qint32 GetPart1(qint32 type){
        return (type>>20)-2;
    }

    static qint32 GetPart100(qint32 type){
        return (type>>20)<<20;
    }

    static qint32 GetPart110(qint32 type){
        return (type>>12)<<12;
    }


    static qint32 GetPart2(qint32 type){
        return ((type-((type>>20)<<20))>>12);
    }



    static qint32 GetPart3(qint32 type){
        return (type-((type>>12)<<12));
    }

signals:
    void rateOfProcess(qint32 value);
    void cacheOfProcess(const HDeviceNames&data,bool isAll=false);
private:

};

#endif // HFINDDEVICE_H
