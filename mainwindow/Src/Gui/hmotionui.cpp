#include "hmotionui.h"
#include "hcommon.h"
#include <QRadioButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QSlider>

#include "Src/Motion/h_motioncontroller.h"


#include "Src/data/hconfigwidget.h"




HMotionUI::HMotionUI(QWidget *parent) : QWidget(parent)
{

    allLayout=new QBoxLayout(QBoxLayout::TopToBottom,this);
    allLayout->setContentsMargins(3,3,3,3);


    ptrMotion=H_MotionController::GetInstance();
    ptrConfgiWid=HConfigWidget::GetInstance();


    auto axisNames= ptrMotion->GetAxisNames();


    ptrWid1=creatStatusWid(axisNames);;
    ptrWid2=creatSettingWid();;
    ptrWid3=creatOperateWid(axisNames);;


    allLayout->addWidget(ptrWid1,3);
    allLayout->addWidget(ptrWid2);
    allLayout->addWidget(ptrWid3,3);



    /***********************************************************/
    QTimer *timer_status=new QTimer(this);
    timer_status->setInterval(100);
    connect(timer_status,&QTimer::timeout,this,&HMotionUI::refreshAxisStatus);


    timer_status->start();

    connect(ptrConfgiWid,&HConfigWidget::DataSaveAndChanged,this,&HMotionUI::reCreatWids);

}

void HMotionUI::refreshAxisStatus()
{
    int count=ptrLabels_status.size();
    for(int k=0;k!=count;++k){

        qint32 axisIndex=k;
        ptrLabels_status.at(k)->setText(ptrMotion->GetAxis(axisIndex)->GetAxisStatusString());
        ptrLabels_position.at(k)->setText(ptrMotion->GetAxis(axisIndex)->GetFeedbackPosString()+"mm");
        ptrLabels_velocity.at(k)->setText(ptrMotion->GetAxis(axisIndex)->GetVelString()+"mm/s");
    }
}

void HMotionUI::reCreatWids()
{

    ptrLabels_status.clear();
    ptrLabels_position.clear();
    ptrLabels_velocity.clear();

    allLayout->removeWidget(ptrWid1);
    allLayout->removeWidget(ptrWid2);
    allLayout->removeWidget(ptrWid3);


    delete ptrWid1;
    delete ptrWid2;
    delete ptrWid3;

    auto axisNames= ptrMotion->GetAxisNames();


    ptrWid1=creatStatusWid(axisNames);
    ptrWid2=creatSettingWid();
    ptrWid3=creatOperateWid(axisNames);


    allLayout->addWidget(ptrWid1,3);
    allLayout->addWidget(ptrWid2);
    allLayout->addWidget(ptrWid3,3);



}

QWidget *HMotionUI::creatStatusWid(const QStringList &names)
{
    QWidget *wid=new QWidget(this);
    QVBoxLayout* VBox=new QVBoxLayout(wid);
    VBox->setMargin(0);
    VBox->setSpacing(0);
    qint32 size=names.size();

    for (qint32 k=0;k<size;++k){

        QHBoxLayout* hBox1=new QHBoxLayout;
        hBox1->setSpacing(6);
        QLabel*label=new QLabel(wid);
        label->setStyleSheet("QLabel {background-color:rgb(180,180,180);border: 3px solid rgb(100,149,237);}");
        label->setFont(QFont("Calibri", 15, QFont::Bold));
        label->setFrameStyle(QFrame::Box | QFrame::Raised);
        label->setLineWidth(2);
        label->setText(names.at(k));
        label->setFixedWidth(60);
        label->setMinimumHeight(45);
        label->setAlignment(Qt::AlignCenter);
        hBox1->addWidget(label);


        label=new QLabel(wid);
        label->setStyleSheet("QLabel {background-color:rgb(180,180,180);border: 3px solid rgb(235,110,36);}");
        label->setFont(QFont("Microsoft Yahei UI", 10, QFont::Normal));
        label->setFrameStyle(QFrame::Box | QFrame::Raised);
        label->setWordWrap(true);
        label->setLineWidth(2);
        label->setMinimumWidth(80);
        label->setAlignment(Qt::AlignCenter);
        hBox1->addWidget(label);
        ptrLabels_status.append(label);


        label=new QLabel(wid);
        label->setStyleSheet("QLabel {background-color:rgb(180,180,180);border: 3px solid rgb(235,110,36);}");
        label->setFont(QFont("Calibri", 13, QFont::Bold));
        label->setFrameStyle(QFrame::Box | QFrame::Raised);
        label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        label->setMinimumWidth(120);
        label->setLineWidth(2);
        hBox1->addWidget(label);
        ptrLabels_position.append(label);

        label=new QLabel(wid);
        label->setStyleSheet("QLabel {background-color:rgb(180,180,180);border: 3px solid rgb(235,110,36);}");
        label->setFont(QFont("Calibri", 13, QFont::Bold));
        label->setFrameStyle(QFrame::Box | QFrame::Raised);
        //label->setMinimumWidth(200);
        label->setMinimumWidth(120);
        label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        label->setLineWidth(2);
        hBox1->addWidget(label);

        ptrLabels_velocity.append(label);


        VBox->addLayout(hBox1);
        VBox->addSpacing(3);
    }

    return wid;
}

