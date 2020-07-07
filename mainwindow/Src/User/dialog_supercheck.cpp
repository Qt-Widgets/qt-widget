#include "dialog_supercheck.h"
#include "ui_dialog_supercheck.h"


#include <QMessageBox>
#include <QCloseEvent>

Dialog_SuperCheck::Dialog_SuperCheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_SuperCheck)
{
    ui->setupUi(this);
    flag=false;
    //this->setWindowFlag(Qt::WindowSystemMenuHint);
}

Dialog_SuperCheck::~Dialog_SuperCheck()
{
    delete ui;
}



void Dialog_SuperCheck::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    if(flag){
        accept();
    }
    else{
        reject();
    }

}

void Dialog_SuperCheck::on_pushButton_pressed()
{
    if(ui->lineEdit->text()=="123456789"){
        flag=true;
        close();
    }
    else{
        QMessageBox::warning(this, tr("Error"), "The password is wrong!");
    }
}

void Dialog_SuperCheck::on_pushButton_2_pressed()
{
    close();
}
