#include "hdialogparameterlibrary.h"
#include "ui_hdialogparameterlibrary.h"

#include "hparalibcontroller.h"
#include "Src/data/hdialogproperty.h"

#include "Src/data/inisettings.h"

#include <QMessageBox>

HDialogParameterLibrary::HDialogParameterLibrary(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HDialogParameterLibrary)
{
    ui->setupUi(this);

    ptrParaController=HParaLibController::GetInstance();

    setModal(true);
    setFont(MyFont1);

    lineEdits_cutter<<ui->general_cutter_focus<<ui->general_cutter_pcount
                   <<ui->laser_cutter_power<<ui->laser_cutter_frequency<<ui->laser_cutter_id1<<ui->laser_cutter_id2<<ui->laser_cutter_id3<<ui->laser_cutter_psn<<ui->laser_cutter_dpk
                  <<ui->cam_cutter_speed<<ui->cam_cutter_acc<<ui->cam_cutter_jumpSpeed<<ui->cam_cutter_jumpAcc<<ui->cam_cutter_jumpDelay<<ui->cam_cutter_td<<ui->cam_cutter_pw<<ui->cam_cutter_dp<<ui->cam_cutter_pc<<ui->cam_cutter_pd;
    lineEdits_scanner<<ui->general_scanner_focus<<ui->general_scanner_pcount
                    <<ui->laser_scanner_power<<ui->laser_scanner_frequency<<ui->laser_scanner_pulseWidth<<ui->laser_scanner_FPK
                   <<ui->cam_scanner_speed<<ui->cam_scanner_acc<<ui->cam_scanner_jumpSpeed<<ui->cam_scanner_jumpAcc<<ui->cam_scanner_jumpDelay<<ui->cam_scanner_markDelay<<ui->cam_scanner_polyDelay<<ui->cam_scanner_openLaserDelay<<ui->cam_scanner_closeLaserDelay;



    setControlEnable(false);

    connect(ui->listWidget,&QListWidget::currentItemChanged,this,&HDialogParameterLibrary::currentItemHasChanged);


}

HDialogParameterLibrary::~HDialogParameterLibrary()
{
    delete ui;
}

void HDialogParameterLibrary::newItem(QListWidget *ptrWid, const QString &defaultName)
{
    qint32 size=ptrWid->count();
    QStringList items;
    for(qint32 k=0;k<size;++k){
        items.append(ptrWid->item(k)->text());
    }
    QString dname=defaultName;
    QString oname=defaultName;
    getSaveName(items,oname,dname);


    auto mk=new QListWidgetItem(dname);
    // mk->setFlags( Qt::ItemIsEnabled|/*Qt::ItemIsEditable|*/Qt::ItemIsSelectable);



    QStringList list;
    list.push_back(dname);
    ptrParaController->addNew(data_mode,dname,list);
    mk->setData(Qt::UserRole,list);


    mk->setIcon(defaultIcon);
    ptrWid->addItem(mk);
}

void HDialogParameterLibrary::getSaveName(const QStringList &names, const QString &oname, QString &name, qint32 index)
{
    if(names.contains(name)){
        ++index;
        name=oname+QString::number(index);
        getSaveName(names,oname,name,index);
    }
}

bool HDialogParameterLibrary::checkSameName(const QString &text, QListWidget *wid)
{
    qint32 size=wid->count();
    QStringList items;
    for(qint32 k=0;k<size;++k){
        items.append(wid->item(k)->text());
    }

    if(text!=wid->currentItem()->text()&&items.contains(text)){
        QMessageBox::warning(this,tr("error"),tr("name has existed"));
        return true;
    }
    return false;
}

void HDialogParameterLibrary::on_toolButton_new_clicked()
{
    newItem(ui->listWidget,"my parameter");

    if(ui->listWidget->count()==1){
        ui->listWidget->item(0)->setIcon(ui->toolButton_set_default->icon());
        ptrParaController->setDefaultName(data_mode,ui->listWidget->item(0)->text());
    }
}

void HDialogParameterLibrary::on_toolButton_delete_clicked()
{
    for(const auto &k:ui->listWidget->selectedItems()){
        ui->listWidget->takeItem(ui->listWidget->row(k));
        ptrParaController->deletePara(data_mode,k->text());
        if(k->text()==ptrParaController->getDefaultName(data_mode)){
            on_toolButton_set_default_clicked();
        }
    }
}

void HDialogParameterLibrary::on_toolButton_clear_clicked()
{
    ui->listWidget->clear();
    ptrParaController->clearParas(data_mode);
}

