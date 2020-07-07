

#include "toolbar.h"

#include <QRandomGenerator>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QPainterPath>
#include <QSpinBox>
#include <QLabel>
#include <QToolTip>
#include <stdlib.h>
#include <QSettings>
#include <QFileDialog>
#include <QTimer>
#include <QMessageBox>
#include <QtConcurrent>
#include <QCloseEvent>

#include "hmathfunctions.h"
#include <QDebug>

#include "Src/data/inisettings.h"
#include "Src/Motion/h_motioncontroller.h"
#include "Src/data/mythreadworker.h"
#include "modbusMaster/hmodbusmainwindow.h"


#include "Src/Motion/h_motioncontroller.h"
#include "Src/Motion/hiodevicecontroller.h"


#include "Src/window/programhandler.h"
#include "Src/Gui/settings/hsystemadjustcontroller.h"
#include "Src/Motion/hscannercontroller.h"



ToolBar::ToolBar(const QString &title,const HToolBars index, QWidget *parent)
    : QToolBar(parent)
{
    setWindowTitle(title);
    setObjectName(title);



    QMenu *menu;
    switch (index) {
    case HToolBars::PROGRAM_TOOLS:
    {
        setIconSize(QSize(50, 50));


        menu = new QMenu(tr("Process"), this);
        menu->setIcon(QIcon(":/resource/icons/start.svg"));

        QActionGroup *startGroup = new QActionGroup(this);
        QAction *ms1=menu->addAction(tr("run once"));
        QAction *ms2=menu->addAction(tr("run with location"));
        QAction *ms3=menu->addAction(tr("run continues"));

        ms1->setCheckable(true);
        ms2->setCheckable(true);
        ms3->setCheckable(true);
        ms1->setChecked(true);

        startGroup->addAction(ms1);
        startGroup->addAction(ms2);
        startGroup->addAction(ms3);

        menu->addSeparator();
        QMenu *menu2 = new QMenu(tr("Range of processing"), this);

        QActionGroup *rangeGroup = new QActionGroup(this);

        QAction * rs1= menu2->addAction(tr("selection"));
        QAction * rs2= menu2->addAction(tr("all"));

        rs1->setCheckable(true);
        rs2->setCheckable(true);
        rs2->setChecked(true);

        rangeGroup->addAction(rs1);
        rangeGroup->addAction(rs2);

        menu->addMenu(menu2);



        QAction *menuAC=menu->menuAction();
        addAction(menuAC);
        connect(menuAC,&QAction::triggered,this,[=](){

            H_MotionController::GetInstance()->GetAxis(0)->TestFunc();

//            MotionCompiler::GetInstance()->createGcodeAndRun("X","Y","Z",
//                                                             rangeGroup->actions().indexOf(rangeGroup->checkedAction()));


//            HPainterPaths paths;
//            for(const auto&k:ProgramHandler::GetInstance()->getActiveProgram()->getData()){
//                for(const auto&m:k.getItems()){
//                    paths<<m->getScenePath();
//                }
//            }
//            HScannerController::GetInstance()->GetScanner(0)->SetJumpVel(200);
//            HScannerController::GetInstance()->GetScanner(0)->SetMarkVel(200);
//            HScannerController::GetInstance()->GetScanner(0)->SetLaserPrf(50);
//            HScannerController::GetInstance()->GetScanner(0)->SetLaserPulseWidth(2);
//            HScannerController::GetInstance()->GetScanner(0)->MarkShapesPtsAndWait(paths);


        });

        addAction(QIcon(":/resource/icons/stop.svg"),tr("stop"),this,[=](){
            MotionCompiler::GetInstance()->StopProgram();

        });

        addSeparator();
        addAction(QIcon(":/resource/icons/home.svg"),tr("home"),this,[=](){
            H_MotionController::GetInstance()->HomeAllMotion();
        });

        addSeparator();
        addSeparator();


        addAction(QStringLiteral("上下料"),this,[=](){
             QSettings settings("QtApp", "inno");
             QPointF pt=QPointF(settings.value("CustomPos/x1").toString().toDouble(),settings.value("CustomPos/y1").toString().toDouble());
             auto x=H_MotionController::GetInstance()->GetAxis("X");
             auto y=H_MotionController::GetInstance()->GetAxis("Y");
             if(x&&y){
                  x->MoveTo(pt.x());
                  y->MoveTo(pt.y());
             }


        });

        addAction(QStringLiteral("裂片"),this,[=](){
            QSettings settings("QtApp", "inno");
            QPointF pt=QPointF(settings.value("CustomPos/x2").toString().toDouble(),settings.value("CustomPos/y2").toString().toDouble());
            auto x=H_MotionController::GetInstance()->GetAxis("X");
            auto y=H_MotionController::GetInstance()->GetAxis("Y");
            if(x&&y){
                 x->MoveTo(pt.x());
                 y->MoveTo(pt.y());
            }

        });

        addAction(QStringLiteral("位置1"),this,[=](){
            QSettings settings("QtApp", "inno");
            QPointF pt=QPointF(settings.value("CustomPos/x3").toString().toDouble(),settings.value("CustomPos/y3").toString().toDouble());
            auto x=H_MotionController::GetInstance()->GetAxis("X");
            auto y=H_MotionController::GetInstance()->GetAxis("Y");
            if(x&&y){
                 x->MoveTo(pt.x());
                 y->MoveTo(pt.y());
            }

        });
        addSeparator();

        addAction(QStringLiteral("位置2"),this,[=](){
            QSettings settings("QtApp", "inno");
            QPointF pt=QPointF(settings.value("CustomPos/xx4").toString().toDouble(),settings.value("CustomPos/yy4").toString().toDouble());
            auto x=H_MotionController::GetInstance()->GetAxis("X");
            auto y=H_MotionController::GetInstance()->GetAxis("Y");
            if(x&&y){
                 x->MoveTo(pt.x());
                 y->MoveTo(pt.y());
            }

        });

        addAction(QStringLiteral("位置3"),this,[=](){
            QSettings settings("QtApp", "inno");
            QPointF pt=QPointF(settings.value("CustomPos/x5").toString().toDouble(),settings.value("CustomPos/y5").toString().toDouble());
            auto x=H_MotionController::GetInstance()->GetAxis("X");
            auto y=H_MotionController::GetInstance()->GetAxis("Y");
            if(x&&y){
                 x->MoveTo(pt.x());
                 y->MoveTo(pt.y());
            }

        });



    }
        break;
    case HToolBars::CONTROL_TOOLS:
    {

        setIconSize(QSize(30, 30));

    }
        break;
    case HToolBars::OTHERS_TOOLS:
    {

    }
        break;
    }




    movableAction = new QAction(tr("Movable"), this);
    movableAction->setCheckable(true);
    connect(movableAction, &QAction::triggered, this, &ToolBar::changeMovable);

    allowedAreasActions = new QActionGroup(this);
    allowedAreasActions->setExclusive(false);

    allowLeftAction = new QAction(tr("Allow on Left"), this);
    allowLeftAction->setCheckable(true);
    connect(allowLeftAction, &QAction::triggered, this, &ToolBar::allowLeft);

    allowRightAction = new QAction(tr("Allow on Right"), this);
    allowRightAction->setCheckable(true);
    connect(allowRightAction, &QAction::triggered, this, &ToolBar::allowRight);

    allowTopAction = new QAction(tr("Allow on Top"), this);
    allowTopAction->setCheckable(true);
    connect(allowTopAction, &QAction::triggered, this, &ToolBar::allowTop);

    allowBottomAction = new QAction(tr("Allow on Bottom"), this);
    allowBottomAction->setCheckable(true);
    connect(allowBottomAction, &QAction::triggered, this, &ToolBar::allowBottom);

    allowedAreasActions->addAction(allowLeftAction);
    allowedAreasActions->addAction(allowRightAction);
    allowedAreasActions->addAction(allowTopAction);
    allowedAreasActions->addAction(allowBottomAction);

    areaActions = new QActionGroup(this);
    areaActions->setExclusive(true);

    leftAction = new QAction(tr("Place on Left") , this);
    leftAction->setCheckable(true);
    connect(leftAction, &QAction::triggered, this, &ToolBar::placeLeft);

    rightAction = new QAction(tr("Place on Right") , this);
    rightAction->setCheckable(true);
    connect(rightAction, &QAction::triggered, this, &ToolBar::placeRight);

    topAction = new QAction(tr("Place on Top") , this);
    topAction->setCheckable(true);
    connect(topAction, &QAction::triggered, this, &ToolBar::placeTop);

    bottomAction = new QAction(tr("Place on Bottom") , this);
    bottomAction->setCheckable(true);
    connect(bottomAction, &QAction::triggered, this, &ToolBar::placeBottom);

    areaActions->addAction(leftAction);
    areaActions->addAction(rightAction);
    areaActions->addAction(topAction);
    areaActions->addAction(bottomAction);

    connect(movableAction, &QAction::triggered, areaActions, &QActionGroup::setEnabled);

    connect(movableAction, &QAction::triggered, allowedAreasActions, &QActionGroup::setEnabled);

    menu = new QMenu(title, this);
    menu->setStyleSheet("background-color:rgb(180,180,180);selection-background-color:rgb(123, 123, 255);border:0px");
    menu->addAction(toggleViewAction());


    menu->addSeparator();
    menu->addAction(movableAction);
    menu->addSeparator();
    menu->addActions(allowedAreasActions->actions());
    menu->addSeparator();
    menu->addActions(areaActions->actions());
    menu->addSeparator();
    menu->addAction(tr("Insert break"), this, &ToolBar::insertToolBarBreak);

    connect(menu, &QMenu::aboutToShow, this, &ToolBar::updateMenu);



}

