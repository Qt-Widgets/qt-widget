#include "himgprocessmwindow.h"
#include "ui_himgprocessmwindow.h"
#include "h_halcon_widget.h"
#include "himgplugin.h"
#include <QtWidgets>
#include <QCloseEvent>
#include "hdialogproperty.h"
#include "dialogcamerasetting.h"

using namespace OpencvCpp;


HImgProcessMWindow::HImgProcessMWindow(HImgPlugin *pInterface, QWidget *parent) :
    QMainWindow(parent),ptrPlugin(pInterface),
    ui(new Ui::HImgProcessMWindow)
{
    setFont(MyFont1);
    ui->setupUi(this);
    //setWindowFlags(Qt::WindowCloseButtonHint|Qt::WindowMinMaxButtonsHint|Qt::Drawer);
    setVisible(false);

    ptrTimer1=new QTimer(this);
    ptrTimer1->setInterval(50);
    connect(ptrTimer1, &QTimer::timeout, this, &HImgProcessMWindow::getImage);

    ptrCameraSetting=new DialogCameraSetting(ptrPlugin,this);

    setupCenterWid();
    setupToolBar();
    setupMenuBar();
    setupImageProcessDockWid();

    setControlEnabled(false);

    QSettings settings("QtApp", "inno");
    restoreGeometry(settings.value("Window/imgProcess/geometry").toByteArray());
    restoreState(settings.value("Window/imgProcess/windowState").toByteArray());

    openFromFile(QCoreApplication::applicationDirPath()+"/plugins/settings/settings.bin");
    initControl();
}

HImgProcessMWindow::~HImgProcessMWindow()
{
    QSettings settings("QtApp","inno");
    settings.setValue("Window/imgProcess/geometry", saveGeometry());
    settings.setValue("Window/imgProcess/windowState", saveState());

    writeToFile(QCoreApplication::applicationDirPath()+"/plugins/settings/settings.bin");
    delete ui;
}

bool HImgProcessMWindow::showImage(uchar *pImage, qint32 width, qint32 height)
{
    return camShow->showImage(pImage,width,height);
}

bool HImgProcessMWindow::processImage(const QString &name, QPointF &result)
{
    double cx,cy,radius;

    QString fname=name;
    if(fname.isEmpty()){
        fname=currentParas;
    }

    if(imgProcessParas.contains(fname)){
        ui->listWidget->setCurrentRow(funcNames.indexOf(fname));
        on_toolButton_set_default_clicked();


        if(reprocessCircleCallipers(cx,cy,radius)){
            QPointF pos(cx,cy);
            QTransform trans=QTransform::fromTranslate(ui->result_offset_x->text().toDouble(),ui->result_offset_y->text().toDouble());
            trans.scale(ui->result_scale_x->text().toDouble(),ui->result_scale_y->text().toDouble()).rotate(ui->result_rotate->text().toDouble());
            pos=trans.map(pos);
            result=pos;
            return true;
        }
    }
    return  false;
}

bool HImgProcessMWindow::processImageLine(const QString &name, QLineF &result)
{

    QString fname=name;
    if(fname.isEmpty()){
        fname=currentParas;
    }

    if(imgProcessParas.contains(fname)){
        ui->listWidget->setCurrentRow(funcNames.indexOf(fname));
        on_toolButton_set_default_clicked();

        QStringList list;
        list<<ui->reprocess_line_1->text();
        list<<ui->reprocess_line_2->text();
        list<<ui->reprocess_line_3->text();
        list<<ui->reprocess_line_4->text();
        list<<ui->reprocess_line_5->text();
        list<<ui->reprocess_line_6->text();
        list<<ui->reprocess_line_7->text();
        list<<ui->reprocess_line_8->text();
        list<<QString::number(ui->reprocess_line_9->value());
        list<<QString::number(ui->reprocess_line_10->value());


        if(camShow->callipersLineProcess(list,result)){

            QTransform trans=QTransform::fromTranslate(ui->result_offset_x->text().toDouble(),ui->result_offset_y->text().toDouble());
            trans.scale(ui->result_scale_x->text().toDouble(),ui->result_scale_y->text().toDouble()).rotate(ui->result_rotate->text().toDouble());
            result=trans.map(result);
            return true;
        }
    }
    return  false;
}

