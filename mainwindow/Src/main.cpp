#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QSharedMemory>
#include <QMessageBox>
#include <Windows.h>
#include <DbgHelp.h>
#include <QIcon>

#include <QDateTime>
#include <QMutex>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QTextCodec>
#include <QTimer>
#include <QTranslator>

#include "Src/User/dialogaccountui.h"

#include "Src/Editor/codeedit.h"


#include "Src/data/hsqlcommon.h"
#include "Src/Motion/h_motioncontroller.h"

#include "Src/window/programhandler.h"

#include "Src/Motion/h_motioncontroller.h"


#include "Src/Motion/hcameracontroller.h"

#include "Src/data/hconfigwidget.h"


#include "Src/data/hsettings.h"

#include "Src/Motion/hiodevicecontroller.h"


#include "Src/Gui/settings/hsystemadjustcontroller.h"

#include "Src/Gui/settings/hparalibcontroller.h"
#include "Src/Motion/hscannercontroller.h"


//#pragma comment(lib, "Dbghelp.lib")

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException){

    /****save your data here****/

    HCameraController::Release();
    H_MotionController::Release();

    /***************************/


    //create Dump file
    //    SYSTEMTIME lpSystemTime ;
    //    GetLocalTime(&lpSystemTime );
    //    QString time=QString::number(lpSystemTime.wYear)+"-"+QString::number(lpSystemTime.wMonth)+"-"+QString::number(lpSystemTime.wDay)+"_"+
    //            QString::number(lpSystemTime.wHour)+QString::number(lpSystemTime.wMinute)+QString::number(lpSystemTime.wSecond)
    //            +QString::number(lpSystemTime.wMilliseconds)+QString::number(lpSystemTime.wDayOfWeek);

    QString path=(QCoreApplication::applicationDirPath()+"/Log/"+QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss-zzz_dddd")+".dump");
    HANDLE hDumpFile = CreateFile(path.toStdWString().c_str(),GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if( hDumpFile != INVALID_HANDLE_VALUE){

        //Dump info
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;

        //write dump data
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, nullptr, nullptr);
        CloseHandle(hDumpFile);

    }
    else{
        qCritical()<<"hDumpFile == null";
    }

    //pop-up a dialog and exit process
    EXCEPTION_RECORD* record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode,16)),errAdr(QString::number(reinterpret_cast<uint>(record->ExceptionAddress),16)),errMod;
    QMessageBox::critical(nullptr,QString::fromLocal8Bit("process crashed"),QString::fromLocal8Bit("<FONT size=4><div><b> An unexpected error caused your application to terminate!</b><br/></div>")+
                          QString(QString::fromLocal8Bit("<div>error code：%1</div><div>error memory addres：%2</div></b><br/></FONT>")).arg(errCode).arg(errAdr)
                          +QString::fromLocal8Bit("<FONT size=4><div> contact us to report bug: hongjianli@inno-laser.com</b><br/></div>"),QMessageBox::Ok);
    //qFatal("Process Crashed");
    return EXCEPTION_EXECUTE_HANDLER;
}


void registerFileRelation(const QString& className,const QString& appPath,const QString& ext,const QString& extDes)
{

    //    QString className("xxx.yyy.com");                           // 自定义的类别

    //    QString appPath("D:\\Program Files\\MyProgram.exe");        // 关联的程序目录

    //    QString ext(".proj");                                       // 关联的文件类型

    //    QString extDes("xx 工程文件");                              // 该文件类型描述

    QString baseUrl("HKEY_CURRENT_USER\\Software\\Classes");    // 要添加的顶层目录
    QSettings settingClasses(baseUrl,QSettings::NativeFormat);

    // 在...Classes\类别下创建一个新的类别，并设置该类别打开文件时的调用参数
    settingClasses.setValue("/" + className + "/Shell/Open/Command/.","\"" + appPath + "\" \"%1\"");

    // 文件类型描述
    settingClasses.setValue("/" + className + "/.",extDes);

    // 设置该类别的默认图标默认图标
    settingClasses.setValue("/" + className + "/DefaultIcon/.",appPath + ",1");
    // 关联ext 和 类别
    settingClasses.setValue("/" + ext + "/OpenWithProgIds/" + className,"");

    // 立即保存该修改
    settingClasses.sync();

}


