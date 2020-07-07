#include "h_processwidget.h"
#include "hcommon.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QTabWidget>
#include <QPushButton>
#include <QStackedLayout>
#include <QTabBar>

#include "Src/window/programhandler.h"
#include "Src/window/programdata.h"
#include "Src/painter/paintermainwidget.h"
#include "Src/data/inisettings.h"



#include "hproperty_para.h"


H_ProcessWidget* H_ProcessWidget::m_pInstance = nullptr;

H_ProcessWidget *H_ProcessWidget::GetInstance(QWidget *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new H_ProcessWidget(parent);
    }
    return m_pInstance;
}


H_ProcessWidget::H_ProcessWidget(QWidget *parent) : QWidget(parent)
{

    ptrdHandler=ProgramHandler::GetInstance();
    ptrIniSettings =IniSettings::GetInstance();

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->setMargin(0);
    ptrTabWidget=new QTabWidget(this);
    ptrTabWidget->tabBar()->setDocumentMode(true);
    //ptrTabWidget->tabBar()->setUsesScrollButtons(false);
    ptrTabWidget->setStyleSheet(HSheetStyle::H_TABWIDGET2);

    layout->addWidget(ptrTabWidget);
    //    QHBoxLayout *hlayout=new QHBoxLayout;
    //    QPushButton *button=new QPushButton(this);
    //    button->setText(tr("Save"));
    //    hlayout->addStretch(1);
    //    hlayout->addWidget(button,2);
    //    hlayout->addStretch(1);
    //    layout->addLayout(hlayout);

    const QStringList  headNames={tr("Type"),tr("Value")};


    QStringList typess;


    typess<<tr("Focus distance(mm)")<<tr("Process count");
    ptrGeneral1Wid=new HProperty_para(headNames,HGraphicsItem::para_genaral,this,nullptr,"0,1");
    ptrGeneral1Wid->setAutoColWidth(false);
    ptrGeneral1Wid->resizeDatas(typess.size());
    ptrGeneral1Wid->setDatasAtCol(0,typess);
    ptrGeneral1Wid->addButton(tr("Save"));
    connect(ptrGeneral1Wid,&HProperty_para::buttonHasPressed,this,&H_ProcessWidget::saveItemData);
    ptrGeneral1Wid->setVisible(false);
    itemParaWids.insert(HGraphicsItem::para_genaral,ptrGeneral1Wid);


    typess.clear();
    typess<<tr("Focus distance(mm)")<<tr("Process count");
    ptrGeneral2Wid=new HProperty_para(headNames,HGraphicsItem::para_genaral,this,nullptr,"0,1");
    ptrGeneral2Wid->setAutoColWidth(false);
    ptrGeneral2Wid->resizeDatas(typess.size());
    ptrGeneral2Wid->setDatasAtCol(0,typess);
    ptrGeneral2Wid->addButton(tr("Save"));
    connect(ptrGeneral2Wid,&HProperty_para::buttonHasPressed,this,&H_ProcessWidget::saveItemData);
    ptrGeneral2Wid->setVisible(false);



    typess.clear();
    typess<<tr("Power (%)")<<tr("Frequency (kHz)")<<tr("ID1 (A)")<<tr("ID2 (A)")<<tr("ID3 (A)")<<tr("PSN")<<tr("DPK");
    ptrLaser1Wid=new HProperty_para(headNames,HGraphicsItem::para_laser,this,nullptr,"0,1");
    ptrLaser1Wid->setAutoColWidth(false);
    ptrLaser1Wid->resizeDatas(typess.size());
    ptrLaser1Wid->setDatasAtCol(0,typess);
    ptrLaser1Wid->addButton(tr("Save"));
    connect(ptrLaser1Wid,&HProperty_para::buttonHasPressed,this,&H_ProcessWidget::saveItemData);
    ptrLaser1Wid->setVisible(false);
    itemParaWids.insert(HGraphicsItem::para_laser,ptrLaser1Wid);


    typess.clear();
    typess<<tr("Power (%)")<<tr("Frequency (kHz)")<<tr("Pulse Width")<<tr("First pulse kill");
    ptrLaser2Wid=new HProperty_para(headNames,HGraphicsItem::para_laser,this,nullptr,"0,1");
    ptrLaser2Wid->setAutoColWidth(false);
    ptrLaser2Wid->resizeDatas(typess.size());
    ptrLaser2Wid->setDatasAtCol(0,typess);
    ptrLaser2Wid->addButton(tr("Save"));
    connect(ptrLaser2Wid,&HProperty_para::buttonHasPressed,this,&H_ProcessWidget::saveItemData);
    ptrLaser2Wid->setVisible(false);


    typess.clear();
    typess<<tr("Speed(mm/s)")<<tr("Acceleration(mm/s2)")<<tr("JumpSpeed(mm/s2)")<<tr("JumpAcc(mm/s2)")<<tr("JumpDelay(s)")
         <<tr("trigDistance(mm)")<<tr("pluseWidth(us)")<<tr("duty(%)")<<tr("pluseCount")<<tr("pluseDelay(s)");
    ptrCam1Wid=new HProperty_para(headNames,HGraphicsItem::para_cam,this,nullptr,"0,1");
    ptrCam1Wid->setAutoColWidth(false);
    ptrCam1Wid->resizeDatas(typess.size());
    ptrCam1Wid->setDatasAtCol(0,typess);
    ptrCam1Wid->addButton(tr("Save"));
    connect(ptrCam1Wid,&HProperty_para::buttonHasPressed,this,&H_ProcessWidget::saveItemData);
    ptrCam1Wid->setVisible(false);
    itemParaWids.insert(HGraphicsItem::para_cam,ptrCam1Wid);

    typess.clear();
    typess<<tr("Speed(mm/s)")<<tr("Acceleration(mm/s2)")<<tr("JumpSpeed(mm/s2)")<<tr("JumpAcc(mm/s2)")<<tr("JumpDelay(s)")<<tr("MarkDelay(s)")<<tr("PolyDelay(s)")
         <<tr("OpenLaser(s)")<<tr("CloseLaser(s)");
    ptrCam2Wid=new HProperty_para(headNames,HGraphicsItem::para_cam,this,nullptr,"0,1");
    ptrCam2Wid->setAutoColWidth(false);
    ptrCam2Wid->resizeDatas(typess.size());
    ptrCam2Wid->setDatasAtCol(0,typess);
    ptrCam2Wid->addButton(tr("Save"));
    connect(ptrCam2Wid,&HProperty_para::buttonHasPressed,this,&H_ProcessWidget::saveItemData);
    ptrCam2Wid->setVisible(false);



    typess.clear();
    typess<<tr("topLeft-X(mm)")<<tr("topLeft-Y(mm)")<<tr("topRight-X(mm)")<<tr("topRight-Y(mm)")<<tr("bottomRight-X(mm)")<<tr("bottomRight-Y(mm)")
         <<tr("bottomLeft-X(mm)")<<tr("bottomLeft-X(mm)")<<tr("center-X(mm)")<<tr("center-X(mm)");
    HProperty_para *ptrItemWiddd=new HProperty_para(headNames,HGraphicsRectItem::Type,this,nullptr,"0,1");
    ptrItemWiddd->resizeDatas(typess.size());
    ptrItemWiddd->setAutoColWidth(false);
    ptrItemWiddd->setDatasAtCol(0,typess);
    ptrItemWiddd->addButton(tr("Save"));
    connect(ptrItemWiddd,&HProperty_para::buttonHasPressed,this,&H_ProcessWidget::saveItemData);
    ptrItemWiddd->setVisible(false);
    itemParaWids.insert(HGraphicsRectItem::Type,ptrItemWiddd);



    typess.clear();
    typess<<tr("Center-X(mm)")<<tr("Center-Y(mm)")<<tr("Radius(mm)")<<tr("AngleStart(deg)")<<tr("AngleEnd(deg)")<<tr("Radius1(mm)")<<tr("Radius2(mm)")<<tr("AntiClockwise");
    ptrItemWiddd=new HProperty_para(headNames,HGraphicsEllipseItem::Type,this,nullptr,"0,1");
    ptrItemWiddd->resizeDatas(typess.size());
    ptrItemWiddd->setAutoColWidth(false);
    ptrItemWiddd->setDatasAtCol(0,typess);
    ptrItemWiddd->addButton(tr("Save"));
    connect(ptrItemWiddd,&HProperty_para::buttonHasPressed,this,&H_ProcessWidget::saveItemData);
    ptrItemWiddd->setVisible(false);
    itemParaWids.insert(HGraphicsEllipseItem::Type,ptrItemWiddd);



    typess.clear();
    typess<<tr("Start-X(mm)")<<tr("Start-Y(mm)")<<tr("End-X(mm)")<<tr("End-Y(mm)");
    ptrItemWiddd=new HProperty_para(headNames,HGraphicsLineItem::Type,this,nullptr,"0,1");
    ptrItemWiddd->setAutoColWidth(false);
    ptrItemWiddd->resizeDatas(typess.size());
    ptrItemWiddd->setDatasAtCol(0,typess);
    ptrItemWiddd->addButton(tr("Save"));
    connect(ptrItemWiddd,&HProperty_para::buttonHasPressed,this,&H_ProcessWidget::saveItemData);
    ptrItemWiddd->setVisible(false);
    itemParaWids.insert(HGraphicsLineItem::Type,ptrItemWiddd);


    typess.clear();
    typess<<tr("Start-X(mm)")<<tr("Start-Y(mm)")<<tr("End-X(mm)")<<tr("End-Y(mm)")<<tr("length(mm)");
    ptrItemWiddd=new HProperty_para(headNames,HGraphicsPathItem::Type,this,nullptr,"0,0");
    ptrItemWiddd->setAutoColWidth(false);
    ptrItemWiddd->resizeDatas(typess.size());
    ptrItemWiddd->setDatasAtCol(0,typess);
    ptrItemWiddd->addButton(tr("Save"));
    connect(ptrItemWiddd,&HProperty_para::buttonHasPressed,this,&H_ProcessWidget::saveItemData);
    ptrItemWiddd->setVisible(false);
    itemParaWids.insert(HGraphicsPathItem::Type,ptrItemWiddd);


    typess.clear();

    const QStringList  headNames1={"X(mm)","Y(mm)"};
    //typess<<tr("Point-1")<<tr("Point-2")<<tr("Point-3")<<tr("Point-4");
    ptrLocationWid=new HProperty_para(headNames1,HGraphicsItem::para_location,this,nullptr,"1,1");
    ptrLocationWid->setAutoColWidth(false);

    QVariantList ghghghgh;
    ghghghgh<<0<<0;
    ptrLocationWid->setDefaultAddData(ghghghgh);
    ptrLocationWid->setVisible(false);

    ptrLocationWid->AddControlButton(tr("Clear"),ptrLocationWid,&HProperty_para::clearAllRows);
    ptrLocationWid->AddControlButton(tr("Delete"),ptrLocationWid,&HProperty_para::deleteSelectedRows);
    ptrLocationWid->AddControlButton(tr("Add"),ptrLocationWid,&HProperty_para::addEmptyRow);

    ptrLocationWid->addCtrlSpace(20);
    captureButton=ptrLocationWid->addButton(tr("set"));
    captureButton->setCheckable(true);
    connect(captureButton,&QPushButton::toggled,this,[=](bool checked){
        if(checked){
            PainterMainWidget::GetInstance()->getActiveView()->setMousePressMode(0);
            connect(PainterMainWidget::GetInstance()->getActiveView(),&HGraphicsView::mousePressed,ptrLocationWid,&HProperty_para::addRowPointData);
        }else{
            PainterMainWidget::GetInstance()->getActiveView()->setMousePressMode(1);
            disconnect(PainterMainWidget::GetInstance()->getActiveView(),&HGraphicsView::mousePressed,ptrLocationWid,&HProperty_para::addRowPointData);
        }
    });


    ptrLocationWid->addButton(tr("Save"));
    connect(ptrLocationWid,&HProperty_para::buttonHasPressed,this,&H_ProcessWidget::saveItemData);
    itemParaWids.insert(HGraphicsItem::para_location,ptrLocationWid);



    //    connect(ptrdHandler,&ProgramHandler::programClosed,this,[=](){
    //        ptrTabWidget->clear();
    //    });

    connect(ptrdHandler,&ProgramHandler::programSelectionChanged,this,[=](qint32 programIndex){

        selectedItems=ptrdHandler->getProgram(programIndex)->getViewPtr()->getScene()->getSelectedItems();

        static qint32 currentIndex=0;
        if(ptrTabWidget->currentIndex()!=-1)
            currentIndex=ptrTabWidget->currentIndex();

        ptrTabWidget->clear();

        ptrGeneral1Wid->setVisible(false);
        ptrGeneral2Wid->setVisible(false);
        ptrLaser1Wid->setVisible(false);
        ptrLaser2Wid->setVisible(false);
        ptrCam1Wid->setVisible(false);
        ptrCam2Wid->setVisible(false);
        ptrLocationWid->setVisible(false);
        captureButton->setChecked(false);

        for(const auto &k:itemParaWids){
            k->setVisible(false);
        }

        if(!selectedItems.isEmpty()){

            auto firstGenaral=selectedItems.first()->data(HGraphicsItem::para_genaral);
            auto firstLaser=selectedItems.first()->data(HGraphicsItem::para_laser);
            auto firstCam=selectedItems.first()->data(HGraphicsItem::para_cam);
            auto firstLocation=selectedItems.first()->data(HGraphicsItem::para_location);
            bool lgenaral=1,lFlag=1,cFlag=1,loFlag=1;

            for(const auto&k:selectedItems){
                if(lgenaral){
                    if(k->data(HGraphicsItem::para_genaral)==firstGenaral){
                        firstGenaral=k->data(HGraphicsItem::para_genaral);
                    }else{
                        firstGenaral.clear();
                        lgenaral=0;
                    }
                }

                if(lFlag){
                    if(k->data(HGraphicsItem::para_laser)==firstLaser){
                        firstLaser=k->data(HGraphicsItem::para_laser);
                    }else{
                        firstLaser.clear();
                        lFlag=0;
                    }
                }

                if(cFlag){
                    if(k->data(HGraphicsItem::para_cam)==firstCam){
                        firstCam=k->data(HGraphicsItem::para_cam);
                    }else{
                        firstCam.clear();
                        cFlag=0;
                    }
                }



                if(loFlag){
                    if(k->data(HGraphicsItem::para_location)==firstLocation){
                        firstLocation=k->data(HGraphicsItem::para_location);
                    }else{
                        firstLocation.clear();
                        loFlag=0;
                    }
                }



                if(!(lgenaral||loFlag||cFlag||lFlag)){
                    break;
                }

            }


            auto mode=ptrIniSettings->GetInstance()->getValue(IniSettings::Edit_mode).toInt();

            auto wid=getGeneralWid(mode);
            wid->setDatasAtCol(1,firstGenaral.toStringList());
            wid->setVisible(true);
            ptrTabWidget->addTab(wid,tr("General"));

            wid=getLaserWid(mode);
            wid->setDatasAtCol(1,firstLaser.toStringList());
            wid->setVisible(true);
            ptrTabWidget->addTab(wid,tr("Laser"));

            wid=getCamWid(mode);

            wid->setDatasAtCol(1,firstCam.toList());
            wid->setVisible(true);
            ptrTabWidget->addTab(wid,tr("CAM"));




            QVariantList m_locationjj;
            auto mhhhh=firstLocation.toList();
            for(qint32 k=0;k<mhhhh.size()/2;++k){
                //m_locationjj<<tr("location point");
                m_locationjj<<mhhhh.at(k*2)<<mhhhh.at(k*2+1);
            }
            ptrLocationWid->setAllDatasUseRow(m_locationjj);
            ptrLocationWid->setVisible(true);
            ptrTabWidget->addTab(ptrLocationWid,tr("Location"));


            if(selectedItems.size()==1){
                auto pwid=itemParaWids.value(selectedItems.first()->type());
                pwid->setDatasAtCol(1,((HGraphicsItem*)selectedItems.first())->getPropertyParas());
                pwid->setVisible(true);
                ptrTabWidget->addTab(pwid,tr("Property"));
            }

            if(currentIndex<ptrTabWidget->count()){
                ptrTabWidget->setCurrentIndex(currentIndex);
            }

        }

    });




}