qint32 HImgProcessMWindow::moveToFocus() const
{
    return ptrCameraSetting->moveToFcous();
}



void HImgProcessMWindow::resetCameras(const QStringList &names)
{

    ui->menucamera->clear();
    for(const auto&k:ptrCameraGroup->actions()){
        ptrCameraGroup->removeAction(k);
        delete k;
    }
    for(const auto&k:names){
        auto mk=new QAction(k);
        mk->setCheckable(true);
        mk->setChecked(cameraName==k);
        ptrCameraGroup->addAction(mk);
    }
    ui->menucamera->addActions(ptrCameraGroup->actions());
    ptrCameraSetting->setCurrentCamera(cameraName);
}

void HImgProcessMWindow::resetAxises(const QStringList &names)
{

    ui->menuAxis_X->clear();
    ui->menuAxis_Y->clear();
    ui->menuAxis_Z->clear();
    for(const auto&k:ptrAxisXGroup->actions()){
        ptrAxisXGroup->removeAction(k);
        delete k;
    }
    for(const auto&k:ptrAxisYGroup->actions()){
        ptrAxisYGroup->removeAction(k);
        delete k;
    }
    for(const auto&k:ptrAxisZGroup->actions()){
        ptrAxisZGroup->removeAction(k);
        delete k;
    }
    for(const auto&k:names){
        auto mk=new QAction(k);
        mk->setCheckable(true);
        mk->setChecked(k==axisXName);
        ptrAxisXGroup->addAction(mk);

        mk=new QAction(k);
        mk->setCheckable(true);
        mk->setChecked(k==axisYName);
        ptrAxisYGroup->addAction(mk);

        mk=new QAction(k);
        mk->setCheckable(true);
        mk->setChecked(k==axisZName);
        ptrAxisZGroup->addAction(mk);
    }
    ui->menuAxis_X->addActions(ptrAxisXGroup->actions());
    ui->menuAxis_Y->addActions(ptrAxisYGroup->actions());
    ui->menuAxis_Z->addActions(ptrAxisZGroup->actions());

}


void HImgProcessMWindow::showStatusMessage(const QString &msg)
{
    ui->statusbar->showMessage(msg);
}

QDataStream &operator>>(QDataStream &input, HImgProcessMWindow &obj){
    QStringList list;
    input>>obj.funcNames>>obj.currentParas>>obj.imgProcessParas>>obj.axisFocus>>obj.cameraName>>obj.axisXName>>obj.axisYName>>obj.axisZName>>list;
    obj.ptrCameraSetting->initData(list);
    return input;
}

QDataStream &operator<<(QDataStream &output, const HImgProcessMWindow &obj){
    output<<obj.funcNames<<obj.currentParas<<obj.imgProcessParas<<obj.axisFocus<<obj.cameraName<<obj.axisXName<<obj.axisYName<<obj.axisZName
         <<obj.ptrCameraSetting->getSaveData();
    return output;
}



void HImgProcessMWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    m_ActionShotContinues->setChecked(false);
    setVisible(false);
    getImageContinues(false);
    m_ActionShotContinues->setChecked(false);
}

bool HImgProcessMWindow::getImage()
{
    uchar *pData=nullptr;
    qint32 width=0;
    qint32 height=0;

    if(emit ptrPlugin->imgShowed(cameraName,pData,width,height)){
        return camShow->showImage(pData,width,height);
    }
    return false;
}


void HImgProcessMWindow::getImageContinues(bool isChecked)
{
    if(isChecked){
        ptrTimer1->start();
    }
    else{
        ptrTimer1->stop();
    }
}

