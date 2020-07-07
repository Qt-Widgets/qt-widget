#include "dialoglayerlopts.h"
#include "ui_dialoglayerlopts.h"
#include "programhandler.h"

DialogLayerLoPts::DialogLayerLoPts(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLayerLoPts)
{
    setFont(MyFont1);
    ui->setupUi(this);
    ui->comboBox->addItem("none");
    ui->comboBox->addItem("1");
    ui->comboBox->addItem("2");
    ui->comboBox->addItem("3");
    ui->comboBox->addItem("4");



}

DialogLayerLoPts::~DialogLayerLoPts()
{
    delete ui;
}

void DialogLayerLoPts::setLayerIndex(qint32 index)
{
    lIndex=index;
}

void DialogLayerLoPts::showEvent(QShowEvent *event)
{
    auto pts=ProgramHandler::GetInstance()->getActiveProgram()->getDataPtr()->value(lIndex).locationPts;

    ui->lineEdit_X1->setText(QString::number(pts.value(0).x()));
    ui->lineEditY1->setText(QString::number(pts.value(0).y()));
    ui->lineEditX2->setText(QString::number(pts.value(1).x()));
    ui->lineEditY2->setText(QString::number(pts.value(1).y()));
    ui->lineEditX3->setText(QString::number(pts.value(1).x()));
    ui->lineEditY3->setText(QString::number(pts.value(1).y()));
    ui->lineEditX4->setText(QString::number(pts.value(1).x()));
    ui->lineEditY4->setText(QString::number(pts.value(1).y()));

    ui->comboBox->setCurrentIndex(ProgramHandler::GetInstance()->getActiveProgram()->getData()[lIndex].locationPts.size());
}

void DialogLayerLoPts::on_pushButton_clicked()
{
    auto pt=ProgramHandler::GetInstance()->getActiveProgram()->getViewPtr()->getCameraPos();
    ui->lineEdit_X1->setText(QString::number(pt.x()));
    ui->lineEditY1->setText(QString::number(pt.y()));

}

void DialogLayerLoPts::on_pushButton_2_clicked()
{
    auto pt=ProgramHandler::GetInstance()->getActiveProgram()->getViewPtr()->getCameraPos();
    ui->lineEditX2->setText(QString::number(pt.x()));
    ui->lineEditY2->setText(QString::number(pt.y()));


}

void DialogLayerLoPts::on_pushButton_3_clicked()
{
    auto pt=ProgramHandler::GetInstance()->getActiveProgram()->getViewPtr()->getCameraPos();
    ui->lineEditX3->setText(QString::number(pt.x()));
    ui->lineEditY3->setText(QString::number(pt.y()));
}


void DialogLayerLoPts::on_pushButton_4_clicked()
{
    auto pt=ProgramHandler::GetInstance()->getActiveProgram()->getViewPtr()->getCameraPos();
    ui->lineEditX4->setText(QString::number(pt.x()));
    ui->lineEditY4->setText(QString::number(pt.y()));
}

void DialogLayerLoPts::on_pushButton_save_clicked()
{
    QVector<QPointF> pts;
    switch (ui->comboBox->currentText().toInt()) {
    case 1:{
        pts<< QPointF(ui->lineEdit_X1->text().toDouble(),
                      ui->lineEditY1->text().toDouble());
    }
        break;
    case 2:{
        pts<<QPointF(ui->lineEdit_X1->text().toDouble(),
                     ui->lineEditY1->text().toDouble())
          <<QPointF(ui->lineEditX2->text().toDouble(),
                    ui->lineEditY2->text().toDouble());
    }
        break;
    case 3:{
        pts<< QPointF(ui->lineEdit_X1->text().toDouble(),
                      ui->lineEditY1->text().toDouble())
           <<QPointF(ui->lineEditX2->text().toDouble(),
                     ui->lineEditY2->text().toDouble())
          <<QPointF(ui->lineEditX3->text().toDouble(),
                    ui->lineEditY3->text().toDouble())
            ;
    }
        break;
    case 4:{
        pts<< QPointF(ui->lineEdit_X1->text().toDouble(),
                      ui->lineEditY1->text().toDouble())
           <<QPointF(ui->lineEditX2->text().toDouble(),
                     ui->lineEditY2->text().toDouble())
          <<QPointF(ui->lineEditX3->text().toDouble(),
                    ui->lineEditY3->text().toDouble())
         <<QPointF(ui->lineEditX4->text().toDouble(),
                   ui->lineEditY4->text().toDouble())
           ;
    }
        break;
    default:{
        ProgramHandler::GetInstance()->getActiveProgram()->getData()[lIndex].locationPts.clear();
    }
        break;
    }
    ProgramHandler::GetInstance()->getActiveProgram()->getData()[lIndex].locationPts.swap(pts);
}
