#include "GtMotion.h"
#include "gts.h"
#include <QThread>
#include <QStringList>
#include <QDebug>

#include "Gucoi.h"


quint32 GtMotion::m_handleRefrenceCount=0;


GtMotion::GtMotion(qint32 axisChannel):HMotionStage(axisChannel)
{
    ++m_handleRefrenceCount;
    for(long k=0;k<axisMaxNum;++k){
        pointMoveSpeeds.insert(k,1000);
    }

    for (qint32 k=0;k<16;++k) {
        m_OutputConfig.insert(k,IOConfiguration());
        m_InputConfig.insert(k,IOConfiguration());
    }

}

GtMotion::~GtMotion()
{
    StopAxis();
    if(m_handleRefrenceCount){
        if(--m_handleRefrenceCount)
            return;
    }
    setOutput(12,1,0);
    close();
}

qint32 GtMotion::IsReady(qint32 cardIndex)
{
    return GT_Open(short(cardIndex));
}



qint32 GtMotion::InitDevice(const QString &configFile)
{
    if(m_handleRefrenceCount>1){
        return 0;
    }else{
        return !init(configFile);
    }
}

qint32 GtMotion::SetAxisConfig( const AxisConfiguration &data)
{
    setPluseRadio(long(data.pluseRadio*(data.isReverse>0.5?-1.0:1.0)));
    setPointMoveVel(m_axisChannel+1,data.vel);
    return  !setPointMovePrm(m_axisChannel+1,data.acc,data.acc);
}

qint32 GtMotion::AxisEnabled()
{
    setOutput(12,1,1);
    return !isAxisEnabled(m_axisChannel+1);
}

qint32 GtMotion::EnabledAxis( bool data)
{
    return  !enableAxis(m_axisChannel+1,data);
}

qint32 GtMotion::EnabledAllAxis(bool data)
{
    return !enableAllAxis(data);
}

qint32 GtMotion::HomeAllAxis()
{

    for(short k=1;k<5;++k){

        if(GT_ClrSts(k))
            return 1;
        if(!homeCapt(k,25,60))
            return 2;
    }
    return 0;
}

qint32 GtMotion::HomeAxis()
{
    if(GT_ClrSts(short(m_axisChannel+1)))
        return 1;
    return !homeCapt(m_axisChannel+1,25,30);
}

qint32 GtMotion::MoveTo( double pos)
{
    return !moveTo(m_axisChannel+1,pos);
}

qint32 GtMotion::MoveTo( double pos, double vel, double acc, double dec, qint32 moveType)
{
    if(setPointMovePrm(m_axisChannel+1,acc,dec)){
        if(moveTo(m_axisChannel+1,vel,pos)){
            if(moveType){
                return  !waitAxisStop(m_axisChannel+1);
            }else{
                return 0;
            }
        }
    }
    return 2;
}

qint32 GtMotion::MoveToWait( double pos)
{
    if(moveTo(m_axisChannel+1,pos)){
        return  !waitAxisStop(m_axisChannel+1);
    }
    return 1;
}

qint32 GtMotion::MoveContinue( bool direct)
{
    return  !moveContinue(m_axisChannel+1,direct);
}

qint32 GtMotion::MoveStep( double step)
{
    return  !stepMove(m_axisChannel+1,step);
}

qint32 GtMotion::MoveStepWait(double step)
{
    if(stepMove(m_axisChannel+1,step)){
        return  !waitAxisStop(m_axisChannel+1);
    }
    return 1;
}

qint32 GtMotion::WaitAllAxisStop(long timeOut)
{
    for(short k=1;k<5;++k){
        if(!waitAxisStop(k,timeOut))
            return 1;
    }
    return 0;
}

qint32 GtMotion::WaitAxisStop( long timeOut)
{
    return !waitAxisStop(m_axisChannel+1,timeOut);
}

qint32 GtMotion::StopAxis()
{
    return !stop(m_axisChannel+1);
}

qint32 GtMotion::StopAllAxis()
{
    for(short k=1;k<5;++k){
        if(!stop(k))
            return 1;
    }
    return 0;
}

QString GtMotion::GetAxisStatusString()
{
    return getAxisStatus_String(m_axisChannel+1);
}

