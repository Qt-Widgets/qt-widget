#include "haxisadjustplugin.h"
#include "haxisadjustmainwindow.h"



HAxisAdjustPlugin::~HAxisAdjustPlugin()
{

}

qint32 HAxisAdjustPlugin::Init(QWidget *parent)
{
    ptrMainWindow=new HAxisAdjustMainWindow(this,parent);
    return 0;
}

void HAxisAdjustPlugin::unInit()
{
    if(ptrMainWindow)
        delete ptrMainWindow;
}

void HAxisAdjustPlugin::refreshDeviceNames(const QStringList &axisNames, const QStringList &funcNames)
{
     ptrMainWindow->refreshWindow(axisNames,funcNames);
}

void HAxisAdjustPlugin::showWindow()
{
    ptrMainWindow->show();

    static int k=1;
    if(k){
        ptrMainWindow->initWindow();
        k=0;
    }
}

QWidget *HAxisAdjustPlugin::getWindow()
{
    return ptrMainWindow;
}


