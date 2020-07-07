#ifndef HSETTINGS_H
#define HSETTINGS_H


class HSqlCommon;

class HSettings
{
public:


    enum Tablenames{/*Available_Motion,Available_Cemra,Available_InPut,Available_OutPut,Available_Scanner,Available_OtherDevice,*/
        Motion,Camera,InPut,OutPut,Scanner,OtherDevice,
        VisionFunc,SystemAdjCamera,SystemAdjVision,SystemAdjCutter,SystemAdjScanner
    };

    enum HeaderNames_Motion{Motion_Type,Motion_Channel,Motion_Name,Motion_Velocity,Motion_Accelerated,Motion_DelayTime,Motion_Reverse,Motion_PluseRadio,Motion_ConfigFile};
    enum HeaderNames_Camera{Camera_Type,Camera_Channel,Camera_Name,Camera_ImgWidth,Camera_ImgHeight,Camera_Reverses,Camera_ConfigFile};
    enum HeaderNames_InPut{Input_Type,Input_Channel,Input_Name,Input_Reverses,InPut_ConfigFile};
    enum HeaderNames_OutPut{OutPut_Type,OutPut_Channel,OutPut_Name,OutPut_Reverses,OutPut_ConfigFile};
    enum HeaderNames_Scanner{Scanner_Type,Scanner_Channel,Scanner_Name,Scanner_LaserMode,Scanner_Coordinate,Scanner_PluseRadio,Scanner_Correction};
    enum HeaderNames_OtherDevice{OtherDevice_Name};

    enum HeaderNames_VisionFunc{VisionFunc_Name,VisionFunc_DLLFilePath,VisionFunc_ParaNames,VisionFunc_ParaTypes,VisionFunc_ParaValues};



    static HSettings* GetInstance();
    static void Release();

    bool Init();

    void resetAllSetting() const;

    bool resetTable(qint32 table) const;

    QVariantList getTableValues(qint32 table) const;
    QVariantList getTableValues(qint32 table,const QVector<qint32>& headIndexs) const;
    QVariantList getTableValues(qint32 table,const QVector<qint32>& headIndexs,qint32 indexCondition,const QVariant& valueCondition) const;
    QVariant getTableValue(qint32 table,qint32 headIndex,qint32 indexCondition,const QVariant& valueCondition) const;


    QVariantList getTableValuesAtCol(qint32 table,qint32 headerIndex) const;

    bool deleteRowValue(qint32 table,qint32 valueConditionIndex,const QVariant&data) const;

    bool updataValue(qint32 table,qint32 headerIndex,qint32 indexCondition,const QVariant&valueCondition,const QVariant&data) const;

    bool updataOneValue(qint32 table,const QString& name,const QVariantList&datas) const;
    bool addOneValue(qint32 table,const QVariantList&datas) const;


    const QStringList& getTables() const {return tables;}

    const QStringList& getTableHeaderNames(qint32 index) const {return headerNamesArray[index];}
    const QStringList& getHeaderNamesMotion() const {return headernames_motion;}
    const QStringList& getHeaderNamesCamera() const {return headernames_camera;}
    const QStringList& getHeaderNamesInput() const {return headernames_input;}
    const QStringList& getHeaderNamesOutput() const {return headernames_output;}
    const QStringList& getHeaderNamesScanner() const {return headernames_scanner;}
    const QStringList& getHeaderNamesOtherDevice() const {return headernames_otherDevice;}


    const QStringList& getAvailableHardware(qint32 index) const {return AvailableHardwareArray[index];}
    const QStringList& getAvailableMotion() const {return AvailableMotion;}
    const QStringList& getAvailableCamera() const {return AvailableCamera;}
    const QStringList& getAvailableInput() const {return AvailableInput;}
    const QStringList& getAvailableOutput() const {return AvailableOutput;}
    const QStringList& getAvailableScanner() const {return AvailableScanner;}
    const QStringList& getAvailableOtherDevice() const {return AvailableOtherDevice;}



private:
    explicit HSettings();
    ~HSettings();
    static HSettings* m_pInstance;

    const QStringList tables={"Motion","Camera","InPut","OutPut","Scanner","OtherDevice","VisionFunc"};

    const QStringList AvailableMotion={"Default","A3200","GTS"};
    const QStringList AvailableCamera={"Default","Basler","HKVision"};
    const QStringList AvailableInput={"Default","A3200","GTS","ADLink","Modbus"};
    const QStringList AvailableOutput={"Default","A3200","GTS","ADLink","Modbus"};
    const QStringList AvailableScanner={"Default","RTC4"};
    const QStringList AvailableOtherDevice={"Default"};
    const QStringList AvailableHardwareArray[6]={AvailableMotion,AvailableCamera,AvailableInput,AvailableOutput,AvailableScanner,AvailableOtherDevice};



    const QStringList headernames_motion={"Type","Channel","Name","Velocity","Accelerated","DelayTime","Reverse","PluseRadio","ConfigFile"};
    const QStringList headernames_camera={"Type","Channel","Name","ImgWidth","ImgHeight","Reverses","ConfigFile"};
    const QStringList headernames_input={"Type","Channel","Name","Reverses","ConfigFile"};
    const QStringList headernames_output={"Type","Channel","Name","Reverses","ConfigFile"};
    const QStringList headernames_scanner={"Type","Channel","Name","LaserMode","Coordinate","PluseRadio","Correction"};
    const QStringList headernames_otherDevice={"Type","Name"};
    const QStringList headernames_VisionFunc={"Name","DLLFilePath","ParaNames","ParaTypes","ParaValues"};

    const QStringList headerNamesArray[7]={headernames_motion,headernames_camera,headernames_input,headernames_output,headernames_scanner,headernames_otherDevice
                                           ,headernames_VisionFunc};



    HSqlCommon *ptrDatabase;
};

#endif // HSETTINGS_H