AxisStatus GtMotion::GetAxisStatus()
{
    short sta= getAxisStatus(m_axisChannel+1);
    if(sta<3){
        return AxisStatus::Ready_AXIS;
    }
    else if(sta==3){
        return AxisStatus::Moving_AXIS;
    }
    else{
        return AxisStatus::Warning_AXIS;
    }
}

double GtMotion::GetFeedbackPos()
{
    return getCurPosMM(m_axisChannel+1);
}

qint32 GtMotion::GetFeedBackPosCounts()
{
    double pos=0.0;
    if(!GT_GetAxisPrfPos(static_cast<short>(m_axisChannel+1),&pos,1)){
        return qint32(pos);
    }
    return -1;
}

qint32 GtMotion::GetFeedBackPosCounts1()
{
    double pos=0.0;
    if(!GT_GetAxisEncPos(static_cast<short>(m_axisChannel+1),&pos,1)){
        return qint32(pos);
    }
    return -1;
}

QString GtMotion::GetFeedbackPosString()
{
    return QString::number(getCurPosMM(m_axisChannel+1),'f',6);
}

double GtMotion::GetVel()
{
    return getCurVelMM(m_axisChannel+1);
}

QString GtMotion::GetVelString()
{
    return QString::number(getCurVelMM(m_axisChannel+1),'f',6);
}


void GT_Follow(short naxis, short maxis,short saxis, int number,int space, const QVector<double>& compensation)
{
    int masterPos;
    double slavePos;
    if (naxis == 1)//X轴补
    {
        masterPos = 0;
        slavePos = 0;
        //设置从轴为Follow模式
        GT_PrfFollow(saxis, 0);// 将从轴设为Follow模式，双向跟随
        GT_SetFollowMemory(saxis,1);
        GT_FollowClear(saxis, 0);// 清空从轴FIFO0
        GT_SetFollowMaster(saxis, maxis, FOLLOW_MASTER_PROFILE, 0);// 设置主轴，默认跟随主轴规划位置
        GT_SetFollowLoop(saxis, 0);// 设置循环次数为无限循环
        GT_SetFollowEvent(saxis, FOLLOW_EVENT_START, 1, 0);// 设置启动跟随条件

        for (int i = 0; i < number; i++)
        {
            masterPos += space;
            slavePos += compensation[i];
            GT_FollowData(saxis, masterPos, slavePos, FOLLOW_SEGMENT_NORMAL, 0);//以普通段方式向FIFO0增加数据
        }
        GT_FollowStart(1 << (saxis - 1), 0);// 启动从轴Follow运动
    }
    if (naxis == 2)//Y轴补
    {
        masterPos = 0;
        slavePos = 0;
        //设置从轴为Follow模式
        GT_PrfFollow(saxis, 0);// 将从轴设为Follow模式，双向跟随
        GT_SetFollowMemory(saxis,1);
        GT_FollowClear(saxis, 0);// 清空从轴FIFO0
        GT_SetFollowMaster(saxis, maxis, FOLLOW_MASTER_PROFILE, 0);// 设置主轴，默认跟随主轴规划位置
        GT_SetFollowLoop(saxis, 0);// 设置循环次数为无限循环
        GT_SetFollowEvent(saxis, FOLLOW_EVENT_START, 1, 0);// 设置启动跟随条件

        for (int i = 0; i < number; i++)
        {
            masterPos += space;
            slavePos += compensation[i];
            GT_FollowData(saxis, masterPos, slavePos, FOLLOW_SEGMENT_NORMAL, 0);//以普通段方式向FIFO0增加数据
        }
        GT_FollowStart(1 << (saxis - 1), 0);// 启动从轴Follow运动
    }
}

qint32 GtMotion::TestFunc()
{
    short master = 1;//X轴号
    short slave = 5;//U轴号
    short master2 = 2;//Y轴号
    short slave2 = 6;//V轴号

    short result=-1;

    QVector<double> CompX,CompY;

    CompX<<-0<<-0<<-0<<-5<<-3<<-2<<-1<<2<<-1<<1<<-4<<-1<<3<<6<<3<<-1;
    CompY<<0<<0<<6<<5<<4<<3<<5<<1<<1<<2<<1<<2<<-4<<-2<<-3<<-1;


    //1,设定U轴跟随X轴的跟随曲线
    GT_Follow(1, master, slave, 16, 20000, CompX);
    //2, 设定V轴跟随X轴的凸轮曲线
    GT_Follow(2, master, slave2, 16, 20000, CompY);
    return 0;
}



