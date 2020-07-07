#ifndef HMAINMENUBAR_H
#define HMAINMENUBAR_H

#include <QMenuBar>
#include <QList>


class IniSettings;
class HMainMenuBar:public QObject
{
    Q_OBJECT
public:

    enum Actions{

        User_login,
        User_logout,
        User_manage,

        File_file,
        File_new,
        File_open,
        File_save,
        File_saveAs,
        File_imdxf,
        File_exit,

        Edit_mode,
        Edit_mode_cutter,
        Edit_mode_scanner,

        Tool_chartShow,
        Tool_flowView,
        Tool_aocLaser,
        Tool_findFocus,

        Setting_modbus,
        Setting_modbusClient,
        Setting_modbusServer,
        Setting_inputOutput,
        Setting_systemAdjust,
        Setting_parameterLibrary,
        Setting_ImgProcess,
        Setting_AxisAdjust,
        Setting_Postion,


        About_help,
        About_support,
        About_report,
        About_language,
        About_about,

    };
    static HMainMenuBar* GetInstance(QMenuBar *pmenubar = nullptr,QObject *parent=nullptr);

    QAction * getAction(qint32 index);

    void setupMenuAbout(QMenuBar *pmenubar);




private:

    QMap<qint32,QAction *> m_actions;

    explicit HMainMenuBar(QMenuBar *pmenubar = nullptr, QObject *parent=nullptr);
    static HMainMenuBar* m_pInstance;

    IniSettings *ptrIni;

    void setupMenuUser(QMenuBar *pmenubar);
    void setupMenuFile(QMenuBar *pmenubar);
    void setupMenuEdit(QMenuBar *pmenubar);
    void setupMenuTool(QMenuBar *pmenubar);
    void setupMenuSetting(QMenuBar *pmenubar);




};

#endif // HMAINMENUBAR_H
