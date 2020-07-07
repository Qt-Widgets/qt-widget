#include "A3200Api.h"
#include "A3200.h"
//#include <QDebug>
#include <QEventLoop>
#include <QThread>
#include <QTimer>
#include <QtConcurrent>
#include <QMap>



const AXISMASK allAxisMask =AXISMASK(AXISMASK_00 | AXISMASK_01|AXISMASK_02|AXISMASK_03|AXISMASK_04|AXISMASK_05);
const unsigned short axisCount=6;


A3200Handle A3200Api::m_handle=nullptr;
qint32 A3200Api::m_handleRefrenceCount=0;

static QMap< unsigned int,QString> AxisFaultInit(){
    QMap< unsigned int,QString> fault;
    fault[AXISFAULT_PositionErrorFault]="PositionErrorFault";
    fault[AXISFAULT_OverCurrentFault]="OverCurrentFault";
    fault[AXISFAULT_CwEndOfTravelLimitFault]="CWEndOfTravelLimitFault";
    fault[AXISFAULT_CcwEndOfTravelLimitFault]="CCWEndOfTravelLimitFault";
    fault[AXISFAULT_CwSoftwareLimitFault]="CWSoftwareLimitFault";
    fault[AXISFAULT_CcwSoftwareLimitFault]="PositionErrorFault";
    fault[AXISFAULT_AmplifierFault]="AmplifierFault";
    fault[AXISFAULT_PositionFeedbackFault]="PositionFeedbackFault";
    fault[AXISFAULT_VelocityFeedbackFault]="VelocityFeedbackFault";
    fault[AXISFAULT_HallSensorFault]="HallSensorFault";
    fault[AXISFAULT_MaxVelocityCommandFault]="MaxVelocityCommandFault";
    fault[AXISFAULT_EmergencyStopFault]="EmergencyStopFault";
    fault[AXISFAULT_VelocityErrorFault]="VelocityErrorFault";
    fault[AXISFAULT_ProbeInputFault]="ProbeInputFault";
    fault[AXISFAULT_ExternalFault]="ExternalFault";
    fault[AXISFAULT_MotorTemperatureFault]="MotorTemperatureFault";
    fault[AXISFAULT_AmplifierTemperatureFault]="AmplifierTemperatureFault";
    fault[AXISFAULT_EncoderFault]="EncoderFault";
    fault[AXISFAULT_CommunicationLostFault]="CommunicationLostFault";
    fault[AXISFAULT_GantryMisalignFault]="GantryMisalignFault";
    fault[AXISFAULT_FeedbackScalingFault]="FeedbackScalingFault";
    fault[AXISFAULT_MarkerSearchFault]="MarkerSearchFault";
    fault[AXISFAULT_SafeZoneFault]="SafeZoneFault";
    fault[AXISFAULT_InPositionTimeoutFault]="InPositionTimeoutFault";
    fault[AXISFAULT_VoltageClampFault]="VoltageClampFault";
    return fault;
}

const QMap< unsigned int,QString> M_Axis_Fault =AxisFaultInit();






A3200Api::A3200Api(qint32 axisChannel):HMotionStage(axisChannel)
{
    lastError="unknown error";
    ++m_handleRefrenceCount;

    for(ulong k=0;k<48;++k){
        outPutData.insert(k,0);
    }

}

A3200Api::~A3200Api()
{
    stopProgramAndWait();
    stopAllAxis();

    if(m_handleRefrenceCount){
        if(--m_handleRefrenceCount)
            return;
    }


    if(m_handle){
        unInitMotionController();
        m_handle=nullptr;
    }
}

qint32 A3200Api::InitDevice(const QString &configFile)
{
    if(m_handle){
        return 0;
    }else{
        if(initMotionController(configFile)){
            return  0;
        }else{
            if(m_handle){
                unInitMotionController();
                m_handle=nullptr;
            }
            return  1;
        }
    }
}

qint32 A3200Api::SetAxisConfig( const AxisConfiguration &data)
{
    m_axisConfig=data;
    QPointF limit;
    double temp=0;
    if(A3200ParameterGetValue(m_handle,PARAMETERID::PARAMETERID_SoftwareLimitLow,DWORD(m_axisChannel),&temp)){
        limit.setX(temp);
        if(A3200ParameterGetValue(m_handle,PARAMETERID::PARAMETERID_SoftwareLimitHigh,DWORD(m_axisChannel),&temp)){
            limit.setY(temp);
            axisLimits.insert(static_cast<unsigned short>(m_axisChannel),limit);
            return !setParameter(m_axisChannel,PARAMETERID::PARAMETERID_ReverseMotionDirection,data.isReverse)?0:2;
        }
    }

    return  1;

}

