#include "hdialogsystemadjust.h"
#include "ui_hdialogsystemadjust.h"
#include <QMessageBox>

#include "htableview.h"

#include "hsystemadjustcontroller.h"
#include "hparalibcontroller.h"

#include "hmathfunctions.h"

#include "Src/Motion/h_motioncontroller.h"
#include "Src/Motion/hcameracontroller.h"
#include "Src/data/hsettings.h"
#include "Src/data/hdialogproperty.h"
#include "Src/data/inisettings.h"

#include "Src/Gui/h_cameracontrollerwidget.h"
#include "interface/hinterfacecontroller.h"



HDialogSystemAdjust::HDialogSystemAdjust(QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent,f),
    ui(new Ui::HDialogSystemAdjust)
{
    ui->setupUi(this);
    setFont(MyFont1);

    ptrMotion=H_MotionController::GetInstance();
    ptrCameraDevice=HCameraController::GetInstance();
    ptrSettings =HSettings::GetInstance();
    ptrIniSetings=IniSettings::GetInstance();
    ptrAdjustController=HSystemAdjustController::GetInstance();
    ptrParaLibController=HParaLibController::GetInstance();

    ptrCameraShow=H_CameraControllerWidget::GetInstance();

    enableWidArray.resize(6);
    enableWidArray[0]<<ui->wid_enable_camera<<ui->wid_enable_camera_2<<ui->wid_enable_camera_3<<ui->wid_enable_camera_5;
    enableWidArray[1]<<ui->wid_enable_vision<<ui->wid_enable_vision_2<<ui->wid_enable_vision_3<<ui->wid_enable_vision_4<<ui->wid_enable_vision_5;
    enableWidArray[2]<<ui->wid_enable_cutter<<ui->wid_enable_cutter_2<<ui->wid_enable_cutter_3<<ui->wid_enable_cutter_4<<ui->wid_enable_cutter_5;
    enableWidArray[3]<<ui->wid_enable_scannerHeader<<ui->wid_enable_scannerHeader_2<<ui->wid_enable_scannerHeader_3<<ui->wid_enable_scannerHeader_4<<ui->wid_enable_scannerHeader_5;
    enableWidArray[4]<<ui->wid_enable_scannerLens<<ui->wid_enable_scannerLens_2<<ui->wid_enable_scannerLens_3<<ui->wid_enable_scannerLens_4<<ui->wid_enable_scannerLens_5;
    enableWidArray[5]<<ui->wid_enable_scannerRange<<ui->wid_enable_scannerRange_2<<ui->wid_enable_scannerRange_3<<ui->wid_enable_scannerRange_4<<ui->wid_enable_scannerRange_5;

    combboxWidArray.resize(6);
    combboxWidArray[0]<<ui->comboBox_camera_axisX<<ui->comboBox_camera_axisY<<ui->comboBox_camera_axisZ<<ui->comboBox_camera_device
                     <<ui->comboBox_camera_tl_func<<ui->comboBox_camera_bl_func<<ui->comboBox_camera_br_func<<ui->comboBox_camera_tr_func<<ui->comboBox_camera_c_func;
    combboxWidArray[1]<<ui->comboBox_vision_camera<<ui->comboBox_vision_tl_func<<ui->comboBox_vision_bl_func<<ui->comboBox_vision_br_func<<ui->comboBox_vision_tr_func;
    combboxWidArray[2]<<ui->comboBox_cutter_axisX<<ui->comboBox_cutter_axisY<<ui->comboBox_cutter_axisZ<<ui->comboBox_cutter_vision<<ui->comboBox_cutter_para<<ui->comboBox_cutter_func
                     <<ui->comboBox_cutter_mark_type;
    combboxWidArray[3]<<ui->comboBox_scannerHeader_axisX<<ui->comboBox_scannerHeader_axisY<<ui->comboBox_scannerHeader_axisZ<<ui->comboBox_scannerHeader_vision
                     <<ui->comboBox_scannerHeader_device<<ui->comboBox_scannerHeader_para<<ui->comboBox_scannerHeader_func<<ui->comboBox_scannerHeader_mark_type;
    combboxWidArray[4]<<ui->comboBox_scannerLens_scannerHeader<<ui->comboBox_scannerLens_para<<ui->comboBox_scannerLens_func<<ui->comboBox_scannerLens_mark_type;
    combboxWidArray[5]<<ui->comboBox_scannerRange_scannerLens<<ui->comboBox_scannerRange_para<<ui->comboBox_scannerRange_func<<ui->comboBox_scannerRange_mark_type;

    tableViewArray<<ui->tableView<<ui->tableView_2<<ui->tableView_3<<ui->tableView_4<<ui->tableView_5<<ui->tableView_6;


    for(qint32 k=0;k<tableViewArray.size();++k){
        tableViewArray.at(k)->setHeaderNames(HVariantList("Name,Status",","));
        tableViewArray.at(k)->setColCtrlEditables("0,0");
        tableViewArray.at(k)->setUniqueColIndex(0);
        tableViewArray.at(k)->setAllDatasUseRow(ptrAdjustController->getOutline(k));
        tableViewArray.at(k)->setHighlightRowsUseFirstColumn(ptrAdjustController->getCurrentOutline(k));
        tableViewArray.at(k)->setUserIndex(k);
        connect(tableViewArray.at(k),&HTableView::currentRowChanged,this,&HDialogSystemAdjust::handlerCurrentItemChanged);
    }

    tableViewClearButtonArray<<ui->toolButton_camera_clear<<ui->toolButton_vision_clear<<ui->toolButton_cutter_clear
                            <<ui->toolButton_scannerHeader_clear<<ui->toolButton_scannerLens_clear<<ui->toolButton_scannerRange_clear;
    for(qint32 k=0;k<tableViewClearButtonArray.size();++k){
        connect(tableViewClearButtonArray.at(k),&QToolButton::clicked,this,[=](){
            tableViewArray.at(k)->clearAllRows();
            ptrAdjustController->clearPage(k);
            handlerCurrentItemChanged(-1,k);
        });


    }
    tableViewAddButtonArray<<ui->toolButton_camera_add<<ui->toolButton_vision_add<<ui->toolButton_cutter_add
                          <<ui->toolButton_scannerHeader_add<<ui->toolButton_scannerLens_add<<ui->toolButton_scannerRange_add;

    for(qint32 k=0;k<tableViewAddButtonArray.size();++k){

        connect(tableViewAddButtonArray.at(k),&QToolButton::clicked,this,[=](){
            HDialogProperty dialog(tr("new parameter"));
            dialog.setLabels("name");
            QVariantList dlist;
            dlist<<"new parameter";
            dialog.setContents("0",dlist);
            dialog.setUniqueKeys(0,tableViewArray.at(k)->getDataAtCol(0));

            if(dialog.exec()){
                auto resdata=dialog.getContentData();
                resdata<<"no adjust";
                tableViewArray.at(k)->addRowData(resdata);
                tableViewArray.at(k)->resizeColumnToContents(0);
                if(ptrAdjustController->addNewData(k,HAdjustOutline(resdata.first().toString(),"no adjust",-1))){
                    tableViewArray.at(k)->setHighlightRowsUseFirstColumn(ptrAdjustController->getCurrentOutline(k));
                }
            }
        });
    }

    tableViewDeleteButtonArray<<ui->toolButton_camera_delete<<ui->toolButton_vision_delete<<ui->toolButton_cutter_delete
                             <<ui->toolButton_scannerHeader_delete<<ui->toolButton_scannerLens_delete<<ui->toolButton_scannerRange_delete;
    for(qint32 k=0;k<tableViewDeleteButtonArray.size();++k){
        connect(tableViewDeleteButtonArray.at(k),&QToolButton::clicked,this,[=](){
            if(ptrAdjustController->deletePageData(k,tableViewArray.at(k)->getCurrentUniqueKey().toString())){
                tableViewArray.at(k)->setHighlightRowsUseFirstColumn(ptrAdjustController->getCurrentOutline(k));
            }
            tableViewArray.at(k)->deleteCurrentRow();
        });

    }

    tableViewSaveButtonArray<<ui->toolButton_camera_save<<ui->toolButton_vision_save<<ui->toolButton_cutter_save
                           <<ui->toolButton_scannerHeader_save<<ui->toolButton_scannerLens_save<<ui->toolButton_scannerRange_save;

    for(qint32 k=0;k<tableViewSaveButtonArray.size();++k){
        connect(tableViewSaveButtonArray.at(k),&QToolButton::clicked,this,[=](){
            auto mk=tableViewArray.at(k)->getDataAtCurrentRow().toStringList_h();
            auto content=getCurrentContents(k);
            if(mk.first()!=content.first()&&tableViewArray.at(k)->getDataAtCol(0).toStringList_h().contains(content.first())){
                QMessageBox::warning(this,tr("parameter error"),QString("name is existed!!"));
                content.first()=mk.first();
            }
            qint32 currentRow=tableViewArray.at(k)->getCurrentRow();
            ptrAdjustController->savePageData(k,mk.first(),HAdjustOutline(content.first(),mk.value(1),mk.value(1)=="OK"?0:-1),content);
            tableViewArray.at(k)->setAllDatasUseRow(ptrAdjustController->getOutline(k));
            tableViewArray.at(k)->setCurrentRow(currentRow);
        });
    }

    tableViewSetButtonArray<<ui->toolButton_camera_set<<ui->toolButton_vision_set<<ui->toolButton_cutter_set
                          <<ui->toolButton_scannerHeader_set<<ui->toolButton_scannerLens_set<<ui->toolButton_scannerRange_set;
    for(qint32 k=0;k<tableViewSetButtonArray.size();++k){
        connect(tableViewSetButtonArray.at(k),&QToolButton::clicked,this,[=](){
            auto mk=tableViewArray.at(k)->getDataAtCurrentRow().toStringList_h();
            qint32 currentRow=tableViewArray.at(k)->getCurrentRow();
            ptrAdjustController->setDefautOutline(k,mk.first());
            tableViewArray.at(k)->setHighlightRowsUseFirstColumn(ptrAdjustController->getCurrentOutline(k));
            tableViewArray.at(k)->setCurrentRow(currentRow);
        });
    }

    contentWidArray.resize(6);
    contentWidArray[0]<<ui->lineEdit_camera_name<<ui->comboBox_camera_axisX<<ui->comboBox_camera_axisY<<ui->comboBox_camera_axisZ<<ui->comboBox_camera_device
                     <<ui->lineEdit_camera_focus<<ui->lineEdit_camera_tl_x
                    <<ui->lineEdit_camera_tl_y<<ui->comboBox_camera_tl_func<<ui->lineEdit_camera_bl_x<<ui->lineEdit_camera_bl_y<<ui->comboBox_camera_bl_func
                   << ui->lineEdit_camera_br_x<<ui->lineEdit_camera_br_y<<ui->comboBox_camera_br_func<<ui->lineEdit_camera_tr_x<<ui->lineEdit_camera_tr_y
                   <<ui->comboBox_camera_tr_func<<ui->lineEdit_camera_c_x<<ui->lineEdit_camera_c_y<<ui->comboBox_camera_c_func;

    contentWidArray[1]<<ui->lineEdit_vision_name<<ui->comboBox_vision_camera<<ui->lineEdit_vision_row<<ui->lineEdit_vision_column<<ui->lineEdit_vision_tl_x
                     <<ui->lineEdit_vision_tl_y<<ui->comboBox_vision_tl_func<<ui->lineEdit_vision_bl_x<<ui->lineEdit_vision_bl_y<<ui->comboBox_vision_bl_func
                    <<ui->lineEdit_vision_br_x<<ui->lineEdit_vision_br_y<<ui->comboBox_vision_br_func<<ui->lineEdit_vision_tr_x<<ui->lineEdit_vision_tr_y
                   <<ui->comboBox_vision_tr_func;

    contentWidArray[2]<<ui->lineEdit_cutter_name<<ui->comboBox_cutter_axisX<<ui->comboBox_cutter_axisY<<ui->comboBox_cutter_axisZ<<ui->comboBox_cutter_vision<<ui->lineEdit_cutter_focus
                     <<ui->comboBox_cutter_para<<ui->comboBox_cutter_func<<ui->comboBox_cutter_mark_type<<ui->lineEdit_cutter_mark_size<<ui->lineEdit_cutter_fadj_x<<ui->lineEdit_cutter_fadj_y<<ui->lineEdit_cutter_tl_x
                    <<ui->lineEdit_cutter_tl_y<<ui->lineEdit_cutter_bl_x<<ui->lineEdit_cutter_bl_y<<ui->lineEdit_cutter_br_x<<ui->lineEdit_cutter_br_y
                   <<ui->lineEdit_cutter_tr_x<<ui->lineEdit_cutter_tr_y<<ui->lineEdit_cutter_row<<ui->lineEdit_cutter_col;

    contentWidArray[3]<<ui->lineEdit_scannerHeader_name<<ui->comboBox_scannerHeader_axisX<<ui->comboBox_scannerHeader_axisY<<ui->comboBox_scannerHeader_axisZ
                     <<ui->comboBox_scannerHeader_vision<<ui->comboBox_scannerHeader_device<<ui->lineEdit_scannerHeader_fadj_x<<ui->lineEdit_scannerHeader_fadj_y
                    <<ui->comboBox_scannerHeader_para<<ui->comboBox_scannerHeader_func<<ui->lineEdit_scannerHeader_centerX<<ui->lineEdit_scannerHeader_centerY
                   <<ui->lineEdit_scannerHeader_width<<ui->lineEdit_scannerHeader_height<<ui->lineEdit_scannerHeader_row<<ui->lineEdit_scannerHeader_column
                  <<ui->lineEdit_scannerHeader_mark_width<<ui->lineEdit_scannerHeader_mark_height<<ui->comboBox_scannerHeader_mark_type;

    contentWidArray[4]<<ui->lineEdit_scannerLens_name<<ui->comboBox_scannerLens_scannerHeader<<ui->comboBox_scannerLens_para<<ui->comboBox_scannerLens_func
                     <<ui->lineEdit_scannerLens_centerX<<ui->lineEdit_scannerLens_centerY<<ui->lineEdit_scannerLens_width<<ui->lineEdit_scannerLens_height
                    <<ui->lineEdit_scannerLens_row<<ui->lineEdit_scannerLens_column<<ui->lineEdit_scannerLens_mark_width<<ui->lineEdit_scannerLens_mark_height
                   <<ui->comboBox_scannerLens_mark_type;

    contentWidArray[5]<<ui->lineEdit_scannerRange_name<<ui->comboBox_scannerRange_scannerLens<<ui->comboBox_scannerRange_para<<ui->comboBox_scannerRange_func
                     <<ui->lineEdit_scannerRange_centerX<<ui->lineEdit_scannerRange_centerY<<ui->lineEdit_scannerRange_piece_row<<ui->lineEdit_scannerRange_piece_column
                    <<ui->lineEdit_scannerRange_piece_width<<ui->lineEdit_scannerRange_piece_height<<ui->checkBox_scannerRange_whole_lens<<ui->lineEdit_scannerRange_mark_row
                   <<ui->lineEdit_scannerRange_mark_column<<ui->lineEdit_scannerRange_mark_width<<ui->lineEdit_scannerRange_mark_height<<ui->comboBox_scannerRange_mark_type;

    for(const auto&k:enableWidArray){
        for(const auto&m:k){
            m->setEnabled(false);
        }
    }


    connect(ui->pushButton_camera_tl_get,&QPushButton::clicked,this,[=](){
        auto paxis1=ptrMotion->GetAxis(ui->comboBox_camera_axisX->currentText());
        auto paxis2=ptrMotion->GetAxis(ui->comboBox_camera_axisY->currentText());
        if(paxis1&&paxis2){
            ui->lineEdit_camera_tl_x->setText(paxis1->GetFeedbackPosString());
            ui->lineEdit_camera_tl_y->setText(paxis2->GetFeedbackPosString());
        }
    });

    connect(ui->pushButton_camera_bl_get,&QPushButton::clicked,this,[=](){
        auto paxis1=ptrMotion->GetAxis(ui->comboBox_camera_axisX->currentText());
        auto paxis2=ptrMotion->GetAxis(ui->comboBox_camera_axisY->currentText());
        if(paxis1&&paxis2){
            ui->lineEdit_camera_bl_x->setText(paxis1->GetFeedbackPosString());
            ui->lineEdit_camera_bl_y->setText(paxis2->GetFeedbackPosString());
        }
    });

    connect(ui->pushButton_camera_br_get,&QPushButton::clicked,this,[=](){
        auto paxis1=ptrMotion->GetAxis(ui->comboBox_camera_axisX->currentText());
        auto paxis2=ptrMotion->GetAxis(ui->comboBox_camera_axisY->currentText());
        if(paxis1&&paxis2){
            ui->lineEdit_camera_br_x->setText(paxis1->GetFeedbackPosString());
            ui->lineEdit_camera_br_y->setText(paxis2->GetFeedbackPosString());
        }
    });

    connect(ui->pushButton_camera_tr_get,&QPushButton::clicked,this,[=](){
        auto paxis1=ptrMotion->GetAxis(ui->comboBox_camera_axisX->currentText());
        auto paxis2=ptrMotion->GetAxis(ui->comboBox_camera_axisY->currentText());
        if(paxis1&&paxis2){
            ui->lineEdit_camera_tr_x->setText(paxis1->GetFeedbackPosString());
            ui->lineEdit_camera_tr_y->setText(paxis2->GetFeedbackPosString());
        }
    });

    connect(ui->pushButton_camera_c_get,&QPushButton::clicked,this,[=](){
        auto paxis1=ptrMotion->GetAxis(ui->comboBox_camera_axisX->currentText());
        auto paxis2=ptrMotion->GetAxis(ui->comboBox_camera_axisY->currentText());
        if(paxis1&&paxis2){
            ui->lineEdit_camera_c_x->setText(paxis1->GetFeedbackPosString());
            ui->lineEdit_camera_c_y->setText(paxis2->GetFeedbackPosString());
        }
    });



    connect(ui->pushButton_vision_tl_get,&QPushButton::clicked,this,[=](){
        auto cameraName=ui->comboBox_vision_camera->currentText();
        if(cameraName.isEmpty()){
            return;
        }
        auto pt=ptrAdjustController->getImageCenterFromCamera(cameraName,ui->comboBox_vision_tl_func->currentText());


        ui->lineEdit_vision_tl_x->setText(QString::number(pt.x()));
        ui->lineEdit_vision_tl_y->setText(QString::number(pt.y()));

    });

    connect(ui->pushButton_vision_tr_get,&QPushButton::clicked,this,[=](){
        auto cameraName=ui->comboBox_vision_camera->currentText();
        if(cameraName.isEmpty()){
            return;
        }
        auto pt=ptrAdjustController->getImageCenterFromCamera(cameraName,ui->comboBox_vision_tr_func->currentText());

        ui->lineEdit_vision_tr_x->setText(QString::number(pt.x()));
        ui->lineEdit_vision_tr_y->setText(QString::number(pt.y()));
    });

    connect(ui->pushButton_vision_br_get,&QPushButton::clicked,this,[=](){
        auto cameraName=ui->comboBox_vision_camera->currentText();
        if(cameraName.isEmpty()){
            return;
        }
        auto pt=ptrAdjustController->getImageCenterFromCamera(cameraName,ui->comboBox_vision_br_func->currentText());

        ui->lineEdit_vision_br_x->setText(QString::number(pt.x()));
        ui->lineEdit_vision_br_y->setText(QString::number(pt.y()));
    });

    connect(ui->pushButton_vision_bl_get,&QPushButton::clicked,this,[=](){

        auto cameraName=ui->comboBox_vision_camera->currentText();
        if(cameraName.isEmpty()){
            return;
        }
        auto pt=ptrAdjustController->getImageCenterFromCamera(cameraName,ui->comboBox_vision_bl_func->currentText());

        ui->lineEdit_vision_bl_x->setText(QString::number(pt.x()));
        ui->lineEdit_vision_bl_y->setText(QString::number(pt.y()));
    });


    connect(ui->pushButton_cutter_focus_get,&QPushButton::clicked,this,[=](){
        auto paxis=ptrMotion->GetAxis(ui->comboBox_cutter_axisZ->currentText());
        if(paxis){
            ui->lineEdit_cutter_focus->setText(paxis->GetFeedbackPosString());
        }
    });


    connect(ui->pushButton_cutter_tl_get,&QPushButton::clicked,this,[=](){
        auto paxis1=ptrMotion->GetAxis(ui->comboBox_cutter_axisX->currentText());
        auto paxis2=ptrMotion->GetAxis(ui->comboBox_cutter_axisY->currentText());
        if(paxis1&&paxis2){
            ui->lineEdit_cutter_tl_x->setText(paxis1->GetFeedbackPosString());
            ui->lineEdit_cutter_tl_y->setText(paxis2->GetFeedbackPosString());
        }
    });

    connect(ui->pushButton_cutter_bl_get,&QPushButton::clicked,this,[=](){
        auto paxis1=ptrMotion->GetAxis(ui->comboBox_cutter_axisX->currentText());
        auto paxis2=ptrMotion->GetAxis(ui->comboBox_cutter_axisY->currentText());
        if(paxis1&&paxis2){
            ui->lineEdit_cutter_bl_x->setText(paxis1->GetFeedbackPosString());
            ui->lineEdit_cutter_bl_y->setText(paxis2->GetFeedbackPosString());
        }
    });

    connect(ui->pushButton_cutter_br_get,&QPushButton::clicked,this,[=](){
        auto paxis1=ptrMotion->GetAxis(ui->comboBox_cutter_axisX->currentText());
        auto paxis2=ptrMotion->GetAxis(ui->comboBox_cutter_axisY->currentText());
        if(paxis1&&paxis2){
            ui->lineEdit_cutter_br_x->setText(paxis1->GetFeedbackPosString());
            ui->lineEdit_cutter_br_y->setText(paxis2->GetFeedbackPosString());
        }
    });

    connect(ui->pushButton_cutter_tr_get,&QPushButton::clicked,this,[=](){
        auto paxis1=ptrMotion->GetAxis(ui->comboBox_cutter_axisX->currentText());
        auto paxis2=ptrMotion->GetAxis(ui->comboBox_cutter_axisY->currentText());
        if(paxis1&&paxis2){
            ui->lineEdit_cutter_tr_x->setText(paxis1->GetFeedbackPosString());
            ui->lineEdit_cutter_tr_y->setText(paxis2->GetFeedbackPosString());
        }
    });


}

