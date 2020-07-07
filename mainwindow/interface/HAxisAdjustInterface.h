#ifndef HAXISADJUSTINTERFACE_H
#define HAXISADJUSTINTERFACE_H


class QWidget;

class  HAxisAdjustInterface
{
public:
    virtual ~HAxisAdjustInterface() {}
    virtual qint32 Init(QWidget* parent=nullptr) = 0;
    virtual void unInit()=0;
    virtual void refreshDeviceNames(const QStringList&axisNames,const QStringList&funcNames)=0;


    virtual void showWindow()=0;
    virtual QWidget *getWindow()=0;


    ///////////////signals
    virtual qint32 processImg(const QString&funcName,QPointF &pt)=0;
    virtual qint32 processImgLine(const QString&funcName,QLineF &pt)=0;
    virtual qint32 getAxisPos(const QString&axisName,double &pos)=0;
    virtual qint32 getAxisPosCount(const QString&axisName,qint32 &pos)=0;
    virtual qint32 axisMoveToWait(const QString&axisName,double pos)=0;
    virtual qint32 axisMoveStepWait(const QString&axisName,double pos)=0;

};

#define HAxisAdjustInterface_iid "HInterface.axisAdjust"
Q_DECLARE_INTERFACE(HAxisAdjustInterface, HAxisAdjustInterface_iid)


#endif // HAXISADJUSTINTERFACE_H