void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = "Debug: ";
        break;
    case QtWarningMsg:
        text = "Warning: ";
        break;
    case QtCriticalMsg:
        text = "Critical: ";
        break;
    case QtInfoMsg:
        text = "Info: ";
        break;
    case QtFatalMsg:
        text = "Fatal: ";
        break;
    }

    QString context_info = QString("File:(%1) Line:(%2) Function:(%3)").arg(QString(context.file)).arg(context.line).arg(context.function);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz dddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);

    QFile file(QCoreApplication::applicationDirPath()+"/Log/log.log");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
    mutex.unlock();

    if(text==QString("Fatal: "))
        quick_exit(-3);
}

qint64 dirFileSize(const QString &path)
{
    QDir dir(path);
    qint64 size = 0;
    //dir.entryInfoList(QDir::Files)  return file infomation
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
    {
        //count size of file
        size += fileInfo.size();
    }
    //dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot)返回所有子目录，并进行过滤
    foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        //若存在子目录，则递归调用dirFileSize()函数
        size += dirFileSize(path + QDir::separator() + subDir);
    }
    return size;
}

bool DelDir(const QString &path)
{
    if (path.isEmpty())
        return false;
    QDir dir(path);
    if(!dir.exists())
        return true;
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
    QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
    foreach (QFileInfo file, fileList){ //遍历文件信息
        if (file.isFile()){ // 是文件，删除
            file.dir().remove(file.fileName());
        }else{ // 递归删除
            DelDir(file.absoluteFilePath());
        }
    }
    return true;
    //return dir.rmpath(dir.absolutePath()); // 删除文件夹
}


//#include "Src/Camera/imgProcess/h_circle_fitting.h"

#include "h_globalapplication.h"
#include <QLoggingCategory>

#include "Src/data/inisettings.h"

