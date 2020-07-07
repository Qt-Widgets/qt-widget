#ifndef HCAMERACONTROLLER_H
#define HCAMERACONTROLLER_H
#include "stable.h"

#include "camera/hcamera.h"

class HSettings;
class HCameraController
{
public:
    static HCameraController* GetInstance();
    static void Release();

    bool Init();

    bool InitAllCamera();
    bool ConfigController();


    HCamera *GetCamera(qint32 index) const;
    HCamera *GetCamera(const QString& name) const;
    const QVector<HCamera *>& GetAllDevice() const{return m_CameraArray;}
    qint32 GetCameraNum() const;

    const QStringList& GetCameraNames() const;



     void ReleaseDeviceRes();

private:

    static HCameraController* m_pInstance;
    HCameraController();
    ~HCameraController();


    QMap<QString,HCamera *> m_CameraMap;
    QVector<HCamera *> m_CameraArray;
    QStringList m_CameraNames;

    HSettings *ptrSettings;
};

#endif // HCAMERACONTROLLER_H