HDialogSystemAdjust::~HDialogSystemAdjust()
{
    delete ui;
}

void HDialogSystemAdjust::handlerCurrentItemChanged(qint32 rowIndex, qint32 itemIndex)
{
    for(const auto&k:enableWidArray.at(itemIndex)){
        k->setEnabled(rowIndex!=-1);
    }
    if(rowIndex==-1)
        return;
    setCurrentContents(itemIndex,ptrAdjustController->getContent(HSystemAdjustController::ContentType(itemIndex),tableViewArray.at(itemIndex)->getCurrentUniqueKey().toString()));
}

void HDialogSystemAdjust::showEvent(QShowEvent *event)
{
    on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
    QDialog::showEvent(event);
}

void HDialogSystemAdjust::showMsg(const QColor &color, const QString &msg)
{
    ui->textBrowser->setTextColor(color);
    ui->textBrowser->append(msg);
}

QVector<QString> HDialogSystemAdjust::getCurrentContents(qint32 index) const
{
    QVector<QString> list;
    for(const auto&k:contentWidArray.at(index)){
        if(qobject_cast<QLineEdit*>(k)){
            list<<qobject_cast<QLineEdit*>(k)->text();
        }else if(qobject_cast<QComboBox*>(k)){
            list<<qobject_cast<QComboBox*>(k)->currentText();
        }else if(qobject_cast<QCheckBox*>(k)){
            list<<QString::number(qobject_cast<QCheckBox*>(k)->checkState());
        }
    }
    return list;
}

