#include <stable.h>

#include "Src/Motion/IO/hiodevice.h"
#include "hmotionstage.h"

class GtMotion :public HMotionStage ,public HIODevice
{
public:

     explicit GtMotion(qint32 axisChannel);
    ~GtMotion();

    qint32 IsReady(qint32 cardIndex=0);

    qint32 InitDevice(const QString &configFile);

    qint32 SetAxisConfig(const AxisConfiguration& data);


    qint32 AxisEnabled();

    qint32 EnabledAxis(bool data) ;
    qint32 EnabledAllAxis(bool data) ;

    qint32 HomeAllAxis();
    qint32 HomeAxis();
    qint32 MoveTo(double pos) ;
    qint32 MoveTo( double pos, double vel, double acc, double dec,qint32 moveType);
    qint32 MoveToWait(double pos);

    qint32 MoveContinue(bool direct) ;
    qint32 MoveStep(double step) ;
    qint32 MoveStepWait(double step) ;

    qint32 WaitAllAxisStop(long timeOut=30000);
    qint32 WaitAxisStop(long timeOut=100000);

    qint32 StopAxis() ;
    qint32 StopAllAxis() ;

    QString GetAxisStatusString();
    AxisStatus GetAxisStatus();

    double GetFeedbackPos() ;
    qint32 GetFeedBackPosCounts();
    qint32 GetFeedBackPosCounts1();
    QString GetFeedbackPosString() ;


    double GetVel() ;
    QString GetVelString() ;


    qint32 TestFunc();


    qint32 SetIOConfig(const IOConfiguration& data);

    qint32 GetInputBit(ulong&data);
    qint32 GetOutputBit(ulong&data);

    qint32 SetOutputBit(ulong data);


private:


    inline bool init(const QString& ConfigPath, int axisStart=1, int axisNum=4);//card init



    /******************there units are mm and mm/s**********************/

    inline bool moveTo(int axis,double pos);
    inline bool moveToStep(int axis,double pos);
    inline bool moveTo(int axis, double vel, double pos);
    inline bool moveToStep(int axis, double vel, double pos);
    inline bool moveContinue(int axis,bool direct);
    inline bool stepMove(short axis, double vel, double step);
    inline bool stepMove(int axis, double step);

    inline bool homeCapt(int axisIndex, double pos, double vel);
    inline bool getCurPosMM(short axis, double &pos);
    inline double getCurPosMM(int axis);
    inline bool getCurVelMM(short axisStart, double &vel);
    inline double getCurVelMM(int axis);
    inline bool getCmdPosMM(short axisStart, double &pos, short axisNum=1);
    inline bool getCmdVelMM(short axisStart, double &vel, short axisNum=1);

    inline void setPointMoveVel(int axis,double vel);
    inline bool setPointMovePrm(int axis,double Acc=0.25,double Dec=0.125,double velStart=0, short SmoothTime=25);

    /********************************************************************/

    inline bool isAxisInit(short axisStart, short axisNum=1);
    inline bool isAxisEnabled(int axis);
    inline bool enableAxis(int axis,bool data) const;
    inline bool enableAllAxis(bool data) const;
    inline bool waitAxisStop(int axis, long timeout=100000);
    inline bool isMove(short axis);
    inline void axisControl(short axis, bool mode); //0:turn off  1:turn on

    inline void close();
    inline bool stop(long axis, long style=0);
    inline bool stopAll( long style=0);

    inline bool getAxisLimit(short axis,QPointF& limit);

    inline void setPluseRadio(qint32 data){pluseRadio= data;}

    inline bool getTrapPrm(short axis, double&Acc, double& VelStart,double &Dec, double&SmoothTime);
    inline bool setTrapPrm(short axis,double Acc=0.25,double velStart=0,double Dec=0.125, short SmoothTime=25);
    inline bool getJogPrm(short axis,double&Acc,double &Dec,double&SmoothTime);
    inline  bool setJogPrm(short axis, double Acc=0.25, double Dec=0.125, short SmoothTime=25);

    inline bool clearError(int axis) const;

    inline  bool posZero(short axis);
    inline  short getAxisStatus(int axis);
    inline  QString getAxisStatus_String(int axis);

    inline  long getInputBits(short type);
    inline long getOutputBits(short type);
    inline bool getInput(short type, int num,ulong &data);//  type 0：正限位。1：负限位。：驱动报警。 3：原点开关。 4：通用输入。5：电机到位信号。 6：手轮 MPG 轴选和倍率信号（5V 电平输入）。
    inline bool getOutput(short type, int num,ulong &data);// 10：驱动器使能。 11：报警清除。 12：通用输出。
    inline bool setOutput(short type, int num, ulong bit); //10：驱动器使能。 11：报警清除。 12：通用输出。

private:



    QMutex mutex;
    QHash<short, QPoint> axisLimit;
    QHash<int, double> pointMoveSpeeds;

    QMap<qint32,IOConfiguration> m_OutputConfig;
    QMap<qint32,IOConfiguration> m_InputConfig;

     static quint32 m_handleRefrenceCount;

    long pluseRadio=1000;//  pluse per mm

    const long axisMaxNum=4;


    inline short isMoveDone(short axis);
    inline bool homeMove(short axis,double vel,long pos) const;



    /************************************there unit are pluse and ms************************************/
    inline  bool pointMove(short axis, double vel, long pos);
    inline  bool pointMove(short axis, long pos);
    inline  bool pointMoveStep(short axis, double vel,long pos);
    inline  bool pointMoveStep(short axis, long pos);


    inline bool getCmdVel(short axisStart, double &Vel, short axisNum);//get cmd pluse vel
    inline bool getCmdPos(short axisStart,double &Pos,short axisNum);//get cmd pluse pos
    inline bool getCmdAcc(short axisStart, double &Acc, short axisNum);//get cmd pluse acc
    inline bool getCurVel(short axisStart, double &Vel, short axisNum);//get cur pluse vel
    inline bool getCurPos(short axisStart,double &Pos,short axisNum) const;//get cur pluse pos
    inline bool getCurAcc(short axisStart,double &Acc,short axisNum);//get cur pluse acc


    inline bool getMotionMode(short axisStart, QString &modename, short axisNum); //get motion mode
    inline bool setMotionMode(short axis, short mode); //set motion mode



    inline bool comparePos(short axis, long Buf1[20], short num, short pulse);
    inline bool setCapturePos(short axis , long pos, double encPos);

    inline bool JogMove(short axis, double Vel);
    inline bool getCurError(short axisStart, double &Error, short axisNum);//get cur pos error

    /*************************************************************************************************/

};