void HImgProcessMWindow::saveParas()
{
    if(currentParas.isEmpty())
        return;
    QStringList list;
    list<<QString::number(ui->region_type->currentIndex());
    list<<ui->region_rect_tl_x->text();
    list<<ui->region_rect_tl_y->text();
    list<<ui->region_rect_br_x->text();
    list<<ui->region_rect_br_y->text();

    list<<QString::number(ui->template_type->currentIndex());
    list<<ui->template_circle_cx->text();
    list<<ui->template_circle_cy->text();
    list<<ui->template_circle_radius->text();
    list<<ui->template_circle_para1->text();
    list<<ui->template_circle_para2->text();
    list<<ui->template_circle_para3->text();
    list<<ui->template_find_para1->text();
    list<<ui->template_find_para2->text();
    list<<ui->template_find_para3->text();
    list<<ui->template_find_para4->text();
    list<<ui->template_find_para5->text();
    list<<ui->template_find_para6->text();
    list<<ui->template_find_para7->text();

    list<<QString::number(ui->reprocess_type->currentIndex());
    list<<ui->reprocess_length->text();
    list<<ui->reprocess_start_angle->text();
    list<<ui->reprocess_end_angle->text();
    list<<ui->reprocess_sigma->text();
    list<<ui->reprocess_threshold->text();
    list<<ui->reprocess_count->text();
    list<<QString::number(ui->reprocess_direction->value());
    list<<QString::number(ui->reprocess_select->value());
    list<<QString::number(ui->reprocess_polarity->value());
    list<<ui->reprocess_line_1->text();
    list<<ui->reprocess_line_2->text();
    list<<ui->reprocess_line_3->text();
    list<<ui->reprocess_line_4->text();
    list<<ui->reprocess_line_5->text();
    list<<ui->reprocess_line_6->text();
    list<<ui->reprocess_line_7->text();
    list<<ui->reprocess_line_8->text();
    list<<QString::number(ui->reprocess_line_9->value());
    list<<QString::number(ui->reprocess_line_10->value());

    list<<ui->result_offset_x->text();
    list<<ui->result_offset_y->text();
    list<<ui->result_scale_x->text();
    list<<ui->result_scale_y->text();
    list<<ui->result_rotate->text();

    imgProcessParas[currentParas]=list;
}

bool HImgProcessMWindow::processImageTest()
{
    return false;
}

void HImgProcessMWindow::regionDrawRect()
{
    QPointF tl,br;
    camShow->drawRect(tl,br);
    ui->region_rect_tl_x->setText(QString::number(tl.x()));
    ui->region_rect_tl_y->setText(QString::number(tl.y()));
    ui->region_rect_br_x->setText(QString::number(br.x()));
    ui->region_rect_br_y->setText(QString::number(br.y()));

}

void HImgProcessMWindow::regionShowRect()
{
    camShow->showRect(ui->region_rect_tl_x->text().toDouble(),
                      ui->region_rect_tl_y->text().toDouble(),
                      ui->region_rect_br_x->text().toDouble(),
                      ui->region_rect_br_y->text().toDouble());
}

void HImgProcessMWindow::templateDrawCircle()
{
    QPointF center;
    double radius;
    camShow->drawCircle(center,radius);
    ui->template_circle_cx->setText(QString::number(center.x()));
    ui->template_circle_cy->setText(QString::number(center.y()));
    ui->template_circle_radius->setText(QString::number(radius));

    QStringList list;
    list<<ui->template_circle_para1->text()<<ui->template_circle_para2->text()<<ui->template_circle_para3->text();
    camShow->createCircleXLDTemplate(QCoreApplication::applicationDirPath()+QString("/plugins/settings/%1.gvt").arg(currentParas)
                                     ,list,center.x(),center.y(),radius);
}

void HImgProcessMWindow::templateShowCircle()
{
    camShow->showCircle(ui->template_circle_cx->text().toDouble(),
                        ui->template_circle_cy->text().toDouble(),
                        ui->template_circle_radius->text().toDouble());
}

bool HImgProcessMWindow::templateCircle(double &cx, double &cy, double &angle, double radius, bool isShow)
{
    QString file=QCoreApplication::applicationDirPath()+QString("/plugins/settings/%1.gvt").arg(currentParas);
    QStringList list;
    list<<ui->template_find_para1->text();
    list<<ui->template_find_para2->text();
    list<<ui->template_find_para3->text();
    list<<ui->template_find_para4->text();
    list<<ui->template_find_para5->text();
    list<<ui->template_find_para6->text();
    list<<ui->template_find_para7->text();


    if(camShow->createRectRegion(ui->region_rect_tl_x->text().toDouble(),
                                 ui->region_rect_tl_y->text().toDouble(),
                                 ui->region_rect_br_x->text().toDouble(),
                                 ui->region_rect_br_y->text().toDouble(),ui->region_type->currentIndex()==1
                                 )){
        return camShow->templateProcess(file,list,cx,cy,angle,radius,isShow);
    }
    return false;
}