void HDialogSystemAdjust::setCurrentContents(qint32 index, const QVector<QString> &datas)
{
    qint32 size=contentWidArray.at(index).size();
    for(qint32 k=0;k<size;++k){
        auto mgh=contentWidArray.at(index).at(k);
        if(qobject_cast<QLineEdit*>(mgh)){
            qobject_cast<QLineEdit*>(mgh)->setText(datas.value(k));
        }else if(qobject_cast<QComboBox*>(mgh)){
            qobject_cast<QComboBox*>(mgh)->setCurrentText(datas.value(k));
        }else if(qobject_cast<QCheckBox*>(mgh)){
            qobject_cast<QCheckBox*>(mgh)->setChecked(datas.value(k).toInt());
        }
    }
}

void HDialogSystemAdjust::on_tabWidget_currentChanged(int index)
{

    QStringList dfing;
    for(const auto&k:combboxWidArray.at(index)){
        dfing<<k->currentText();
        k->clear();
    }

    auto funcs=HInterfaceController::GetInstance()->imgInterface->getProcessFuncNames();
    auto axiss=ptrMotion->GetAxisNames();


    const QStringList markTypes={"circle","cross","rectangle"};

    switch (index) {
    case 0:{
        ui->comboBox_camera_axisX->addItems(axiss);
        ui->comboBox_camera_axisY->addItems(axiss);
        ui->comboBox_camera_axisZ->addItems(axiss);
        ui->comboBox_camera_device->addItems(ptrCameraDevice->GetCameraNames());

        ui->comboBox_camera_tl_func->addItems(funcs);
        ui->comboBox_camera_bl_func->addItems(funcs);
        ui->comboBox_camera_br_func->addItems(funcs);
        ui->comboBox_camera_tr_func->addItems(funcs);
        ui->comboBox_camera_c_func->addItems(funcs);
    }
        break;
    case 1:{
        ui->comboBox_vision_camera->addItems(tableViewArray.at(0)->getDataAtCol(0).toStringList_h());

        ui->comboBox_vision_tl_func->addItems(funcs);
        ui->comboBox_vision_bl_func->addItems(funcs);
        ui->comboBox_vision_br_func->addItems(funcs);
        ui->comboBox_vision_tr_func->addItems(funcs);
    }
        break;
    case 2:{
        ui->comboBox_cutter_axisX->addItems(axiss);
        ui->comboBox_cutter_axisY->addItems(axiss);
        ui->comboBox_cutter_axisZ->addItems(axiss);

        ui->comboBox_cutter_vision->addItems(tableViewArray.at(1)->getDataAtCol(0).toStringList_h());

        ui->comboBox_cutter_para->addItems(ptrParaLibController->getNames(0));
        ui->comboBox_cutter_func->addItems(funcs);

        ui->comboBox_cutter_mark_type->addItems(markTypes);
    }
        break;
    case 3:{
        ui->comboBox_scannerHeader_axisX->addItems(axiss);
        ui->comboBox_scannerHeader_axisY->addItems(axiss);
        ui->comboBox_scannerHeader_axisZ->addItems(axiss);

        ui->comboBox_scannerHeader_vision->addItems(tableViewArray.at(1)->getDataAtCol(0).toStringList_h());

        //ui->comboBox_scannerHeader_device
        ui->comboBox_scannerHeader_para->addItems(ptrParaLibController->getNames(1));

        ui->comboBox_scannerHeader_func->addItems(funcs);
        ui->comboBox_scannerHeader_mark_type->addItems(markTypes);
    }
        break;
    case 4:{
        ui->comboBox_scannerLens_scannerHeader->addItems(tableViewArray.at(3)->getDataAtCol(0).toStringList_h());
        ui->comboBox_scannerLens_para->addItems(ptrParaLibController->getNames(1));
        ui->comboBox_scannerLens_func->addItems(funcs);
        ui->comboBox_scannerLens_mark_type->addItems(markTypes);
    }
        break;
    case 5:{
        ui->comboBox_scannerRange_scannerLens->addItems(tableViewArray.at(4)->getDataAtCol(0).toStringList_h());
        ui->comboBox_scannerRange_para->addItems(ptrParaLibController->getNames(1));
        ui->comboBox_scannerRange_func->addItems(funcs);
        ui->comboBox_scannerRange_mark_type->addItems(markTypes);
    }
        break;
    default:
        return;
    }

    for(const auto&k:combboxWidArray.at(index)){
        k->setCurrentText(dfing.takeFirst());
    }
}