qint32 GtMotion::SetIOConfig(const IOConfiguration &data)
{
    m_IODeviceConfig=data;
    return 0;
}


qint32 GtMotion::GetInputBit(ulong &data)
{
    bool isOk=true;
    ulong m_data=0;
    isOk&=getInput(4,m_IODeviceConfig.channel,m_data);
    data=m_IODeviceConfig.isReverse?!m_data:m_data;
    return  !isOk;
}

qint32 GtMotion::GetOutputBit( ulong &data)
{

    bool isOk=true;
    ulong m_data=0;
    isOk&=getOutput(12,m_IODeviceConfig.channel,m_data);
    data=m_IODeviceConfig.isReverse?!m_data:m_data;
    return  !isOk;
}

qint32 GtMotion::SetOutputBit( ulong data)
{
    return !setOutput(12,m_IODeviceConfig.channel,m_IODeviceConfig.isReverse?!data:data);
}



bool GtMotion::init(const QString &ConfigPath, int axisStart, int axisNum)
{

    std::string str = ConfigPath.toStdString();
    char *p = const_cast<char*>(str.data());

    if(!GT_Open())
        if(!ConfigPath.isEmpty())
            if(!GT_Reset())
                if(!GT_LoadConfig(p))
                    if(!GT_ClrSts(static_cast<short>(axisStart),static_cast<short>(axisNum)))
                        return true;
    return false;
}

bool GtMotion::moveTo(int axis, double pos)
{
    return pointMove(static_cast<short>(axis),long(pos*pluseRadio+0.5));
}


bool GtMotion::moveToStep(int axis, double pos)
{
    return  pointMoveStep(static_cast<short>(axis),long(pos*pluseRadio+0.5));
}

bool GtMotion::moveTo(int axis, double vel, double pos)
{
    return pointMove(static_cast<short>(axis),vel/1000*pluseRadio,long(pos*pluseRadio+0.5));
}

bool GtMotion::moveToStep(int axis, double vel, double pos)
{
    return pointMoveStep(static_cast<short>(axis),vel/1000*pluseRadio,long(pos*pluseRadio+0.5));
}

bool GtMotion::moveContinue(int axis, bool direct)
{
    return moveTo(static_cast<short>(axis),direct?(axisLimit.value(static_cast<short>(axis)).y()-1000):(axisLimit.value(static_cast<short>(axis)).x()+1000));
}

bool GtMotion::stepMove(short axis, double vel, double step)
{
    if(qAbs(step)<(1.0/pluseRadio)){
        return true;
    }

    if(moveToStep(axis,vel,step))
        return true;
    return false;
}

bool GtMotion::stepMove(int axis, double step)
{
    if(qAbs(step)<(1.0/pluseRadio)){
        return true;
    }
    if(moveToStep(axis,step))
        return true;
    return false;
}

inline bool GtMotion::homeCapt(int axisIndex, double pos, double vel)
{
    double limitMin=0,limitMax=0;
    long rePos=long(pos*pluseRadio);
    vel=vel/1000*pluseRadio;
    short axis=static_cast<short>(axisIndex);

    if(homeMove(axis,vel,LONG_MAX/2*(rePos/qAbs(rePos))))
        if(homeMove(axis,vel,-8*pluseRadio*(rePos/qAbs(rePos))))
            if(homeMove(axis,double(pluseRadio)/1000.0,LONG_MAX/2*(rePos/qAbs(rePos))))
                if(homeMove(axis,vel,-LONG_MAX/2*(rePos/qAbs(rePos)))){
                    if(getCurPos(axis,limitMax,1)){
                        limitMax=qAbs(limitMax);
                        if(qAbs(rePos)>long(limitMax)-pluseRadio)
                            return false;
                        if(homeMove(axis,vel,8*pluseRadio*(rePos/qAbs(rePos))))
                            if(homeMove(axis,double(pluseRadio)/1000.0,-LONG_MAX/2*(rePos/qAbs(rePos))))
                                if(homeMove(axis,vel,rePos))
                                    if(getCurPos(axis,limitMin,1))
                                        if(!GT_ZeroPos(axis))
                                            if(!GT_SetPrfPos(axis, 0))
                                                if(!GT_ClrSts(axis)){
                                                    QPoint limit;
                                                    if(limitMin<0){
                                                        limit.setX(-long(limitMax)-long(limitMin)-pluseRadio);
                                                        limit.setY(-long(limitMin)+pluseRadio);
                                                    }
                                                    else{
                                                        limit.setX(-long(limitMin)-pluseRadio);
                                                        limit.setY(long(limitMax)-long(limitMin)+pluseRadio);
                                                    }
                                                    axisLimit.insert(axis,limit);
                                                    return true;
                                                }
                    }
                }
    return false;
}