void HDialogParameterLibrary::currentItemHasChanged(QListWidgetItem *current)
{
    setControlEnable(current);
    if(current==nullptr){
        return;
    }
    auto name=current->data(Qt::UserRole).toString();

    auto list=ptrParaController->getDatas(data_mode,name);

    ui->lineEdit_name->setText(list.value(0));
    qint32 size=getLineEdits().size();
    for(qint32 k=0;k<size;++k){
        getLineEdits().at(k)->setText(list.value(k+1));
    }
}

void HDialogParameterLibrary::setControlEnable(bool enable)
{
    ui->toolButton_delete->setEnabled(enable);
    ui->toolButton_set_default->setEnabled(enable);
    ui->widget_name->setEnabled(enable);
    ui->tabWidget->setEnabled(enable);
    //    ui->tab_general->setEnabled(enable);
    //    ui->tab_cam1->setEnabled(enable);
    //    ui->tab_cam2->setEnabled(enable);
    //    ui->tab_laser1->setEnabled(enable);
    //    ui->tab_laser2->setEnabled(enable);
    ui->widget_button->setEnabled(enable);
}

void HDialogParameterLibrary::on_toolButton_save_clicked()
{
    QStringList list;
    QString dname=ui->listWidget->currentItem()->text();
    if(checkSameName(ui->lineEdit_name->text(),ui->listWidget)){
        ui->lineEdit_name->setText(dname);
        list.push_back(dname);
    }else{
        ui->listWidget->currentItem()->setText(ui->lineEdit_name->text());
        list.push_back(ui->lineEdit_name->text());

        if(dname==ptrParaController->getDefaultName(data_mode)){
            ptrParaController->setDefaultName(data_mode,list.first());
        }
    }

    for(const auto&k:getLineEdits()){
        list.push_back(k->text());
    }
    ui->listWidget->currentItem()->setData(Qt::UserRole,list.first());

    ptrParaController->setDatas(data_mode,dname,list);
}

void HDialogParameterLibrary::on_toolButton_set_default_clicked()
{
    qint32 size=ui->listWidget->count();
    if(size==0)
        return;
    for(qint32 k=0;k<size;++k){
        ui->listWidget->item(k)->setIcon(defaultIcon);
    }
    ui->listWidget->currentItem()->setIcon(ui->toolButton_set_default->icon());

    ptrParaController->setDefaultName(data_mode,ui->listWidget->currentItem()->text());

}

void HDialogParameterLibrary::showEvent(QShowEvent *event)
{
    data_mode=IniSettings::GetInstance()->getValue(IniSettings::Edit_mode).toInt();
    refreshAllControl(data_mode);
    ui->listWidget->setCurrentItem(ui->listWidget->item(ptrParaController->getDefaultIndex(data_mode)));
    QDialog::showEvent(event);
}

inline QVector<QLineEdit *> &HDialogParameterLibrary::getLineEdits()
{
    switch (data_mode) {
    case 0:
        return lineEdits_cutter;
    case 1:
        return lineEdits_scanner;
    default:
        return lineEdits_cutter;
    }
}

void HDialogParameterLibrary::refreshAllControl(qint32 mode)
{
    ui->listWidget->clear();
    auto defaultName=ptrParaController->getDefaultName(mode);
    for(const auto &k:ptrParaController->getNames(mode)){

        auto mk=new QListWidgetItem(k);
        // mk->setFlags( Qt::ItemIsEnabled|/*Qt::ItemIsEditable|*/Qt::ItemIsSelectable);

        mk->setData(Qt::UserRole,k);

        mk->setIcon(defaultName==k?ui->toolButton_set_default->icon():defaultIcon);
        ui->listWidget->addItem(mk);
    }

    switch (data_mode) {
    case 0:{
        ui->tabWidget->clear();
        ui->tabWidget->addTab(ui->tab_general_cutter,tr("General"));
        ui->tabWidget->addTab(ui->tab_laser_cutter,tr("Laser"));
        ui->tabWidget->addTab(ui->tab_cam_cutter,tr("Cam"));
    }
        break;
    case 1:{
        ui->tabWidget->clear();
        ui->tabWidget->addTab(ui->tab_general_scanner,tr("General"));
        ui->tabWidget->addTab(ui->tab_laser_scanner,tr("Laser"));
        ui->tabWidget->addTab(ui->tab_cam_scanner,tr("Cam"));
    }
        break;
    default:{
        ui->tabWidget->clear();
    }
        break;
    }

}