int main(int argc, char **argv)
{
    ///////for test/////////
    //    if (nullptr != argv) {
    //        qDebug() << argv[0]<<123;
    //    }
    //    std::vector<POINT1> points;

    //    points.push_back(POINT1(23.78,2341.323));
    //    points.push_back(POINT1(545.123,33.887));
    //    points.push_back(POINT1(23.4443,97.098));
    //    //points.push_back(POINT1(-0.1,-2));
    //    H_Circle_Fitting kl;
    //    double a1=0;
    //    double a2=0.2;
    //    double a3=-1.2;
    //    qDebug()<<kl.circleFitL1(points,a1,a2,a3);
    //    qDebug()<<QString::number(a1,'f',5)<<QString::number(a2,'f',5)<<QString::number(a3,'f',5);
    ////////////////////////

    //QLoggingCategory::setFilterRules(QStringLiteral("qt.modbus* = true"));

    //HConfigCtr::GetInstance()->WriteConfigFile("E:/A.GDF");



    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);


    QApplication app(argc, argv);
    QTranslator lang;

    switch(IniSettings::GetInstance()->getValue(IniSettings::View_Language).toInt()){
    case 0:{

    }
        break;
    case 1:{

        lang.load(":/lang_zh.qm");
        app.installTranslator(&lang);
    }
        break;
    default:
        break;
    }

    //SetUnhandledExceptionFilter(static_cast<LPTOP_LEVEL_EXCEPTION_FILTER>(ApplicationCrashHandler));// for dump
    //qInstallMessageHandler(outputMessage); //for log

    if(dirFileSize(QCoreApplication::applicationDirPath()+"/Log")>1073741824){//del log file
        DelDir(QCoreApplication::applicationDirPath()+"/Log");
    }



    app.setWindowIcon(QIcon("inno.ico"));//set app icon

    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());


    registerFileRelation("hongjianli",QString::fromLocal8Bit(argv[0]),".hpro","HLaser");//associate file type

    int nFlag = -1;
    QSharedMemory *g_pShareMem = new QSharedMemory(QString("centralMoniSystem"));

    if (g_pShareMem->attach(/*QSharedMemory::ReadOnly*/)){
        if(QString(argv[1]).isEmpty()){
            QMessageBox::information(nullptr,QObject::tr("warning"),QObject::tr("Application is running!!!"));
            g_pShareMem->detach();
        }
        else{
            g_pShareMem->lock();
            char * to = static_cast<char*>(g_pShareMem->data());
            //auto str=QString::fromLocal8Bit(argv[1]);
            memmove(to,argv[1],strlen(argv[1]));
            //memcpy(to, from, strlen(from));
            g_pShareMem->unlock();
        }
        g_pShareMem->detach();
    }


    if (g_pShareMem->create(2048)){

        QTimer *timer=new QTimer;
        QObject::connect(timer, &QTimer::timeout, [=](){
            if (g_pShareMem->isAttached()){
                g_pShareMem->lock();
                char * from = static_cast<char*>(g_pShareMem->data());
                QString str=QString::fromLocal8Bit(from);
                if(!str.isEmpty()){
                    ProgramHandler::GetInstance()->openProgramFile(str.replace('\\','/'));
                }
                memset(from,0,2048);
                g_pShareMem->unlock();
            }});
        timer->start(200);


        //        DialogAccountUI* userDialog=new  DialogAccountUI(true);
        //        userDialog->exec();


        //       auto ptrCfg=HConfigCtr::GetInstance();

        //        if(ptrCfg->IsConfigEmpty()){
        //            HFirstGuideWid wid("kkk");
        //            if(!wid.exec()){

        //                if(g_pShareMem->isAttached()){
        //                    timer->stop();
        //                    g_pShareMem->detach();
        //                }
        //                delete g_pShareMem;
        //                g_pShareMem = nullptr;
        //                return nFlag;

        //            }
        //            HConfigWidget::GetInstance()->exec();

        //        }

        HSettings::GetInstance()->Init();
        HCameraController::GetInstance()->Init();
        H_MotionController::GetInstance()->Init();
        HIODeviceController::GetInstance()->Init();
        HScannerController::GetInstance()->InitScannerCtr();


        HParaLibController::GetInstance()->Init();
        HSystemAdjustController::GetInstance()->Init();


        QMessageBox::StandardButton button;
        button=QMessageBox::question(nullptr,QObject::tr("Message From Motion"),QObject::tr("Homing motion system now ?"),QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(button==QMessageBox::No)
        {

        }
        else if(button==QMessageBox::Yes)
        {
            HIODeviceController::GetInstance()->GetOutputPort(0)->SetOutputBit(1);
            HIODeviceController::GetInstance()->GetOutputPort(1)->SetOutputBit(1);
            QThread::msleep(500);

            H_MotionController::GetInstance()->GetAxis(0)->EnabledAxis(1);
            H_MotionController::GetInstance()->GetAxis(1)->EnabledAxis(1);

            //H_MotionController::GetInstance()->GetAxis(0)->HomeAxis();

            //            HIODeviceController::GetInstance()->GetOutputPort(2)->SetOutputBit(1);
            //            HIODeviceController::GetInstance()->GetOutputPort(3)->SetOutputBit(1);
            //            QThread::msleep(500);
                        H_MotionController::GetInstance()->HomeAllMotion();
                        H_MotionController::GetInstance()->GetAxis(0)->TestFunc();
        }
        else{

            if(g_pShareMem->isAttached()){
                timer->stop();
                g_pShareMem->detach();
            }
            delete g_pShareMem;
            g_pShareMem = nullptr;

            exit(0);
        }


        //QMessageBox::information(nullptr,QObject::tr("Warning"),QObject::tr("Motion system init failed!"));




        MainWindow mainWin;
        //mainWin.loadSettings();

        mainWin.show();

        if(!QString(argv[1]).isEmpty()){
            ProgramHandler::GetInstance()->openProgramFile(QString(argv[1]).replace('\\','/'));
        }

        nFlag = app.exec();
        HIODeviceController::GetInstance()->GetOutputPort(0)->SetOutputBit(0);
        HIODeviceController::GetInstance()->GetOutputPort(1)->SetOutputBit(0);

        //        HIODeviceController::GetInstance()->GetOutputPort(2)->SetOutputBit(0);
        //        HIODeviceController::GetInstance()->GetOutputPort(3)->SetOutputBit(0);


        if(g_pShareMem->isAttached()){
            timer->stop();
            g_pShareMem->detach();
        }
        delete g_pShareMem;
        g_pShareMem = nullptr;


        return nFlag;
    }
}