void HDialogSystemAdjust::on_toolButton_camera_location_run_clicked()
{
    QVector<QPointF>  src;
    QVector<QPointF>  dst;
    //QPointF firstPt=QPointF(data.first().toDouble(),data.at(1).toDouble());
    //QPointF firstCamPt;
    dst<<QPointF(ui->lineEdit_camera_tl_x->text().toDouble(),ui->lineEdit_camera_tl_y->text().toDouble());
    dst<<QPointF(ui->lineEdit_camera_bl_x->text().toDouble(),ui->lineEdit_camera_bl_y->text().toDouble());
    dst<<QPointF(ui->lineEdit_camera_br_x->text().toDouble(),ui->lineEdit_camera_br_y->text().toDouble());
    dst<<QPointF(ui->lineEdit_camera_tr_x->text().toDouble(),ui->lineEdit_camera_tr_y->text().toDouble());

    dst<<QPointF(ui->lineEdit_camera_c_x->text().toDouble(),ui->lineEdit_camera_c_y->text().toDouble());



    auto axisx=ptrMotion->GetAxis(ui->comboBox_camera_axisX->currentText());
    auto axisy=ptrMotion->GetAxis(ui->comboBox_camera_axisY->currentText());
    auto axisz=ptrMotion->GetAxis(ui->comboBox_camera_axisZ->currentText());

    auto cameraName=ui->comboBox_camera_device->currentText();
    auto camera=ptrCameraDevice->GetCamera(cameraName);

    if(!axisx||!axisy||!axisz||!camera){
        showMsg(Qt::red,tr("device no exist!!"));
        return;
    }

    QStringList funcs;
    funcs<<ui->comboBox_camera_tl_func->currentText()<<ui->comboBox_camera_bl_func->currentText()<<ui->comboBox_camera_br_func->currentText()
        <<ui->comboBox_camera_tr_func->currentText()<<ui->comboBox_camera_c_func->currentText();

    auto ptrImgProcess=HInterfaceController::GetInstance()->imgInterface;
    if(!ptrImgProcess){
        showMsg(Qt::red,tr("plugin of image process load failed"));
        return;
    }

    if(axisz->MoveToWait(ui->lineEdit_camera_focus->text().toDouble())){
        showMsg(Qt::red,tr("axis Z move failed"));
        return;
    }
    for(const auto&k:dst){
        if(axisx->MoveTo(k.x())){
            showMsg(Qt::red,tr("axis X move failed"));
            return;
        }
        if(axisy->MoveTo(k.y())){
            showMsg(Qt::red,tr("axis Y move failed"));
            return;
        }
        if(axisx->WaitAxisStop()||axisy->WaitAxisStop()){
            showMsg(Qt::red,tr("axis wait stop failed"));
            return;
        }
        if(ptrCameraShow->showOneImg(cameraName)){
            showMsg(Qt::red,tr("image capture failed"));
            return;
        }
        qint32 w,h;
        uchar *pData=ptrCameraShow->getCurrentImage(w,h);
        if(ptrImgProcess->showImage(pData,w,h)){
            showMsg(Qt::red,tr("get image failed"));
            return;
        }
        QPointF result;
        if(ptrImgProcess->processImage(funcs.takeFirst(),result)){
            showMsg(Qt::red,tr("image process failed"));
            return;
        }
        ptrCameraShow->addShowCross(result);
        ptrCameraShow->addShowText("OK",Qt::darkGreen);

        src<<result;

        continue;
    }


    QTransform trans=HMathFunc::GetPerspectiveTransform(src,dst);
    QPointF error=dst.last()-trans.map(src.last());
    ui->textBrowser->setTextColor(Qt::blue);
    ui->textBrowser->append(tr("complete!!"));
    ui->textBrowser->append(QString(tr("result: x=%1    y=%2")).arg(error.x(),0,'f',6).arg(error.y(),0,'f',6));

    QStringList coffset;
    QVector<QPointF>  coff;

    auto centerppt=trans.map(QPointF(0,0));
    for(auto &k:dst){
        k=k-centerppt;
        coff<<k;
    }
    coffset<<HMathFunc::pointsToString(coff);
    trans=HMathFunc::GetPerspectiveTransform(src,dst);

    QMap<HAdjustTransQPolygonF, QTransform> transff;
    transff.insert(HAdjustTransQPolygonF(),trans);
    auto mk=tableViewArray.at(0)->getDataAtCurrentRow().toStringList_h();
    auto content=getCurrentContents(0);
    if(mk.first()!=content.first()&&tableViewArray.at(0)->getDataAtCol(0).toStringList_h().contains(content.first())){
        QMessageBox::warning(this,tr("parameter error"),QString("name is existed!!"));
        content.first()=mk.first();
    }
    qint32 currentRow=tableViewArray.at(0)->getCurrentRow();

    ptrAdjustController->saveAllData(HSystemAdjustController::camera,mk.first(),HAdjustOutline(content.first(),"OK",0),content,coffset,transff);
    tableViewArray.at(0)->setAllDatasUseRow(ptrAdjustController->getOutline(0));
    tableViewArray.at(0)->setCurrentRow(currentRow);

    return;




}

