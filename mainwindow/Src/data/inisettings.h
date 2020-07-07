#ifndef INISETTINGS_H
#define INISETTINGS_H

#include <stable.h>




class QSettings;

class IniSettings
{

public:
    static IniSettings* GetInstance();
    static void Release();

    enum IniType{

        View_Language,

        Edit_mode,

        ModBus_Client_ConnectType,
        ModBus_Client_ComName,
        ModBus_Client_IP,
        ModBus_Client_ServerAddr,
        ModBus_Client_SigType,
        ModBus_Client_Read_AddrStart,
        ModBus_Client_Read_Count,
        ModBus_Client_Write_AddrStart,
        ModBus_Client_Write_Count,
        ModBus_Client_AutoConnect,

        A3200_EmulatedQuadratureDivider,
        A3200_TrajectoryFIRFilter,

        painter_Antialiasing,


        Config_microStep,
        Config_optimizeAngle,
        Config_optimizeLength,

    };

    QVariant getValue(IniType index);
    void setValue(IniType index, const QVariant &value);




private:
    explicit IniSettings();
    ~IniSettings();
    void initData();
    void writeData();

    static IniSettings* m_pInstance;

    QMap<qint32, QVariant> hashInfo;
    QSettings *configIni;

    const QStringList iniName={
        "/View/Language"

        ,"/Edit/mode"

        ,"/ModBus_Client/ConnectType","/ModBus_Client/ComName","/ModBus_Client/IP","/ModBus_Client/ServerAddr"
        ,"/ModBus_Client/SigType","/ModBus_Client/Read_AddrStart","/ModBus_Client/Read_Count"
        ,"/ModBus_Client/Write_AddrStart","/ModBus_Client/Write_Count","/ModBus_Client/AutoConnect"

        ,"/A3200/EmulatedQuadratureDivider","/A3200/TrajectoryFIRFilter"

        ,"/painter/Antialiasing"

        ,"/Config/microStep","/Config/optimizeAngle","/Config/optimizeLength"

    };

};

#endif // INISETTINGS_H