void HImgProcessMWindow::templateTestCircle()
{
    double cx,cy,angle;
    templateCircle(cx,cy,angle,ui->template_circle_radius->text().toDouble());
}

bool HImgProcessMWindow::reprocessCircleCallipers(double &cx, double &cy, double &radius)
{
    QStringList list;

    if(ui->template_type->currentIndex()==1){
        double angle;
        if(templateCircle(cx,cy,angle,ui->template_circle_radius->text().toDouble(),false)){
            list<<QString::number(cx)<<QString::number(cy)<<ui->template_circle_radius->text();
        }else{
            return false;
        }
    }else{
        list<<ui->template_circle_cx->text()<<ui->template_circle_cy->text()<<ui->template_circle_radius->text();
    }

    list<<ui->reprocess_length->text();
    list<<ui->reprocess_start_angle->text();
    list<<ui->reprocess_end_angle->text();
    list<<ui->reprocess_sigma->text();
    list<<ui->reprocess_threshold->text();
    list<<ui->reprocess_count->text();
    list<<QString::number(ui->reprocess_direction->value());
    list<<QString::number(ui->reprocess_select->value());
    list<<QString::number(ui->reprocess_polarity->value());

    switch (ui->reprocess_type->currentIndex()) {
    case 0:{
        return camShow->noneReProcess(cx,cy,radius);
    }
    case 1:{
        return camShow->callipersCircleProcess(list,cx,cy,radius);
    }

    default:
        return  false;
    }
}

void HImgProcessMWindow::reprocessCircleCallipersTest()
{
    double cx,cy,radius;
    reprocessCircleCallipers(cx,cy,radius);
}

void HImgProcessMWindow::on_toolButton_new_clicked()
{
    HDialogProperty dialog(tr("Add Function"));
    dialog.setLabels("Name");
    dialog.setContents("0");
    dialog.setUniqueKeys(0,imgProcessParas.keys());

    if(dialog.exec()){
        newItem(dialog.getContentData().first().toString());
    }

}

void HImgProcessMWindow::on_toolButton_delete_clicked()
{
    for(const auto &k:ui->listWidget->selectedItems()){
        ui->listWidget->takeItem(ui->listWidget->row(k));
        imgProcessParas.remove(k->text());
        funcNames.removeOne(k->text());
        if(k->text()==currentParas){
            on_toolButton_set_default_clicked();
        }
    }
}

void HImgProcessMWindow::on_toolButton_clear_clicked()
{
    ui->listWidget->clear();
    currentParas=nullptr;
    imgProcessParas.clear();
    funcNames.clear();
    on_toolButton_set_default_clicked();
}



void HImgProcessMWindow::on_toolButton_set_default_clicked()
{
    qint32 size=ui->listWidget->count();
    if(size==0||!ui->listWidget->currentItem()){
        setControlEnabled(false);
        return;
    }
    for(qint32 k=0;k<size;++k){
        ui->listWidget->item(k)->setIcon(defaultIcon);
    }
    ui->listWidget->currentItem()->setIcon(ui->toolButton_set_default->icon());

    currentParas=ui->listWidget->currentItem()->text();
    refreshControl();
    setControlEnabled(true);

}


void HImgProcessMWindow::setupCenterWid()
{
    camShow=new H_Halcon_Widget(this);
    setCentralWidget(camShow);
}