qint32 A3200Api::AxisEnabled()
{
    return !IsAxisEnabled(m_axisChannel);
}

qint32 A3200Api::EnabledAxis( bool data)
{
    return !enableMotionAxis(m_axisChannel,data);
}

qint32 A3200Api::EnabledAllAxis(bool data)
{
    return !(data?enableAllAxis():disableAllAxis());
}

qint32 A3200Api::HomeAllAxis()
{
    return !homeAllAxis();
}

qint32 A3200Api::HomeAxis()
{
    return !homeAxis(m_axisChannel);
}

qint32 A3200Api::MoveTo( double pos)
{
    return !moveToPos(m_axisChannel,pos);
}

qint32 A3200Api::MoveTo( double pos, double vel, double acc, double dec, qint32 moveType)
{
    if(moveToPos(m_axisChannel,vel,pos,acc,dec)){
        if(moveType){
            return !waitAxisStop(m_axisChannel);
        }
        else{
            return 0;
        }
    }
    return 2;
}

qint32 A3200Api::MoveToWait( double pos)
{
    if(moveToPos(m_axisChannel,pos)){
        return !waitAxisStop(m_axisChannel);
    }
    return 2;
}

qint32 A3200Api::MoveContinue( bool direct)
{
    return !moveContinue(m_axisChannel,direct);
}

qint32 A3200Api::MoveStep( double step)
{
    return !moveIncStep(m_axisChannel,step);
}

qint32 A3200Api::MoveStepWait(double step)
{
    if(moveIncStep(m_axisChannel,step)){
       return !waitAxisStop(m_axisChannel,100000);
    }
    return 1;
}

qint32 A3200Api::WaitAllAxisStop(long timeOut)
{
    return !waitAllAxisStop(timeOut);
}

qint32 A3200Api::WaitAxisStop( long timeOut)
{
    return !waitAxisStop(m_axisChannel,timeOut);
}

qint32 A3200Api::StopAxis()
{
    return !stopAxis(m_axisChannel);
}

qint32 A3200Api::StopAllAxis()
{
    return !stopAllAxis();
}

QString A3200Api::GetAxisStatusString()
{
    return getAxisStatus_String(m_axisChannel);
}

AxisStatus A3200Api::GetAxisStatus()
{
    int a3200sta=getAxisStatus(m_axisChannel);


    if(a3200sta==0){
        return AxisStatus::Ready_AXIS;
    }
    else if(a3200sta<3){
        return AxisStatus::Moving_AXIS;
    }
    else{
        return AxisStatus::Warning_AXIS;
    }
}

double A3200Api::GetFeedbackPos()
{
    return getFeedBackPos(m_axisChannel);
}


QString A3200Api::GetFeedbackPosString()
{
    return getFeedBackPosString(m_axisChannel);
}

double A3200Api::GetVel()
{
    return getFeedbackVel(m_axisChannel);
}

QString A3200Api::GetVelString()
{
    return getFeedbackVelString(m_axisChannel);
}


qint32 A3200Api::SetIOConfig(const IOConfiguration &data)
{
    m_IODeviceConfig=data;
    return 0;
}



qint32 A3200Api::GetInputBit( ulong &data)
{
    bool isOK=1;
    ulong m_data=0;
    isOK&=getInput(ushort(m_axisChannel),ulong(m_IODeviceConfig.channel),m_data);
    data=m_IODeviceConfig.isReverse?!m_data:m_data;
    return !isOK;
}

qint32 A3200Api::GetOutputBit( ulong &data)
{
    ulong m_data=outPutData.value(ulong(m_IODeviceConfig.channel));
    data=m_IODeviceConfig.isReverse?!m_data:m_data;
    return 0;
}

qint32 A3200Api::SetOutputBit(ulong data)
{
    return !setOutput(ushort(m_axisChannel),ulong(m_IODeviceConfig.channel),m_IODeviceConfig.isReverse?!data:data);
}

qint32 A3200Api::RunProgramFile(const QString &programFile)
{
    return !runProgram(programFile.toLocal8Bit());
}

