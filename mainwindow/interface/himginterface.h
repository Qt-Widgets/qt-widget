#ifndef HIMGINTERFACE_H
#define HIMGINTERFACE_H

class QWidget;

class  HImgInterface
{
public:
    virtual ~HImgInterface() {}
    virtual qint32 Init(QWidget* parent=nullptr) = 0;
    virtual void unInit()=0;
    virtual void resetCameraNames(const QStringList&names)=0;
    virtual void resetAxisNames(const QStringList&names)=0;

    virtual void showWindow()=0;
    virtual QWidget *getWindow()=0;

    virtual QStringList  getProcessFuncNames()=0;
    virtual QString getDefaultProcessFuncName()=0;
    virtual qint32 moveToFocus()=0;
    virtual qint32 showImage(uchar * pData,qint32 width,qint32 height)=0;
    virtual qint32 processImage(const QString& funcName,QPointF &result)=0;
    virtual qint32 processImageLine(const QString& funcName,QLineF &result)=0;


    ///////////////signals
    virtual bool imgShowed(const QString&camName,uchar *& pData,qint32 &width,qint32 &height)=0;
    virtual qint32 cameraSettingsRefreshed(const QString&camName,QStringList& data)=0;
    virtual qint32 cameraGainChanged(const QString&camName,qint32 gain)=0;
    virtual qint32 cameraExposureChanged(const QString&camName,qint32 exposure)=0;
    virtual qint32 getAxisPos(const QString&axisName,double &pos)=0;
    virtual qint32 axisMoveToWait(const QString&axisName,double pos)=0;

};

#define HImgInterface_iid "HInterface.imageProcess"
Q_DECLARE_INTERFACE(HImgInterface, HImgInterface_iid)


#endif // HIMGINTERFACE_H