void HImgProcessMWindow::setupToolBar()
{
    ptrToolBar=new QToolBar(tr("tool bar"),this);
    ptrToolBar->setIconSize(QSize(30,30));
    ptrToolBar->setObjectName("toolBar");
    ptrToolBar->addAction(QIcon(":/resource/icons/rectangle.svg"),tr("capture"),this,&HImgProcessMWindow::getImage);
    m_ActionShotContinues=ptrToolBar->addAction(QIcon(":/resource/icons/rectangles.svg"),tr("capture"));
    m_ActionShotContinues->setCheckable(true);
    connect(m_ActionShotContinues,&QAction::toggled,this,&HImgProcessMWindow::getImageContinues);

    ptrToolBar->addAction(QIcon(":/resource/icons/folder.svg"),tr("open file"),camShow,&H_Halcon_Widget::showImageFromFile);
    ptrToolBar->addSeparator();

    ptrToolBar->addAction(QIcon(":/resource/icons/sweep.svg"),tr("clear"),camShow,&H_Halcon_Widget::clearWindow);
    ptrToolBar->addAction(QIcon(":/resource/icons/maximize.svg"),tr("reset"),camShow,&H_Halcon_Widget::resetWindow);

    ptrToolBar->addSeparator();
    m_ActionSave=ptrToolBar->addAction(QIcon(":/resource/icons/save.svg"),tr("save"),this,&HImgProcessMWindow::saveParas);
    m_ActionTest=ptrToolBar->addAction(QIcon(":/resource/icons/test1.svg"),tr("test"),this,&HImgProcessMWindow::processImageTest);
    m_ActionMoveFocus=ptrToolBar->addAction(QIcon(":/resource/icons/paper-plane.svg"),tr("move focus"),ptrCameraSetting,&DialogCameraSetting::moveToFcous);
    ptrToolBar->addSeparator();


    addToolBar(ptrToolBar);
}

void HImgProcessMWindow::setupMenuBar()
{
    ptrCameraGroup=new QActionGroup(ui->menucamera);
    connect(ptrCameraGroup,&QActionGroup::triggered,this,[=](QAction * action){cameraName=action->text(); ptrCameraSetting->setCurrentCamera(cameraName);});

    ptrAxisXGroup=new QActionGroup(ui->menuAxis_X);
    connect(ptrAxisXGroup,&QActionGroup::triggered,this,[=](QAction * action){axisXName=action->text();});
    ptrAxisYGroup=new QActionGroup(ui->menuAxis_Y);
    connect(ptrAxisYGroup,&QActionGroup::triggered,this,[=](QAction * action){axisYName=action->text();});
    ptrAxisZGroup=new QActionGroup(ui->menuAxis_Z);
    connect(ptrAxisZGroup,&QActionGroup::triggered,this,[=](QAction * action){axisZName=action->text();});


    connect(ui->actioncamera,&QAction::triggered,this,[=](){
        ptrCameraSetting->show();
    });

    //ui->menucamera->addActions(ptrCameraGroup->actions());
}

