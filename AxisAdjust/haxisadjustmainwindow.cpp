#include "haxisadjustmainwindow.h"
#include "ui_haxisadjustmainwindow.h"
#include "haxisadjustplugin.h"
#include "dialogexport.h"
#include "hmathfunctions.h"

#include <QMessageBox>

HAxisAdjustMainWindow::HAxisAdjustMainWindow(HAxisAdjustPlugin *ptrPlugin, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HAxisAdjustMainWindow)
  ,ptrPluginInterface(ptrPlugin)
{
    setFont(MyFont1);
    ui->setupUi(this);
    QSettings settings("QtApp", "inno");
    restoreGeometry(settings.value("Window/axisAdjust/geometry").toByteArray());
    restoreState(settings.value("Window/axisAdjust/windowState").toByteArray());


    dataHeader<<tr("position")<<tr("posCounts")<<tr("pixelPos1")<<tr("pixelPos2")<<tr("offset1")<<tr("offset1Counts")<<tr("offset2")<<tr("offset2Counts");
    ui->tableView->setHeaderNames(dataHeader);
}

HAxisAdjustMainWindow::~HAxisAdjustMainWindow()
{
    QSettings settings("QtApp","inno");
    settings.setValue("Window/axisAdjust/geometry", saveGeometry());
    settings.setValue("Window/axisAdjust/windowState", saveState());

    writeToFile(QCoreApplication::applicationDirPath()+"/plugins/settings/settings_Adjust.bin");

    delete ui;
}

void HAxisAdjustMainWindow::initWindow()
{
    openFromFile(QCoreApplication::applicationDirPath()+"/plugins/settings/settings_Adjust.bin");
}

void HAxisAdjustMainWindow::refreshWindow(const QStringList &axisNames, const QStringList &funcNames)
{
    QString currentText1=ui->comboBox_1->currentText();
    QString currentText2=ui->comboBox_2->currentText();
    QString currentText3=ui->comboBox_3->currentText();
    QString currentText4=ui->comboBox_4->currentText();
    ui->comboBox_1->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();

    ui->comboBox_1->addItems(axisNames);
    ui->comboBox_1->setCurrentText(currentText1);
    ui->comboBox_2->addItems(axisNames);
    ui->comboBox_2->setCurrentText(currentText2);
    ui->comboBox_3->addItems(funcNames);
    ui->comboBox_3->setCurrentText(currentText3);
    ui->comboBox_4->addItems(funcNames);
    ui->comboBox_4->setCurrentText(currentText4);
}

QStringList HAxisAdjustMainWindow::getControlData() const
{
    QStringList list;
    list<<QString::number(ui->comboBox_1->currentIndex())<<QString::number(ui->comboBox_2->currentIndex())
       <<ui->lineEdit_1->text()<<ui->lineEdit_2->text()<<ui->lineEdit_3->text()<<ui->lineEdit_4->text()
      <<ui->lineEdit_5->text()<<ui->lineEdit_6->text()<<ui->lineEdit_7->text()<<ui->lineEdit_8->text()<<ui->lineEdit_9->text()
     <<ui->lineEdit_10->text()<<ui->lineEdit_11->text()<<ui->lineEdit_12->text()<<ui->lineEdit_13->text()<<ui->lineEdit_14->text()
    <<QString::number(ui->comboBox_3->currentIndex())<<QString::number(ui->comboBox_4->currentIndex());
    return list;
}

void HAxisAdjustMainWindow::setControlData(const QStringList &data)
{
    qint32 i=-1;
    ui->comboBox_1->setCurrentIndex(data.value(++i).toInt());
    ui->comboBox_2->setCurrentIndex(data.value(++i).toInt());
    ui->lineEdit_1->setText(data.value(++i));
    ui->lineEdit_2->setText(data.value(++i));
    ui->lineEdit_3->setText(data.value(++i));
    ui->lineEdit_4->setText(data.value(++i));
    ui->lineEdit_5->setText(data.value(++i));
    ui->lineEdit_6->setText(data.value(++i));
    ui->lineEdit_7->setText(data.value(++i));
    ui->lineEdit_8->setText(data.value(++i));
    ui->lineEdit_9->setText(data.value(++i));
    ui->lineEdit_10->setText(data.value(++i));
    ui->lineEdit_11->setText(data.value(++i));
    ui->lineEdit_12->setText(data.value(++i));
    ui->lineEdit_13->setText(data.value(++i));
    ui->lineEdit_14->setText(data.value(++i));
    ui->comboBox_3->setCurrentIndex(data.value(++i).toInt());
    ui->comboBox_4->setCurrentIndex(data.value(++i).toInt());
}


