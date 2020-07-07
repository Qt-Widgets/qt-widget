#ifndef HAXISADJUSTPLUGIN_H
#define HAXISADJUSTPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include "HAxisAdjustInterface.h"

class HAxisAdjustMainWindow;

class HAxisAdjustPlugin :public QObject,public HAxisAdjustInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "HInterface.axisAdjust" FILE "plugin.json")
    Q_INTERFACES(HAxisAdjustInterface)
public:
    ~HAxisAdjustPlugin() override;
    qint32 Init(QWidget* parent=nullptr) override;
    void unInit() override;
    void refreshDeviceNames(const QStringList&axisNames,const QStringList&funcNames) override;


    void showWindow() override;
    QWidget *getWindow() override;


    HAxisAdjustMainWindow *ptrMainWindow=nullptr;

signals:

    qint32 processImg(const QString&funcName,QPointF &pt) override;
    qint32 processImgLine(const QString&funcName,QLineF &pt) override;
    qint32 getAxisPos(const QString&axisName,double &pos) override;
    qint32 getAxisPosCount(const QString&axisName,qint32 &pos) override;
    qint32 axisMoveToWait(const QString&axisName,double pos) override;
    qint32 axisMoveStepWait(const QString&axisName,double pos) override;
};

#endif // HAXISADJUSTPLUGIN_H

