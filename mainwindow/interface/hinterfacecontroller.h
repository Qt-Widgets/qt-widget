#ifndef HINTERFACECONTROLLER_H
#define HINTERFACECONTROLLER_H

#include <QObject>
#include "himginterface.h"
#include "HAxisAdjustInterface.h"

class H_CameraControllerWidget;
class HCameraController;

class HInterfaceController :public QObject
{
     Q_OBJECT
public:
    static HInterfaceController* GetInstance(QObject *parent=nullptr);
    static void Release();

    bool Init();

    HImgInterface *imgInterface=nullptr;
    HAxisAdjustInterface *axisAdjustInterface=nullptr;

private slots:
    bool getImage(const QString &name,uchar*& pData,qint32 &width,qint32&height);
    qint32 setCameraGain(const QString &name,qint32 gain);
    qint32 setCameraExposure(const QString &name,qint32 exposure);
    qint32 getCameraSettings(const QString &name,QStringList& data);

    qint32 getAxisPosition(const QString &name,double &pos);
    qint32 getAxisPositionCount(const QString &name,qint32 &pos);
    qint32 axisMoveToWaitStop(const QString &name,double pos);
    qint32 axisMoveStepWaitStop(const QString &name,double pos);

    qint32 processImage(const QString &func,QPointF&pt);
    qint32 processImageLine(const QString &func,QLineF &line);

private:
    static HInterfaceController* m_pInstance;
    explicit HInterfaceController(QObject *parent);
    ~HInterfaceController() Q_DECL_OVERRIDE;

    bool loadImgPlugin();
    bool loadAxisAdjustPlugin();

    H_CameraControllerWidget *ptrCameraShow;
    HCameraController *ptrCameraController;
};

#endif // HINTERFACECONTROLLER_H
