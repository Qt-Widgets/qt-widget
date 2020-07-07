#include "custompositiondialog.h"
#include "ui_custompositiondialog.h"
#include "Src/window/programhandler.h"
#include "Src/Motion/h_motioncontroller.h"

CustomPositionDialog::CustomPositionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomPositionDialog)
{
    ui->setupUi(this);
    setFont(MyFont1);

    QSettings settings("QtApp", "inno");
    ui->lineEdit->setText(settings.value("CustomPos/x1").toString());
    ui->lineEdit_2->setText(settings.value("CustomPos/y1").toString());
    ui->lineEdit_3->setText(settings.value("CustomPos/x2").toString());
    ui->lineEdit_4->setText(settings.value("CustomPos/y2").toString());
    ui->lineEdit_5->setText(settings.value("CustomPos/x3").toString());
    ui->lineEdit_6->setText(settings.value("CustomPos/y3").toString());
    ui->lineEdit_7->setText(settings.value("CustomPos/xx4").toString());
    ui->lineEdit_8->setText(settings.value("CustomPos/yy4").toString());
    ui->lineEdit_9->setText(settings.value("CustomPos/x5").toString());
    ui->lineEdit_10->setText(settings.value("CustomPos/y5").toString());
}

CustomPositionDialog::~CustomPositionDialog()
{
    on_pushButton_6_clicked();
    delete ui;
}

void CustomPositionDialog::on_pushButton_clicked()
{
    auto x=H_MotionController::GetInstance()->GetAxis("X");
    auto y=H_MotionController::GetInstance()->GetAxis("Y");
    if(x&&y){
        ui->lineEdit->setText(x->GetFeedbackPosString());
        ui->lineEdit_2->setText(y->GetFeedbackPosString());
    }

}

void CustomPositionDialog::on_pushButton_2_clicked()
{
    auto x=H_MotionController::GetInstance()->GetAxis("X");
    auto y=H_MotionController::GetInstance()->GetAxis("Y");
    if(x&&y){
        ui->lineEdit_3->setText(x->GetFeedbackPosString());
        ui->lineEdit_4->setText(y->GetFeedbackPosString());
    }
}

void CustomPositionDialog::on_pushButton_3_clicked()
{
    auto x=H_MotionController::GetInstance()->GetAxis("X");
    auto y=H_MotionController::GetInstance()->GetAxis("Y");
    if(x&&y){
        ui->lineEdit_5->setText(x->GetFeedbackPosString());
        ui->lineEdit_6->setText(y->GetFeedbackPosString());
    }
}

void CustomPositionDialog::on_pushButton_4_clicked()
{
    auto x=H_MotionController::GetInstance()->GetAxis("X");
    auto y=H_MotionController::GetInstance()->GetAxis("Y");
    if(x&&y){
        ui->lineEdit_7->setText(x->GetFeedbackPosString());
        ui->lineEdit_8->setText(y->GetFeedbackPosString());
    }
}

void CustomPositionDialog::on_pushButton_5_clicked()
{
    auto x=H_MotionController::GetInstance()->GetAxis("X");
    auto y=H_MotionController::GetInstance()->GetAxis("Y");
    if(x&&y){
        ui->lineEdit_9->setText(x->GetFeedbackPosString());
        ui->lineEdit_10->setText(y->GetFeedbackPosString());
    }
}

void CustomPositionDialog::on_pushButton_6_clicked()
{
    QSettings settings("QtApp","inno");
    settings.setValue("CustomPos/x1", ui->lineEdit->text());
    settings.setValue("CustomPos/y1", ui->lineEdit_2->text());
    settings.setValue("CustomPos/x2", ui->lineEdit_3->text());
    settings.setValue("CustomPos/y2", ui->lineEdit_4->text());
    settings.setValue("CustomPos/x3", ui->lineEdit_5->text());
    settings.setValue("CustomPos/y3", ui->lineEdit_6->text());
    settings.setValue("CustomPos/xx4", ui->lineEdit_7->text());
    settings.setValue("CustomPos/yy4", ui->lineEdit_8->text());
    settings.setValue("CustomPos/x5", ui->lineEdit_9->text());
    settings.setValue("CustomPos/y5", ui->lineEdit_10->text());
}
