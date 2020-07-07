#include "hfindfocusdialog.h"
#include "ui_hfindfocusdialog.h"
#include "Src/Motion/h_motioncontroller.h"
#include "Src/Gui/settings/hparalibcontroller.h"
#include "Src/data/inisettings.h"
#include "Src/Motion/motionStage/MotionCompiler.h"

HFindFocusDialog::HFindFocusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HFindFocusDialog)
{
    ui->setupUi(this);
    setFont(MyFont1);

    ptrParaLib=HParaLibController::GetInstance();
    ptrMotion=H_MotionController::GetInstance();
    data_mode=IniSettings::GetInstance()->getValue(IniSettings::Edit_mode).toInt();


    ui->comboBox_para->addItems(ptrParaLib->getNames(data_mode));
    ui->comboBox_para->setCurrentText(ptrParaLib->getDefaultName(data_mode));
    ui->comboBox_para_type->addItem(tr("focus distance"));
    ui->comboBox_mark_type->addItem(tr("line"));
    ui->comboBox_mark_type->addItem(tr("circle"));
    ui->lineEdit_para_value->setText(ptrParaLib->getGeneralDatas(data_mode).value(0));

    connect(ui->comboBox_para,&QComboBox::currentTextChanged,this,[=](const QString&name ){
        ui->lineEdit_para_value->setText(ptrParaLib->getGeneralDatas(data_mode,name).value(0));
        //        ui->comboBox_para->clear();
        //        ui->comboBox_para->addItems(ptrParaLib->getParaNames());
    });

    connect(ui->toolButton_pos_get,&QToolButton::pressed,this,[=](){
        ui->lineEdit_posX->setText(ptrMotion->GetAxis(0)->GetFeedbackPosString());
        ui->lineEdit_posY->setText(ptrMotion->GetAxis(1)->GetFeedbackPosString());
    });

    connect(ui->comboBox_result, QOverload<int>::of(&QComboBox::currentIndexChanged),this,[=](qint32 index){
        double sdsd=ptrParaLib->getGeneralDatas(data_mode,ui->comboBox_para->currentText()).value(0).toDouble()+ui->lineEdit_step->text().toDouble()*index;
        ui->lineEdit_para_value->setText(QString::number(sdsd));
    });

    openFromFile(QCoreApplication::applicationDirPath()+"/config/find_focus.bin");

    ui->lineEdit_posX->setText(QString::number(m_position.x()));
    ui->lineEdit_posY->setText(QString::number(m_position.y()));
    ui->checkBox_use_mark->setChecked(m_useMark);
    ui->lineEdit_number->setText(m_number);
    ui->lineEdit_step->setText(m_step);
    ui->comboBox_mark_type->setCurrentIndex(m_markType);
    ui->lineEdit_size->setText(m_markSize);
    ui->lineEdit_interval->setText(m_markInterval);

    connect(ui->comboBox_axisX,&QComboBox::currentTextChanged,this,[=](const QString &str){m_axisX=str;});
    connect(ui->comboBox_axisY,&QComboBox::currentTextChanged,this,[=](const QString &str){m_axisY=str;});
    connect(ui->comboBox_axisZ,&QComboBox::currentTextChanged,this,[=](const QString &str){m_axisZ=str;});
    connect(ui->lineEdit_posX,&QLineEdit::textChanged,this,[=](const QString &text){m_position.setX(text.toDouble());});
    connect(ui->lineEdit_posY,&QLineEdit::textChanged,this,[=](const QString &text){m_position.setY(text.toDouble());});
    connect(ui->checkBox_use_mark,&QCheckBox::toggled,this,[=](bool checked){m_useMark=checked;});
    connect(ui->lineEdit_number,&QLineEdit::editingFinished,this,[=](){m_number=ui->lineEdit_number->text();});
    connect(ui->lineEdit_step,&QLineEdit::editingFinished,this,[=](){m_step=ui->lineEdit_step->text();});
    connect(ui->comboBox_mark_type,QOverload<int>::of(&QComboBox::currentIndexChanged),this,[=](qint32 index){m_markType=index;});
    connect(ui->lineEdit_size,&QLineEdit::editingFinished,this,[=](){m_markSize=ui->lineEdit_size->text();});
    connect(ui->lineEdit_interval,&QLineEdit::editingFinished,this,[=](){m_markInterval=ui->lineEdit_interval->text();});
}



HFindFocusDialog::~HFindFocusDialog()
{
    writeToFile(QCoreApplication::applicationDirPath()+"/config/find_focus.bin");
    delete ui;
}



QDataStream &operator>>(QDataStream &input, HFindFocusDialog &obj)
{
    input>>obj.m_axisX>>obj.m_axisY>>obj.m_axisZ>>obj.m_position>>obj.m_useMark>>obj.m_number>>obj.m_step>>
            obj.m_markType>>obj.m_markSize>>obj.m_markInterval;
    return input;
}

QDataStream &operator<<(QDataStream &output, const HFindFocusDialog &obj)
{
    output<<obj.m_axisX<<obj.m_axisY<<obj.m_axisZ<<obj.m_position<<obj.m_useMark<<obj.m_number<<obj.m_step<<
            obj.m_markType<<obj.m_markSize<<obj.m_markInterval;
    return output;
}

void HFindFocusDialog::on_toolButton_cut_clicked()
{
    MotionCompiler::GetInstance()->createFocusGcodeAndRun(m_axisX,m_axisY,m_axisZ,
                ui->comboBox_para->currentText(),ui->comboBox_para_type->currentIndex(),ui->lineEdit_para_value->text().toDouble(),m_step.toDouble(),m_position,
                                                          m_number.toInt(),m_useMark,m_markType,m_markSize.toDouble(),m_markInterval.toDouble());
    QStringList list;
    for(qint32 k=0;k<ui->lineEdit_number->text().toInt();++k){
        list<<QString::number(k);
    }
    ui->comboBox_result->clear();
    ui->comboBox_result->addItems(list);
}

void HFindFocusDialog::on_toolButton_save_clicked()
{
    ptrParaLib->setData(data_mode,ui->comboBox_para->currentText(),HParaLibController::Cutter_General_focus,ui->lineEdit_para_value->text());
}

void HFindFocusDialog::showEvent(QShowEvent *event)
{
    auto mkgg=ui->comboBox_para->currentText();
    ui->comboBox_para->clear();
    ui->comboBox_para->addItems(ptrParaLib->getNames(data_mode));
    ui->comboBox_para->setCurrentText(mkgg);
    ui->lineEdit_para_value->setText(ptrParaLib->getGeneralDatas(data_mode,mkgg).value(0));
    auto listAxis=ptrMotion->GetAxisNames();

    QString axisX=m_axisX;
    QString axisY=m_axisY;
    QString axisZ=m_axisZ;
    ui->comboBox_axisX->clear();
    ui->comboBox_axisX->addItems(listAxis);
    ui->comboBox_axisX->setCurrentText(axisX);
    ui->comboBox_axisY->clear();
    ui->comboBox_axisY->addItems(listAxis);
    ui->comboBox_axisY->setCurrentText(axisY);
    ui->comboBox_axisZ->clear();
    ui->comboBox_axisZ->addItems(listAxis);
    ui->comboBox_axisZ->setCurrentText(axisZ);

    QDialog::showEvent(event);
}


bool HFindFocusDialog::writeToFile(const QString &filePath)
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

bool HFindFocusDialog::openFromFile(const QString &filePath)
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
