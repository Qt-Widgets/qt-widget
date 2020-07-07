#include "programdata.h"
#include "programhandler.h"
#include "Src/dxflib/m_dxfio.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

#include "Src/painter/math/RDxfImporter.h"



#include "Src/painter/paintermainwidget.h"

//#define getBitValue(x,k) ((x>>((k)-0)) & 1)
//#define setBitValue(x,k,value) (x) = ((value>0) ?((x) | (1<<((k)-0))) : ((x) & ~(1<<((k)-0))))
//#define getBitValues( a,fromBit,toBit)  (((a)>> (fromBit)) & ( (0x01) << ((toBit) + (fromBit) - 1)))
//#define setBitValues(var,fromBit,toBit,val) (var) = ((var) & ~(((0x1<<((toBit)+1-(fromBit)))-1)<< (fromBit))) | ((val) << (fromBit + 0))

ProgramData::ProgramData(ProgramHandler *parent):ptrProgramHandler(parent)
{

    ptrPainterWid=PainterMainWidget::GetInstance();
}

ProgramData::~ProgramData()
{
    if(ptrItemView)
        ptrPainterWid->closeShowWindow(ptrItemView);
}

void ProgramData::setCurentProject()
{
    ptrPainterWid->setCurrentShowWindow(ptrItemView);
}


bool ProgramData::openDxf(const QString &path)
{

    ptrItemView->getScene()->clearItems();
    RDxfImporter imp;
    imp.setItemViewPtr(ptrItemView->getScene());

    if(imp.importFile(path)){
        return true;
    }else{
        return false;
    }


}

bool ProgramData::openProject(const QString &filePath)
{
    ptrItemView=ptrPainterWid->newShowWindow(QStringList(filePath.split("/")).last());
    if(openFromFile(filePath)){
        ptrItemView->getScene()->updateScene();
        return true;
    }
    return false;
}

void ProgramData::newProjrct(const QString &name)
{
    ptrItemView=ptrPainterWid->newShowWindow(name);
    LayerInfo layer;
    layer.name="0";
    layer.setScenePtr( ptrItemView->getScene());
    ptrItemView->getScene()->addLayer(layer);
}

bool ProgramData::writeProject(const QString &filePath)
{
    return  writeToFile(filePath);
}

void ProgramData::translateProject(const QPointF &pt)
{

    //    auto mk=QTransform::fromTranslate(pt.x()-m_ReferencePoint.x(),pt.y()-m_ReferencePoint.y());

    //    for(auto& k:locationPts){
    //        k=mk.map(k);
    //    }


    //    m_ReferencePoint=pt;
}

bool ProgramData::isActived() const
{
    return this==ptrProgramHandler->getActiveProgram();
}


bool ProgramData::GetGcode(QString &code, QLineF &pline, const QTransform &transRE, qint32 range) const
{

    return true;
}

HGraphicsView *ProgramData::getViewPtr() const
{
    return ptrItemView;
}

HLayerVector *ProgramData::getDataPtr() const
{
    return  ptrItemView->getScene()->getLayerPtr();
}

const HLayerVector &ProgramData::getData() const
{
    return ptrItemView->getScene()->getLayer();
}

HLayerVector &ProgramData::getData()
{
    return ptrItemView->getScene()->getLayer();
}

void ProgramData::setHighlight(bool highLight) const
{
    for( auto&k:ptrItemView->getScene()->getLayer()){
        k.setHighlight(highLight);
    }
}

void ProgramData::setSelected(bool selected) const
{
    for( auto&k:ptrItemView->getScene()->getLayer()){
        k.setSelected(selected);
    }
}

qint32 ProgramData::getSelectedState() const
{
    qint32 state=ptrItemView->getScene()->getLayer().first().getCheckedState();
    if(state==1){
        return 1;
    }
    for( const auto&k:ptrItemView->getScene()->getLayer()){
        if(k.getCheckedState()==state){
            continue;
        }else{
            return 1;
        }
    }
    return state;
}

bool ProgramData::writeToFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)){
        return false;
    }
    QDataStream ds(&file);
    ds << *this ;
    file.close();
    return true;
}

bool ProgramData::openFromFile(const QString &filePath)
{
    QFile f(filePath);
    if(!f.open(QIODevice::ReadOnly))
    {
        return false;
    }

    bool isok;
    QDataStream ds(&f);
    ds >> *this;
    isok=ds.atEnd();
    f.close();
    return isok;
}



