#include <pylon/PylonIncludes.h>
#include <pylon/TransportLayer.h>
#include <pylon/gige/PylonGigEIncludes.h>
#include <pylon/gige/ActionTriggerConfiguration.h>
#include <pylon/gige/BaslerGigEInstantCamera.h>
#include <include/ConfigurationEventPrinter.h>
#include <include/ImageEventPrinter.h>

#include <stable.h>

#include "hcamera.h"


namespace PCam {
using namespace Pylon;
using namespace GenICam;
using namespace GenApi;
using namespace Basler_GigECameraParams;
using namespace Basler_GigEStreamParams;
using namespace Basler_ImageFormatConverterParams;
using namespace Basler_InstantCameraParams;
using namespace Basler_GigEEventParams;
using namespace Basler_GigETLParams;
using namespace Basler_GigECamera;
using namespace Basler_GigEChunkData;
}



class BaslerCamera : public HCamera
{

public:

    explicit BaslerCamera(qint32 index);
    ~BaslerCamera();


    qint32 InitDevice();
    void UnInitDevice();

    qint32 OpenCamera();
    qint32 CloseCamera();

    QString GetDeviceName();

    QStringList GetAllDeviceNames();

    qint32 SetConfigFile(const QString &configFile);


    qint32 SetDeviceConfig(const CameraConfiguration& data);

    qint32 GetImgWidthHeight(qint32 &width,qint32 &height);
    qint32 SetImgCaptureMode(qint32 mode=0);
    uchar *GetImgBufferOnce();
    uchar *GetImgBufferContinues(); //only used in continues mode
    HColoredImgBuffer GetColoredImgBufferContinues(); //only used in continues mode


    qint32 GetGain(qint32 &value);
    qint32 GetExposure(qint32 &value);

    qint32 SetGain(qint32 value);
    qint32 SetExposure(qint32 value);

    double GetFPS();


public:
    bool InitAllDevices();
    void FreeAllDevices();

    bool openCamera(qint32 dindex);
    bool closeCamera(qint32 dindex);

    QStringList GetCameraNames();


private:

    /*****************Golobal Object**********************/
    const uint devicesNumMax=10;

    static uint32_t m_deviceRefrenceCount;
    typedef Pylon::CBaslerGigEInstantCamera Camera_t;
    static QVector<Camera_t*> ptrCameraDevices;

    static QStringList CameraInformations;


    QString errorInfo;

    QString configFilePath=nullptr;

    static PCam::IGigETransportLayer* ptrPTL;
    static PCam::CInstantCameraArray* ptrCameras;
    static PCam::CGrabResultPtr* ptrGrabResult;
    static PCam::DeviceInfoList_t* ptrDevices;
    static PCam::CDeviceInfo* ptrInfo;

    /***************************************************/



private:
    /////////////////////////////////////////////////////
    struct CameraIP
    {
        PCam::gcstring m_ipaddress;
        PCam::gcstring m_subnet;
        PCam::gcstring m_gataway;
        PCam::gcstring m_macaddress;

        int64_t netWorkMode;
        bool dhcp_support;
        bool ipfix_support;
        bool autoip_support;
    };
    /////////////////////////////////////////////////////


    /////////////////////////////////////////////////////
    struct DeviceInfo
    {
        PCam::gcstring deviceVendorName;
        PCam::gcstring deviceModelName;
        PCam::gcstring deviceManufacturerInfo;
        PCam::gcstring deviceVersion;
        PCam::gcstring deviceFirmwareVersion;
        PCam::gcstring deviceID;
        PCam::gcstring deviceUserID;
        int64_t deviceScanType;
        int64_t sensorWidth;
        int64_t sensorHeight;
        int64_t WidthMax;
        int64_t HeightMax;

    };
    /////////////////////////////////////////////////////


    ////////////////////////////////////////////////////
    struct AnalogControl
    {
        int64_t autoGain;//GainAuto_Off,GainAuto_Once,GainAuto_Continuous
        int64_t gainMode;//GainSelector_All,GainSelector_AnalogAll,GainSelector_DigitalAll,GainSelector_Tap1,GainSelector_Tap2,GainSelector_Tap3,GainSelector_Tap4,GainSelector_Red,GainSelector_Green,GainSelector_Blue
        int64_t gainRaw;
        int64_t gainRawMin;
        int64_t gainRawMax;

