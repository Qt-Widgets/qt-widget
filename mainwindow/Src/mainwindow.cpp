#include "mainwindow.h"
#include "hcommon.h"
#include <QAction>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QFile>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QProcess>
#include <QSettings>
#include <QSystemTrayIcon>


#include "Src/User/dialogaccountui.h"
#include "Src/User/h_authority.h"


#include "Src/main-menu/hmainmenubar.h"
#include "Src/painter/paintermainwidget.h"
#include "Src/Gui/itemlist.h"
#include "Src/Gui/h_cameracontrollerwidget.h"
#include "Src/Gui/hmotionui.h"
#include "Src/Gui/h_processwidget.h"
#include "modbusMaster/hmodbusmainwindow.h"



#include "Src/window/programhandler.h"
#include "interface/hinterfacecontroller.h"

#include "toolBar/hmaintoolbarwid.h"
#include "Src/Motion/h_motioncontroller.h"




Q_DECLARE_METATYPE(QDockWidget::DockWidgetFeatures)

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{

    //chenck process uniqueness
    ////////////////////////////////////////////////////////////////////////////////
    //    QProcess process;
    //    process.start("tasklist.exe");
    //    if(process.waitForFinished())
    //    {
    //        QStringList list=QString(process.readAll()).split("\r\n");
    //        int kg=0;
    //        foreach(QString s,list)
    //            if(s.contains("HPSCSWL.exe",Qt::CaseInsensitive)){
    //                ++kg;
    //            }
    //        //qDebug()<<kg;
    //        if(kg>1){
    //            QMessageBox::warning(nullptr,"warning","Application is running!");
    //            exit(-1);
    //        }

    //    }
    ////////////////////////////////////////////////////////////////////////////////
    /// \brief setFont

    //    QIcon icon = QIcon(":/resource/icons/stop.svg");
    //    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    //    trayIcon->setIcon(icon);
    //    trayIcon->setToolTip("a trayicon example");
    //    trayIcon->show();
    //    minimizeAction = new QAction("min", this);
    //    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    //    maximizeAction = new QAction("max~", this);
    //    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
    //    restoreAction = new QAction("normal", this);
    //    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    //    quitAction = new QAction("exit", this);
    //    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    //    trayIconMenu = new QMenu(this);
    //    trayIconMenu->addAction(minimizeAction);
    //    trayIconMenu->addAction(maximizeAction);
    //    trayIconMenu->addAction(restoreAction);
    //    trayIconMenu->addSeparator();
    //    trayIconMenu->addAction(quitAction);
    //    trayIcon->setContextMenu(trayIconMenu);
    //    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));


    setFont(MyFont1);
    //    QTimer *timer = new QTimer(this);
    //    timer->setInterval(100);
    //    connect(timer, SIGNAL(timeout()), this, SLOT(changeStatus()));
    //    timer->start();



    setObjectName("MainWindow");
    setWindowState(Qt::WindowState::WindowActive);

    HModBusMainWindow::GetInstance(this);
    ProgramHandler::GetInstance(this);
    MotionCompiler::GetInstance(this);

    setupToolBar();
    setupMenuBar();

    setupDockWidgets();
    setCentralWidget(PainterMainWidget::GetInstance(this));

    ProgramHandler::GetInstance()->Init();
    HInterfaceController::GetInstance(this)->Init();


    connect(HMainMenuBar::GetInstance()->getAction(HMainMenuBar::User_manage),&QAction::triggered,this,&MainWindow::openUserDialog);


    //connect(ProgramHandler::GetInstance(),&ProgramHandler::programFileAdded,this,&MainWindow::showMaximized);


    //    statusBar()->setStyleSheet("color:rgb(168,255,255);background-color:rgb(70,70,70);selection-color:rgb(255, 0, 0);");
    //    statusBar()->showMessage(tr("Ready"));

    loadSettings();

    QTimer::singleShot(400, this,&MainWindow::loadSettings);

}



void MainWindow::openUserDialog()
{
    DialogAccountUI*user=new DialogAccountUI(1,H_Authority::GetInstance()->getAuthority());
    user->exec();
}

void MainWindow::changeStatus()
{
    QString str;

    switch(H_Authority::GetInstance()->getAuthority()){
    case 0:
        str=tr("(operater)");
        break;
    case 1:
        str=tr("(expert)");
        break;
    case 2:
        str=tr("(administrator)");
        break;
    }
    setWindowTitle(tr("AOC Laser Cutting System")+str);

}