bool GtMotion::getCurPosMM(short axis, double &pos)
{
    if(!GT_GetAxisEncPos(axis,&pos,1)){
        pos=pos/double(pluseRadio);

        return true;
    }
    return false;
}

double GtMotion::getCurPosMM(int axis)
{
    double pos=0.0;
    if(!GT_GetAxisEncPos(static_cast<short>(axis),&pos,1)){
        return pos=pos/double(pluseRadio);
    }
    return 0.0;
}

bool GtMotion::getCurVelMM(short axisStart, double &vel)
{
    if(!GT_GetAxisEncVel(axisStart,&vel,1)){
        vel=vel/double(pluseRadio)*1000;
        return true;
    }
    return false;
}

double GtMotion::getCurVelMM(int axis)
{
    double vel=0.0;
    if(!GT_GetAxisEncVel(static_cast<short>(axis),&vel,1)){
        return vel/double(pluseRadio)*1000;
    }
    return 0.0;
}

bool GtMotion::getCmdPosMM(short axisStart, double &pos, short axisNum)
{
    if(!GT_GetAxisPrfPos(axisStart,&pos,axisNum)){
        pos=pos/double(pluseRadio);
        return true;}
    return false;
}

bool GtMotion::getCmdVelMM(short axisStart, double &vel, short axisNum)
{
    if(!GT_GetAxisPrfVel(axisStart,&vel,axisNum)){

        vel=vel/double(pluseRadio)*1000;
        return true;
    }
    return false;
}

void GtMotion::setPointMoveVel(int axis, double vel)
{
    pointMoveSpeeds.insert(axis,vel/1000*pluseRadio);
}

bool GtMotion::setPointMovePrm(int axis, double Acc, double Dec, double velStart, short SmoothTime)
{
    return  setTrapPrm(static_cast<short>(axis),Acc*pluseRadio/1000000,velStart*pluseRadio/1000,Dec*pluseRadio/1000000,SmoothTime/1000);
}

bool GtMotion::isAxisInit(short axisStart,short axisNum)
{
    for (int i = axisStart; i < axisNum+1; i++)
    {
        if(getAxisStatus(axisStart)>3)
        {
            return false;
        }
    }
    return true;

}

bool GtMotion::isAxisEnabled(int axis)
{
    long lAxisStatus;
    if(GT_GetSts(static_cast<short>(axis), &lAxisStatus))
        return false;
    if(lAxisStatus & 0x200)
        return true;
    else{
        return false;
    }
}

bool GtMotion::enableAxis(int axis, bool data) const
{
    if(data){
        return !GT_AxisOn(static_cast<short>(axis));
    }
    else{
        return !GT_AxisOff(static_cast<short>(axis));
    }
}

bool GtMotion::enableAllAxis(bool data) const
{
    if(data){

        for(short k=1;k<5;++k){
            if(GT_AxisOn(k))
                return false;
        }
        return true;

    }else{
        for(short k=1;k<5;++k){
            if(GT_AxisOff(k))
                return false;
        }
        return true;
    }
}

bool GtMotion::waitAxisStop(int axis, long timeout)
{
    long sts;
    long a=0;
    short axisIndex=static_cast<short>(axis);
    do{
        if(a==timeout)
            return false;
        if(GT_GetSts(axisIndex, &sts))
            return false;
        QThread::msleep(10);
        a++;
    }
    while(sts&0x400);
    QEventLoop loop;
    QTimer::singleShot(qint32(m_axisConfig.delayTime*1000), &loop, SLOT(quit()));
    loop.exec();
    return true;

}