        int64_t blackLevelSelector; //BlackLevelSelector_All,BlackLevelSelector_AnalogAll,BlackLevelSelector_DigitalAll,BlackLevelSelector_Tap1,BlackLevelSelector_Tap2,BlackLevelSelector_Tap3,BlackLevelSelector_Tap4,BlackLevelSelector_Red,BlackLevelSelector_Green,BlackLevelSelector_Blue

        int64_t blackLevelRaw;
        int64_t blackLevelRawMin;
        int64_t blackLevelRawMax;

        bool gammaEnable;
        int64_t gammmaSelector;//GammaSelector_User,GammaSelector_sRGB
        double gamma;
        int64_t digitalShift;

    };
    /////////////////////////////////////////////////////

    ///////////////////////////////////////////////////
    struct AcquisitionControl
    {
        int64_t exposureAuto;
        int64_t exposureTimeRaw;
        int64_t exposureTimeRawMin;
        int64_t exposureTimeRawMax;
        bool globalResetReleaseModeEnable;
        bool acquisitionFrameRateEnable;
        double acquisitionFrameRateAbs;
        double resultingFrameRateAbs;

    };
    ///////////////////////////////////////////////////


    ///////////////////////////////////////////////////
    struct ImageFormatControl
    {
        int64_t pixelFormat;
        bool reverseX;
    };



    //////////////////////////////////////////////////
    struct AOIControl
    {
        int64_t width;
        int64_t height;

        int64_t xOffset;
        int64_t yOffset;

        bool centerX;
        bool centerY;

        bool reverseX;
        bool reverseY;
    };
    /////////////////////////////////////////////////////


    //////////////////////////////////////////////////
    struct TransportLayer
    {
        int64_t heartbeatTimeout;

    };
    /////////////////////////////////////////////////////

private:

    /************************************User Operation*******************************/


    bool LoadParameterFile(qint32 dindex,const QString &FileNme);
    bool SaveParameterToFile(qint32 dindex,const QString &FileNme);

    unsigned char * GetImageBuffer(qint32 dindex,uint timeOut=5000);// only be used in mode of ContinuesShot, for higher performance,there is not check for mode.
    unsigned char * GetOneImageBuffer(qint32 dindex,uint timeOut=5000);

    bool startContinuesShot(qint32 dindex);
    bool stopContinuesShot(qint32 dindex);
    QString GetLastErrorInfo();
    /*********************************************************************************/



    /*********************************************************************************/
    bool LoadDefaultSetting(qint32 dindex);
    bool SaveAsDefaultSetting(qint32 dindex);
    bool LoadSettingFromFile(qint32 dindex,const PCam::gcstring &FileName);
    bool SaveSettingToFile(qint32 dindex,char FileName[]);

    bool FramePerSecond(qint32 dindex,double&value,bool mode);
    bool ExposureAuto(qint32 dindex,int64_t& value,bool mode);
    bool ExposureTimeRaw(qint32 dindex,int64_t &value,bool mode);
    bool Gain(qint32 dindex,int64_t&value,bool mode);
    bool GainAuto(qint32 dindex,int64_t&value,bool mode);
    bool Height(qint32 dindex,int64_t &value , bool mode);
    bool Width(qint32 dindex, int64_t &value , bool mode);
    bool XOffset(qint32 dindex,int64_t & value ,bool mode);
    bool YOffset(qint32 dindex,int64_t & value ,bool mode);
    bool CenterX(qint32 dindex,bool &value,bool mode);
    bool CenterY(qint32 dindex,bool &value,bool mode);
    bool PixelFormat(qint32 dindex,int64_t&value,bool mode);
    bool GetGainRawMin(qint32 dindex,int64_t&value);
    bool GetGainRawMax(qint32 dindex,int64_t&value);
    bool GetExposureTimeRawMin(qint32 dindex,int64_t&value);
    bool GetExposureTimeRawMax(qint32 dindex,int64_t&value);
    /*********************************************************************************/



