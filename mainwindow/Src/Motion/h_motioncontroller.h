#ifndef H_MOTIONCONTROLLER_H
#define H_MOTIONCONTROLLER_H


#include <stable.h>

#include "motionStage/hmotionstage.h"

#include "motionStage/MotionCompiler.h"


class HSettings;

//typedef QPair<HMotionStage *,qint32 > HMotionAxis;

//class HMotionAxis :public HMotionStage{
//public:
//    explicit HMotionAxis(HMotionStage *ptr,qint32 inde):ptrsta(ptr),m_Axisindex(inde){

//    }


//protected:
//    HMotionStage *ptrsta;
//    qint32 m_Axisindex;

//};

class H_MotionController
{
public:
    static H_MotionController* GetInstance();
    static void Release();

    bool Init();

    bool InitAllMotion();  //when call in runtime ,pay attention that class IODeviceController may be init too!!

    bool ConfigController();


    HMotionStage * GetAxis(qint32 index) const;
    HMotionStage * GetAxis(const QString& name) const;
    const QVector<HMotionStage *> & GetAllMotion() const {return m_MotionAxisArray;}
    qint32 GetAxisNum() const;
    const QStringList&  GetAxisNames() const;


    bool HomeAllMotion() const;
    bool StopAllMotion() const;





private:

    static H_MotionController* m_pInstance;
    H_MotionController();
    ~H_MotionController();

    void releaseDeviceRes();

    QMap<QString,HMotionStage *> m_MotionAxisMap;
    QVector<HMotionStage *> m_MotionAxisArray;
    QStringList m_MotionNames;

    HSettings *ptrSettings;

};

#endif // H_MOTIONCONTROLLER_H
