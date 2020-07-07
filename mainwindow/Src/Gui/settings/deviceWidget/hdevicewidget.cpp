#include "hdevicewidget.h"
#include "hdelegatecombbox.h"
#include "hcommon.h"
#include "tablemodel.h"

#include "Src/data/hsqlcommon.h"
#include "hwsidgetdelegate.h"

#include "Src/Motion/hiodevicecontroller.h"



#include <QTableView>
#include <QHeaderView>
#include <QStackedLayout>

#include <QMessageBox>


#include <QRadioButton>
#include <QTimer>
#include <QSettings>
#include <QVBoxLayout>



HDeviceWidget::HDeviceWidget(QWidget *parent, Qt::WindowFlags f) : QDialog(parent,f)
{
    resize(700,500);
    QVBoxLayout *layout=new QVBoxLayout(this);
    setVisible(false);
    ptrTableView =new QTableView(this);

    layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);


    ptrTableView->setItemDelegate(new HDelegateCombbox(this));

    // createConnection();

    TableModel *model=new TableModel(this);


    ptrTableView->setModel(model);
    ptrTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ptrTableView->setAlternatingRowColors(true);
    ptrTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //ptrTableView->setFocusPolicy(Qt::NoFocus);

    ptrTableView->setStyleSheet(HSheetStyle::H_TABLEVIEW1);


    ptrTableView->horizontalHeader()->setStretchLastSection(true);
//    QSettings settings("AOC", "inno");
//    ptrTableView->horizontalHeader()->restoreState(settings.value("inory/DeviceTableViewHeaderState").toByteArray());

    ptrTableView->verticalHeader()->hide();
    ptrTableView->setShowGrid(false);

    //QTimer* timer = new QTimer(this);
    //timer->setInterval(30);

    //timer->connect(timer,&QTimer::timeout,this,[=](){
    //    ptrTableView->update();
    //});
    //timer->start();

    qint32 count=HIODeviceController::GetInstance()->GetOutputNum();
    for(int k=0;k<count;++k){

        HWsidgetDelegate *wid=new HWsidgetDelegate(this);

        //wid->setStyleSheet("background-color:lightgreen;");

        ptrTableView->setIndexWidget(model->index(k, ColumnType::output), wid);

        ptrTableView->setRowHeight(k,30);

        //            HIODeviceController::GetInstance()->GetOutputPort(0)->SetOutputBit(1);
        //            HIODeviceController::GetInstance()->GetOutputPort(1)->SetOutputBit(1);

        ulong dssdsd;
        auto mkkk=HIODeviceController::GetInstance()->GetOutputPort(k);
        if(mkkk){
          mkkk->GetOutputBit(dssdsd);
        }

        wid->radioBtn->setValue(dssdsd);

        connect(wid->radioBtn,&QSlider::valueChanged,this,[=](bool value){
            //GtMotion::GetInstance()->SetOutput(12,k,value);
            HIODeviceController::GetInstance()->GetOutputPort(k)->SetOutputBit(value);
            //ulong gf=0;

//            if(k<8)
//                A3200Api::GetInstance()->SetOutput(0,k,value);
//            else{
                
//                A3200Compiler podf;
//                podf.RunTask2(QString("PORT X DIRECTION 1 OUTPUT\n"
//                                      "$DO[%1].X=%2").arg(k+16).arg(qint32(value)));
//            }
        });

    }

    layout->addWidget(ptrTableView);

    QTimer *timer=new QTimer(this);

    connect(timer,&QTimer::timeout,this,[&](){
        ptrTableView->update();
    });
    timer->start(200);
}

HDeviceWidget::~HDeviceWidget()
{
//    QSettings settings("AOC", "inno");
//    settings.setValue("inory/DeviceTableViewHeaderState", /*ui->treeView->saveGeometry()*/ptrTableView->horizontalHeader()->saveState());
}

//void HDeviceWidget::resizeEvent(QResizeEvent *event)
//{
//    Q_UNUSED(event);
//    ptrTableView->setGeometry(rect());
//}
