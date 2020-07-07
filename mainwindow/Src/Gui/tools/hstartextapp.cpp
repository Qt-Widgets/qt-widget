#include "hstartextapp.h"

#include <QProcess>

HStartExtApp* HStartExtApp::m_pInstance = nullptr;

HStartExtApp *HStartExtApp::GetInstance(QObject *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HStartExtApp(parent);
    }
    return m_pInstance;
}


bool HStartExtApp::StartAOCLaser()
{

    QStringList para;
    para<<" ";

    aocLaser->start(QCoreApplication::applicationDirPath()+"/externalApp/AOCLaser.exe",para);

    bool a=aocLaser->waitForStarted(5000);


    if(!a){
       // HFastControl::GetInstance()->HandlerSerialPort(1);
    }

    isFineshed=!a;
    return a;

}

HStartExtApp::HStartExtApp(QObject *parent) : QObject(parent)
{
    aocLaser=new QProcess(this);


    connect(aocLaser,QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus){
        if(isExited){
            return ;
        }
       // HFastControl::GetInstance()->HandlerSerialPort(1);
        isFineshed=true;
    });
}

HStartExtApp::~HStartExtApp()
{
    isExited=true;
    aocLaser->close();
   // aocLaser->waitForFinished();
    aocLaser->deleteLater();

//    QProcess ext;
//    QStringList para;
//    para<<"taskkill -f -im AOCLaser.exe";
//    ext.start("C:/Windows/system32/cmd.exe",para);
//    ext.close();
}
