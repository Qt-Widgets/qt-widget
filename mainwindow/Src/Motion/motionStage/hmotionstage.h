#ifndef HMOTIONSTAGE_H
#define HMOTIONSTAGE_H

#include "stable.h"


enum AxisStatus{
    Ready_AXIS=0,
    Moving_AXIS,
    Warning_AXIS
};

struct AxisConfiguration
{
    AxisConfiguration(double mvel=30,double macc=100,double dtime=0.1,double misReverse=1.0,int m_pluse=1000){
        vel=mvel;
        acc=macc;
        delayTime=dtime;
        isReverse=misReverse;
        pluseRadio=m_pluse;
    }
    double vel;
    double acc;
    double delayTime;
    double isReverse;
    int pluseRadio;
};


class HMotionStage
{
public:
    explicit HMotionStage(qint32 axisChannel);

    virtual ~HMotionStage();


    virtual qint32 InitDevice(const QString &configFile) ;

    virtual qint32 SetAxisConfig(const AxisConfiguration& data);


    virtual qint32 AxisEnabled();

    virtual qint32 EnabledAxis(bool data) ;
    virtual qint32 EnabledAllAxis(bool data) ;

    virtual qint32 HomeAllAxis() ;
    virtual qint32 HomeAxis() ;
    virtual qint32 MoveTo(double pos,double vel,double acc,double dec,qint32 moveType) ;
    virtual qint32 MoveTo(double pos) ;
    virtual qint32 MoveToWait(double pos);
    virtual qint32 MoveContinue(bool direct) ;
    virtual qint32 MoveStep(double step) ;
    virtual qint32 MoveStepWait(double step) ;

    virtual qint32 WaitAllAxisStop(long timeOut=30000);
    virtual qint32 WaitAxisStop(long timeOut=100000) ;

    virtual qint32 StopAxis() ;
    virtual qint32 StopAllAxis();

    virtual QString GetAxisStatusString() ;
    virtual AxisStatus GetAxisStatus() ;

    virtual double GetFeedbackPos();
    virtual qint32 GetFeedBackPosCounts();
    virtual QString GetFeedbackPosString();



    virtual double GetVel();
    virtual QString GetVelString();

    virtual qint32 TestFunc();



    virtual qint32 RunProgramFile(const QString &programFile);
    virtual qint32 RunProgramFileWait(const QString &programFile);
    virtual qint32 GetProgramStatus();
    virtual qint32 StopProgramWait();

    virtual qint32 RegistrFirstCallBack();
    virtual qint32 StartWaitCallback(QVariantList&args);
    virtual qint32 EndCallback(double returnCode);
    virtual qint32 ExitCallbackWait();

protected:

    AxisConfiguration m_axisConfig;
    qint32 m_axisChannel;



private:
    QMap<qint32,double> m_simulationPoss;

    double getDistanceTime(double step,double acc,double dec,double vel);

    bool isWaitCallBack=false;
};

#endif // HMOTIONSTAGE_H
