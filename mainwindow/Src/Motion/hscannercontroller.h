#ifndef HSCANNERCONTROLLER_H
#define HSCANNERCONTROLLER_H

#include "stable.h"

#include "scanner/hscanner.h"





class HScannerController
{
public:

    static HScannerController* GetInstance();
    static void Release();

    HScanner *GetScanner(qint32 index);

    qint32 InitScannerCtr();

private:

    HScannerController();
    static HScannerController* m_pInstance;
     ~HScannerController();

    QVector<HScanner *> m_scannerArray;



};

#endif // HSCANNLABRTC4_H
