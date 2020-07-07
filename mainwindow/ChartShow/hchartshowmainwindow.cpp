#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

#include "hchartshowmainwindow.h"
#include "ui_mainwindow.h"

#include "Gui/tablewidget.h"
#include "SerialPort/hserialdialog.h"
#include "Gui/hsamplewidget.h"
#include "Src/hdatacollect.h"
#include "Gui/hdatacollectsetting.h"



HChartShowMainWindow::HChartShowMainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::HChartShowMainWindow)
{
    ui->setupUi(this);

    setFont(QFont("Microsoft YaHei UI",9));
    ptrSerialDialog= HSerialDialog::GetInstance(this);

    HSampleWidget *ptrSampleDialog=HSampleWidget::GetInstance(this);
    HDataCollectSetting *ptrCollectionDialog=HDataCollectSetting::GetInstance(this);
    ptrTableWidget =new TableWidget(this);
    ptrDataCollector= HDataCollect::GetInstance();
    ptrTimer=new QTimer(this);

    connect(ptrTimer,&QTimer::timeout,this,&HChartShowMainWindow::collectData);




    ptrTimer->setInterval(1000);
    connect(ptrSampleDialog,&HSampleWidget::ParaSaved,this,[=](){
        ptrTimer->setInterval(qint32(ptrSampleDialog->GetPara(SampleParaType::SAMPLE_INTERVAL)));
    });


    setCentralWidget(ptrTableWidget);

    connect(ui->actionExit,&QAction::triggered,this,[=](){
        this->close();
    });

    connect(ui->actionSample_Set,&QAction::triggered,this,[=](){
        ptrSampleDialog->show();
    });

    connect(ui->actionCommunication,&QAction::triggered,this,[=](){
        ptrSerialDialog->show();
    });

    connect(ui->actionCollection_Settings,&QAction::triggered,this,[=](){
        ptrCollectionDialog->show();
    });

    connect(ui->actionExport,&QAction::triggered,this,[=](){

        QString filePath= QFileDialog::getSaveFileName(this, tr("Save Data"),"ResultData",tr("TXT (*.txt)"));
        if (filePath.isEmpty())
            return;
        //QString fileName=QCoreApplication::applicationDirPath()+"user";
        if(filePath.isEmpty())
            return;
        QFile file(filePath);
        if(file.open(QFile::WriteOnly)){
            file.write(ptrTableWidget->GetDataString().toLocal8Bit());
            file.close();

            QMessageBox::information(this, tr("Note"),tr("Write data to file succeeded"));
        }
        else{
            QMessageBox::warning(this, tr("Error"),tr("Failed to open %1\n%2").arg(QDir::toNativeSeparators(filePath), file.errorString()));
        }
    });



    ui->mainToolBar->setIconSize(QSize(30,30));
    listAction.append(ui->mainToolBar->addAction(QIcon(":/resource/icons/start.svg"),"start collect",this,&HChartShowMainWindow::startCollect));
    listAction.append(ui->mainToolBar->addAction(QIcon(":/resource/icons/stop.svg"),"stop collect",this,&HChartShowMainWindow::stopCollect));
    listAction.append(ui->mainToolBar->addAction(QIcon(":/resource/icons/delete.svg"),"delete all data",this,&HChartShowMainWindow::ClearData));


    listAction[0]->setEnabled(true);
    listAction[1]->setEnabled(false);


    connect(ptrCollectionDialog,&HDataCollectSetting::SettingsChanged,this,&HChartShowMainWindow::ClearData);
}

HChartShowMainWindow::~HChartShowMainWindow()
{
    delete ui;
}

void HChartShowMainWindow::ClearData()
{
    timeCount=0;
    ptrTableWidget->clearAllData();
}

void HChartShowMainWindow::collectData()
{

    if(ptrDataCollector->CollectDataOnce(timeCount*double(ptrTimer->interval())/1000.0)){
        ptrTableWidget->appendData();
          ++timeCount;
    }else{

//        ptrTimer->stop();


//        listAction[0]->setEnabled(true);
//        listAction[1]->setEnabled(false);

        ui->statusBar->setStyleSheet("color:red");
        ui->statusBar->showMessage(tr("an error has occured, please check!"),-1);

       // timeCount=0;
    }

}

void HChartShowMainWindow::startCollect()
{
    ptrTimer->start();

    listAction[0]->setEnabled(false);
    listAction[1]->setEnabled(true);

    ui->statusBar->setStyleSheet("color:blue");

    ui->statusBar->showMessage(tr("collecting data..."),-1);
}

void HChartShowMainWindow::stopCollect()
{
    ptrTimer->stop();

    //timeCount=0;

    listAction[0]->setEnabled(true);
    listAction[1]->setEnabled(false);

    ui->statusBar->setStyleSheet("color:black");
    ui->statusBar->showMessage(tr("collection is stopped"),-1);
}