ToolBar::~ToolBar()
{

}

void ToolBar::updateMenu()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
    Q_ASSERT(mainWindow != nullptr);

    const Qt::ToolBarArea area = mainWindow->toolBarArea(this);
    const Qt::ToolBarAreas areas = allowedAreas();

    movableAction->setChecked(isMovable());

    allowLeftAction->setChecked(isAreaAllowed(Qt::LeftToolBarArea));
    allowRightAction->setChecked(isAreaAllowed(Qt::RightToolBarArea));
    allowTopAction->setChecked(isAreaAllowed(Qt::TopToolBarArea));
    allowBottomAction->setChecked(isAreaAllowed(Qt::BottomToolBarArea));

    if (allowedAreasActions->isEnabled()) {
        allowLeftAction->setEnabled(area != Qt::LeftToolBarArea);
        allowRightAction->setEnabled(area != Qt::RightToolBarArea);
        allowTopAction->setEnabled(area != Qt::TopToolBarArea);
        allowBottomAction->setEnabled(area != Qt::BottomToolBarArea);
    }

    leftAction->setChecked(area == Qt::LeftToolBarArea);
    rightAction->setChecked(area == Qt::RightToolBarArea);
    topAction->setChecked(area == Qt::TopToolBarArea);
    bottomAction->setChecked(area == Qt::BottomToolBarArea);

    if (areaActions->isEnabled()) {
        leftAction->setEnabled(areas & Qt::LeftToolBarArea);
        rightAction->setEnabled(areas & Qt::RightToolBarArea);
        topAction->setEnabled(areas & Qt::TopToolBarArea);
        bottomAction->setEnabled(areas & Qt::BottomToolBarArea);
    }
}