void MainWindow::setupToolBar()
{

    // setMenuWidget(new HMainToolBarWid(this));

    // addToolBar(new HToolBar(this));
    toolBars.append(new ToolBar(tr("Program"),HToolBars::PROGRAM_TOOLS, this));
    addToolBar(toolBars.at(0));

    toolBars.append(new ToolBar(tr("Control"),HToolBars::CONTROL_TOOLS, this));
    addToolBar(toolBars.at(1));
}



void MainWindow::setupMenuBar()
{
    HMainMenuBar * inory=HMainMenuBar::GetInstance(menuBar(),this);

    mainWindowMenu = menuBar()->addMenu(tr("View"));

    QAction *action = mainWindowMenu->addAction(tr("Animated docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AnimatedDocks);
    ptrActions_view.append(action);

    connect(action, &QAction::toggled, this, &MainWindow::setDockOptions);

    action = mainWindowMenu->addAction(tr("Allow nested docks"));
    action->setCheckable(true);
    ptrActions_view.append(action);
    action->setChecked(dockOptions() & AllowNestedDocks);

    connect(action, &QAction::toggled, this, &MainWindow::setDockOptions);

    action = mainWindowMenu->addAction(tr("Allow tabbed docks"));
    action->setCheckable(true);
    ptrActions_view.append(action);
    action->setChecked(dockOptions() & AllowTabbedDocks);
    connect(action, &QAction::toggled, this, &MainWindow::setDockOptions);


    action = mainWindowMenu->addAction(tr("Force tabbed docks"));
    action->setCheckable(true);
    ptrActions_view.append(action);
    action->setChecked(dockOptions() & ForceTabbedDocks);
    connect(action, &QAction::toggled, this, &MainWindow::setDockOptions);

    action = mainWindowMenu->addAction(tr("Vertical tabs"));
    action->setCheckable(true);
    ptrActions_view.append(action);
    action->setChecked(dockOptions() & VerticalTabs);
    connect(action, &QAction::toggled, this, &MainWindow::setDockOptions);

    action = mainWindowMenu->addAction(tr("Grouped dragging"));
    action->setCheckable(true);
    ptrActions_view.append(action);
    action->setChecked(dockOptions() & GroupedDragging);
    connect(action, &QAction::toggled, this, &MainWindow::setDockOptions);

    mainWindowMenu->addSeparator();

    action=mainWindowMenu->addAction(tr("Switch layout direction"),this, [=](){
        if (layoutDirection() == Qt::LeftToRight)
            QApplication::setLayoutDirection(Qt::RightToLeft);
        else
            QApplication::setLayoutDirection(Qt::LeftToRight);
    });
    action->setCheckable(true);
    ptrActions_view.append(action);

    mainWindowMenu->addSeparator();

    mainWindowMenu->addAction(tr("Save layout as..."), this, &MainWindow::saveSettingsAsFile);
    mainWindowMenu->addAction(tr("Load layout from..."), this, &MainWindow::loadSettingsFromFile);
    mainWindowMenu->addAction(tr("Rset layout"), this, &MainWindow::resetSettings);

    mainWindowMenu->addSeparator();

    //    QMenu * hm=mainWindowMenu->addMenu(tr("ToolBars"));

    //    for (int i = 0; i < toolBars.count(); ++i){
    //        hm->addMenu(toolBars.at(i)->toolbarMenu());
    //        toolBars.at(i)->toolbarMenu()->setStyleSheet(HSheetStyle::H_MENU);
    //    }
    mainWindowMenu->addSeparator();
    inory->setupMenuAbout(menuBar());


    //    menuBar()->setStyleSheet( "\
    //                         QMenu {\
    //                         background-color:rgb(89,87,87); /*整个背景*/\
    //                         border: 3px solid rgb(235,110,36);/*整个菜单边缘*/\
    //                         }\
    //                     QMenu::item {\
    //                         font-size: 10pt; \
    //                         color: rgb(225,225,225);  /*字体颜色*/\
    //                         border: 3px solid rgb(60,60,60);    /*item选框*/\
    //                         background-color:rgb(89,87,87);\
    //                         padding:4px 4px; /*设置菜单项文字上下和左右的内边距，效果就是菜单中的条目左右上下有了间隔*/\
    //                         margin:2px 2px;/*设置菜单项的外边距*/\
    //                          }\
    //                     QMenu::item:selected { \
    //                         background-color:rgb(235,110,36);/*选中的样式*/\
    //                         }\
    //                     QMenu::item:pressed {/*菜单项按下效果*/\
    //                                          border: 1px solid rgb(60,60,61); \
    //                                               background-color: rgb(220,80,6); \
    //                                           }\
    //                        ");

}

void MainWindow::setDockOptions()
{
    DockOptions opts;
    QList<QAction*> actions = mainWindowMenu->actions();

    if (actions.at(0)->isChecked())
        opts |= AnimatedDocks;
    if (actions.at(1)->isChecked())
        opts |= AllowNestedDocks;
    if (actions.at(2)->isChecked())
        opts |= AllowTabbedDocks;
    if (actions.at(3)->isChecked())
        opts |= ForceTabbedDocks;
    if (actions.at(4)->isChecked())
        opts |= VerticalTabs;
    if (actions.at(5)->isChecked())
        opts |= GroupedDragging;

    QMainWindow::setDockOptions(opts);
}



void MainWindow::saveSettings()
{
    //QString fileName= QFileDialog::getSaveFileName(this, tr("Save layout"));

    QSettings settings("QtApp","inno");
    settings.setValue("Window/geometry", saveGeometry());
    settings.setValue("Window/windowState", saveState());

    QByteArray km;

    int count =ptrActions_view.size();
    for(int k=0;k!=count;++k){
        km.append((uchar)ptrActions_view.at(k)->isChecked());
    }

    settings.setValue("View/checkedState", km);

}

void MainWindow::saveSettingsAsFile()
{
    QString filePath= QFileDialog::getSaveFileName(this, tr("Save layout"));
    if (filePath.isEmpty())
        return;
    //QString fileName=QCoreApplication::applicationDirPath()+"user";
    if(filePath.isEmpty())
        return;
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly)){
        QMessageBox::warning(this, tr("Error"),tr("Failed to open %1\n%2").arg(QDir::toNativeSeparators(filePath), file.errorString()));
        return;
    }

    QByteArray geo_data = saveGeometry();
    QByteArray layout_data = saveState();

    if (file.putChar((uchar)geo_data.size()))
        if(file.write(geo_data) == geo_data.size())
            if(file.write(layout_data) == layout_data.size()){
                file.close();
                return;
            }
    QMessageBox::warning(this, tr("Error"), tr("Error writing to %1\n%2").arg(QDir::toNativeSeparators(filePath), file.errorString()));
    file.close();

}