bool GtMotion::isMove(short axis)
{
    long sts;
    if(!GT_GetSts(axis, &sts))
        return (sts&0x400);
    return false;
}

void GtMotion::axisControl(short axis,bool mode)
{

    if (mode)
    {
        GT_AxisOn(axis);
    }
    else
    {
        GT_AxisOff(axis);
        GT_SetDoBit(10,0,0);
    }

}

void GtMotion::close()
{
    for(short k=0;k<axisMaxNum;++k){
        GT_Stop(k,0);
        GT_AxisOff(k+1);
    }
    GT_Close();
}

bool GtMotion::stop(long axis,long style)
{
    return !GT_Stop(axis,style);
}

bool GtMotion::stopAll(long style)
{
    bool isOk=true;
    for(long k=0;k<axisMaxNum;++k){
        isOk&=!GT_Stop(k,style);
    }
    return isOk;
}

bool GtMotion::getAxisLimit(short axis, QPointF &limit)
{
    if(axisLimit.contains(axis)){
        limit=QPointF(axisLimit.value(axis))/pluseRadio;
        return true;
    }
    limit=QPointF(0,0);
    return false;
}



inline bool GtMotion::clearError(int axis) const
{
    return !GT_ClrSts(static_cast<short>(axis));
}


bool GtMotion::posZero(short axis)
{
    if(!GT_ZeroPos(axis))
        return !GT_SetPrfPos(axis, 0);
    return false;
}
short GtMotion::getAxisStatus(int axis)
{

    long lAxisStatus;
    if(GT_GetAxisStatus(static_cast<short>(axis), &lAxisStatus))
        return 5;
    if(lAxisStatus & 0x2)//报警
        return 6;
    if(lAxisStatus & 0x10)//出错
        return 7;
    if(!(lAxisStatus & 0x200))//伺服关闭
        return 8;
    if(lAxisStatus & 0x20)//正限位
        return 9;
    if(lAxisStatus & 0x40)//负限位
        return 10;
    if(lAxisStatus & 0x80)//平滑停止
        return 0;
    if(lAxisStatus & 0x100)//紧急停止
        return 1;
    if(!(lAxisStatus & 0x400))//规划停止
        return 2;
    if((lAxisStatus & 0x400))//规划运动
        return 3;
    return 4;

    //    switch (type)
    //    {
    //    case 0:// 伺服报警标志
    //    {
    //        if (lAxisStatus & 0x2)
    //        {
    //            return 1;

    //        }
    //        else
    //        {
    //            return 2;
    //        }
    //    }
    //    case 1:// 跟随误差越限标志
    //    {
    //        if (lAxisStatus & 0x10)
    //        {
    //            return 3;

    //        }
    //        else
    //        {
    //            return 4;

    //        }
    //    }
    //    case 2:// 正向限位
    //    {
    //        if (lAxisStatus & 0x20)
    //        {
    //            return 5;

    //        }
    //        else
    //        {
    //            return 6;

    //        }
    //    }
    //    case 3:// 负向限位
    //    {
    //        if (lAxisStatus & 0x40)
    //        {
    //            return 7;

    //        }
    //        else
    //        {
    //            return 8;

    //        }
    //    }
    //    case 4:// 平滑停止
    //    {
    //        if (lAxisStatus & 0x80)
    //        {
    //            return 9;

    //        }
    //        else
    //        {
    //            return 10;

    //        }
    //    }
    //    case 5:// 急停标志
    //    {
    //        if (lAxisStatus & 0x100)
    //        {
    //            return 11;

    //        }
    //        else
    //        {

    //            return 12;
    //        }
    //    }
    //    case 6:// 伺服使能标志
    //    {
    //        if(lAxisStatus & 0x200)
    //        {

    //            return 13;
    //        }
    //        else
    //        {
    //            return 14;

    //        }
    //    }
    //    case 7:// 规划器正在运动标志
    //    {
    //        if (lAxisStatus & 0x400)
    //        {
    //            return 15;

    //        }
    //        else
    //        {
    //            return 16;

    //        }
    //    }


    //    default:
    //        return 0;
    //    }

    //    if (lAxisStatus& 0x2)
    //        printf("伺服报警\n");
    //    else
    //        printf("伺服正常\n");
    //    if (lAxisStatus& 0x10)  // 跟随误差越限标志
    //        printf("运动出错\n");
    //    else
    //        printf("运动正常\n");   // 正向限位
    //    if (lAxisStatus& 0x20)
    //        printf("正限位触发\n");
    //    else
    //        printf("正限位未触发\n");    // 负向限位
    //    if (lAxisStatus& 0x40)
    //        printf("负限位触发\n");
    //    else
    //        printf("负限位未触发\n");    // 平滑停止
    //    if (lAxisStatus& 0x80)  {     printf("平滑停止触发\n");  }  else  {      printf("平滑停止未触发\n");  }  // 急停标志
    //    if (lAxisStatus& 0x100)  {    printf("急停触发\n");  }  else  {    printf("急停未触发\n");  }  // 伺服使能标志
    //    if(lAxisStatus& 0x200)  {     printf("伺服使能\n");  }  else  {     printf("伺服关闭\n");  }  // 规划器正在运动标志
    //    if (lAxisStatus& 0x400)  {     printf("规划器正在运动\n");  }  else  {      printf("规划器已停止\n");
    //    }
}