    /*************************************User Setting********************************/
    bool NetWorkParameters(qint32 dindex,CameraIP &networkpara,bool mode);
    bool AnalogControlParameters(qint32 dindex,AnalogControl &analogcontrol,bool mode);
    bool AcquisitionControlParameters(qint32 dindex,AcquisitionControl &acquisitionControl,bool mode);
    bool ImageFormatControlParameters(qint32 dindex,ImageFormatControl &imageformatcontrol,bool mode);
    bool AOIControlParameters(qint32 dindex,AOIControl &aoicontrol,bool mode);
    bool TransportLayerParameters(qint32 dindex,TransportLayer &transportlayer,bool mode);
    bool GetDeviceInfo(qint32 dindex,DeviceInfo&deviceInfo);
    /*********************************************************************************/


private:

    /*************************************************************************************************************************/
    /************************************************camera  devices**********************************************************/
    /*************************************************************************************************************************/
    /**************Analog Controls*****************/
    void GainAuto(qint32 dindex,PCam::GainAutoEnums&value,bool mode);
    void GainSelector(qint32 dindex,PCam::GainSelectorEnums&value,bool mode);
    void Gain1(qint32 dindex,int64_t&value,bool mode);
    void BlackLevelSelector(qint32 dindex,PCam::BlackLevelSelectorEnums&value,bool mode);
    void BlackLevel(qint32 dindex,int64_t &value,bool mode);
    void GammaEnable(qint32 dindex,bool &value,bool mode);
    void GammaSelector(qint32 dindex,Basler_GigECamera::GammaSelectorEnums &value, bool mode);
    void Gamma(qint32 dindex,double &value,bool mode);
    void DigitalShift(qint32 dindex,int64_t &value,bool mode);
    /********************************************/

    /*******************Image Format Controls***********************/
    void PixelFormat(qint32 dindex,PCam::PixelFormatEnums&value,bool mode);
    void GetPixelSize(qint32 dindex,PCam::PixelSizeEnums &value);
    void GetPixelColorFilter(qint32 dindex,PCam::PixelColorFilterEnums& value);
    void GetPixelDynamicRangeMin(qint32 dindex,int64_t &value);
    void GetPixelDynamicRangeMax(qint32 dindex,int64_t &value);
    void ReverseX(qint32 dindex,bool &value,bool mode);
    void TestImageSelector(qint32 dindex,PCam::TestImageSelectorEnums& value, bool mode);
    /*************************************************************/


    /**********************AOI Controls*************************/
    void Width1(qint32 dindex,int64_t & value ,bool mode);
    void Height1(qint32 dindex,int64_t & value ,bool mode);
    void XOffset1(qint32 dindex,int64_t & value ,bool mode);
    void YOffset1(qint32 dindex,int64_t & value ,bool mode);
    void CenterX1(qint32 dindex,bool &value,bool mode);
    void CenterY1(qint32 dindex,bool &value,bool mode);
    void BinningModeHorizontal(qint32 dindex,PCam::BinningModeHorizontalEnums& value,bool mode);
    void BinningHorizontal(qint32 dindex,int64_t & value ,bool mode);
    void BinningModeVertical(qint32 dindex,PCam::BinningModeVerticalEnums&value, bool mode);
    void BinningVertical(qint32 dindex,int64_t &value,bool mode);
    void DecimationHorizontal(qint32 dindex,int64_t &value,bool mode);
    void DecimationVertical(qint32 dindex,int64_t &value,bool mode);
    /**********************************************************/


