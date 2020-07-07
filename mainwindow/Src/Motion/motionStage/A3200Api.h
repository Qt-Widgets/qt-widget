#include <stable.h>

#include "hmotionstage.h"
#include "Src/Motion/IO/hiodevice.h"

#include <A3200ParameterId.h>




class A3200Api : public HMotionStage,public HIODevice
{

public:
    explicit A3200Api(qint32 axisChannel);
    ~A3200Api();

    qint32 InitDevice(const QString &configFile);
    qint32 SetAxisConfig(const AxisConfiguration& data);


    qint32 AxisEnabled();

    qint32 EnabledAxis(bool data) ;
    qint32 EnabledAllAxis(bool data) ;

    qint32 HomeAllAxis() ;
    qint32 HomeAxis() ;
    qint32 MoveTo(double pos) ;
    qint32 MoveTo( double pos, double vel, double acc, double dec,qint32 moveType);
    qint32 MoveToWait(double pos);

    qint32 MoveContinue(bool direct) ;
    qint32 MoveStep(double step) ;
    qint32 MoveStepWait(double step);

    qint32 WaitAllAxisStop(long timeOut=10000);
    qint32 WaitAxisStop(long timeOut=100000);

    qint32 StopAxis() ;
    qint32 StopAllAxis() ;

    QString GetAxisStatusString();
    AxisStatus GetAxisStatus();

    double GetFeedbackPos() ;
    QString GetFeedbackPosString() ;


    double GetVel() ;
    QString GetVelString();


    ///////////////////////////io///////////////////////////////////


    qint32 SetIOConfig(const IOConfiguration& data);


    qint32 GetInputBit(ulong&data);
    qint32 GetOutputBit(ulong&data);

    qint32 SetOutputBit(ulong data);

    ///////////////////////////////////////////////////////////////


    qint32 RunProgramFile(const QString &programFile);
    qint32 RunProgramFileWait(const QString &programFile);
    qint32 GetProgramStatus();
    qint32 StopProgramWait();

    qint32 RegistrFirstCallBack();
    qint32 StartWaitCallback(QVariantList&args);
    qint32 EndCallback(double returnCode);
    qint32 ExitCallbackWait();




private:

    inline QString getLastError() const {return lastError;}
    inline QString getA3200LastError() const;

    inline bool initMotionController(const QString&configFile=nullptr);
    inline bool clearAllError() const;
    inline bool enableMotionAxis(int axisIndex,bool data) const;
    inline bool enableAllAxis() const;
    inline bool disableMotionAxis(unsigned short axisIndex) const;
    inline bool disableAllAxis() const;
    inline bool unInitMotionController() const;
    inline bool homeAxis(int axisIndex) const;
    inline bool homeAxisForce(int axisIndex) const;
    inline bool isAxisHomed(int axisIndex) const;
    inline bool IsAxisEnabled(int axisIndex);
    inline bool homeAllAxis() const;
    inline bool stopAxis(int axisIndex) const;
    inline bool stopAllAxis() const;
    inline bool moveIncStep(unsigned short axisIndex, double vel, double step, double acc=100, double dec=100) const;
    inline bool moveIncStep(int axisIndex,  double step) const;
    inline bool moveContinue(int axisIndex,  bool direct) const;
    inline bool moveToPos(int axisIndex, double vel, double pos, double acc=100, double dec=100) const;
    inline bool moveToPos(int axisIndex, double pos) const;
    inline bool waitAxisStop(int axisIndex, long timeOutms=20000);
    inline bool waitAllAxisStop(long timeOutms=20000);

    inline bool setParameter(int axisIndex,PARAMETERID parameterId,double data) const;


    inline bool getCommandPos(unsigned short axisIndex, double &pos) const;
    inline bool getFeedBackPos(unsigned short axisIndex, double &pos) const;
    inline double getFeedBackPos(int axisIndex) const;
    inline QString getFeedBackPosString(int axisIndex) const;
    inline bool getFeedbackVel(unsigned short axisIndex, double &pos) const;
    inline double getFeedbackVel(int axisIndex) const;
    inline QString getFeedbackVelString(int axisIndex) const;
    inline bool isAxisMove(int axisIndex) const;


    inline bool loadProgram(const char * programPath) const;
    inline bool runProgram(const char * programPath) const;
    inline bool runProgramAndWait(const char * programPath) const;
    inline bool startProgram() const;
    inline bool stopProgram() const;
    inline bool stopProgramAndWait(unsigned long timeOut=1000) const;
    inline bool pauseProgramAndWait(unsigned long timeOut=1000) const;

    inline bool registrFirstCall(unsigned long callbackNumber=0) const;
    inline bool waitCallbackMessage(QVariantList& args,unsigned long callbackNumber=0);
    inline bool callBackWaitCancle() const;
    inline bool callbackDone(double returnData=0.0, unsigned long callbackNumber=0) ;

    inline int getProgramStatus();

    inline int getAxisStatus(int AxisIndex);
    inline QString getAxisStatus_String(int axis) ;

    inline bool setOutput(unsigned short axisIndex, unsigned long outPutIndex, unsigned long nValue);
    inline bool getInput(unsigned short axisIndex, unsigned long outPutIndex, unsigned long &nValue);

private:

    QMutex mutex1;
    QMutex mutex2;
    QMutex mutex3;
    QMutex mutex4;
    QMutex mutex5;
    QMutex mutex6;
    QMutex mutex7;

    QString lastError;

    static void* m_handle;
    static qint32 m_handleRefrenceCount;


    QMap<unsigned short,QPointF> axisLimits;


    QMap<ulong,ulong> outPutData;



};


