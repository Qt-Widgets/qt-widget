#include "hmotionstage.h"

#include "A3200Api.h"
#include "GtMotion.h"


HMotionStage::HMotionStage(qint32 axisChannel):m_axisChannel(axisChannel)
{
    m_simulationPoss.insert(0,0);
    m_simulationPoss.insert(1,0);
    m_simulationPoss.insert(2,0);
    m_simulationPoss.insert(3,0);
    m_simulationPoss.insert(4,0);
    m_simulationPoss.insert(5,0);
    m_simulationPoss.insert(6,0);
    m_simulationPoss.insert(7,0);

}

HMotionStage::~HMotionStage()
{

}

qint32 HMotionStage::InitDevice(const QString &configFile)
{
    return  0;
}


qint32 HMotionStage::SetAxisConfig( const AxisConfiguration &data)
{
    m_axisConfig=data;

    return 0;
}

qint32 HMotionStage::AxisEnabled()
{
    return 0;
}

qint32 HMotionStage::EnabledAxis( bool data)
{
    return 0;
}

qint32 HMotionStage::EnabledAllAxis(bool data)
{
    return 0;
}

qint32 HMotionStage::HomeAllAxis()
{
    for(auto&k:m_simulationPoss){
        k=0;
    }
    return 0;
}

qint32 HMotionStage::HomeAxis()
{
    m_simulationPoss[m_axisChannel]=0;
    return 0;
}

qint32 HMotionStage::MoveTo( double pos, double vel, double acc, double dec, qint32 moveType)
{
    m_simulationPoss[m_axisChannel]=pos;
    return 0;
}

qint32 HMotionStage::MoveTo( double pos)
{
    m_simulationPoss[m_axisChannel]=pos;
    return 0;
}

qint32 HMotionStage::MoveToWait( double pos)
{
    m_simulationPoss[m_axisChannel]=pos;
    return 0;
}

qint32 HMotionStage::MoveContinue( bool direct)
{
    m_simulationPoss[m_axisChannel]=direct?999:-999;
    return 0;
}

qint32 HMotionStage::MoveStep(double step)
{
    m_simulationPoss[m_axisChannel]=m_simulationPoss.value(m_axisChannel)+step;
    return 0;
}

qint32 HMotionStage::MoveStepWait(double step)
{
    return 0;
}

qint32 HMotionStage::WaitAllAxisStop(long timeOut)
{
    return 0;
}

qint32 HMotionStage::WaitAxisStop( long timeOut)
{
    return 0;
}

qint32 HMotionStage::StopAxis()
{
    return 0;
}

qint32 HMotionStage::StopAllAxis()
{
    return 0;
}

QString HMotionStage::GetAxisStatusString()
{
    return "ready";
}

AxisStatus HMotionStage::GetAxisStatus()
{
    return  AxisStatus::Ready_AXIS;
}

double HMotionStage::GetFeedbackPos()
{
    return m_simulationPoss.value(m_axisChannel);
}

qint32 HMotionStage::GetFeedBackPosCounts()
{
    return 0;
}

QString HMotionStage::GetFeedbackPosString()
{
    return QString::number(m_simulationPoss.value(m_axisChannel),'f',4);
}



double HMotionStage::GetVel()
{
    return 0;
}

QString HMotionStage::GetVelString()
{
    return "0.0000";
}

qint32 HMotionStage::TestFunc()
{
    return 0;
}

qint32 HMotionStage::RunProgramFile(const QString &programFile)
{
    return 0;
}

qint32 HMotionStage::RunProgramFileWait(const QString &programFile)
{
    return 0;
}

qint32 HMotionStage::GetProgramStatus()
{
    return 0;
}

qint32 HMotionStage::StopProgramWait()
{
    return 0;
}

qint32 HMotionStage::RegistrFirstCallBack()
{
    isWaitCallBack=true;
    return 0;
}

qint32 HMotionStage::StartWaitCallback(QVariantList &args)
{
    return isWaitCallBack;
}

qint32 HMotionStage::EndCallback(double returnCode)
{
    return 0;
}

qint32 HMotionStage::ExitCallbackWait()
{
    isWaitCallBack=false;
    return 0;
}

double HMotionStage::getDistanceTime(double step, double acc, double dec, double vel)
{
    double dis1=acc*(vel/acc)*(vel/acc);

    if(step<dis1){

    }
    return 0.0;
}


