#ifndef HIMGPLUGIN_H
#define HIMGPLUGIN_H
#include <QObject>
#include <QtPlugin>
#include "himginterface.h"


class HImgProcessMWindow;

class HImgPlugin :public QObject,public  HImgInterface
{

    Q_OBJECT
    Q_PLUGIN_METADATA(IID "HInterface.imageProcess" FILE "plugin.json")
    Q_INTERFACES(HImgInterface)
public:
    HImgProcessMWindow *ptrMainWindow=nullptr;

    qint32 Init(QWidget* parent=nullptr) override;
    void unInit() override;

    void resetCameraNames(const QStringList&names) override;
    void resetAxisNames(const QStringList&names) override;


    QStringList getProcessFuncNames() override;
    QString getDefaultProcessFuncName() override;
    qint32 moveToFocus() override;
    qint32 showImage(uchar * pData,qint32 width,qint32 height) override;
    qint32 processImage(const QString& funcName,QPointF &result) override;
    qint32 processImageLine(const QString& funcName,QLineF &result) override;

    void showWindow() override;
    QWidget * getWindow() override;

signals:
    bool imgShowed(const QString&camName,uchar*& pData,qint32 &width,qint32 &height) override;
    qint32 cameraSettingsRefreshed(const QString&camName,QStringList& data) override;
    qint32 cameraGainChanged(const QString&camName,qint32 gain) override;
    qint32 cameraExposureChanged(const QString&camName,qint32 exposure) override;

    qint32 getAxisPos(const QString&axisName,double &pos) override;
    qint32 axisMoveToWait(const QString&axisName,double pos) override;
};

#endif // HIMGPLUGIN_H
