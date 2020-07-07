#ifndef HDATACOLLECT_H
#define HDATACOLLECT_H

#include <QObject>
#include <QVector>

class HSerialDialog;

class HDataCollect : public QObject
{
    Q_OBJECT
public:
    static HDataCollect* GetInstance(QVariantList commondList=QVariantList(),QWidget *parent = nullptr);

    const QList<QVariant>& GetData(){return m_data_one;}

    void setInstructions(const QVariantList &inss);

signals:


public slots:

    bool CollectDataOnce(qreal time);

private slots:
    void handler();

    void handlerSerialRespose(bool isSucceeded,const QString &str);

private:

    explicit HDataCollect(QVariantList commondList=QVariantList(),QObject *parent = nullptr);
    ~HDataCollect();
    static HDataCollect* m_pInstance;
    QList<QVariant> m_data_one;

    qint32 m_index=-2;

    bool isSerialSucceed=false;

    QLibrary myLib;
    typedef int (*HV_GetVersion)();
    HV_GetVersion GetVersion ;

    typedef bool (*HV_AddParameters)(int,void*);
    HV_AddParameters AddParameters ;

    typedef void (*HV_ClearParameters)();
    HV_ClearParameters ClearParameters ;

    typedef bool (*HV_ProcessGreyImage)(unsigned char*,int,int);
    HV_ProcessGreyImage ProcessGreyImage ;

    typedef const char* (*HV_GetLastError)();
    HV_GetLastError GetLastError;

    typedef  double (*HV_GetResult1)(int);
    HV_GetResult1 GetResult1;

    typedef  double (*HV_GetResult2)(int);
    HV_GetResult2 GetResult2 ;

    typedef  double (*HV_GetResult3)(int);
    HV_GetResult3 GetResult3 ;

    typedef  double (*HV_GetResult4)(int);
    HV_GetResult4 GetResult4 ;

    QVariantList m_inss;

    HSerialDialog*ptrSerialDialog;
};

#endif // HDATACOLLECT_H