    /*******************Acquisition Controls***********************/
    void EnableBurstAcquisition(qint32 dindex,bool &value,bool mode);
    void AcquisitionFrameCount(qint32 dindex,int64_t & value ,bool mode);
    void TriggerSelector(qint32 dindex,PCam::TriggerSelectorEnums & value ,bool mode);
    void TriggerMode(qint32 dindex,PCam::TriggerModeEnums & value ,bool mode);
    void TriggerSoftware(qint32 dindex);
    void TriggerSource(qint32 dindex,PCam::TriggerSourceEnums& value ,bool mode);
    void TriggerActivation(qint32 dindex,PCam::TriggerActivationEnums& value ,bool mode);
    void TriggerDelayAbs(qint32 dindex,double& value ,bool mode);
    void ExposureMode(qint32 dindex,PCam::ExposureModeEnums& value ,bool mode);
    void ExposureAuto(qint32 dindex,PCam::ExposureAutoEnums& value,bool mode);
    void ExposureTimeAbs(qint32 dindex,double& value ,bool mode);
    void ExposureTimeRaw1(qint32 dindex,int64_t &value,bool mode);
    void ReadoutTimeAbs(qint32 dindex,double &value,bool mode);
    void ShutterMode(qint32 dindex,PCam::ShutterModeEnums &value,bool mode);
    void AcquisitionFrameRateEnable(qint32 dindex,bool &value,bool mode);
    void AcquisitionFrameRateAbs(qint32 dindex,double &value,bool mode);
    void ResultingFramePeriodAbs(qint32 dindex,double &value,bool mode);
    void ResultingFrameRateAbs(qint32 dindex,double &value,bool mode);
    void AcquisitionStatusSelector(qint32 dindex,PCam::AcquisitionStatusSelectorEnums &value,bool mode);
    void AcquisitionStatus(qint32 dindex,bool &value,bool mode);
    /************************************************************/


    /*******************Digital Controls***********************/
    void LineSelector(qint32 dindex,PCam::LineSelectorEnums &value,bool mode);
    void LineMode(qint32 dindex,PCam::LineModeEnums &value,bool mode);
    void LineLogic(qint32 dindex,PCam::LineLogicEnums &value,bool mode);
    void LineFormat(qint32 dindex,PCam::LineFormatEnums &value,bool mode);
    void LineSource(qint32 dindex,PCam::LineSourceEnums &value,bool mode);
    void LineInverter(qint32 dindex,bool&value,bool mode);
    void LineDebouncerTimeAbs(qint32 dindex,double&value,bool mode);
    void MinOutPulseWidthAbs(qint32 dindex,double&value,bool mode);
    void LineStatus(qint32 dindex,bool&value,bool mode);
    void LineStatusAll(qint32 dindex,int64_t&value,bool mode);
    void UserOutputSelector(qint32 dindex,PCam::UserOutputSelectorEnums&value,bool mode);
    void UserOutputValue(qint32 dindex,bool&value,bool mode);
    void UserOutputValueAll(qint32 dindex,int64_t&value,bool mode);
    void SyncUserOutputSelector(qint32 dindex,PCam::SyncUserOutputSelectorEnums&value,bool mode);
    void SyncUserOutputValue(qint32 dindex,bool&value,bool mode);
    void SyncUserOutputValueAll(qint32 dindex,int64_t&value,bool mode);
    /************************************************************/



    /*****************Action Control****************************/
    void ActionCommandCount(qint32 dindex,int64_t &value,bool mode);
    void ActionDeviceKey(qint32 dindex,int64_t &value,bool mode);
    void ActionSelector(qint32 dindex,int64_t &value,bool mode);
    void ActionGroupKey(qint32 dindex,int64_t &value,bool mode);
    void ActionGroupMask(qint32 dindex,int64_t &value,bool mode);
    /***********************************************************/



    /*******************Sequence Controls***********************/
    void SequenceEnable(qint32 dindex,bool&value,bool mode);
    void SequenceCurrentSet(qint32 dindex,int64_t&value,bool mode);
    void SequenceAsyncRestart(qint32 dindex);
    void SequenceAsyncAdvance(qint32 dindex);
    void SequenceAdvanceMode(qint32 dindex,PCam::SequenceAdvanceModeEnums& value,bool mode);
    void SequenceControlSelector(qint32 dindex,PCam::SequenceControlSelectorEnums& value,bool mode);
    void SequenceControlSource(qint32 dindex,PCam::SequenceControlSourceEnums& value,bool mode);
    void SequenceAddressBitSelector(qint32 dindex,PCam::SequenceAddressBitSelectorEnums& value,bool mode);
    void SequenceAddressBitSource(qint32 dindex,PCam::SequenceAddressBitSourceEnums& value,bool mode);
    void SequenceSetTotalNumber(qint32 dindex,int64_t& value,bool mode);
    void SequenceSetIndex(qint32 dindex,int64_t& value,bool mode);
    void SequenceSetStore(qint32 dindex);
    void SequenceSetLoad(qint32 dindex);
    void SequenceSetExecutions(qint32 dindex,int64_t& value,bool mode);
    /************************************************************/


