#include "dialog_newuser.h"
#include "ui_dialog_newuser.h"

#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlRecord>

Dialog_newuser::Dialog_newuser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_newuser)
{
    ui->setupUi(this);
    flag=false;
    QStringList a;
    a<<"operater"<<"expert"<<"administrator";
    ui->comboBox_authority->addItems(a);
}

Dialog_newuser::~Dialog_newuser()
{
    delete ui;
}

void Dialog_newuser::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    if(flag)
    {
        accept();
    }
    else
    {
        reject();
    }
}

void Dialog_newuser::on_pushButton_ok_pressed()
{
    if(ui->lineEdit_name->text().isEmpty()||ui->lineEdit_password->text().isEmpty()||
            ui->lineEdit_password->text()!=ui->lineEdit_password2->text())
    {
        QMessageBox::warning(this, tr("Warning"), "infomation wrong!");
    }
    else
    {

        QSqlDatabase db = QSqlDatabase::database("connection_user");
        QSqlQuery query(db);
        QString str=QString("SELECT count(*) FROM account WHERE accountname='%1';").arg(ui->lineEdit_name->text());
        query.exec(str);
        query.next();
        if(query.value(0).toInt()>0)
        {
            QMessageBox::warning(this, tr("Warning"), "User already exist!");
        }
        else
        {
           str=QString("INSERT INTO account VALUES ('%1','%2','%3','%4')").arg(ui->lineEdit_name->text())
                                                                          .arg(ui->lineEdit_password2->text())
                                                                          .arg(QString::number(ui->comboBox_authority->currentIndex()))
                                                                          .arg(ui->comboBox_authority->currentText());
           query.exec(str);
           flag=true;
           close();
        }

    }
}

void Dialog_newuser::on_pushButton_cancle_pressed()
{
    close();
}