void HImgProcessMWindow::setupImageProcessDockWid()
{
    QStringList list;
    list<<tr("none")<<tr("rectangle");
    connect(ui->region_type,QOverload<int>::of(&QComboBox::currentIndexChanged),this,[=](qint32 index){
        switch (index) {
        case 0:{
            ui->region_group_rect->setEnabled(false);
        }
            break;
        case 1:{
            ui->region_group_rect->setEnabled(true);
        }
            break;
        default:
            break;
        }
    });

    ui->region_type->addItems(list);


    list.last()=tr("circle");
    connect(ui->template_type,QOverload<int>::of(&QComboBox::currentIndexChanged),this,[=](qint32 index){
        switch (index) {
        case 0:{
            ui->template_group_circle->setEnabled(false);
            ui->template_group_find->setEnabled(false);
        }
            break;
        case 1:{
            ui->template_group_circle->setEnabled(true);
            ui->template_group_find->setEnabled(true);
        }
            break;
        default:{
            ui->template_group_circle->setEnabled(false);
            ui->template_group_find->setEnabled(false);
        }
            break;
        }
    });
    ui->template_type->addItems(list);

    list.last()=tr("circle callipers");
    connect(ui->reprocess_type,QOverload<int>::of(&QComboBox::currentIndexChanged),this,[=](qint32 index){
        switch (index) {
        case 0:{
            ui->reprocess_group_circle_callipers->setEnabled(false);
        }
            break;
        case 1:{
            ui->reprocess_group_circle_callipers->setEnabled(true);
        }
            break;
        default:{
            ui->reprocess_group_circle_callipers->setEnabled(false);
        }
            break;
        }
    });
    list<<tr("line callipers");
    ui->reprocess_type->addItems(list);


    connect(ui->region_draw_rect,&QToolButton::clicked,this,&HImgProcessMWindow::regionDrawRect);
    connect(ui->region_show_rect,&QToolButton::clicked,this,&HImgProcessMWindow::regionShowRect);
    connect(ui->template_circle_draw,&QToolButton::clicked,this,&HImgProcessMWindow::templateDrawCircle);
    connect(ui->template_circle_show,&QToolButton::clicked,this,&HImgProcessMWindow::templateShowCircle);
    connect(ui->template_find_test,&QToolButton::clicked,this,&HImgProcessMWindow::templateTestCircle);
    connect(ui->reprocess_circle_callipers_test,&QToolButton::clicked,this,&HImgProcessMWindow::reprocessCircleCallipersTest);
    connect(ui->template_circle_reset,&QToolButton::clicked,this,[=](){
        ui->template_circle_para1->setText("-0.39");
        ui->template_circle_para2->setText("0.79");
        ui->template_circle_para3->setText("5");
    });
    connect(ui->template_find_reset,&QToolButton::clicked,this,[=](){
        ui->template_find_para1->setText("-0.39");
        ui->template_find_para2->setText("0.79");
        ui->template_find_para3->setText("0.5");
        ui->template_find_para4->setText("1");
        ui->template_find_para5->setText("0.5");
        ui->template_find_para6->setText("0");
        ui->template_find_para7->setText("0.9");

    });

    connect(ui->reprocess_circle_callipers_reset,&QToolButton::clicked,this,[=](){
        ui->reprocess_length->setText("100");
        ui->reprocess_start_angle->setText("0");
        ui->reprocess_end_angle->setText("360");
        ui->reprocess_sigma->setText("0.8");
        ui->reprocess_threshold->setText("30");
        ui->reprocess_count->setText("600");
        ui->reprocess_direction->setValue(0);
        ui->reprocess_select->setValue(0);
        ui->reprocess_polarity->setValue(0);
    });



}

void HImgProcessMWindow::initControl()
{
    ui->listWidget->clear();
    qint32 m=0,index=-1;
    for(const auto&k:funcNames){
        newItem(k,false);
        if(k==currentParas){
            index=m;
        }
        ++m;
    }
    if(index!=-1){
        ui->listWidget->setCurrentRow(index);
        on_toolButton_set_default_clicked();
        QStringList list;
        list<<ui->template_circle_para1->text()<<ui->template_circle_para2->text()<<ui->template_circle_para3->text();
        if(ui->template_type->currentIndex()==1)
            camShow->createCircleXLDTemplate(QCoreApplication::applicationDirPath()+QString("/plugins/settings/%1.gvt").arg(currentParas)
                                             ,list, ui->template_circle_cx->text().toDouble(),ui->template_circle_cy->text().toDouble(),ui->template_circle_radius->text().toDouble());
    }


}

