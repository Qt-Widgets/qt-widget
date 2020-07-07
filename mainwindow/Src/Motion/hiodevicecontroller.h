#ifndef HIODEVICECONTROLLER_H
#define HIODEVICECONTROLLER_H

#include <stable.h>
#include "IO/hiodevice.h"


class HSettings;

class HIODeviceController
{
public:
    static HIODeviceController* GetInstance();
    static void Release();

    bool Init();

    bool InitAllIODevice();
    bool ConfigController();

    HIODevice *GetInputPort(qint32 index) const;
    HIODevice *GetInputPort(const QString& name) const;
    HIODevice *GetOutputPort(qint32 index) const;
    HIODevice *GetOutputPort(const QString&  name) const;


    qint32 GetInputNum() const;
    qint32 GetOutputNum() const;

    const QStringList & GetInputNames() const;
    const QStringList & GetOutputNames() const;



private:

    static HIODeviceController* m_pInstance;
    explicit HIODeviceController();
    ~HIODeviceController();


    QMap<QString,HIODevice *> m_InputMap;
    QMap<QString,HIODevice *> m_OutputMap;

    QVector<HIODevice *> m_InputArray;
    QVector<HIODevice *> m_OutputArray;

    QStringList m_InputNames;
    QStringList m_OutputNames;

    void releaseDeviceRes();


    HSettings *ptrSettings;

};

#endif // HIODEVICECONTROLLER_H
