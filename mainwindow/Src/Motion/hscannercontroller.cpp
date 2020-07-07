#include "hscannercontroller.h"

#include <stdlib.h>
#include "scanner/rtc4func.h"


HScannerController* HScannerController::m_pInstance = nullptr;

HScannerController *HScannerController::GetInstance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HScannerController;
        atexit(Release);
    }
    return m_pInstance;
}
void HScannerController::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}



HScanner *HScannerController::GetScanner(qint32 index)
{
    if(index<m_scannerArray.size()&&index>-1){
        return m_scannerArray.at(0);
    }
    else{
        return nullptr;
    }
}

qint32 HScannerController::InitScannerCtr()
{
    for(const auto&k:m_scannerArray){
        delete k;
    }
    m_scannerArray.clear();


    qint32 reCode=0;
    auto mkk=new RTC4Func;
    mkk->InitScanner(QCoreApplication::applicationDirPath()+"/RTC4D2.hex",1,QCoreApplication::applicationDirPath()+"/D2_1.ctb");
    m_scannerArray<<mkk;

    return  reCode;
}



HScannerController::HScannerController()
{

}

HScannerController::~HScannerController()
{


}