    /*******************Counter And Timer Controls***********************/
    void TimerDelayTimebaseAbs(qint32 dindex,double &value,bool mode);
    void TimerDurationTimebaseAbs(qint32 dindex,double &value,bool mode);
    void TimerSelector(qint32 dindex,PCam::TimerSelectorEnums &value,bool mode);
    void TimerDelayAbs(qint32 dindex,double &value,bool mode);
    void TimerDelayRaw(qint32 dindex,int64_t &value,bool mode);
    void TimerDurationAbs(qint32 dindex,double &value, bool mode);
    void TimerTriggerSource(qint32 dindex,PCam::TimerTriggerSourceEnums &value,bool mode);
    void TimerTriggerActivation(qint32 dindex,PCam::TimerTriggerActivationEnums &value,bool mode);
    void CounterSelector(qint32 dindex,PCam::CounterSelectorEnums &value,bool mode);
    void CounterEventSource(qint32 dindex,PCam::CounterEventSourceEnums &value,bool mode);
    void CounterResetSource(qint32 dindex,PCam::CounterResetSourceEnums &value,bool mode);
    void CounterReset(qint32 dindex);
    /************************************************************/


    /**********************LUT Controls*************************/
    void LUTSelector(qint32 dindex,PCam::LUTSelectorEnums &value,bool mode);
    void LUTEnable(qint32 dindex,bool &value,bool mode);
    void LUTIndex(qint32 dindex,int64_t &value,bool mode);
    void LUTValue(qint32 dindex,int64_t &value,bool mode);
    /************************************************************/


    /*********************User Define Values Controls*************************/
    void UserDefinedValueSelector(qint32 dindex,PCam::UserDefinedValueSelectorEnums &value,bool mode);
    void UserDefinedValue(qint32 dindex,int64_t &value,bool mode);
    /************************************************************/


    /*********************Features Set Controls*************************/
    void FeatureSet(qint32 dindex,PCam::FeatureSetEnums &value,bool mode);
    /************************************************************/



    /*****************Device Information********************/
    void GetDeviceVendorName(qint32 dindex,PCam::gcstring &value);
    void GetDeviceModelName(qint32 dindex,PCam::gcstring &value);
    void GetDeviceManufacturerInfo(qint32 dindex,PCam::gcstring &value);
    void GetDeviceVersion(qint32 dindex,PCam::gcstring &value);
    void GetDeviceFirmwareVersion(qint32 dindex,PCam::gcstring &value);
    void GetDeviceID(qint32 dindex,PCam::gcstring &value);
    void DeviceUserID(qint32 dindex,PCam::gcstring &value,bool mode);
    void DeviceScanType(qint32 dindex,PCam::DeviceScanTypeEnums&value,bool mode);
    void GetSensorWidth(qint32 dindex,int64_t &value);
    void GetSensorHeight(qint32 dindex,int64_t &value);
    void GetWidthMax(qint32 dindex,int64_t &value);
    void GetHeightMax(qint32 dindex,int64_t &value);
    void GetLastError(qint32 dindex,PCam::LastErrorEnums&value);
    void ClearLastError(qint32 dindex);
    /*****************************************************/

    /**********************Chunk Data Streams Controls*************************/
    void ChunkModeActive(qint32 dindex,bool &value,bool mode);
    void ChunkSelector(qint32 dindex,PCam::ChunkSelectorEnums &value,bool mode);
    void ChunkEnable(qint32 dindex,bool &value,bool mode);
    /************************************************************/


    /**********************Device Control*************************/
    void DeviceRegistersStreamingStart(qint32 dindex);
    void DeviceRegistersStreamingEnd(qint32 dindex);
    /************************************************************/