QString GtMotion::getAxisStatus_String(int axis)
{
    long lAxisStatus;
    if(GT_GetAxisStatus(static_cast<short>(axis), &lAxisStatus))
        return "Get Status Failed";


    if(lAxisStatus & 0x2)//报警
        return "Driver Warning";
    if(lAxisStatus & 0x10)//出错
        return "Motion Error";
    if(!(lAxisStatus & 0x200))//伺服关闭
        return "Disable";
    if(lAxisStatus & 0x20)//正限位
        return "Limit+";
    if(lAxisStatus & 0x40)//负限位
        return "Limit-";
    if(lAxisStatus & 0x80)//平滑停止
        return "Smooth Stop";
    if(lAxisStatus & 0x100)//紧急停止
        return "Emergency Stop";
    if((lAxisStatus & 0x400))//规划运动
        return "Moving";
    if(!(lAxisStatus & 0x400))//规划停止
        return "Ready";
    if(lAxisStatus & 0x200)//enable
        return "Enable";
    return "Status Unknown";
}

long GtMotion::getInputBits(short type)
{
    mutex.lock();
    long lGpiValue=-1;
    if(!GT_GetDi(type,&lGpiValue)){
        mutex.unlock();
        return lGpiValue;
    }
    mutex.unlock();
    return -1;

}

long GtMotion::getOutputBits(short type)
{
    mutex.lock();
    long lGpiValue=-1;
    if(!GT_GetDo(type,&lGpiValue)){
        mutex.unlock();
        return lGpiValue;
    }
    mutex.unlock();
    return -1;
}

bool GtMotion::getInput(short type,int num,ulong &data)
{
    mutex.lock();
    long lGpiValue;
    bool bit;
    if(!GT_GetDi(type,&lGpiValue)){
        bit=(lGpiValue & long(1<<num));
        data=bit;
        mutex.unlock();
        return true;
    }
    mutex.unlock();
    return false;
}


bool GtMotion::getOutput(short type, int num,ulong &data)
{
    mutex.lock();
    long lGpiValue;
    bool bit=0;
    if(!GT_GetDo(type,&lGpiValue)){
        bit=(lGpiValue&long(1<<num));
        data=!bit;
        mutex.unlock();
        return true;
    }
    mutex.unlock();
    return false;
}

bool GtMotion::setOutput(short type, int num, ulong bit)
{
    mutex.lock();
    ++num;
    bool k= !GT_SetDoBit(type,static_cast<short>(num),!bit);
    mutex.unlock();
    return k;
}





inline short GtMotion::isMoveDone(short axis)
{
    short type=getAxisStatus(axis);
    if(type==4||type==5||type==6||type==7||type==8)
        return -1;
    else if(type==9||type==10||type==0||type==1||type==2){
        return 0;
    }
    else if(type==3)
        return  1;
    return type;
}

