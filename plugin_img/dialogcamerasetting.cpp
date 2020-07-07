#include "dialogcamerasetting.h"
#include "ui_dialogcamerasetting.h"
#include "himgprocessmwindow.h"
#include "himgplugin.h"

DialogCameraSetting::DialogCameraSetting(HImgPlugin *ptrInterfance,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCameraSetting),
    ptrPlugin(ptrInterfance)
{
    ui->setupUi(this);
    setFont(MyFont1);

    connect(ui->lineEdit_gain,&QLineEdit::returnPressed,this,[=](){

        auto gain=ui->lineEdit_gain->text().toInt();

        if(ptrPlugin->cameraGainChanged(cameraName,gain)){
            return;
        }
        auto ogain=ui->horizontalSlider_gain->value();
        ui->horizontalSlider_gain->setMaximum(gain+qAbs(gain-ogain)/2);
        ui->horizontalSlider_gain->setMinimum((gain-qAbs(gain-ogain)/2)<0?0:(gain-qAbs(gain-ogain)/2));
        ui->horizontalSlider_gain->setValue(gain);
    });

    connect(ui->lineEdit_exposure,&QLineEdit::returnPressed,this,[=](){
        auto exposure=ui->lineEdit_exposure->text().toInt();

        if(ptrPlugin->cameraExposureChanged(cameraName,exposure)){
            return;
        }
        auto oexposure=ui->horizontalSlider_exposure->value();

        ui->horizontalSlider_exposure->setMaximum(exposure+qAbs(exposure-oexposure)/2);
        ui->horizontalSlider_exposure->setMinimum((exposure-qAbs(exposure-oexposure)/2)<38?38:(exposure-qAbs(exposure-oexposure)/2));


        ui->horizontalSlider_exposure->setValue(exposure);
    });

    connect(ui->horizontalSlider_gain,&QSlider::sliderMoved,this,[=](qint32 value){
        ui->lineEdit_gain->setText(QString::number(value));
        ptrPlugin->cameraGainChanged(cameraName,value);
    });

    connect(ui->horizontalSlider_exposure,&QSlider::sliderMoved,this,[=](qint32 value){
        ui->lineEdit_exposure->setText(QString::number(value));
        ptrPlugin->cameraExposureChanged(cameraName,value);
    });

    connect(ui->toolButton_autoFocus_get,&QToolButton::clicked,this,[=](){
        auto pa=(HImgProcessMWindow*)(this->parent());
        double pos=0;
        if(ptrPlugin->getAxisPos(pa->getAxisZName(),pos)){
            return;
        }
        ui->lineEdit_autoFocus_focus->setText(QString::number(pos));
    });

    QSettings settings("QtApp", "inno");
    restoreGeometry(settings.value("Window/imgProcess/cameraSetting/geometry").toByteArray());


    ui->horizontalSlider_gain->setMinimum(settings.value("Window/imgProcess/cameraSetting/gainMin").toInt());
    ui->horizontalSlider_gain->setMaximum(settings.value("Window/imgProcess/cameraSetting/gainMax").toInt());
    ui->horizontalSlider_exposure->setMinimum(settings.value("Window/imgProcess/cameraSetting/exposureMin").toInt());
    ui->horizontalSlider_exposure->setMaximum(settings.value("Window/imgProcess/cameraSetting/exposureMax").toInt());
}

DialogCameraSetting::~DialogCameraSetting()
{
    QSettings settings("QtApp","inno");
    settings.setValue("Window/imgProcess/cameraSetting/geometry", saveGeometry());

    settings.setValue("Window/imgProcess/cameraSetting/gainMin", ui->horizontalSlider_gain->minimum());
    settings.setValue("Window/imgProcess/cameraSetting/gainMax", ui->horizontalSlider_gain->maximum());
    settings.setValue("Window/imgProcess/cameraSetting/exposureMin", ui->horizontalSlider_exposure->minimum());
    settings.setValue("Window/imgProcess/cameraSetting/exposureMax", ui->horizontalSlider_exposure->maximum());
    delete ui;
}



void DialogCameraSetting::setCurrentCamera(const QString &cname)
{
    cameraName=cname;
    QStringList data;
    if(ptrPlugin->cameraSettingsRefreshed(cameraName,data)){
        return;
    }
    auto gain=data.value(0).toInt();
    auto exposure=data.value(1).toInt();
    ui->lineEdit_gain->setText(QString::number(gain));

    if(gain<ui->horizontalSlider_gain->minimum()||gain>ui->horizontalSlider_gain->maximum()){
        ui->horizontalSlider_gain->setMaximum(gain+100);
        ui->horizontalSlider_gain->setMinimum((gain-100)<0?0:(gain-100));
    }
    ui->horizontalSlider_gain->setValue(gain);

    ui->lineEdit_exposure->setText(QString::number(exposure));
    if(exposure<ui->horizontalSlider_exposure->minimum()||exposure>ui->horizontalSlider_exposure->maximum()){
        ui->horizontalSlider_exposure->setMaximum(exposure+1000);
        ui->horizontalSlider_exposure->setMinimum((exposure-1000)<38?38:(exposure-1000));
    }
    ui->horizontalSlider_exposure->setValue(exposure);
}

double DialogCameraSetting::getFocus() const
{
    return ui->lineEdit_autoFocus_focus->text().toDouble();
}

void DialogCameraSetting::initData(const QStringList &data)
{
    qint32 k=-1;
    ui->lineEdit_autoFocus_Zmin->setText(data.value(++k));
    ui->lineEdit_autoFocus_Zmax->setText(data.value(++k));
    ui->lineEdit_autoFocus_step1->setText(data.value(++k));
    ui->lineEdit_autoFocus_step2->setText(data.value(++k));
    ui->lineEdit_autoFocus_focus->setText(data.value(++k));


}

QStringList DialogCameraSetting::getSaveData() const
{
    QStringList list;
    list<<ui->lineEdit_autoFocus_Zmin->text()<<ui->lineEdit_autoFocus_Zmax->text()<<ui->lineEdit_autoFocus_step1->text()
       <<ui->lineEdit_autoFocus_step2->text()<<ui->lineEdit_autoFocus_focus->text();
    return  list;
}

qint32 DialogCameraSetting::moveToFcous()
{
    return ptrPlugin->axisMoveToWait(((HImgProcessMWindow*)(this->parent()))->getAxisZName(),ui->lineEdit_autoFocus_focus->getNumberF());
}