QWidget *HMotionUI::creatSettingWid()
{
    QWidget *wid=new QWidget(this);

    QHBoxLayout* hBox=new QHBoxLayout(wid);


    QRadioButton*b1= new QRadioButton(tr("continues"),wid);
    QRadioButton*b2= new QRadioButton(tr("0.01mm"),wid);
    QRadioButton*b3= new QRadioButton(tr("0.1mm"),wid);
    QRadioButton*b4= new QRadioButton(tr("1mm"),wid);
    QRadioButton*b5= new QRadioButton("",wid);
    lineEdit=new QLineEdit(wid);
    QLabel *mlabel=new QLabel(wid);
    mlabel->setText("mm");
    mlabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    lineEdit->setEnabled(false);

    hBox->addWidget(b1);
    hBox->addWidget(b2);
    hBox->addWidget(b3);
    hBox->addWidget(b4);
    //allLayout->addLayout(hBox);
    QBoxLayout *leftLayout=new QBoxLayout(QBoxLayout::LeftToRight);

    leftLayout->addWidget(b5,0,Qt::AlignRight);
    leftLayout->addWidget(lineEdit,0,Qt::AlignLeft);
    leftLayout->addWidget(mlabel,0,Qt::AlignLeft);
    hBox->addLayout(leftLayout);

    QButtonGroup * buttonGroup=new QButtonGroup(wid);

    buttonGroup->addButton(b1,MoveMode::move_continuous);
    buttonGroup->addButton(b2,MoveMode::move_step_10);
    buttonGroup->addButton(b3,MoveMode::move_step_100);
    buttonGroup->addButton(b4,MoveMode::move_step_1000);
    buttonGroup->addButton(b5,MoveMode::move_step_custom);
    connect(buttonGroup,QOverload<int, bool>::of(&QButtonGroup::buttonToggled),wid,[=](int index,bool checked){
        if(index==move_step_custom&&checked){
            lineEdit->setEnabled(true);
        }
        else{
            lineEdit->setEnabled(false);
        }
        enuMovemode=MoveMode(index);
    });
    b1->setChecked(true);

    return wid;
}

QWidget *HMotionUI::creatOperateWid(const QStringList &names)
{
    QWidget *wid=new QWidget(this);
    QVBoxLayout* VBox=new QVBoxLayout(wid);

    qint32 size=names.size();
    for(qint32 k=0;k!=size;++k){
        QHBoxLayout* mk=new QHBoxLayout;

        QPushButton* button=new QPushButton(iconLeft,names.at(k)+"-",wid);
        button->setIconSize(QSize(38,38));
        button->setStyleSheet(HSheetStyle::H_BUTTON1);
        button->setMinimumWidth(45);
        button->setMinimumHeight(45);

        mk->addWidget(button);

        connect(button,&QPushButton::pressed,wid,[=](){
            switch (enuMovemode) {

            case MoveMode::move_continuous:{
                ptrMotion->GetAxis(k)->MoveContinue(false);
            }
                break;
            case MoveMode::move_step_10:{
                ptrMotion->GetAxis(k)->MoveStep(-0.01);
            }
                break;
            case MoveMode::move_step_100:{
                ptrMotion->GetAxis(k)->MoveStep(-0.1);
            }
                break;
            case MoveMode::move_step_1000:{
                ptrMotion->GetAxis(k)->MoveStep(-1.0);
            }
                break;
            case MoveMode::move_step_custom:{
                ptrMotion->GetAxis(k)->MoveStep(-lineEdit->text().toDouble());
            }
                break;

            }
        });

        connect(button,&QPushButton::released,wid,[=](){
            if(enuMovemode==MoveMode::move_continuous){
                ptrMotion->GetAxis(k)->StopAxis();
            }
        });


        button=new QPushButton(iconRight,names.at(k)+"+",wid);
        button->setIconSize(QSize(38,38));
        button->setStyleSheet(HSheetStyle::H_BUTTON1);
        button->setMinimumWidth(45);
        button->setMinimumHeight(45);
        mk->addWidget(button);

        connect(button,&QPushButton::pressed,this,[=](){


            switch (enuMovemode) {
            case MoveMode::move_continuous:{
                ptrMotion->GetAxis(k)->MoveContinue(true);
            }
                break;
            case MoveMode::move_step_10:{
                ptrMotion->GetAxis(k)->MoveStep(0.01);
            }
                break;
            case MoveMode::move_step_100:{
                ptrMotion->GetAxis(k)->MoveStep(0.1);
            }
                break;
            case MoveMode::move_step_1000:{
                ptrMotion->GetAxis(k)->MoveStep(1.0);
            }
                break;
            case MoveMode::move_step_custom:{
                ptrMotion->GetAxis(k)->MoveStep(lineEdit->text().toDouble());
            }
                break;

            }
        });

        connect(button,&QPushButton::released,wid,[=](){
            if(enuMovemode==MoveMode::move_continuous){
                ptrMotion->GetAxis(k)->StopAxis();
            }
            ptrMotion->GetAxis(0)->TestFunc();
        });

        QLineEdit * mlineEdit=new QLineEdit(this);
        connect(mlineEdit,&QLineEdit::returnPressed,this,[=](){
            ptrMotion->GetAxis(k)->MoveTo(mlineEdit->text().toDouble());
        });
        mlineEdit->setMaximumWidth(120);
        mlineEdit->setMinimumHeight(45);
        mk->addWidget(mlineEdit);

        VBox->addLayout(mk);
    }

    return wid;
}