inline bool GtMotion::homeMove(short axis, double vel, long pos) const
{
    long status;
    if(!GT_ClrSts(axis))
        if(!GT_ZeroPos(axis))
            if(!GT_SetPrfPos(axis, 0))
                if(!GT_SetVel(axis, vel))
                    if(!GT_SetPos(axis,pos))
                        if(!GT_Update(long(1<<(axis-1)))){
                            do{
                                if(GT_GetSts(axis, &status)){
                                    return false;
                                }
                            }while( status & 0x400 );
                            QThread::msleep(200);
                            return true;
                        }
    return false;
}

bool GtMotion::pointMove(short axis,double vel,long pos)
{
    if(pos<axisLimit.value(axis).x()||pos>axisLimit.value(axis).y()){
        return false;
    }
    if(!GT_ClrSts(axis))
        if(!GT_SetPos(axis, pos))
            if(!GT_SetVel(axis,vel))
                if(!GT_Update(long(1<<(axis-1)))){
                    //                    do
                    //                    {
                    //                        if(isMoveDone(axis)==0)
                    //                            break;
                    //                        else if(isMoveDone(axis)==-1)
                    //                            return false;
                    //                        else
                    //                            continue;
                    //                    }while(true);
                    //                    QThread::msleep(200);
                    return true;
                }
    return false;
}

bool GtMotion::pointMove(short axis, long pos)
{
    if(pos<axisLimit.value(axis).x()||pos>axisLimit.value(axis).y()){
        return false;
    }
    if(!GT_ClrSts(axis))
        if(!GT_SetPos(axis, pos))
            if(!GT_SetVel(axis,pointMoveSpeeds.value(axis)))
                if(!GT_Update(long(1<<(axis-1)))){
                    //                    do
                    //                    {
                    //                        if(isMoveDone(axis)==0)
                    //                            break;
                    //                        else if(isMoveDone(axis)==-1)
                    //                            return false;
                    //                        else
                    //                            continue;
                    //                    }while(true);
                    //                    QThread::msleep(200);
                    return true;
                }
    return false;
}

bool GtMotion::pointMoveStep(short axis, double vel, long pos)
{
    double cpos;
    if(GT_GetAxisEncPos(axis, &cpos, 1)){
        return false;
    }
    pos+=cpos;
    if(pos<axisLimit.value(axis).x()||pos>axisLimit.value(axis).y()){
        return false;
    }
    if(!GT_ClrSts(axis))
        if(!GT_SetPos(axis, pos))
            if(!GT_SetVel(axis,vel))
                if(!GT_Update(long(1<<(axis-1)))){
                    //                    do
                    //                    {
                    //                        if(isMoveDone(axis)==0)
                    //                            break;
                    //                        else if(isMoveDone(axis)==-1)
                    //                            return false;
                    //                        else
                    //                            continue;
                    //                    }while(true);
                    //                    QThread::msleep(200);
                    return true;
                }
    return false;
}

bool GtMotion::pointMoveStep(short axis, long pos)
{
    double cpos;
    if(GT_GetAxisEncPos(axis, &cpos, 1)){
        return false;
    }
    pos+=cpos;
    if(pos<axisLimit.value(axis).x()||pos>axisLimit.value(axis).y()){
        return false;
    }
    if(!GT_ClrSts(axis))
        if(!GT_SetPos(axis, pos))
            if(!GT_SetVel(axis,pointMoveSpeeds.value(axis)))
                if(!GT_Update(long(1<<(axis-1)))){
                    //                    do
                    //                    {
                    //                        if(isMoveDone(axis)==0)
                    //                            break;
                    //                        else if(isMoveDone(axis)==-1)
                    //                            return false;
                    //                        else
                    //                            continue;
                    //                    }while(true);
                    //                    QThread::msleep(200);
                    return true;
                }
    return false;
}


bool GtMotion::getCmdVel(short axisStart,double &Vel,short axisNum)
{
    return !GT_GetAxisPrfVel(axisStart,&Vel,axisNum);

}

bool GtMotion::getCmdPos(short axisStart, double &Pos, short axisNum)
{
    return !GT_GetAxisPrfPos(axisStart, &Pos, axisNum);

}