void HDialogSystemAdjust::on_toolButton_vision_run_clicked()
{

    auto cameraName=ui->comboBox_vision_camera->currentText();
    if(cameraName.isEmpty()){
        return;
    }

    if(ptrAdjustController->getOutline(HSystemAdjustController::camera,cameraName).state){
        showMsg(Qt::red,tr("selected camera is no adjust!"));
        return;
    }
    auto contentData=ptrAdjustController->getContent(HSystemAdjustController::camera,cameraName);

    auto axisX=contentData.value(1);
    auto axisY=contentData.value(2);
    auto axisZ=contentData.value(3);
    auto deviceName=contentData.value(4);

    auto paxisX=ptrMotion->GetAxis(axisX);
    auto paxisY=ptrMotion->GetAxis(axisY);
    auto paxisZ=ptrMotion->GetAxis(axisZ);
    auto camera=ptrCameraDevice->GetCamera(deviceName);
    if(!paxisX||!paxisY||!paxisZ||!camera){
        showMsg(Qt::red,tr("device no exist!!"));
        return;
    }

    auto focus=contentData.value(5).toDouble();

    QStringList funcs;
    funcs<<contentData.value(8)<<contentData.value(11)<<contentData.value(14)<<contentData.value(17);

    QVector<QPointF>  src;
    QVector<QPointF>  dst;

    qint32 row=ui->lineEdit_vision_row->getNumberI();
    qint32 column=ui->lineEdit_vision_column->getNumberI();


    src<<QPointF(0,row)<<QPointF(0,0)<<QPointF(column,0)<<QPointF(column,row);
    dst<<QPointF(ui->lineEdit_vision_tl_x->getNumberF(),ui->lineEdit_vision_tl_y->getNumberF());
    dst<<QPointF(ui->lineEdit_vision_bl_x->getNumberF(),ui->lineEdit_vision_bl_y->getNumberF());
    dst<<QPointF(ui->lineEdit_vision_br_x->getNumberF(),ui->lineEdit_vision_br_y->getNumberF());
    dst<<QPointF(ui->lineEdit_vision_tr_x->getNumberF(),ui->lineEdit_vision_tr_y->getNumberF());


    QTransform trans=HMathFunc::GetPerspectiveTransform(src,dst);

    double width=qAbs(dst.at(0).x()-dst.at(1).x())/qAbs(column);
    double height=qAbs(dst.at(0).y()-dst.at(2).y())/qAbs(row);

    QString reason;
    auto ptrImgProcess=HInterfaceController::GetInstance()->imgInterface;


    if(paxisZ->MoveToWait(focus)){
        showMsg(Qt::red,tr("Axis Z move failed!"));
        return;
    }

    const QVector<QPointF>  coff=HMathFunc::stringToPoints(ptrAdjustController->getParas(HSystemAdjustController::camera,cameraName).first());


    QMap<HAdjustTransQPolygonF, QTransform> transff;
    for(qint32 r=0;(row>0)?(r<row+1):(r>row-1);(row>0)?(++r):(--r)){
        for(qint32 c=0;(column>0)?(c<column+1):(c>column-1);(column>0)?(++c):(--c)){
            auto cpt=trans.map(QPointF(c,r));

            QVector<QPointF>  src;
            QVector<QPointF>  dst;


                                    if(paxisX->MoveTo(cpt.x())){

                                    }
                                    if(paxisY->MoveTo(cpt.y())){

                                    }
                                    if(paxisX->WaitAxisStop()||paxisY->WaitAxisStop()){
                                        showMsg(Qt::red,tr("Axis wait stop failed!"));
                                        return;
                                    }
                                    if(ptrCameraShow->showOneImg(deviceName)){
                                        showMsg(Qt::red,tr("image capture failed"));
                                        return;
                                    }
                                    qint32 w,h;
                                    uchar *pData=ptrCameraShow->getCurrentImage(w,h);
                                    if(ptrImgProcess->showImage(pData,w,h)){
                                        showMsg(Qt::red,tr("get image failed"));
                                        return;
                                    }
                                    QPointF result;
                                    if(ptrImgProcess->processImage(funcs.at(0),result)){
                                            return;
                                    }
//            for(qint32 j=0;j<4;++j){
//                QPointF pt=cpt+coff.value(j);

//                dst<<pt;
//                if(paxisX->MoveTo(pt.x())){
//                    showMsg(Qt::red,tr("Axis X move failed!"));
//                    return;
//                }
//                if(paxisY->MoveTo(pt.y())){
//                    showMsg(Qt::red,tr("Axis Y move failed!"));
//                    return;
//                }
//                if(paxisX->WaitAxisStop()||paxisY->WaitAxisStop()){
//                    showMsg(Qt::red,tr("Axis wait stop failed!"));
//                    return;
//                }

//                if(ptrCameraShow->showOneImg(deviceName)){
//                    showMsg(Qt::red,tr("image capture failed"));
//                    return;
//                }
//                qint32 w,h;
//                uchar *pData=ptrCameraShow->getCurrentImage(w,h);
//                if(ptrImgProcess->showImage(pData,w,h)){
//                    showMsg(Qt::red,tr("get image failed"));
//                    return;
//                }
//                QPointF result;
//                if(ptrImgProcess->processImage(funcs.at(j),result)){
//                    showMsg(Qt::red,tr("image process failed"));
//                    return;
//                }
//                src<<result;
//                ptrCameraShow->addShowCross(result);
//                ptrCameraShow->addShowText("OK",Qt::darkGreen);
//            }

            QTransform mtrans=HMathFunc::GetPerspectiveTransform(src,dst);

            auto centerppt=mtrans.map(QPointF(0,0));
            for(auto &k:dst){
                k=k-centerppt;
            }
            mtrans=HMathFunc::GetPerspectiveTransform(src,dst);

            QRectF rect;
            rect.setWidth(width*1.001);
            rect.setHeight(height*1.001);
            rect.moveCenter(cpt);
            rect=rect.normalized();
            transff.insertMulti(HAdjustTransQPolygonF(rect,centerppt),mtrans);

        }
    }

    auto mk=tableViewArray.at(HSystemAdjustController::vision)->getDataAtCurrentRow().toStringList_h();
    auto content=getCurrentContents(HSystemAdjustController::vision);
    if(mk.first()!=content.first()&&tableViewArray.at(HSystemAdjustController::vision)->getDataAtCol(0).toStringList_h().contains(content.first())){
        QMessageBox::warning(this,tr("parameter error"),QString("name is existed!!"));
        content.first()=mk.first();
    }
    qint32 currentRow=tableViewArray.at(HSystemAdjustController::vision)->getCurrentRow();

    ptrAdjustController->saveAllData(HSystemAdjustController::vision,mk.first(),HAdjustOutline(content.first(),"OK",0),content,QStringList(),transff);
    tableViewArray.at(HSystemAdjustController::vision)->setAllDatasUseRow(ptrAdjustController->getOutline(HSystemAdjustController::vision));
    tableViewArray.at(HSystemAdjustController::vision)->setCurrentRow(currentRow);

    showMsg(Qt::darkGreen,tr("vision adjust succeeded!"));
}


