#include <QtSql>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QFont>
#include <QSqlTableModel>
#include <QHeaderView>


#include "dialogaccountui.h"
#include "ui_dialogaccountui.h"
#include "dialog_supercheck.h"
#include "dialog_newuser.h"
#include "h_authority.h"




DialogAccountUI::DialogAccountUI(bool isFirst, int deauthority, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAccountUI)
{
    ui->setupUi(this);
    setFixedSize(this->width(), this->height());
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->button_cancle,SIGNAL(pressed()),this,SLOT(close()));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(checkSupperAccount(int)));
    connectDatabase();

    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    ui->button_login->setStyleSheet("QPushButton{font-family:Calibri;font-size:25px;border-radius:5px; background:rgb(110, 190, 10); color:black;}"
                                    "QPushButton:hover{font-family:Calibri;font-size:25px;background:rgb(106, 181, 255, 100);border-radius:5px; border:1px solid rgb(210, 225, 230);}");
    ui->button_cancle->setStyleSheet("QPushButton{font-family:Calibri;font-size:25px;border-radius:5px; background:rgb(110, 190, 10); color:red;}"
                                     "QPushButton:hover{font-family:Calibri;font-size:25px;background:rgb(106, 181, 255, 100);border-radius:5px; border:1px solid rgb(210, 225, 230);}");
    ui->button_cancle->setVisible(false);

    ui->tabWidget->setStyleSheet("QTabWidget::pane{border: 1px;background:white;}"
                                 "QTabWidget::tab-bar{alignment:left;background:red;}"
                                 "QTabBar::tab{background:transparent;color:black; min-width:30ex; min-height:5ex; }"
                                 "QTabBar::tab:hover{background:rgb(106, 181, 255, 100);}"
                                 "QTabBar::tab:selected{border-color: white;background:white;color:green;}");

    //    ui->comboBox_account->setStyleSheet("QComboBox{border-width: 1px; border-radius: 3px;font-family:Calibri;font-size:18px; color: black; border:1px solid gray;}"
    //                                    "QComboBox:hover{border-width: 1px; border-radius: 3px; font-size:18px; color: black; border:1px solid rgb(70, 200, 50);}");

    ui->lineEdit_password->setStyleSheet("QLineEdit{border-width: 1px; border-radius: 0px; font-size:20px; color: black; border:1px solid gray;}"
                                         "QLineEdit:hover{border-width: 1px; border-radius: 0px; font-size:20px; color: black; border:1px solid rgb(70, 200, 50);}");

    showDatabase();
    authority=deauthority;
    isFirstLogin=isFirst;

}

DialogAccountUI::~DialogAccountUI()
{
    delete ui;
}

void DialogAccountUI::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    ui->tabWidget->setGeometry(this->rect());
    //ui->tableView->setGeometry(this->rect());
}

void DialogAccountUI::closeEvent(QCloseEvent *event)
{

    Q_UNUSED(event);
    if(authority==-1&&isFirstLogin)
        exit(0);
    db.close();

}

void DialogAccountUI::checkSupperAccount(int index)
{
    if(index==1){
        Dialog_SuperCheck* dlg=new Dialog_SuperCheck(this);


        if(dlg->exec()!=QDialog::Accepted){
            ui->tabWidget->setCurrentIndex(0);
        }

    }

}

void DialogAccountUI::connectDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE","connection_user");
    db.setDatabaseName(QCoreApplication::applicationDirPath()+"/account.db");
    if (!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open userfile"),
                              QObject::tr("Unable to open the user file.\n"
                                          "please connect the INNO LASER to get help.\n\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);

        exit(0);
    }


    QSqlQuery query(db);
    if(!db.tables().contains("account")){
        QMessageBox::warning(this,"","account information missed!!");
        QString str="CREATE TABLE account( "
                    "accountname VARCHAR (255) NOT NULL UNIQUE,"
                    "password  VARCHAR (255) NOT NULL,"
                    "type INT NOT NULL,"
                    "authority VARCHAR (255) NOT NULL)";
        query.exec(str);
    }


    query.exec("select accountname from account");
    while (query.next())
    {
        ui->comboBox_account->addItem(query.value(0).toString());
    }


    //    model = new QSqlRelationalTableModel(this);
    //    model->setTable("person");
    //    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    //    typeIndex = model->fieldIndex("typeid");

    //    model->setRelation(typeIndex,
    //           QSqlRelation("addresstype", "id", "description"));
    //    model->select();

}

void DialogAccountUI::showDatabase()
{
    // Create the data model

    model = new QSqlTableModel(this,db);
    model->setTable("account");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();


    model->setHeaderData(0, Qt::Horizontal, tr("User"));
    model->setHeaderData(2, Qt::Horizontal, tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, tr("Description"));


    ui->tableView->setModel(model);
    ui->tableView->hideColumn(1);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setFixedHeight(25);
    //ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;font: 75 12pt 'Microsoft YaHei UI';}");
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    QFont font("Microsoft YaHei UI",12);
    ui->tableView->horizontalHeader()->setFont(font);
    //ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->horizontalHeader()->setHighlightSections(false);
    //ui->tableView->resize(this->width(),this->height());

}



void DialogAccountUI::on_button_login_clicked()
{
    QString str=QString("SELECT count(*) FROM account WHERE accountname='%1';").arg(ui->comboBox_account->currentText());
    QSqlQuery query(db);
    query.exec(str);
    query.next();
    if(query.value(0).toInt()==1){
        str=QString("SELECT password,type FROM account WHERE accountname='%1'").arg(ui->comboBox_account->currentText());
        query.exec(str);
        query.next();
        if(query.value(0).toString()==ui->lineEdit_password->text()){
            authority=query.value(1).toInt();
            H_Authority::GetInstance()->setAuthority(authority);
            close();
        }
        else{
            QMessageBox::warning(this, tr("Warning"), "password wrong !");
        }
    }

    else{
        QMessageBox::warning(this, tr("Warning"), "the user is nonexistent!");
    }

}

void DialogAccountUI::on_pushButton_new_pressed()
{
    Dialog_newuser dlg(this);
    if(dlg.exec()==QDialog::Accepted){
        model->select();
    }

}

void DialogAccountUI::on_pushButton_delete_pressed()
{
    model->removeRow(ui->tableView->currentIndex().row());
    model->select();

}