bool GtMotion::getCmdAcc(short axisStart,double &Acc,short axisNum)
{
    return !GT_GetAxisPrfAcc(axisStart, &Acc, axisNum);

}
bool GtMotion::getCurVel(short axisStart,double &Vel,short axisNum)
{
    return !GT_GetAxisEncVel(axisStart,&Vel,axisNum);

}

inline bool GtMotion::getCurPos(short axisStart, double &Pos, short axisNum) const
{
    return !GT_GetAxisEncPos(axisStart, &Pos, axisNum);

}

bool GtMotion::getCurAcc(short axisStart,double &Acc,short axisNum)
{
    return !GT_GetAxisEncAcc(axisStart, &Acc, axisNum);
}


bool GtMotion::getMotionMode(short axisStart, QString &modename, short axisNum)
{
    long mode;
    GT_GetPrfMode(axisStart,&mode,axisNum);
    QStringList typeName;
    typeName<<"点位运动"<<"Jog模式"<<"PT模式"<<"电子齿轮模式"<<"Fllow模式"<<"插补模式"<<"PVT模式";
    modename=typeName[mode];
    return true;
}

bool GtMotion::setMotionMode(short axis, short mode)
{
    switch (mode) {
    case 0:{
        return !GT_PrfTrap(axis);
    }
    case 1:{
        return !GT_PrfJog(axis);
    }
    default:{
        return !GT_PrfTrap(axis);
    }
    }
}
bool GtMotion::getTrapPrm(short axis,  double&Acc, double& VelStart,double &Dec, double&SmoothTime)
{
    TTrapPrm trapPrm;
    if(!GT_GetTrapPrm(axis, &trapPrm)){
        Acc=trapPrm.acc;
        VelStart=trapPrm.velStart;
        Dec=trapPrm.dec;
        SmoothTime=trapPrm.smoothTime;
        return true;
    }
    return false;
}

bool GtMotion::setTrapPrm(short axis, double Acc, double velStart, double Dec, short SmoothTime)
{
    TTrapPrm trapPrm;

    trapPrm.acc=Acc;
    trapPrm.velStart=velStart;
    trapPrm.dec=Dec;
    trapPrm.smoothTime=SmoothTime;
    if(!GT_SetTrapPrm(axis, &trapPrm))
        return true;

    return false;
}



bool GtMotion::getJogPrm(short axis, double &Acc, double &Dec, double &SmoothTime)
{
    JogPrm jogPrm;
    if(!GT_GetJogPrm(axis, &jogPrm)){
        Acc=jogPrm.acc;
        Dec=jogPrm.dec;
        SmoothTime=jogPrm.smooth;
        return true;
    }
    return false;
}



bool GtMotion::setJogPrm(short axis, double Acc, double Dec, short SmoothTime)
{
    JogPrm jogPrm;
    jogPrm.acc=Acc;
    jogPrm.dec=Dec;
    jogPrm.smooth=SmoothTime;
    if(GT_SetJogPrm(axis, &jogPrm))
        return false;
    return true;
}

bool GtMotion::comparePos(short axis,long Buf1[20],short num,short pulse)
{
    if(!GT_ZeroPos(axis))
        if(!GT_SetPrfPos(axis, 0))
            if(!GT_PrfTrap(axis))
                if(!GT_ComparePulse(3,1,pulse))
                    return !GT_CompareData(axis, 1, 1, 0,pulse , NULL, NULL,Buf1,num);
    return false;
}

bool GtMotion::setCapturePos(short axis ,long pos,double encPos)
{

    short  status;


    GT_SetCaptureMode(axis, CAPTURE_HOME);
    do
    {

        GT_GetCaptureStatus(axis, &status, &pos);

        GT_GetEncPos(axis, &encPos);
        return false;


    }while( 0 == status );
    return true;
}

bool GtMotion::JogMove(short axis,double Vel)
{
    // 将1轴设为Jog模式
    GT_PrfJog(axis);
    // 设置1轴的目标速度
    GT_SetVel(axis, Vel);
    // 启动1轴的运动
    GT_Update(long(1<<(axis-1)));
    return true;
}


bool GtMotion::getCurError(short axisStart,double &Error,short axisNum)
{
    return !GT_GetAxisError(axisStart, &Error, axisNum);

}












