#include "hmainmenubar.h"
#include "hcommon.h"


#include "modbusMaster/hmodbusmainwindow.h"
#include "modbusSlave/hmodbusslavemainwindow.h"
#include "Src/data/inisettings.h"

#include "Src/Gui/tools/hstartextapp.h"

#include "Src/data/hconfigwidget.h"


#include "ChartShow/hchartshowmainwindow.h"
#include "diagramscene/hflowviewmainwindow.h"

#include "Src/Gui/settings/hdialogsystemadjust.h"
#include "Src/Gui/settings/hdialogparameterlibrary.h"
#include "Src/Gui/settings/deviceWidget/hdevicewidget.h"

#include "Src/Gui/tools/hfindfocusdialog.h"

#include "custompositiondialog.h"

#include <QActionGroup>
#include <QMessageBox>

HMainMenuBar* HMainMenuBar::m_pInstance = nullptr;

HMainMenuBar *HMainMenuBar::GetInstance(QMenuBar *pmenubar, QObject *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HMainMenuBar(pmenubar,parent);
    }
    return m_pInstance;
}

QAction *HMainMenuBar::getAction(qint32 index)
{
    return m_actions.value(index);
}


HMainMenuBar::HMainMenuBar(QMenuBar *pmenubar, QObject *parent):QObject (parent)
{

    //pmenubar->setStyleSheet(HSheetStyle::H_MENU);
    ptrIni=IniSettings::GetInstance();

    setupMenuFile(pmenubar);
    setupMenuUser(pmenubar);
    setupMenuEdit(pmenubar);
    setupMenuTool(pmenubar);
    setupMenuSetting(pmenubar);


    //    HModBusSlaveMainWindow *modbusslave=new HModBusSlaveMainWindow(pmenubar);

    //    HChartShowMainWindow* chartshow=new HChartShowMainWindow(pmenubar);



    HFlowViewMainWindow *flowView=new HFlowViewMainWindow(pmenubar);
    HFindFocusDialog *findFocus=new HFindFocusDialog(pmenubar);


    HStartExtApp *exteralApp=HStartExtApp::GetInstance(this);

    //connect(action_tool.at(MENU_TOOL::chart_show),&QAction::triggered,chartshow,&HChartShowMainWindow::show);
    connect(m_actions.value(Tool_flowView),&QAction::triggered,flowView,&HFlowViewMainWindow::show);
    connect(m_actions.value(Tool_aocLaser),&QAction::triggered,exteralApp,&HStartExtApp::StartAOCLaser);
    connect(m_actions.value(Tool_findFocus),&QAction::triggered,findFocus,&HFindFocusDialog::show);


    // connect(action_setting.at(MENU_SETTING::modbus_setting),&QAction::triggered,HModBusMainWindow::GetInstance(),&HModBusMainWindow::show);
    //connect(action_setting.at(MENU_SETTING::modbusslave_setting),&QAction::triggered,modbusslave,&HModBusSlaveMainWindow::show);

    connect(m_actions.value(Setting_inputOutput),&QAction::triggered,new HDeviceWidget(pmenubar),&HDeviceWidget::show);

    HDialogSystemAdjust *adjust=new HDialogSystemAdjust(pmenubar);
    connect(m_actions.value(Setting_systemAdjust),&QAction::triggered,adjust,&HDialogSystemAdjust::show);

    HDialogParameterLibrary *paraLib=new HDialogParameterLibrary(pmenubar);
    connect(m_actions.value(Setting_parameterLibrary),&QAction::triggered,paraLib,&HDialogParameterLibrary::show);


    CustomPositionDialog *ppppppp=new CustomPositionDialog(pmenubar);
    connect(m_actions.value(Setting_Postion),&QAction::triggered,ppppppp,&CustomPositionDialog::show);

}



void HMainMenuBar::setupMenuAbout(QMenuBar *pmenubar)
{
    QMenu * inory=pmenubar->addMenu(tr("About"));

    m_actions[About_help]=inory->addAction(tr("Help"));
    inory->addSeparator();
    m_actions[About_support]=inory->addAction(tr("Technical Support"));

    m_actions[About_report]=inory->addAction(tr("Report Bug"));

    inory->addSeparator();
    QMenu *menu=new QMenu(tr("Language"),pmenubar);
    QActionGroup *languageGroup = new QActionGroup(this);;

    languageGroup->addAction(menu->addAction("English"));
    languageGroup->addAction(menu->addAction(QStringLiteral("简体中文")));
    languageGroup->addAction(menu->addAction(QStringLiteral("日本語")));
    languageGroup->addAction(menu->addAction(QStringLiteral("Deutsch")));


    QStringList ll1,ll2,ll3;
    ll1<<"note"<<QStringLiteral("提示")<<QStringLiteral("プロンプト")<<QStringLiteral("Nachricht");
    ll2<<"Please restart the software to make the change effective."<<QStringLiteral("请重启软件使更改生效！")<<QStringLiteral("ソフトウェアを再起動して下さい")
      <<QStringLiteral("Bitte erneuern sie die software für die änderung");
    ll3<<"OK"<<QStringLiteral("确定")<<QStringLiteral("おーけー")<<QStringLiteral("Okay");

    for(qint32 k=0;k<languageGroup->actions().size();++k){
        connect(languageGroup->actions().at(k),&QAction::triggered,this,[=](){
            if(IniSettings::GetInstance()->getValue(IniSettings::View_Language).toInt()!=k)
                QMessageBox::information(pmenubar,ll1.at(k),ll2.at(k),ll3.at(k));
            IniSettings::GetInstance()->setValue(IniSettings::View_Language,k);
        });
        languageGroup->actions().at(k)->setCheckable(true);

    }

    languageGroup->actions().at(IniSettings::GetInstance()->getValue(IniSettings::View_Language).toInt())->setChecked(true);


    inory->addAction(menu->menuAction());
    m_actions[About_language]=menu->menuAction();
    m_actions[About_about]=inory->addAction(tr("About H-Laser"));

}



