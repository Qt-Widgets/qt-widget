#include "hiodevicecontroller.h"

#include "motionStage/A3200Api.h"
#include "motionStage/GtMotion.h"
#include "Src/data/hsettings.h"
#include "h_motioncontroller.h"
#include "hfinddevice.h"
#include "IO/hmodbus.h"


HIODeviceController* HIODeviceController::m_pInstance = nullptr;

HIODeviceController *HIODeviceController::GetInstance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HIODeviceController;
        atexit(Release);
    }
    return m_pInstance;
}
void HIODeviceController::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

bool HIODeviceController::Init()
{
    if(InitAllIODevice())
        return ConfigController();
    return false;
}

bool HIODeviceController::InitAllIODevice()
{

    releaseDeviceRes();

    auto inputs=ptrSettings->getTableValuesAtCol(HSettings::InPut,HSettings::Input_Type);
    auto inputNames=ptrSettings->getTableValuesAtCol(HSettings::InPut,HSettings::Input_Name);
    auto inputConfigs=ptrSettings->getTableValuesAtCol(HSettings::InPut,HSettings::InPut_ConfigFile);

    auto outputs=ptrSettings->getTableValuesAtCol(HSettings::OutPut,HSettings::OutPut_Type);
    auto outputNames=ptrSettings->getTableValuesAtCol(HSettings::OutPut,HSettings::OutPut_Name);
    auto ouputConfigs=ptrSettings->getTableValuesAtCol(HSettings::OutPut,HSettings::OutPut_ConfigFile);



    auto availableInputs=ptrSettings->getAvailableInput();
    auto availableOutputs=ptrSettings->getAvailableOutput();

    bool isOk=true;

    for(qint32 k=0;k<inputs.size();++k){
        HIODevice * ioDevice=nullptr;

        switch (availableInputs.indexOf(inputs.at(k).toString())) {
        case 0:
            ioDevice= new HIODevice;
            break;
        case 1:
            ioDevice =new A3200Api(0);
            break;
        case 2:
            ioDevice =new GtMotion(0);
            break;
        case 3:
            ioDevice= new HIODevice;
            break;
        case 4:
            ioDevice =new HModbus;
            break;
        default:
            ioDevice= new HIODevice;
            break;
        }

        isOk&=(!ioDevice->InitDevice(inputConfigs.at(k).toString()));

        auto inname=inputNames.at(k).toString();
        m_InputArray.append(ioDevice);
        m_InputMap.insertMulti(inname,ioDevice);
        m_InputNames.append(inname);
    }

    for(qint32 k=0;k<outputs.size();++k){
        HIODevice * ioDevice=nullptr;

        switch (availableOutputs.indexOf(outputs.at(k).toString())) {
        case 0:
            ioDevice= new HIODevice;
            break;
        case 1:
            ioDevice =new A3200Api(0);
            break;
        case 2:
            ioDevice =new GtMotion(0);
            break;
        case 3:
            ioDevice= new HIODevice;
            break;
        case 4:
            ioDevice =new HModbus;
            break;
        default:
            ioDevice= new HIODevice;
            break;
        }


        isOk&=(!ioDevice->InitDevice(ouputConfigs.at(k).toString()));

        auto outname=outputNames.at(k).toString();
        m_OutputArray.append(ioDevice);
        m_OutputMap.insertMulti(outname,ioDevice);
        m_OutputNames.append(outname);
    }

    return isOk;
}

bool HIODeviceController::ConfigController()
{
    bool isOk=true;

    auto inputChannels=ptrSettings->getTableValuesAtCol(HSettings::InPut,HSettings::Input_Channel);
    auto inputReserve=ptrSettings->getTableValuesAtCol(HSettings::InPut,HSettings::Input_Reverses);

    auto outputChannels=ptrSettings->getTableValuesAtCol(HSettings::OutPut,HSettings::OutPut_Channel);
    auto outputReserve=ptrSettings->getTableValuesAtCol(HSettings::OutPut,HSettings::OutPut_Reverses);


    qint32 size=m_InputArray.size();
    for(qint32 k=0;k<size;++k){
        isOk &=m_InputArray.at(k)->SetIOConfig(IOConfiguration(inputChannels.at(k).toString().toInt(),inputReserve.at(k).toString().toInt()));
    }

    size=m_OutputArray.size();
    for(qint32 k=0;k<size;++k){
        isOk &=m_OutputArray.at(k)->SetIOConfig(IOConfiguration(outputChannels.at(k).toString().toInt(),outputReserve.at(k).toString().toInt()));
    }

    return isOk;
}

HIODevice *HIODeviceController::GetInputPort(qint32 index) const
{
    return m_InputArray.value(index);
}

HIODevice *HIODeviceController::GetInputPort(const QString &name) const
{
    return m_InputMap.value(name);
}

HIODevice *HIODeviceController::GetOutputPort(qint32 index) const
{
    return m_OutputArray.value(index);
}

HIODevice *HIODeviceController::GetOutputPort(const QString &name) const
{
    return m_OutputMap.value(name);
}

qint32 HIODeviceController::GetInputNum() const
{
    return  m_InputArray.size();
}

qint32 HIODeviceController::GetOutputNum() const
{
    return m_OutputArray.size();
}

const QStringList &HIODeviceController::GetInputNames() const
{
    return m_InputNames;
}

const QStringList &HIODeviceController::GetOutputNames() const
{
    return m_OutputNames;
}


HIODeviceController::HIODeviceController()
{
    ptrSettings=HSettings::GetInstance();
}

HIODeviceController::~HIODeviceController()
{
    releaseDeviceRes();
}

void HIODeviceController::releaseDeviceRes()
{
    for(const auto &k:m_OutputArray){
        delete  k;
    }
    for(const auto &k:m_InputArray){
        delete  k;
    }

    m_InputMap.clear();
    m_OutputMap.clear();

    m_InputArray.clear();
    m_OutputArray.clear();

    m_InputNames.clear();
    m_OutputNames.clear();
}
