#include "hconfigwidget.h"
#include "stable.h"

#include "hproperty_para.h"

#include "hdialogproperty.h"

#include "Src/Motion/h_motioncontroller.h"
#include "Src/Motion/hcameracontroller.h"
#include "Src/Motion/hiodevicecontroller.h"


#include "hsettings.h"

#include <QVBoxLayout>
#include <QHBoxLayout>




HConfigWidget* HConfigWidget::m_pInstance = nullptr;
HConfigWidget *HConfigWidget::GetInstance(QWidget *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HConfigWidget(parent);
    }
    if(parent&&m_pInstance->parent()==nullptr){
        delete m_pInstance;
        m_pInstance = new HConfigWidget(parent);
    }
    return m_pInstance;
}

void HConfigWidget::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}


HConfigWidget::HConfigWidget(QWidget *parent, Qt::WindowFlags f) : QDialog(parent,f)
{
    resize(900,600);
    setWindowTitle(tr("system setting"));

    iconUnfold=QIcon(":/resource/icons/down-arrow.svg");
    iconfold=QIcon(":/resource/icons/right-arrow.svg");

    ptrSettings=HSettings::GetInstance();

    QHBoxLayout *hLayout= new QHBoxLayout;
    hLayout->setMargin(0);
    QVBoxLayout *vLayout1= new QVBoxLayout;
    vLayout1->setMargin(0);
    vLayout1->setContentsMargins(0,0,0,0);
    vLayout1->setSpacing(0);
    vLayout1->setAlignment(Qt::AlignTop);
    //QVBoxLayout *vLayout2= new QVBoxLayout;
    //vLayout2->setAlignment(Qt::AlignTop);
    //vLayout2->addSpacing(25);
    hLayout->addLayout(vLayout1,6);
    //hLayout->addLayout(vLayout2,1);


    const QStringList editables={
        "0,0,0,1,1,1,1,1,1",
        "0,0,0,1,1,1,1",
        "0,1,1,1,1",
        "0,1,1,1,1",
        "0,0,0,1,1,1,1",
        "0,1"
    };

    const QStringList buttons[6]={
        {"Add"},{"Add"},{"Add"},{"Add"},{"Add"},{"Add"}
    };

    const QStringList ctrlTypes={
        "0,0,0,0,0,0,0,0,2",
        "0,0,0,0,0,0,2",
        "0,0,0,0,2",
        "0,0,0,0,2",
        "0,0,0,0,0,0,2",
        "0,0"
    };

    auto tables=ptrSettings->getTables();
    for(qint32 k=0;k<6;++k){
        addButtonContent(vLayout1,ptrSettings->getTableHeaderNames(k),ctrlTypes.at(k),editables.at(k),tables.at(k),k,buttons[k]);
    }



    setLayout(hLayout);

}

HConfigWidget::~HConfigWidget()
{

}



void HConfigWidget::openCustomDialog(const QString &name, qint32 index)
{

    AddDialogParas paras;
    paras.title=ptrSettings->getTables().at(index);
    paras.labels=ptrSettings->getTableHeaderNames(index);
    paras.ctrltypes=customDialogctrlTypes.at(index);
    paras.uniqueKeys=ptrSettings->getTableValuesAtCol(index,uniqueKey);

    paras.datas.push_back(ptrSettings->getAvailableHardware(index));

    auto daaaa=defaultValues.at(index);
    for(const auto&k:daaaa.split(",")){
        paras.datas.push_back(k.split(";"));
    }


    if(openAddDialog(paras)){

        m_contentWids.at(index)->addRowData(paras.datas);
        ptrSettings->addOneValue(index,paras.datas);

        switch (index) {
        case 0:
            H_MotionController::GetInstance()->Init();
            break;
        case 1:
            HCameraController::GetInstance()->Init();
            break;
        case 2:
            HIODeviceController::GetInstance()->Init();
            break;
        default:
            break;
        }
        emit DataSaveAndChanged();
    }
}