qint32 A3200Api::RunProgramFileWait(const QString &programFile)
{
    return !runProgramAndWait(programFile.toLocal8Bit());
}

qint32 A3200Api::GetProgramStatus()
{
    return !getProgramStatus();
}

qint32 A3200Api::StopProgramWait()
{
    return !stopProgramAndWait();
}

qint32 A3200Api::RegistrFirstCallBack()
{
    return !registrFirstCall();
}

qint32 A3200Api::StartWaitCallback(QVariantList &args)
{
    return !waitCallbackMessage(args);
}

qint32 A3200Api::EndCallback(double returnCode)
{
    return !callbackDone(returnCode);
}

qint32 A3200Api::ExitCallbackWait()
{
    return !callBackWaitCancle();
}












inline QString A3200Api::getA3200LastError() const
{
    char p[1024]="A3200 error";
    if(A3200GetLastErrorString(p, 1024))
        return  QString(p);
    return "get error failed!";
}




inline bool A3200Api::initMotionController(const QString&configFile)
{
    if(A3200Connect(&m_handle)){
        QEventLoop loop;
        QTimer::singleShot(400, &loop, SLOT(quit()));
        loop.exec();

        if(!configFile.isEmpty()){
            A3200ConfigurationHandle config=nullptr;
            if(A3200ConfigurationOpen(&config)){
                if(A3200ConfigurationParameterFileSet(config,configFile.toLocal8Bit().data())){
                    if(A3200ConfigurationSave(config)){
                        if(A3200ConfigurationClose(config)){
                            if(!A3200Reset(m_handle)){
                                return false;
                            }

                        }else{
                            return false;
                        }
                    }else{
                        return false;
                    }
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }

        if(A3200AcknowledgeAll(m_handle, TASKID_01)){
            //if(A3200Reset(m_handle)){
            BOOL isInitialized=0;
            if(A3200IsInitialized(&isInitialized)){
                return isInitialized;
            }
            // }
        }
    }
    lastError=getA3200LastError();
    return false;
}

inline bool A3200Api::clearAllError() const
{
    if(A3200AcknowledgeAll(m_handle, TASKID_Library))
        return true;
    return false;

}

inline bool A3200Api::unInitMotionController() const
{
    if(A3200MotionDisable(m_handle, TASKID_Library, allAxisMask))
        if(A3200Disconnect(m_handle))
            return true;
    return false;
}


inline bool A3200Api::enableMotionAxis(int axisIndex, bool data) const
{
    if(data){
        if(A3200MotionEnable(m_handle, TASKID_Library, AXISMASK(1<<static_cast<unsigned short>(axisIndex))))
            return true;
    }
    else{
        if(A3200MotionDisable(m_handle, TASKID_Library, AXISMASK(1<<static_cast<unsigned short>(axisIndex))))
            return true;
    }
    return false;
}

inline bool A3200Api::enableAllAxis() const
{ 
    if(A3200MotionEnable(m_handle, TASKID_Library, allAxisMask))
        return true;
    return false;
}


inline bool A3200Api::disableMotionAxis(unsigned short axisIndex) const
{
    if(A3200MotionDisable(m_handle, TASKID_Library,AXISMASK(1<<axisIndex)))
        return true;
    return false;
}

inline bool A3200Api::disableAllAxis() const
{
    if(A3200MotionDisable(m_handle, TASKID_Library, allAxisMask))
        return true;
    return false;
}

inline bool A3200Api::homeAxis(int axisIndex) const
{
    if(isAxisHomed(axisIndex))
        return true;
    if(A3200MotionHome(m_handle, TASKID_Library, AXISMASK(1<<static_cast<unsigned short>(axisIndex))))
        return true;
    return false;
}

inline bool A3200Api::homeAxisForce(int axisIndex) const
{
    if(A3200MotionHome(m_handle, TASKID_Library, AXISMASK(1<<static_cast<unsigned short>(axisIndex))))
        return true;
    return false;
}

inline bool A3200Api::isAxisHomed(int axisIndex) const
{
    DOUBLE Axis_status;
    if(A3200StatusGetItem(m_handle,static_cast<unsigned short>(axisIndex), STATUSITEM_AxisStatus, 0, &Axis_status)){
        if ((static_cast<long>(Axis_status) & AXISSTATUS_Homed))
            return true;
    }

    return false;
}

inline bool A3200Api::IsAxisEnabled(int axisIndex)
{
    return getAxisStatus(axisIndex)<3;
}

inline bool A3200Api::homeAllAxis() const
{
    if(A3200MotionHome(m_handle, TASKID_Library, allAxisMask))
        return true;
    return false;
}

inline bool A3200Api::stopAxis(int axisIndex) const
{
    unsigned short axis=static_cast<unsigned short>(axisIndex);
    if(A3200MotionAbort(m_handle,  AXISMASK(1<<axis)))
        return true;
    return false;
}

inline bool A3200Api::stopAllAxis() const
{
    if(A3200MotionAbort(m_handle, allAxisMask))
        return true;
    return false;
}

inline bool A3200Api::moveIncStep(unsigned short axisIndex, double vel, double step, double acc, double dec) const
{
    //acc*=0.1;dec*=0.1;
    double pos=0;
    getFeedBackPos(axisIndex,pos);
    pos+=step;
    auto limt=axisLimits.value(axisIndex);
    if(pos>limt.x()&&pos<limt.y()){
        if(A3200MotionSetupRampRateAccelAxis(m_handle, TASKID_Library,AXISMASK(1<<axisIndex),&acc))
            if(A3200MotionSetupRampRateDecelAxis(m_handle, TASKID_Library,AXISMASK(1<<axisIndex),&dec))
                if(A3200MotionMoveInc(m_handle, TASKID_Library, AXISINDEX(axisIndex), step, vel))
                    return true;
    }

    return false;
}

inline bool A3200Api::moveIncStep(int axisIndex, double step) const
{
    unsigned short axis=static_cast<unsigned short>(axisIndex);
    double pos=0,vel=m_axisConfig.vel,acc=m_axisConfig.acc,dec=m_axisConfig.acc;

    getFeedBackPos(axis,pos);
    pos+=step;
    auto limt=axisLimits.value(axis);
    if(pos>limt.x()&&pos<limt.y()){
        if(A3200MotionSetupRampRateAccelAxis(m_handle, TASKID_Library,AXISMASK(1<<axis),&acc))
            if(A3200MotionSetupRampRateDecelAxis(m_handle, TASKID_Library,AXISMASK(1<<axis),&dec))
                if(A3200MotionMoveInc(m_handle, TASKID_Library, AXISINDEX(axis), step, vel))
                    return true;
    }
    return false;
}

inline bool A3200Api::moveContinue(int axisIndex, bool direct) const
{
    unsigned short axis=static_cast<unsigned short>(axisIndex);
    double vel=m_axisConfig.vel,acc=m_axisConfig.acc,dec=m_axisConfig.acc;

    auto limt=axisLimits.value(axis);
    double pos=direct?limt.y():limt.x();
    if(A3200MotionSetupRampRateAccelAxis(m_handle, TASKID_Library,AXISMASK(1<<axis),&acc))
        if(A3200MotionSetupRampRateDecelAxis(m_handle, TASKID_Library,AXISMASK(1<<axis),&dec))
            if(A3200MotionMoveAbs(m_handle, TASKID_Library, AXISINDEX(axis), pos, vel))
                return true;

    return false;
}

inline bool A3200Api::moveToPos(int axisIndex, double vel, double pos, double acc, double dec) const
{
    //acc*=0.1;dec*=0.1;
    auto limt=axisLimits.value(static_cast<unsigned short>(axisIndex));
    if(pos>limt.x()&&pos<limt.y()){
        if(A3200MotionSetupRampRateAccelAxis(m_handle, TASKID_Library,AXISMASK(1<<axisIndex),&acc))
            if(A3200MotionSetupRampRateDecelAxis(m_handle, TASKID_Library,AXISMASK(1<<axisIndex),&dec))
                if(A3200MotionMoveAbs(m_handle, TASKID_Library, AXISINDEX(axisIndex), pos, vel))
                    return true;
    }
    return false;
}

inline bool A3200Api::moveToPos(int axisIndex, double pos) const
{
    unsigned short axis=static_cast<unsigned short>(axisIndex);
    double vel=m_axisConfig.vel,acc=m_axisConfig.acc,dec=m_axisConfig.acc;

    auto limt=axisLimits.value(axis);
    if(pos>limt.x()&&pos<limt.y()){
        if(A3200MotionSetupRampRateAccelAxis(m_handle, TASKID_Library,AXISMASK(1<<axis),&acc))
            if(A3200MotionSetupRampRateDecelAxis(m_handle, TASKID_Library,AXISMASK(1<<axis),&dec))
                if(A3200MotionMoveAbs(m_handle, TASKID_Library, AXISINDEX(axis), pos, vel))
                    return true;
    }

    return false;
}

inline bool A3200Api::waitAxisStop(int axisIndex,  long timeOutms)
{
    long a=0;
    timeOutms/=20;
    while (isAxisMove(axisIndex)) {
        QEventLoop loop;
        QTimer::singleShot(20, &loop, SLOT(quit()));
        loop.exec();
        ++a;
        if(a>timeOutms){
            lastError=QObject::tr("wait stop timeout");
            return false;
        }
    }

    if(getAxisStatus(axisIndex)>0){
        return false;
    }

    QEventLoop loop;
    QTimer::singleShot(qint32(m_axisConfig.delayTime*1000), &loop, SLOT(quit()));
    loop.exec();

    return true;
}

inline bool A3200Api::waitAllAxisStop( long timeOutms)
{
    long a=0;
    while (1){
        for(WORD k=0;k<axisCount;++k){
            if(isAxisMove(k))
                break;
            else{
                if(k==axisCount-1)
                    return true;
            }
        }
        Sleep(50);
        ++a;
        if(a>timeOutms/50){
            lastError=QObject::tr("wait stop timeout");
            return false;
        }
    }

}


inline bool A3200Api::setParameter(int axisIndex, PARAMETERID parameterId, double data) const
{
    DOUBLE ddata=0.0;
    if(A3200ParameterGetValue(m_handle,parameterId,static_cast<DWORD>(axisIndex),&ddata)){
        if(qAbs(ddata-data)<0.00000001){
            return true;
        }
        else{
            return  A3200ParameterSetValue(m_handle,parameterId,static_cast<DWORD>(axisIndex),data);
        }
    }
    else{
        return false;
    }
}


inline bool A3200Api::getCommandPos(unsigned short axisIndex,double &Pos) const
{
    if(A3200StatusGetItem(m_handle, axisIndex, STATUSITEM_ProgramPositionCommand, 0, &Pos))
        return true;
    return false;
}

inline bool A3200Api::getFeedBackPos(unsigned short axisIndex,double &Pos) const
{
    if(A3200StatusGetItem(m_handle, axisIndex, STATUSITEM_ProgramPositionFeedback, 0, &Pos))
        return true;
    return false;
}

inline double A3200Api::getFeedBackPos(int axisIndex) const
{
    double pos=0.0;
    if(A3200StatusGetItem(m_handle, static_cast<unsigned short>(axisIndex), STATUSITEM_ProgramPositionFeedback, 0, &pos))
        return pos;
    return 0.0;
}

inline QString A3200Api::getFeedBackPosString(int axisIndex) const
{
    double pos=0.0;
    if(A3200StatusGetItem(m_handle, static_cast<unsigned short>(axisIndex), STATUSITEM_ProgramPositionFeedback, 0, &pos))
        return QString::number(pos,'f',6);
    else{
        return "0.000000";
    }
}

inline bool A3200Api::getFeedbackVel(unsigned short axisIndex,double &Pos) const
{
    if(A3200StatusGetItem(m_handle, axisIndex, STATUSITEM_ProgramVelocityFeedback, 0, &Pos))
        return true;
    return false;
}

inline double A3200Api::getFeedbackVel(int axisIndex) const
{
    double vel=0.0;
    if(A3200StatusGetItem(m_handle, static_cast<unsigned short>(axisIndex), STATUSITEM_ProgramVelocityFeedback, 0, &vel))
        return vel;
    return 0.0;
}

QString A3200Api::getFeedbackVelString(int axisIndex) const
{
    double vel=0.0;
    if(A3200StatusGetItem(m_handle, static_cast<unsigned short>(axisIndex), STATUSITEM_ProgramVelocityFeedback, 0, &vel))
        return QString::number(vel,'f',6);
    return "0.000000";
}

inline bool A3200Api::isAxisMove(int axisIndex) const
{
    DOUBLE axis_status;
    unsigned short axis=static_cast<unsigned short>(axisIndex);
    if(A3200StatusGetItem(m_handle, axis, STATUSITEM_AxisStatus, 0, &axis_status)){
        return !(static_cast<long>(axis_status) & AXISSTATUS_MoveDone);
    }
    else{
        return false;
    }
}

inline bool A3200Api::loadProgram(const char *programPath) const
{
    if(A3200ProgramRun(m_handle, TASKID_01,programPath))
        return true;
    return false;
}


inline bool A3200Api::runProgram(const char * programPath) const
{
    if(A3200ProgramRun(m_handle, TASKID_01,programPath))
        return true;
    return false;
}

inline bool A3200Api::runProgramAndWait(const char *programPath) const
{
    if(A3200ProgramRun(m_handle, TASKID_01,programPath)){

        while (true) {

            QEventLoop loop;
            QTimer::singleShot(500, &loop, SLOT(quit()));
            loop.exec();

            TASKSTATE taskState;

            if(!A3200ProgramGetTaskState(m_handle, TASKID_01, &taskState)){
                return false;
            }
            if(taskState==TASKSTATE_ProgramRunning||taskState==TASKSTATE_ProgramReady||taskState==TASKSTATE_ProgramPaused){
                continue;
            }
            else if(taskState==TASKSTATE_ProgramComplete/*||taskState==TASKSTATE_Idle*/){
                return true;
            }
            else{
                //                 char p[1024]="A3200 error";
                //                 A3200ProgramGetTaskStateString(m_handle, TASKID_01,p,1024);
                //                 qDebug()<<"a32002"<<taskState<<p;
                return false;
            }

        }
    }
    return false;

}

inline bool A3200Api::startProgram() const
{
    if(A3200ProgramStart(m_handle, TASKID_01))
        return true;
    return false;
}

inline bool A3200Api::stopProgram() const
{
    if(A3200ProgramStop(m_handle, TASKID_01))
        return true;
    return false;
}

inline bool A3200Api::stopProgramAndWait(unsigned long timeOut) const
{
    if(A3200ProgramStopAndWait(m_handle, TASKID_01,timeOut))
        return true;

    return false;

}

inline bool A3200Api::pauseProgramAndWait(unsigned long timeOut) const
{
    if(A3200ProgramPauseAndWait(m_handle, TASKID_01,timeOut))
        return true;
    return false;
}

inline bool A3200Api::registrFirstCall(unsigned long callbackNumber) const
{
    return  A3200CallbackRegistrationAdd(m_handle, TASKID_01, callbackNumber);
}



inline bool A3200Api::waitCallbackMessage(QVariantList &args, unsigned long callbackNumber)
{
    QMutexLocker locker(&mutex1);
    TASKID taskId=TASKID_01;
    DWORD argsCount=0;
    CALLBACKARGUMENTTYPE argType;


    //if(A3200CallbackRegistrationAdd(m_handle, TASKID_01, callbackNumber))
    if(A3200CallbackWait(m_handle, &taskId, &callbackNumber, INFINITE))
        if(A3200CallbackArgsGetCount(m_handle, taskId, &argsCount)){

            for (DWORD counter = 0; counter < argsCount; counter++) {
                if(A3200CallbackArgsGetType(m_handle, taskId, counter, &argType)){
                    if (argType == CALLBACKARGUMENTTYPE_Double) {

                        DOUBLE argValue;
                        if(A3200CallbackArgsGetDouble(m_handle, taskId, counter, &argValue)){
                            args<<argValue;
                        }else{
                            return false;
                        }

                    }
                    else if(argType== CALLBACKARGUMENTTYPE_String)
                    {
                        char argValue[1024];
                        if(A3200CallbackArgsGetString(m_handle, taskId, counter, argValue,1024)){
                            args<<QString(argValue);
                        }else{
                            return false;
                        }

                    }else{
                        return false;
                    }
                }else{
                    return false;
                }

            }
            return true;
        }

    return false;
}

inline bool A3200Api::callBackWaitCancle() const
{
    return  A3200CallbackWaitCancel(m_handle);
}

inline bool A3200Api::callbackDone(double returnData, unsigned long callbackNumber)
{
    QMutexLocker locker(&mutex2);
    if(A3200CallbackRegistrationRemove(m_handle, TASKID_01, callbackNumber)){
        if(A3200CallbackRegistrationAdd(m_handle, TASKID_01, callbackNumber))
            return A3200CallbackReturnDouble(m_handle, TASKID_01, returnData, A3200GetLastError(), 0.0,0.0);
    }
    return false;
}

inline int A3200Api::getProgramStatus()
{
    QMutexLocker locker(&mutex3);

    TASKSTATE taskState;
    A3200ProgramGetTaskState(m_handle, TASKID_01, &taskState);

    return TASKSTATE(taskState);
}



inline int A3200Api::getAxisStatus(int AxisIndex)
{
    QMutexLocker locker(&mutex4);
    unsigned short axis=static_cast<unsigned short>(AxisIndex);

    DOUBLE Axis_status=0;
    BOOL isInitialized=TRUE;
    if(A3200IsInitialized(&isInitialized)){
        if(isInitialized){
            if(A3200StatusGetItem(m_handle,axis, STATUSITEM_AxisFault,0, &Axis_status)){
                if(static_cast<unsigned long>(Axis_status)==0){
                    if(A3200StatusGetItem(m_handle,axis, STATUSITEM_DriveStatus,0, &Axis_status)){
                        if ((static_cast<unsigned long>(Axis_status) & DRIVESTATUS_Enabled)){
                            if(A3200StatusGetItem(m_handle,axis, STATUSITEM_AxisStatus,0, &Axis_status)){
                                if ((static_cast<unsigned long>(Axis_status) & AXISSTATUS_Homing)){
                                    return 1;//homing
                                }
                                else if((static_cast<unsigned long>(Axis_status) & AXISSTATUS_MoveDone)){
                                    return 0;//ready
                                }
                                else{
                                    return 2;//moving
                                }
                            }
                        }
                        else{
                            return 3;//disable
                        }
                    }
                }
                else{
                    return 4;//axis error
                }
            }
        }
    }
    return 5;//disconnected
}

inline QString A3200Api::getAxisStatus_String(int axis)
{
    QMutexLocker locker(&mutex5);

    DOUBLE Axis_status=0;
    BOOL isInitialized=TRUE;
    unsigned short AxisIndex=static_cast<WORD>(axis);
    if(A3200IsInitialized(&isInitialized)){
        if(isInitialized){
            if(A3200StatusGetItem(m_handle,AxisIndex, STATUSITEM_AxisFault,0, &Axis_status)){
                if(static_cast<unsigned int>(Axis_status)==0){
                    if(A3200StatusGetItem(m_handle,AxisIndex, STATUSITEM_DriveStatus,0, &Axis_status)){
                        if ((static_cast<unsigned int>(Axis_status) & DRIVESTATUS_Enabled)){
                            if(A3200StatusGetItem(m_handle,AxisIndex, STATUSITEM_AxisStatus,0, &Axis_status)){
                                if ((static_cast<unsigned int>(Axis_status) & AXISSTATUS_Homing)){
                                    return "Homing";//homing
                                }
                                else if((static_cast<unsigned int>(Axis_status) & AXISSTATUS_Jogging)){
                                    return "Moving";//moving
                                }
                                else if((static_cast<unsigned int>(Axis_status) & AXISSTATUS_MoveDone)){
                                    return "Ready";//enabled
                                }
                                else if((static_cast<unsigned int>(Axis_status) & AXISSTATUS_Profiling)){
                                    return "Profiling";//enabled
                                }
                                else if((static_cast<unsigned int>(Axis_status) & AXISSTATUS_Homed)){
                                    return "Homed";//homed
                                }
                                else{
                                    return "Enabled";//enabled
                                }

                            }
                        }
                        else{
                            return "Disable";//disable
                        }
                    }
                }
                else{

                    if(A3200StatusGetItem(m_handle,AxisIndex, STATUSITEM_AxisFault,0, &Axis_status)){
                        return M_Axis_Fault.value(static_cast<unsigned int>(Axis_status));
                    }
                }
            }
        }
    }
    return "Disconnected";//disconnected
}


inline bool A3200Api::setOutput(unsigned short axisIndex,unsigned long outPutIndex, unsigned long nValue)
{
    QMutexLocker locker(&mutex6);

    if(A3200IODigitalOutputBit(m_handle, TASKID_01, outPutIndex, AXISINDEX(axisIndex), nValue)){
        outPutData[outPutIndex]=nValue;
        return true;
    }
    lastError=getA3200LastError();
    return false;

}

inline bool A3200Api::getInput(unsigned short axisIndex,unsigned long inPutIndex, unsigned long &nValue)
{
    QMutexLocker locker(&mutex7);

    if(A3200IODigitalInputBit(m_handle, TASKID_01, inPutIndex, AXISINDEX(axisIndex), &nValue))
        return true;
    lastError=getA3200LastError();
    return false;

}