void HImgProcessMWindow::refreshControl()
{
    auto list=imgProcessParas.value(currentParas);
    qint32 i=-1;
    ui->region_type->setCurrentIndex(list.value(++i).toInt());
    ui->region_rect_tl_x->setText(list.value(++i));
    ui->region_rect_tl_y->setText(list.value(++i));
    ui->region_rect_br_x->setText(list.value(++i));
    ui->region_rect_br_y->setText(list.value(++i));

    ui->template_type->setCurrentIndex(list.value(++i).toInt());
    ui->template_circle_cx->setText(list.value(++i));
    ui->template_circle_cy->setText(list.value(++i));
    ui->template_circle_radius->setText(list.value(++i));
    ui->template_circle_para1->setText(list.value(++i));
    ui->template_circle_para2->setText(list.value(++i));
    ui->template_circle_para3->setText(list.value(++i));
    ui->template_find_para1->setText(list.value(++i));
    ui->template_find_para2->setText(list.value(++i));
    ui->template_find_para3->setText(list.value(++i));
    ui->template_find_para4->setText(list.value(++i));
    ui->template_find_para5->setText(list.value(++i));
    ui->template_find_para6->setText(list.value(++i));
    ui->template_find_para7->setText(list.value(++i));

    ui->reprocess_type->setCurrentIndex(list.value(++i).toInt());
    ui->reprocess_length->setText(list.value(++i));
    ui->reprocess_start_angle->setText(list.value(++i));
    ui->reprocess_end_angle->setText(list.value(++i));
    ui->reprocess_sigma->setText(list.value(++i));
    ui->reprocess_threshold->setText(list.value(++i));
    ui->reprocess_count->setText(list.value(++i));
    ui->reprocess_direction->setValue(list.value(++i).toInt());
    ui->reprocess_select->setValue(list.value(++i).toInt());
    ui->reprocess_polarity->setValue(list.value(++i).toInt());
    ui->reprocess_line_1->setText(list.value(++i));
    ui->reprocess_line_2->setText(list.value(++i));
    ui->reprocess_line_3->setText(list.value(++i));
    ui->reprocess_line_4->setText(list.value(++i));
    ui->reprocess_line_5->setText(list.value(++i));
    ui->reprocess_line_6->setText(list.value(++i));
    ui->reprocess_line_7->setText(list.value(++i));
    ui->reprocess_line_8->setText(list.value(++i));
    ui->reprocess_line_9->setValue(list.value(++i).toInt());
    ui->reprocess_line_10->setValue(list.value(++i).toInt());

    ui->result_offset_x->setText(list.value(++i));
    ui->result_offset_y->setText(list.value(++i));
    ui->result_scale_x->setText(list.value(++i));
    ui->result_scale_y->setText(list.value(++i));
    ui->result_rotate->setText(list.value(++i));

}

void HImgProcessMWindow::setControlEnabled(bool checked)
{
    m_ActionSave->setEnabled(checked);
    m_ActionTest->setEnabled(checked);

    for(qint32 k=1;k<5;++k){
        ui->tabWidget->setTabEnabled(k,checked);
    }
}


void HImgProcessMWindow::newItem(const QString &name, bool isNew, bool isActived)
{
    auto mk=new QListWidgetItem(name);
    // mk->setFlags( Qt::ItemIsEnabled|/*Qt::ItemIsEditable|*/Qt::ItemIsSelectable);

    mk->setIcon(isActived?ui->toolButton_set_default->icon():defaultIcon);
    ui->listWidget->addItem(mk);
    if(isNew){
        funcNames<<name;
        imgProcessParas[name]=QStringList();
    }
}


void HImgProcessMWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    ui->toolButton_set_default->setEnabled(current);
    ui->toolButton_delete->setEnabled(current);
}


bool HImgProcessMWindow::writeToFile(const QString &filePath)
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

bool HImgProcessMWindow::openFromFile(const QString &filePath)
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


void HImgProcessMWindow::on_reprocess_line_callipers_draw_clicked()
{
    QPointF rc,ll;
    double phi;
    camShow->drawRect1(rc,ll,phi);
    ui->reprocess_line_1->setText(QString::number(rc.x(),'f',4));
    ui->reprocess_line_2->setText(QString::number(rc.y(),'f',4));
    ui->reprocess_line_3->setText(QString::number(ll.x(),'f',4));
    ui->reprocess_line_4->setText(QString::number(ll.y(),'f',4));
    ui->reprocess_line_5->setText(QString::number(phi,'f',4));
}

void HImgProcessMWindow::on_reprocess_line_callipers_test_clicked()
{

    QStringList list;
    list<<ui->reprocess_line_1->text();
    list<<ui->reprocess_line_2->text();
    list<<ui->reprocess_line_3->text();
    list<<ui->reprocess_line_4->text();
    list<<ui->reprocess_line_5->text();
    list<<ui->reprocess_line_6->text();
    list<<ui->reprocess_line_7->text();
    list<<ui->reprocess_line_8->text();
    list<<QString::number(ui->reprocess_line_9->value());
    list<<QString::number(ui->reprocess_line_10->value());

    QLineF line;
    camShow->callipersLineProcess(list,line);

}