H_ProcessWidget::~H_ProcessWidget()
{

}

inline HProperty_para *H_ProcessWidget::getGeneralWid(qint32 mode) const
{
    switch (mode) {
    case 0:{
        return  ptrGeneral1Wid;
    }
    case 1:{
        return  ptrGeneral2Wid;
    }
    default:
        return  nullptr;
    }
}

inline HProperty_para *H_ProcessWidget::getLaserWid(qint32 mode) const
{
    switch (mode) {
    case 0:{
        return  ptrLaser1Wid;
    }
    case 1:{
        return  ptrLaser2Wid;
    }
    default:
        return  nullptr;
    }
}

inline HProperty_para *H_ProcessWidget::getCamWid(qint32 mode) const
{
    switch (mode) {
    case 0:{
        return  ptrCam1Wid;
    }
    case 1:{
        return  ptrCam2Wid;
    }
    default:
        return  nullptr;
    }
}

void H_ProcessWidget::saveItemData(const QString &name,qint32 type)
{
    if(type==HGraphicsItem::para_location){
        for(const auto &k:selectedItems){
            k->setData(type,itemParaWids.value(type)->getAllDataUseRow().toStringList_h());
        }
        return;
    }
    for(const auto &k:selectedItems){
        k->setData(type,itemParaWids.value(type)->getDataAtCol(1).toStringList_h());
    }
}