bool HAxisAdjustMainWindow::writeToFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)){
        return false;
    }
    QDataStream ds(&file);
    ds << *this ;
    file.close();
    return true;
}

bool HAxisAdjustMainWindow::openFromFile(const QString &filePath)
{
    QFile f(filePath);
    if(!f.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QDataStream ds(&f);
    ds >> *this;
    f.close();
    return true;
}

QDataStream &operator>>(QDataStream &input, HAxisAdjustMainWindow &obj){
    QStringList list;
    input>>list;
    obj.setControlData(list);
    return input;
}

QDataStream &operator<<(QDataStream &output, const HAxisAdjustMainWindow &obj){
    output<<obj.getControlData();
    return output;
}


void HAxisAdjustMainWindow::on_pushButton_clicked()
{
    double pos=0;
    ptrPluginInterface->getAxisPos(ui->comboBox_1->currentText(),pos);
    ui->lineEdit_2->setText(QString::number(pos,'f',5));
    ptrPluginInterface->getAxisPos(ui->comboBox_2->currentText(),pos);
    ui->lineEdit_3->setText(QString::number(pos,'f',5));
}

void HAxisAdjustMainWindow::on_pushButton_2_clicked()
{
    double pos=0;
    ptrPluginInterface->getAxisPos(ui->comboBox_1->currentText(),pos);
    ui->lineEdit_4->setText(QString::number(pos,'f',5));
    ptrPluginInterface->getAxisPos(ui->comboBox_2->currentText(),pos);
    ui->lineEdit_5->setText(QString::number(pos,'f',5));
}

void HAxisAdjustMainWindow::on_pushButton_3_clicked()
{
    double pos=0;
    ptrPluginInterface->getAxisPos(ui->comboBox_1->currentText(),pos);
    ui->lineEdit_6->setText(QString::number(pos,'f',5));
    ptrPluginInterface->getAxisPos(ui->comboBox_2->currentText(),pos);
    ui->lineEdit_7->setText(QString::number(pos,'f',5));
}

void HAxisAdjustMainWindow::on_pushButton_4_clicked()
{
    double pos=0;
    ptrPluginInterface->getAxisPos(ui->comboBox_1->currentText(),pos);
    ui->lineEdit_8->setText(QString::number(pos,'f',5));
    ptrPluginInterface->getAxisPos(ui->comboBox_2->currentText(),pos);
    ui->lineEdit_9->setText(QString::number(pos,'f',5));
}

void HAxisAdjustMainWindow::on_toolButton_1_clicked()
{
    isStop=false;

    ui->tableView->clearAllRows();

    auto perPixel=ui->lineEdit_1->getNumberF();
    auto perUnits=ui->lineEdit_13->getNumberF();
    auto panelInterval=ui->lineEdit_14->getNumberF();

    auto axis1=ui->comboBox_1->currentText();
    auto axis2=ui->comboBox_2->currentText();
    auto step1=ui->lineEdit_10->getNumberF();
    auto step2=ui->lineEdit_11->getNumberF();

    auto crossFunc=ui->comboBox_3->currentText();
    auto markFunc=ui->comboBox_4->currentText();

    double panelAngle=0;

    if(ptrPluginInterface->axisMoveToWait(axis1,ui->lineEdit_4->getNumberF())){
        QMessageBox::critical(this,tr("error"),tr("axis move failed!"));
        return;
    }
    if(ptrPluginInterface->axisMoveToWait(axis2,ui->lineEdit_5->getNumberF())){
        QMessageBox::critical(this,tr("error"),tr("axis move failed!"));
        return;
    }

            QThread::msleep(1800);

    QLineF llllllllll;
    if(ptrPluginInterface->processImgLine(crossFunc,llllllllll)){
        QMessageBox::critical(this,tr("error"),tr("image process failed!"));
        return;
    }
    panelAngle=llllllllll.angle();
    //panelAngle+=panelAngle>M_PI_2?-M_PI_2:M_PI_2;



    double oxxxx=ui->lineEdit_8->getNumberF()-ui->lineEdit_6->getNumberF();
    double oyyyy=ui->lineEdit_9->getNumberF()-ui->lineEdit_7->getNumberF();


    QPointF stratPt,endPt;
    double angleMotion=0.0;

    if(ptrPluginInterface->axisMoveToWait(axis1,ui->lineEdit_6->getNumberF())){
        QMessageBox::critical(this,tr("error"),tr("axis move failed!"));
        return;
    }
    if(ptrPluginInterface->axisMoveToWait(axis2,ui->lineEdit_7->getNumberF())){
        QMessageBox::critical(this,tr("error"),tr("axis move failed!"));
        return;
    }
            QThread::msleep(1800);

    if(ptrPluginInterface->processImg(markFunc,stratPt)){
        QMessageBox::critical(this,tr("error"),tr("image process failed!"));
        return;
    }

    if(ptrPluginInterface->axisMoveToWait(axis1,ui->lineEdit_8->getNumberF())){
        QMessageBox::critical(this,tr("error"),tr("axis move failed!"));
        return;
    }
    if(ptrPluginInterface->axisMoveToWait(axis2,ui->lineEdit_9->getNumberF())){
        QMessageBox::critical(this,tr("error"),tr("axis move failed!"));
        return;
    }
            QThread::msleep(1800);

    if(ptrPluginInterface->processImg(markFunc,endPt)){
        QMessageBox::critical(this,tr("error"),tr("image process failed!"));
        return;
    }
    //auto yyyyyy=(stratPt-endPt).y();
    //angleMotion=atan2(qAbs(ui->lineEdit_6->getNumberF()-ui->lineEdit_8->getNumberF())/perPixel,yyyyyy)*180/M_PI;
    //angleMotion=QLineF(0,0,qAbs(ui->lineEdit_6->getNumberF()-ui->lineEdit_8->getNumberF())/perPixel,yyyyyy).angle();
    angleMotion=QLineF(endPt,stratPt).angle();

    //auto mmmmmLine=QLineF(stratPt,endPt);
    //angleMotion+=angleMotion>M_PI_2?-M_PI_2:M_PI_2;


    QVector<double> posArray;
    QVector<double> posCountsArray;
    QVector<double> pxielArray1;
    QVector<double> pxielArray2;

    QVector<double> offsetArray1;
    QVector<double> offsetCountArray1;
    QVector<double> offsetArray2;
    QVector<double> offsetCountArray2;


    QPointF ccposs=QPointF(ui->lineEdit_2->getNumberF(),ui->lineEdit_3->getNumberF());


    if(ptrPluginInterface->axisMoveToWait(axis1,ccposs.x())){
        QMessageBox::critical(this,tr("error"),tr("axis move failed!"));
        return;
    }
    if(ptrPluginInterface->axisMoveToWait(axis2,ccposs.y())){
        QMessageBox::critical(this,tr("error"),tr("axis move failed!"));
        return;
    }
            QThread::msleep(1800);




    for(qint32 k=0;k<ui->lineEdit_12->getNumberI();++k){

        if(isStop){
            QMessageBox::information(this,tr("error"),tr("please restart!"));
            return;
        }

        if(ptrPluginInterface->axisMoveToWait(axis1,ccposs.x()+k*step1)){
            QMessageBox::critical(this,tr("error"),tr("axis move failed!"));
            return;
        }
        if(ptrPluginInterface->axisMoveToWait(axis2,ccposs.y()+k*step2)){
            QMessageBox::critical(this,tr("error"),tr("axis move failed!"));
            return;
        }
        double poss=0;
                QThread::msleep(1800);

        if(ptrPluginInterface->getAxisPos(axis1,poss)){
            QMessageBox::critical(this,tr("error"),tr("get axis position failed!"));
            return;
        }
        posArray<<poss;

        qint32 posc=0;
        if(ptrPluginInterface->getAxisPosCount(axis1,posc)){
            QMessageBox::critical(this,tr("error"),tr("get axis position failed!"));
            return;
        }
        posCountsArray<<posc;

        QPointF result1,result2;

        if(ptrPluginInterface->processImg(markFunc,result1)){
            QMessageBox::critical(this,tr("error"),tr("image process failed!"));
            return;
        }


        pxielArray1<<result1.x();
        pxielArray2<<result1.y();


//        if(ptrPluginInterface->axisMoveToWait(axis1,ccposs.x()+k*step1+oxxxx)){
//            QMessageBox::critical(this,tr("error"),tr("axis move failed!"));
//            return;
//        }
//        if(ptrPluginInterface->axisMoveToWait(axis2,ccposs.y()+k*step2+oyyyy)){
//            QMessageBox::critical(this,tr("error"),tr("axis move failed!"));
//            return;
//        }

//        if(ptrPluginInterface->processImg(markFunc,result2)){
//            QMessageBox::critical(this,tr("error"),tr("image process failed!"));
//            return;
//        }

//        auto mgggggg=mmmmmLine.p2()-(result2+(mmmmmLine.p1()-result1));

//        offsetArray1<<mgggggg.x()*perPixel;
//        offsetCountArray1<<mgggggg.x()*perPixel*perUnits;
//        offsetArray2<<mgggggg.y()*perPixel;
//        offsetCountArray2<<mgggggg.y()*perPixel*perUnits;

    }

    if(pxielArray1.size()<2){
        QMessageBox::critical(this,tr("error"),tr("parameter error!"));
        return;
    }

        auto fpt=QPointF(pxielArray1.first(),pxielArray2.first());
        offsetArray1<<0;
        offsetCountArray1<<0;
        offsetArray2<<0;
        offsetCountArray2<<0;

        qDebug()<<fpt;


        auto mmmmmLine=QLineF(fpt,QPointF(pxielArray1.last(),pxielArray2.last()));
        auto mlllllll=mmmmmLine.length()/(pxielArray1.size()-1);


        for(qint32 k=1;k<pxielArray1.size();++k){

//            double gfgfgfg=-(angleMotion-panelAngle)/180*M_PI;

//            QLineF line1,line2;

//            line1.setP1(QPointF(0,0));
//            line1.setP2(QPointF(1,1));
//            line1.setAngle(angleMotion);
//            line1.setLength((step1*k)/perPixel);

//            line2.setP1(QPointF(0,0));
//            line2.setP2(QPointF(1,1));
//            line2.setAngle(panelAngle);
//            line2.setLength(panelInterval*k/perPixel);

            mmmmmLine.setLength(mlllllll*k);


//            qDebug()<<QPointF(cos(gfgfgfg)*step1*k/perPixel+panelInterval*k/perPixel,sin(gfgfgfg)*step1*k/perPixel);
//            qDebug()<<QPointF(pxielArray1.value(k),pxielArray2.value(k));


//            auto offsetttt=fpt+QPointF(cos(gfgfgfg)*step1*k/perPixel+panelInterval*k/perPixel,sin(gfgfgfg)*step1*k/perPixel)+-QPointF(pxielArray1.value(k),pxielArray2.value(k));
//            QTransform trans=QTransform::fromTranslate(0,0).rotate(angleMotion);
//            offsetttt=trans.map(offsetttt);

            auto offset=mmmmmLine.p2()-QPointF(pxielArray1.value(k),pxielArray2.value(k));

            offsetArray1<<offset.x()*perPixel;
            offsetCountArray1<<offset.x()*perPixel*perUnits;
            offsetArray2<<offset.y()*perPixel;
            offsetCountArray2<<offset.y()*perPixel*perUnits;
        }

    for(qint32 k=0;k<offsetArray1.size();++k){

        HVariantList list;
        list<<posArray.value(k)<<qint32(posCountsArray.value(k))<<pxielArray1.value(k)<<pxielArray2.value(k)<<offsetArray1.value(k)<</*(qint32(offsetCountArray1.value(k))-
                                                                                                                                      qint32(offsetCountArray1.value(k-1)))*/
              qint32(offsetCountArray1.value(k))
           <<offsetArray2.value(k)<</*(qint32(offsetCountArray2.value(k))-qint32(offsetCountArray2.value(k-1)))*/qint32(offsetCountArray2.value(k));
        ui->tableView->addRowData(list);
    }

    QMessageBox::information(this,tr("message"),tr("adjust completed!"));
    return;
}

void HAxisAdjustMainWindow::on_toolButton_2_clicked()
{
    isStop=true;
}

void HAxisAdjustMainWindow::on_toolButton_3_clicked()
{
    DialogExport dlg(this);
    dlg.setData(dataHeader,ui->tableView->getAllDataUseCol());
    dlg.exec();
}