void HDialogSystemAdjust::on_pushButton_first_cut_clicked()
{
    auto axisx=ptrMotion->GetAxis(ui->comboBox_cutter_axisX->currentText());
    auto axisy=ptrMotion->GetAxis(ui->comboBox_cutter_axisY->currentText());

    ui->lineEdit_cutter_fadj_x->setText(axisx->GetFeedbackPosString());
    ui->lineEdit_cutter_fadj_y->setText(axisy->GetFeedbackPosString());

    MotionCompiler::GetInstance()->createMarkGcodeAndRun(ui->comboBox_cutter_axisX->currentText(),ui->comboBox_cutter_axisY->currentText(),ui->comboBox_cutter_axisZ->currentText(),
                                                         QPointF(axisx->GetFeedbackPos(),axisy->GetFeedbackPos()),ui->comboBox_cutter_mark_type->currentIndex(),
                                                         ui->lineEdit_cutter_mark_size->getNumberF(),ui->comboBox_cutter_para->currentText());

}

void HDialogSystemAdjust::on_pushButton_first_cal_clicked()
{
    QPointF pos;
    if(ptrAdjustController->getAxisPosFromImageUseVision(pos,ui->comboBox_cutter_vision->currentText(),ui->comboBox_cutter_func->currentText(),false,ui->lineEdit_cutter_focus->getNumberF())){
        showMsg(Qt::red,tr("get vision position failed!!"));
        return;
    }
    auto offset=QPointF(ui->lineEdit_cutter_fadj_x->getNumberF(),ui->lineEdit_cutter_fadj_y->getNumberF())-pos;

    ptrAdjustController->saveParasData(HSystemAdjustController::cutter,ui->lineEdit_cutter_name->text(),QStringList(HMathFunc::transToString(QTransform::fromTranslate(offset.x(),offset.y()))));

    //   ui->lineEdit_cutter_fadj_x->setText(QString::number(offset.x()));
    //   ui->lineEdit_cutter_fadj_y->setText(QString::number(offset.y()));
}