    /**********************Event Generation*************************/
    void EventSelector(qint32 dindex,PCam::EventSelectorEnums &value,bool mode);
    void EventNotification(qint32 dindex,PCam::EventNotificationEnums &value,bool mode);
    /************************************************************/


    /**********************Configuration Sets*************************/
    void UserSetSelector(qint32 dindex,PCam::UserSetSelectorEnums &value,bool mode);
    void UserSetLoad(qint32 dindex);
    void UserSetSave(qint32 dindex);
    void UserSetDefaultSelector(qint32 dindex,PCam::UserSetDefaultSelectorEnums &value,bool mode);
    /************************************************************/


    /**********************Transport Layer*************************/
    void PayloadSize(qint32 dindex,int64_t &value,bool mode);
    void GevSCPSPacketSize(qint32 dindex,int64_t &value,bool mode);
    void GevSCPD(qint32 dindex,int64_t &value,bool mode);
    void GevSCFTD(qint32 dindex,int64_t &value,bool mode);
    void GevSCBWR(qint32 dindex,int64_t &value,bool mode);
    void GevSCBWRA(qint32 dindex,int64_t &value,bool mode);
    void GevSCBWA(qint32 dindex,int64_t &value,bool mode);
    void GevSCDMT(qint32 dindex,int64_t &value,bool mode);
    void GevSCDCT(qint32 dindex,int64_t &value,bool mode);
    void GevSCFJM(qint32 dindex,int64_t &value,bool mode);
    void GetGevVersionMajor(qint32 dindex,int64_t &value);
    void GetGevVersionMinor(qint32 dindex,int64_t &value);
    void GetGevDeviceModeIsBigEndian(qint32 dindex,bool &value);
    void GetGevDeviceModeCharacterSet(qint32 dindex,int64_t &value);
    void GevInterfaceSelector(qint32 dindex,PCam::GevInterfaceSelectorEnums &value,bool mode);
    void GetGevMACAddress(qint32 dindex,int64_t &value);
    void GetGevSupportedIPConfigurationLLA(qint32 dindex,bool &value);
    void GetGevSupportedIPConfigurationDHCP(qint32 dindex,bool &value);
    void GetGevSupportedIPConfigurationPersistentIP(qint32 dindex,bool &value);
    void GevCurrentIPConfiguration(qint32 dindex,int64_t &value,bool mode);
    void GetGevCurrentIPAddress(qint32 dindex,int64_t &value);
    void GetGevCurrentSubnetMask(qint32 dindex,int64_t &value);
    void GetGevCurrentDefaultGateway(qint32 dindex,int64_t &value);
    void GevPersistentIPAddress(qint32 dindex,int64_t &value,bool mode);
    void GevPersistentSubnetMask(qint32 dindex,int64_t &value,bool mode);
    void GevPersistentDefaultGateway(qint32 dindex,int64_t &value,bool mode);
    void GetGevLinkSpeed(qint32 dindex,int64_t &value);
    void GetGevLinkMaster(qint32 dindex,bool &value);
    void GetGevLinkFullDuplex(qint32 dindex,bool &value);
    void GetGevLinkCrossover(qint32 dindex,bool &value);
    void GetGevFirstURL(qint32 dindex,PCam::gcstring &value);
    void GetGevSecondURL(qint32 dindex,PCam::gcstring &value);
    void GetGevNumberOfInterfaces(qint32 dindex,int64_t &value);
    void GetGevMessageChannelCount(qint32 dindex,int64_t &value);
    void GetGevStreamChannelCount(qint32 dindex,int64_t &value);
    void GetGevSupportedIEEE1588(qint32 dindex,bool &value);
    void GetGevSupportedOptionalCommandsEVENTDATA(qint32 dindex,bool &value);
    void GetGevSupportedOptionalCommandsEVENT(qint32 dindex,bool &value);
    void GetGevSupportedOptionalCommandsPACKETRESEND(qint32 dindex,bool &value);
    void GetGevSupportedOptionalCommandsWRITEMEM(qint32 dindex,bool &value);
    void GevHeartbeatTimeout(qint32 dindex,int64_t &value,bool mode);
    void GetGevTimestampTickFrequency(qint32 dindex,int64_t &value);
    void GevTimestampControlLatchReset(qint32 dindex);
    void GevCCP(qint32 dindex,PCam::GevCCPEnums &value,bool mode);
    void GevSCPInterfaceIndex(qint32 dindex,int64_t &value,bool mode);
    void GevSCPSDoNotFragment(qint32 dindex,bool &value,bool mode);
    void GevSCPSBigEndian(qint32 dindex,bool &value,bool mode);
    /************************************************************/