void HMainMenuBar::setupMenuUser(QMenuBar *pmenubar)
{
    QMenu * inory=pmenubar->addMenu(tr("User"));

    m_actions[User_login]=inory->addAction(tr("Login"));
    m_actions[User_logout]=inory->addAction(tr("Logout"));
    inory->addSeparator();
    m_actions[User_manage]=inory->addAction(tr("Manage User"));
}

void HMainMenuBar::setupMenuFile(QMenuBar *pmenubar)
{
    QMenu * inory=pmenubar->addMenu(tr("File"));
    m_actions[File_file]=inory->menuAction();
    m_actions[File_new]=inory->addAction(tr("New Program"));


    m_actions[File_open]=inory->addAction(tr("Open Program"));
    m_actions[File_save]=inory->addAction(tr("Save Program"));
    m_actions[File_saveAs]=inory->addAction(tr("Save Program as..."));
    inory->addSeparator();
    m_actions[File_imdxf]=inory->addAction(tr("Import DXF File"));
    m_actions[File_imdxf]->setEnabled(false);


    inory->addSeparator();
    m_actions[File_exit]=inory->addAction(tr("Exit"),this,[=](){
        qobject_cast<QMainWindow*>(this->parent())->close();
    });
}

void HMainMenuBar::setupMenuEdit(QMenuBar *pmenubar)
{
    QMenu * inory=pmenubar->addMenu(tr("Edit"));

    QMenu* mode= inory->addMenu(tr("Mode"));
    m_actions[Edit_mode]=mode->menuAction();

    QActionGroup *modeGroup=new QActionGroup(this);
    m_actions[Edit_mode_cutter]=mode->addAction(tr("cutter"));
    m_actions[Edit_mode_cutter]->setCheckable(true);
    m_actions[Edit_mode_scanner]=mode->addAction(tr("scanner"));
    m_actions[Edit_mode_scanner]->setCheckable(true);
    modeGroup->addAction(m_actions[Edit_mode_cutter]);
    modeGroup->addAction(m_actions[Edit_mode_scanner]);
    auto ac=modeGroup->actions().value(ptrIni->getValue(IniSettings::Edit_mode).toInt());
    if(ac){
        ac->setChecked(true);
    }else{
        m_actions[Edit_mode_cutter]->setChecked(true);
        ptrIni->setValue(IniSettings::Edit_mode,0);
    }
    connect(modeGroup,&QActionGroup::triggered,this,[=](QAction *action){ptrIni->setValue(IniSettings::Edit_mode,modeGroup->actions().indexOf(action));});
    //    inory->addSeparator();
    //    action_edit<<inory->addAction(tr("&Remove"));
    //    action_edit<<inory->addAction(tr("&Cut"));
    //    action_edit<<inory->addAction(tr("&Copy"));
    //    action_edit<<inory->addAction(tr("&Paste"));
    //    inory->addSeparator();
    //    action_edit<<inory->addAction(tr("&Select All"));

}

void HMainMenuBar::setupMenuTool(QMenuBar *pmenubar)
{
    QMenu * inory=pmenubar->addMenu(tr("Tool"));
    m_actions[Tool_chartShow]=inory->addAction(tr("Chart Show"));
    m_actions[Tool_flowView]=inory->addAction(tr("Flow View"));
    inory->addSeparator();
    m_actions[Tool_aocLaser]=inory->addAction(tr("AOC Laser"));
    m_actions[Tool_findFocus]=inory->addAction(tr("find focus"));
}

void HMainMenuBar::setupMenuSetting(QMenuBar *pmenubar)
{

    QMenu * inory=pmenubar->addMenu(tr("Setting"));


    inory->addAction(tr("System Setting"),HConfigWidget::GetInstance(pmenubar),&HConfigWidget::show);




    QMenu *modbus=new QMenu(tr("ModBus"),pmenubar);
    m_actions[Setting_modbus]=modbus->menuAction();
    m_actions[Setting_modbusClient]=modbus->addAction(tr("modbus client"));
    m_actions[Setting_modbusServer]=modbus->addAction(tr("modbus server"));
    inory->addAction(modbus->menuAction());

    m_actions[Setting_inputOutput]=inory->addAction(tr("input and output"));
    inory->addSeparator();

    m_actions[Setting_systemAdjust]=inory->addAction(tr("system adjust"));
    m_actions[Setting_parameterLibrary]=inory->addAction(tr("parameter library"));
    m_actions[Setting_ImgProcess]=inory->addAction(tr("image process"));
    m_actions[Setting_AxisAdjust]=inory->addAction(tr("axis adjust"));
    m_actions[Setting_Postion]=inory->addAction(tr("position setting"));

}