void MainWindow::loadSettings()
{
    QSettings settings("QtApp", "inno");
    restoreGeometry(settings.value("Window/geometry").toByteArray());
    restoreState(settings.value("Window/windowState").toByteArray());

    //    static QByteArray jk1,jk2;
    //    if(jk1.isEmpty()){
    //        jk1= settings.value("Window/geometry").toByteArray();
    //         jk2= settings.value("Window/windowState").toByteArray();
    //    }else{
    //        qDebug()<<(jk1==settings.value("Window/geometry").toByteArray())<<(jk2==settings.value("Window/windowState").toByteArray());
    //    }

    QByteArray km=settings.value("View/checkedState").toByteArray();

    int count =ptrActions_view.size();
    if(count==km.size())
        for(int k=0;k<count;++k){
            ptrActions_view.at(k)->setChecked((bool)km.at(k));
        }
}

void MainWindow::resetSettings()
{
    QString fileName=QCoreApplication::applicationDirPath()+"/defaultView";
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to open %1\n%2").arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    uchar geo_size ;
    QByteArray geo_data,layout_data;

    if(file.getChar((char*)&geo_size)){
        geo_data=file.read(geo_size);
        if(geo_data.size() == geo_size){
            layout_data=file.readAll();
            if(restoreGeometry(geo_data))
                if(restoreState(layout_data)){
                    file.close();
                    return;
                }
        }
    }

    QMessageBox::warning(this, tr("Error"), tr("Error reading %1").arg(QDir::toNativeSeparators(fileName)));
    file.close();
    return;
}

void MainWindow::loadSettingsFromFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Load layout"));
    if (filePath.isEmpty())
        return;
    // QString fileName=QCoreApplication::applicationDirPath()+"user";
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly)){
        QMessageBox::warning(this, tr("Error"), tr("Failed to open %1\n%2").arg(QDir::toNativeSeparators(filePath), file.errorString()));
        return;
    }

    uchar geo_size ;
    QByteArray geo_data,layout_data;

    if(file.getChar((char*)&geo_size)){
        geo_data=file.read(geo_size);
        if(geo_data.size() == geo_size){
            layout_data=file.readAll();
            if(restoreGeometry(geo_data))
                if(restoreState(layout_data)){
                    file.close();
                    return;
                }
        }
    }
    QMessageBox::warning(this, tr("Error"), tr("Error reading %1").arg(QDir::toNativeSeparators(filePath)));
    file.close();
    return;
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    QMessageBox::StandardButton button;
    button=QMessageBox::question(this,tr("Warning"),tr("Are You Sure to Exit ?"),QMessageBox::Yes|QMessageBox::No);
    if(button==QMessageBox::No)
    {
        event->ignore();
    }
    else if(button==QMessageBox::Yes)
    {
        //        if(!programFilePath.isNull())
        //        {
        //            settings.setValue("HPLPS/defaultProgramFile", programFilePath);
        //        }

        //saveLayout();

        //        QEventLoop eventloop;
        //        QTimer::singleShot(500, &eventloop, SLOT(quit())); //wait 0.5s
        //        eventloop.exec();




        saveSettings();

        event->accept();
    }
}