    /**********************File Access Control*************************/
    void FileSelector(qint32 dindex,PCam::FileSelectorEnums &value,bool mode);
    void FileOperationSelector(qint32 dindex,PCam::FileOperationSelectorEnums &value,bool mode);
    void FileOpenMode(qint32 dindex,PCam::FileOpenModeEnums &value,bool mode);
    void GetFileAccessBuffer(qint32 dindex,uint8_t *value);
    void FileAccessOffset(qint32 dindex,int64_t &value,bool mode);
    void FileAccessLength(qint32 dindex,int64_t &value,bool mode);
    void FileOperationStatus(qint32 dindex,PCam::FileOperationStatusEnums &value,bool mode);
    void FileOperationResult(qint32 dindex,int64_t &value,bool mode);
    void FileSize(qint32 dindex,int64_t &value,bool mode);
    void FileOperationExecute(qint32 dindex);
    /************************************************************/


    /*****************Auto Function Parameters********************/
    void AutoTargetValue(qint32 dindex,int64_t & value ,bool mode);
    void GrayValueAdjustmentDampingAbs(qint32 dindex,double & value ,bool mode);
    void GrayValueAdjustmentDampingRaw(qint32 dindex,int64_t & value ,bool mode);
    void BalanceWhiteAdjustmentDampingAbs(qint32 dindex,double & value ,bool mode);
    void BalanceWhiteAdjustmentDampingRaw(qint32 dindex,int64_t & value ,bool mode);
    void AutoGainRawLowerLimit(qint32 dindex,int64_t & value ,bool mode);
    void AutoGainRawUpperLimit(qint32 dindex,int64_t & value ,bool mode);
    void AutoExposureTimeAbsLowerLimit(qint32 dindex,double & value ,bool mode);
    void AutoExposureTimeAbsUpperLimit(qint32 dindex,double & value ,bool mode);
    void AutoFunctionProfile(qint32 dindex,PCam::AutoFunctionProfileEnums& value,bool mode);
    /////////////Auto Function AOI////////////////
    void AutoFunctionAOISelector(qint32 dindex,PCam::AutoFunctionAOISelectorEnums & value,bool mode);
    void AutoFunctionAOIWidth(qint32 dindex,int64_t & value ,bool mode);
    void AutoFunctionAOIHeight(qint32 dindex,int64_t & value ,bool mode);
    void AutoFunctionAOIOffsetX(qint32 dindex,int64_t & value ,bool mode);
    void AutoFunctionAOIOffsetY(qint32 dindex,int64_t & value ,bool mode);
    void AutoFunctionAOIUsageIntensity(qint32 dindex,bool &value,bool mode);
    void AutoFunctionAOIUsageWhiteBalance(qint32 dindex,bool &value,bool mode);
    /*************************************************************/

    /**********************Image Quality***********************/
    void MedianFilter(qint32 dindex,bool& value,bool mode);
    /**********************************************************/

    /**********************Image Quality***********************/
    void ParameterSelector(qint32 dindex,PCam::ParameterSelectorEnums& value,bool mode);
    /**********************************************************/

    /**********************Stream Parameters***********************/
    void TypeofStreamGrabber(qint32 dindex,PCam::TypeEnums& value,bool mode);
    void RemoveLimits(qint32 dindex,bool& value,bool mode);
    /*************************************************************/


    /**********************Expert Feature Accesss***********************/
    void ExpertFeatureAccessSelector(qint32 dindex,PCam::ExpertFeatureAccessSelectorEnums& value,bool mode);
    void ExpertFeatureAccessKey(qint32 dindex,int64_t& value,bool mode);
    void ExpertFeatureEnable(qint32 dindex,bool& value,bool mode);
    /*************************************************************/

