#include "himgplugin.h"
#include "himgprocessmwindow.h"

#include <QtWidgets>


qint32 HImgPlugin::Init(QWidget *parent)
{
    ptrMainWindow=new HImgProcessMWindow(this,parent);
    return 0;
}

void HImgPlugin::unInit()
{
    if(ptrMainWindow)
        delete ptrMainWindow;
}

void HImgPlugin::resetCameraNames(const QStringList &names)
{
    ptrMainWindow->resetCameras(names);
}

void HImgPlugin::resetAxisNames(const QStringList &names)
{
    ptrMainWindow->resetAxises(names);
}


QStringList HImgPlugin::getProcessFuncNames()
{
    return ptrMainWindow->getFuncNames();
}

QString HImgPlugin::getDefaultProcessFuncName()
{
    return ptrMainWindow->getDefaultFuncName();
}

qint32 HImgPlugin::moveToFocus()
{
   return ptrMainWindow->moveToFocus();
}


qint32 HImgPlugin::showImage(uchar *pData, qint32 width, qint32 height)
{
    return !ptrMainWindow->showImage(pData,width,height);
}

qint32 HImgPlugin::processImage(const QString &funcName, QPointF &result)
{
    return !ptrMainWindow->processImage(funcName,result);
}

qint32 HImgPlugin::processImageLine(const QString &funcName, QLineF &result)
{
    return !ptrMainWindow->processImageLine(funcName,result);
}

void HImgPlugin::showWindow()
{
    ptrMainWindow->show();
}

QWidget *HImgPlugin::getWindow()
{
    return ptrMainWindow;
}