void HDialogSystemAdjust::on_toolButton_cutter_run_clicked()
{
    auto visionName=ui->comboBox_cutter_vision->currentText();
    if(visionName.isEmpty()){
        return;
    }

    //    if(ptrAdjustController->getOutline(HSystemAdjustController::vision,visionName).state){
    //        return;
    //    }
    auto cameraContentData=ptrAdjustController->getContent(HSystemAdjustController::camera,ptrAdjustController->getContent(HSystemAdjustController::vision,visionName).value(1));

    auto paxisX=ptrMotion->GetAxis(ui->comboBox_cutter_axisX->currentText());
    auto paxisY=ptrMotion->GetAxis(ui->comboBox_cutter_axisY->currentText());
    auto paxisZ=ptrMotion->GetAxis(ui->comboBox_cutter_axisZ->currentText());
    auto deviceName=cameraContentData.value(4);
    auto camera=ptrCameraDevice->GetCamera(deviceName);

    if(!paxisX||!paxisY||!paxisZ||!camera){
        showMsg(Qt::red,tr("device no exist!!"));
        return;
    }

    QPainterPath path;
    QVector<QPointF> polygonPts;
    polygonPts<<QPointF(ui->lineEdit_cutter_tl_x->getNumberF(),ui->lineEdit_cutter_tl_y->getNumberF())<<QPointF(ui->lineEdit_cutter_bl_x->getNumberF(),ui->lineEdit_cutter_bl_y->getNumberF())
             <<QPointF(ui->lineEdit_cutter_br_x->getNumberF(),ui->lineEdit_cutter_br_y->getNumberF())<<QPointF(ui->lineEdit_cutter_tr_x->getNumberF(),ui->lineEdit_cutter_tr_y->getNumberF());
    path.moveTo(polygonPts.at(0));
    path.lineTo(polygonPts.at(1));
    path.lineTo(polygonPts.at(2));
    path.lineTo(polygonPts.at(3));
    QRectF rect=path.controlPointRect();

    rect=QRectF(rect.topRight(),rect.bottomLeft());



    QVector<QPointF> axisPts;
    QVector<QPointF> markPts;

    qint32 row=ui->lineEdit_cutter_row->getNumberI();
    qint32 column=ui->lineEdit_cutter_col->getNumberI();

    double width=qAbs(rect.width());
    double height=qAbs(rect.height());


    QVector<QPointF> rowColPtsSrc;
    QVector<QPointF> rowColPtsDst;
    QVector<QPointF> painerPtsSrc;
    rowColPtsSrc<<QPointF(0,row)<<QPointF(0,0)<<QPointF(column,0)<<QPointF(column,row);
    rowColPtsDst<<rect.topLeft()<<rect.bottomLeft()<<rect.bottomRight()<<rect.topRight();
    painerPtsSrc<<QPointF(-width*0.5,height*0.5)<<QPointF(-width*0.5,-height*0.5)<<QPointF(width*0.5,-height*0.5)<<QPointF(width*0.5,height*0.5);


    QTransform rowColTrans=HMathFunc::GetPerspectiveTransform(rowColPtsSrc,rowColPtsDst);
    QTransform markTrans=HMathFunc::stringToTrans(ptrAdjustController->getParas(HSystemAdjustController::cutter,ui->lineEdit_cutter_name->text()).value(0));
    QTransform painterTrans=HMathFunc::GetPerspectiveTransform(painerPtsSrc,rowColPtsDst);

    for(qint32 r=0;(row>0)?(r<row+1):(r>row-1);(row>0)?(++r):(--r)){
        for(qint32 c=0;(column>0)?(c<column+1):(c>column-1);(column>0)?(++c):(--c)){
            axisPts<<rowColTrans.map(QPointF(c,r));
            markPts<<markTrans.map(axisPts.last());
        }
    }

    MotionCompiler::GetInstance()->createPointsAndRunWait(ui->comboBox_cutter_axisX->currentText(),ui->comboBox_cutter_axisY->currentText(),ui->comboBox_cutter_axisZ->currentText(),
                                                          markPts,ui->comboBox_cutter_mark_type->currentIndex(),
                                                          ui->lineEdit_cutter_mark_size->getNumberF(),ui->comboBox_cutter_para->currentText());



    // auto ptrImgProcess=HInterfaceController::GetInstance()->imgInterface;
    auto func=ui->comboBox_cutter_func->currentText();

    QVector<QPointF> visionPoss;
    for(const auto&k:axisPts){

        QPointF pos=k;

        if(ptrAdjustController->moveToAxisPosAndGetPosFromImageUseVision(pos,ui->comboBox_cutter_vision->currentText(),
                                                                         ui->comboBox_cutter_func->currentText(),
                                                                         false,ui->lineEdit_cutter_focus->getNumberF())){
            showMsg(Qt::red,tr("get vision position failed!!"));
            return;
        }
        visionPoss<<pos;


        //        if(ptrCameraShow->showOneImg(deviceName)){
        //            showMsg(Qt::red,tr("image capture failed"));
        //            return;
        //        }
        //        qint32 w,h;
        //        uchar *pData=ptrCameraShow->getCurrentImage(w,h);
        //        if(ptrImgProcess->showImage(pData,w,h)){
        //            showMsg(Qt::red,tr("get image failed"));
        //            return;
        //        }
        //        QPointF result;
        //        if(ptrImgProcess->processImage(func,result)){
        //            showMsg(Qt::red,tr("image process failed"));
        //            return;
        //        }
        //        ptrCameraShow->addShowCross(result);
        //        ptrCameraShow->addShowText("OK",Qt::darkGreen);
    }
    qint32 count=0;
    QMap<HAdjustTransQPolygonF, QTransform> transff;
    for(qint32 k=0;k<qAbs(row);++k){
        for(qint32 m=0;m<qAbs(column);++m){
            QVector<QPointF> src;
            QVector<QPointF> dst;

            src<<visionPoss.at(count+qAbs(column)+1)<<visionPoss.at(count)<<visionPoss.at(count+1)<<visionPoss.at(count+qAbs(column)+2)<<visionPoss.at(count+qAbs(column)+1);
            dst<<markPts.at(count+qAbs(column)+1)<<markPts.at(count)<<markPts.at(count+1)<<markPts.at(count+qAbs(column)+2);
            QTransform   mtrans=HMathFunc::GetPerspectiveTransform(src,dst);
            QPointF pt;
            QLineF(src.at(0),src.at(2)).intersect(QLineF(src.at(1),src.at(3)),&pt);
            transff.insertMulti(HAdjustTransQPolygonF(src,pt),mtrans);
            ++count;
        }
    }

    auto mk=tableViewArray.at(HSystemAdjustController::cutter)->getDataAtCurrentRow().toStringList_h();
    auto content=getCurrentContents(HSystemAdjustController::cutter);
    if(mk.first()!=content.first()&&tableViewArray.at(HSystemAdjustController::cutter)->getDataAtCol(0).toStringList_h().contains(content.first())){
        QMessageBox::warning(this,tr("parameter error"),QString("name is existed!!"));
        content.first()=mk.first();
    }
    qint32 currentRow=tableViewArray.at(HSystemAdjustController::cutter)->getCurrentRow();

    ptrAdjustController->saveAllData(HSystemAdjustController::cutter,mk.first(),HAdjustOutline(content.first(),"OK",0),content,QStringList(),transff);
    tableViewArray.at(HSystemAdjustController::cutter)->setAllDatasUseRow(ptrAdjustController->getOutline(HSystemAdjustController::cutter));
    tableViewArray.at(HSystemAdjustController::cutter)->setCurrentRow(currentRow);

    QStringList paras;
    paras<<HMathFunc::transToString(markTrans)<<HMathFunc::transToString(painterTrans);
    ptrAdjustController->saveParasData(HSystemAdjustController::cutter,ui->lineEdit_cutter_name->text(),paras);

    showMsg(Qt::darkGreen,tr("cutter adjust succeeded!"));

}