    /********************************************************************************************************/
    /********************************************************************************************************/
    /********************************************************************************************************/




    /********************************************************************************************************/
    /**************************************Transport Layer***************************************************/
    /********************************************************************************************************/
    void ReadTimeout(qint32 dindex,int64_t& value,bool mode);
    void WriteTimeout(qint32 dindex,int64_t& value,bool mode);
    void MaxRetryCountRead(qint32 dindex,int64_t& value,bool mode);
    void MaxRetryCountWrite(qint32 dindex,int64_t& value,bool mode);
    void HeartbeatTimeout(qint32 dindex,int64_t& value,bool mode);
    void CommandDuplicationEnable(qint32 dindex,bool& value,bool mode);
    void StatisticReadWriteTimeoutCount(qint32 dindex,int64_t& value,bool mode);
    /********************************************************************************************************/
    /********************************************************************************************************/
    /********************************************************************************************************/




    /********************************************************************************************************/
    /**************************************Stream Parameters***************************************************/
    /********************************************************************************************************/

    /************************************Debug****************************************/
    void Type(qint32 dindex,PCam::TypeEnums& value,bool mode);
    //void Status(StatusEnums& value,bool mode);
    void AccessMode(qint32 dindex,PCam::AccessModeEnums& value,bool mode);
    void TypeIsWindowsIntelPerformanceDriverAvailable(qint32 dindex,int64_t& value,bool mode);
    void TypeIsSocketDriverAvailable(qint32 dindex,int64_t& value,bool mode);
    /********************************************************************************/

    /************************************Statistic****************************************/
    void Statistic_Total_Buffer_Count(qint32 dindex,int64_t& value,bool mode);
    void Statistic_Failed_Buffer_Count(qint32 dindex,int64_t& value,bool mode);
    void Statistic_Buffer_Underrun_Count(qint32 dindex,int64_t& value,bool mode);
    void Statistic_Total_Packet_Count(qint32 dindex,int64_t& value,bool mode);
    void Statistic_Failed_Packet_Count(qint32 dindex,int64_t& value,bool mode);
    void Statistic_Resend_Request_Count(qint32 dindex,int64_t& value,bool mode);
    void Statistic_Resend_Packet_Count(qint32 dindex,int64_t& value,bool mode);
    /********************************************************************************/


    /************************************IP Configuration****************************************/
    void TransmissionType(qint32 dindex,PCam::TransmissionTypeEnums& value,bool mode);
    void DestinationAddr(qint32 dindex,PCam::gcstring& value,bool mode);
    void DestinationPort(qint32 dindex,int64_t& value,bool mode);
    void MaxNumBuffer(qint32 dindex,int64_t& value,bool mode);
    void MaxBufferSize(qint32 dindex,int64_t& value,bool mode);
    void EnableResend(qint32 dindex,bool& value,bool mode);
    void PacketTimeout(qint32 dindex,int64_t& value,bool mode);
    void AutoPacketSize(qint32 dindex,bool& value,bool mode);
    void FrameRetention(qint32 dindex,int64_t& value,bool mode);
    void ReceiveThreadPriorityOverride(qint32 dindex,int64_t& value,bool mode);
    void ReceiveThreadPriority(qint32 dindex,int64_t& value,bool mode);
    void SocketBufferSize(qint32 dindex,int64_t& value,bool mode);
    /********************************************************************************/



    /********************************************************************************************************/
    /**************************************Image Format Conversion***************************************************/
    /********************************************************************************************************/
    void OutputBitAlignment(qint32 dindex,PCam::OutputBitAlignmentEnums& value,bool mode);
    void OutputOrientation(qint32 dindex,PCam::OutputOrientationEnums& value,bool mode);

    //////Mono Conversion////////
    void MonoConversionMethod(qint32 dindex,PCam::MonoConversionMethodEnums& value,bool mode);
    void Gamma1(qint32 dindex,double &value , bool mode);
    void AdditionalLeftShift(qint32 dindex,int64_t & value ,bool mode);
    /*************************************************************/

};