void HConfigWidget::addButtonContent(QVBoxLayout *vlayout1, const QStringList &headNames, const QString &colTypes, const QString&editables, const QString &text,
                                     qint32 index, const QStringList &buttons)
{

    QPushButton *button1=new QPushButton(this);
    button1->setText(text);
    button1->setCheckable(true);
    //    button1->setChecked(!index);
    //    button1->setIcon(index==0?iconUnfold:iconfold);
    button1->setIcon(iconfold);

    button1->setIconSize(QSize(18,18));
    button1->setFixedHeight(30);
    button1->setStyleSheet(QString("QPushButton{font-family:MicroSoft YaHei;font-size:13px;font-weight:bold;border-radius:0px; background:%1; color:black;border-style:hidden;text-align: left;}"
                                   "QPushButton:hover{font-size:15px;font-weight:bold;background:rgb(106, 181, 255);border-radius:0px; /*border:2px solid rgb(100, 100, 100);*/}"
                                   "QPushButton:checked{font-size:15px;font-weight:bold;background:lightgreen;border-radius:0px; border:0px solid rgb(210, 225, 230);}"
                                   "QPushButton:pressed{font-size:15px;font-weight:bold;background:rgb(100, 180, 255);border-radius:0px; border:0px solid rgb(210, 225, 230);}")
                           .arg((index%2)==0?"lightgrey":"gray"));



    HProperty_para *pwid1=new HProperty_para(headNames,index,this,colTypes,editables);

    pwid1->AddControlButton(tr("clear"),this,[=](){
        pwid1->clearAllRows();
        ptrSettings->resetTable(index);
        switch (index) {
        case 0:
            H_MotionController::GetInstance()->Init();
            break;
        case 1:
            HCameraController::GetInstance()->Init();
            break;
        case 2:
            HIODeviceController::GetInstance()->Init();
            break;
        default:
            break;
        }
         emit DataSaveAndChanged();
    });

    pwid1->addCtrlSpace(30);

    pwid1->AddControlButton(tr("delete"),this,[=](){

        ptrSettings->deleteRowValue(index,uniqueKey,pwid1->deleteCurrentRow());
        switch (index) {
        case 0:
            H_MotionController::GetInstance()->Init();
            break;
        case 1:
            HCameraController::GetInstance()->Init();
            break;
        case 2:
            HIODeviceController::GetInstance()->Init();
            break;
        default:
            break;
        }
         emit DataSaveAndChanged();

    });

    pwid1->addCtrlSpace(50);



    connect(pwid1,&HProperty_para::dataChangedUseKeyName,this,[=](const QVariant&key,qint32 col,const QVariant&data){
        ptrSettings->updataValue(index,col,uniqueKey,key,data);
        switch (index) {
        case 0:
            H_MotionController::GetInstance()->ConfigController();
            break;
        case 1:
            HCameraController::GetInstance()->ConfigController();
            break;
        case 2:
            HIODeviceController::GetInstance()->ConfigController();
            break;
        default:
            break;
        }
    });


    //pwid1->setVisible(!index);
    pwid1->setVisible(false);

    pwid1->setUniqueColIndex(uniqueKey);



    for(const auto&k:buttons){
        pwid1->addButton(k);
    }
    connect(pwid1,&HProperty_para::buttonHasPressed,this,&HConfigWidget::openCustomDialog);

    pwid1->setAllDatasUseCol(ptrSettings->getTableValues(index));

    connect(button1,&QPushButton::toggled,this,[=](bool checked){
        pwid1->setVisible(checked);
        button1->setIcon(checked?iconUnfold:iconfold);
    });

    m_contentWids<<pwid1;

    QVBoxLayout *vlayout= new QVBoxLayout;
    vlayout->addWidget(button1);
    vlayout->addWidget(pwid1);

    vlayout1->addLayout(vlayout);
}

bool HConfigWidget::openAddDialog(HConfigWidget::AddDialogParas &paras)
{
    HDialogProperty dialog(paras.title);
    dialog.setLabels(paras.labels);
    dialog.setContents(paras.ctrltypes,paras.datas);
    dialog.setUniqueKeys(uniqueKey,paras.uniqueKeys);

    if(dialog.exec()){
        paras.datas=dialog.getContentData();
        return true;
    }
    return false;
}