void ToolBar::allow(Qt::ToolBarArea area, bool a)
{
    Qt::ToolBarAreas areas = allowedAreas();
    areas = a ? areas | area : areas & ~area;
    setAllowedAreas(areas);

    if (areaActions->isEnabled()) {
        leftAction->setEnabled(areas & Qt::LeftToolBarArea);
        rightAction->setEnabled(areas & Qt::RightToolBarArea);
        topAction->setEnabled(areas & Qt::TopToolBarArea);
        bottomAction->setEnabled(areas & Qt::BottomToolBarArea);
    }
}

void ToolBar::place(Qt::ToolBarArea area, bool p)
{
    if (!p)
        return;
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
    Q_ASSERT(mainWindow != nullptr);

    mainWindow->addToolBar(area, this);

    if (allowedAreasActions->isEnabled()) {
        allowLeftAction->setEnabled(area != Qt::LeftToolBarArea);
        allowRightAction->setEnabled(area != Qt::RightToolBarArea);
        allowTopAction->setEnabled(area != Qt::TopToolBarArea);
        allowBottomAction->setEnabled(area != Qt::BottomToolBarArea);
    }
}

void ToolBar::changeMovable(bool movable)
{ setMovable(movable); }

void ToolBar::allowLeft(bool a)
{ allow(Qt::LeftToolBarArea, a); }

void ToolBar::allowRight(bool a)
{ allow(Qt::RightToolBarArea, a); }

void ToolBar::allowTop(bool a)
{ allow(Qt::TopToolBarArea, a); }

void ToolBar::allowBottom(bool a)
{ allow(Qt::BottomToolBarArea, a); }

void ToolBar::placeLeft(bool p)
{ place(Qt::LeftToolBarArea, p); }

void ToolBar::placeRight(bool p)
{ place(Qt::RightToolBarArea, p); }

void ToolBar::placeTop(bool p)
{ place(Qt::TopToolBarArea, p); }

void ToolBar::placeBottom(bool p)
{ place(Qt::BottomToolBarArea, p); }

void ToolBar::insertToolBarBreak()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
    Q_ASSERT(mainWindow != nullptr);

    mainWindow->insertToolBarBreak(this);
}