QAction * MainWindow::addCornerAction(const QString &text, QMainWindow *mw, QMenu *menu, QActionGroup *group,
                                      Qt::Corner c, Qt::DockWidgetArea a)
{
    QAction *result = menu->addAction(text, mw, [=]() { mw->setCorner(c, a); });
    result->setCheckable(true);
    group->addAction(result);
    ptrActions_view.append(result);
    return result;
}

struct Set {

    Set(QString m_name="",QWidget* m_widget=nullptr,Qt::DockWidgetArea m_area=Qt::LeftDockWidgetArea){
        name=m_name;
        widget=m_widget;
        area=m_area;
    }

    QString name;
    QWidget* widget;
    Qt::DockWidgetArea area;
};

void MainWindow::setupDockWidgets()
{
    qRegisterMetaType<QDockWidget::DockWidgetFeatures>();


    QVector<Set> WidSet;

    WidSet<<Set(tr("Item List"),ItemList::GetInstance(this), Qt::LeftDockWidgetArea );

    WidSet<<Set(tr("Laser Process Parameters"),  H_ProcessWidget::GetInstance(this), Qt::BottomDockWidgetArea);


    WidSet<<Set(tr("Camera Viewer"), H_CameraControllerWidget::GetInstance(this), Qt::RightDockWidgetArea);



    WidSet<<Set(tr("Motion Controller"), new HMotionUI(this), Qt::BottomDockWidgetArea);

    QMenu *kh=mainWindowMenu->addMenu(tr("Windows"));

    for (int i = 0; i < WidSet.size(); ++i)
    {
        swatchArray.append(new ColorSwatch(WidSet[i].name,WidSet[i].widget,this, Qt::WindowFlags(Qt::Sheet)));

        addDockWidget(WidSet[i].area, swatchArray.last());
        swatchArray.last()->colorSwatchMenu()->setStyleSheet(HSheetStyle::H_MENU);
        kh->addMenu(swatchArray.last()->colorSwatchMenu());
    }
    mainWindowMenu->addSeparator();

    QMenu *cornerMenu = mainWindowMenu->addMenu(tr("Top left corner"));
    QActionGroup *group = new QActionGroup(this);
    group->setExclusive(true);
    QAction *cornerAction = addCornerAction(tr("Top dock area"), this, cornerMenu, group, Qt::TopLeftCorner, Qt::TopDockWidgetArea);
    cornerAction->setChecked(true);
    addCornerAction(tr("Left dock area"), this, cornerMenu, group, Qt::TopLeftCorner, Qt::LeftDockWidgetArea);

    cornerMenu = mainWindowMenu->addMenu(tr("Top right corner"));
    group = new QActionGroup(this);
    group->setExclusive(true);
    cornerAction = addCornerAction(tr("Top dock area"), this, cornerMenu, group, Qt::TopRightCorner, Qt::TopDockWidgetArea);
    cornerAction->setChecked(true);
    addCornerAction(tr("Right dock area"), this, cornerMenu, group, Qt::TopRightCorner, Qt::RightDockWidgetArea);

    cornerMenu = mainWindowMenu->addMenu(tr("Bottom left corner"));
    group = new QActionGroup(this);
    group->setExclusive(true);
    cornerAction = addCornerAction(tr("Bottom dock area"), this, cornerMenu, group, Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
    cornerAction->setChecked(true);
    addCornerAction(tr("Left dock area"), this, cornerMenu, group, Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

    cornerMenu = mainWindowMenu->addMenu(tr("Bottom right corner"));
    group = new QActionGroup(this);
    group->setExclusive(true);
    cornerAction = addCornerAction(tr("Bottom dock area"), this, cornerMenu, group, Qt::BottomRightCorner, Qt::BottomDockWidgetArea);
    cornerAction->setChecked(true);
    addCornerAction(tr("Right dock area"), this, cornerMenu, group, Qt::BottomRightCorner, Qt::RightDockWidgetArea);


}







