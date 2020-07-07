#include "h_motioncontroller.h"

#include "motionStage/A3200Api.h"
#include "motionStage/GtMotion.h"

#include "Src/data/hsettings.h"

H_MotionController* H_MotionController::m_pInstance = nullptr;

H_MotionController *H_MotionController::GetInstance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new H_MotionController;
        atexit(Release);
    }
    return m_pInstance;
}
void H_MotionController::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

bool H_MotionController::Init()
{
    if(InitAllMotion())
        return ConfigController();
    return  false;
}

bool H_MotionController::InitAllMotion()
{

    releaseDeviceRes();

    auto motions=ptrSettings->getTableValuesAtCol(HSettings::Motion,HSettings::Motion_Type);
    auto motionsConfig=ptrSettings->getTableValuesAtCol(HSettings::Motion,HSettings::Motion_ConfigFile);
    auto motionName=ptrSettings->getTableValuesAtCol(HSettings::Motion,HSettings::Motion_Name);
    auto motionChannels=ptrSettings->getTableValuesAtCol(HSettings::Motion,HSettings::Motion_Channel);


    auto availableMotions=ptrSettings->getAvailableMotion();

    bool isOk=true;


    for(qint32 k=0;k<motions.size();++k){


        HMotionStage * motion=nullptr;

        auto motionType=motions.at(k).toString();
        qint32 type=availableMotions.indexOf(motionType);
        if(type==0){
            motion=new HMotionStage(motionChannels.at(k).toString().toInt());

        }else if(type==1){
            motion=new A3200Api(motionChannels.at(k).toString().toInt());
        }
        else if(type==2){
            motion=new GtMotion(motionChannels.at(k).toString().toInt());
        }else{
            motion=new HMotionStage(motionChannels.at(k).toString().toInt());
        }


        isOk&=(!motion->InitDevice(motionsConfig.at(k).toString()));


        auto axisName=motionName.at(k).toString();

        m_MotionAxisArray<<motion;
        m_MotionAxisMap.insertMulti(axisName,motion);
        m_MotionNames<<axisName;

    }
    return isOk;

}

bool H_MotionController::ConfigController()
{

    auto accs=ptrSettings->getTableValuesAtCol(HSettings::Motion,HSettings::Motion_Accelerated);
    auto vels=ptrSettings->getTableValuesAtCol(HSettings::Motion,HSettings::Motion_Velocity);
    auto delays=ptrSettings->getTableValuesAtCol(HSettings::Motion,HSettings::Motion_DelayTime);
    auto dirs=ptrSettings->getTableValuesAtCol(HSettings::Motion,HSettings::Motion_Reverse);
    auto pluseRadios=ptrSettings->getTableValuesAtCol(HSettings::Motion,HSettings::Motion_PluseRadio);

    bool isOk=true;


    qint32 size=m_MotionAxisArray.size();
    for(qint32 k=0;k<size;++k){

        isOk&=(!m_MotionAxisArray.at(k)->SetAxisConfig(AxisConfiguration( vels.at(k).toString().toDouble(),
                                                                        accs.at(k).toString().toDouble(),
                                                                        delays.at(k).toString().toDouble(),
                                                                        dirs.at(k).toString()=="reverse",
                                                                        pluseRadios.at(k).toString().toInt()

                                                                        )));
    }
    return isOk;

}


HMotionStage *H_MotionController::GetAxis(qint32 index) const
{

    return  m_MotionAxisArray.at(index);

}

HMotionStage *H_MotionController::GetAxis(const QString &name) const
{
    return m_MotionAxisMap.value(name);
}



qint32 H_MotionController::GetAxisNum() const
{
    return m_MotionAxisArray.size();
}

const QStringList &H_MotionController::GetAxisNames() const
{
    return m_MotionNames;
}

bool H_MotionController::HomeAllMotion() const
{
    bool isOK=true;
    for(qint32 k=m_MotionAxisArray.size()-1;k!=-1;--k){
        isOK&=(!m_MotionAxisArray.at(k)->EnabledAxis(1));
    }
    QThread::msleep(500);
    for(qint32 k=m_MotionAxisArray.size()-1;k!=-1;--k){
        isOK&=(!m_MotionAxisArray.at(k)->HomeAxis());
    }

    return isOK;
}




bool H_MotionController::StopAllMotion() const
{
    bool isOK=true;
    for(const auto &k:m_MotionAxisArray){
        isOK&=(!k->StopAxis());
    }
    return isOK;
}


H_MotionController::H_MotionController()
{
    ptrSettings=HSettings::GetInstance();
}

H_MotionController::~H_MotionController()
{
    releaseDeviceRes();
}

void H_MotionController::releaseDeviceRes()
{
    for(const auto &k:m_MotionAxisArray){
        delete  k;
    }
    m_MotionAxisArray.clear();
    m_MotionAxisMap.clear();
    m_MotionNames.clear();
}
