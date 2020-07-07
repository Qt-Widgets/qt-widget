#include "MotionCompiler.h"


#include <QDateTime>

#include <QFile>
#include <QCoreApplication>

#include "Src/window/programhandler.h"

#include "Src/Gui/settings/hparalibcontroller.h"

#include "Src/data/inisettings.h"
#include "Src/Motion/h_motioncontroller.h"

#include "Src/Gui/settings/hsystemadjustcontroller.h"


MotionCompiler * MotionCompiler::m_pInstance = nullptr;
MotionCompiler *MotionCompiler::GetInstance(QObject *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new MotionCompiler(parent);
    }
    return m_pInstance;
}



bool MotionCompiler::createGcodeAndRun(const QString &axisX, const QString &axisY, const QString &axisZ, qint32 range)
{
    QString code;

    code.append(QString("'Created time: %1\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd  hh-mm-ss-zzz")));
    code.append(QString("'User: %1\n\n").arg("INNO"));


    //    code.append("#DEFINE CALLBACK_RETURNCODE $global[0]\n");
    code.append("MotionInterpolationMode = 1\n");
    //    code.append("EncoderDivider.X = 0\n");
    //    code.append("EncoderDivider.Y = 1\n");
    //    code.append(QString("EmulatedQuadratureDivider.X=%1\n").arg(IniSettings::GetInstance()->getValue(IniSettings::A3200_EmulatedQuadratureDivider).toInt()));
    //    code.append(QString("EmulatedQuadratureDivider.Y=%1\n").arg(IniSettings::GetInstance()->getValue(IniSettings::A3200_EmulatedQuadratureDivider).toInt()));
    code.append(QString("TrajectoryFIRFilter.X=%1\n").arg(IniSettings::GetInstance()->getValue(IniSettings::A3200_TrajectoryFIRFilter).toInt()));
    code.append(QString("TrajectoryFIRFilter.Y=%1\n").arg(IniSettings::GetInstance()->getValue(IniSettings::A3200_TrajectoryFIRFilter).toInt()));
    code.append("ENABLE X Y\n");
    code.append("HOME X Y CONDITIONAL\n");
    code.append("PSOCONTROL X RESET\n");
    code.append("PSOTRACK X INPUT 2,1\n");
    //code.append("PSOTRACK X INPUT 0,1\n");
    code.append("PSOTRACK X RESET 32\n");
    code.append("PSOOUTPUT X CONTROL 0,1\n");
    code.append("PSOOUTPUT X PULSE\n");
    code.append("ABSOLUTE\n");
    code.append("VELOCITY ON\n");
    code.append("G17\n\n\n\n");

    auto pData=ProgramHandler::GetInstance()->getActiveProgram();

    if(pData==nullptr)
        return false;


    QStringList paras;
    RLine mggg(LLONG_MAX,LLONG_MAX,0,0);
    for(const auto&k:ProgramHandler::GetInstance()->getActiveProgram()->getData()){


        qint32 mode=!k.locationPts.isEmpty();

        QTransform trans=QTransform(1,0,0,0,1,0,0,0,1);
        HSystemAdjustController::GetInstance()->getCutterTrans(trans);

        if(!k.locationPts.isEmpty()){
            if(HSystemAdjustController::GetInstance()->getTransFromPainterPossUseCutter(trans,k.locationPts))
                return false;
        }


        for(const auto&m:k.getItems()){
            switch (range) {
            case 0:{
                if(m->isSelected()){
                    if(!m->getA3200Gcode(code,paras,mggg,mode,trans)){
                        return false;
                    }
                }

            }
                break;
            case 1:{

                if(!m->getA3200Gcode(code,paras,mggg,mode,trans)){
                    return false;
                }
            }
                break;
            default:
                break;
            }

        }
    }

    code.append(A3200_PSO_OFF());
    code.append("END PROGRAM");


    QFile file(QCoreApplication::applicationDirPath()+"/A3200/taskCode/task1-1.pgm");
    if(file.open(QFile::WriteOnly)){
        file.write(code.toLocal8Bit());
        file.close();
        return H_MotionController::GetInstance()->GetAxis(axisX)->RunProgramFile(QCoreApplication::applicationDirPath()+"/A3200/taskCode/task1-1.pgm");

    }
    else{
        return false;
    }

}

bool MotionCompiler::createFocusGcodeAndRun(const QString &axisX, const QString &axisY, const QString &axisZ,
                                            const QString &namePara, qint32 type, double valueStart, double valueStep, const QPointF &pos, qint32 num, bool useMark, qint32 markType, double markSize, double step)
{

    auto ptrIni=IniSettings::GetInstance();
    QString code;

    code.append(QString("'Created time: %1\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd  hh-mm-ss-zzz")));
    code.append(QString("'User: %1\n\n").arg("INNO"));


    //code.append("#DEFINE CALLBACK_RETURNCODE $global[0]\n");
    code.append("MotionInterpolationMode = 1\n");
    //code.append("EncoderDivider.X = 0\n");
    //code.append("EncoderDivider.Y = 1\n");
    //code.append(QString("EmulatedQuadratureDivider.X=%1\n").arg(IniSettings::GetInstance()->getValue(IniSettings::A3200_EmulatedQuadratureDivider).toInt()));
    //code.append(QString("EmulatedQuadratureDivider.Y=%1\n").arg(IniSettings::GetInstance()->getValue(IniSettings::A3200_EmulatedQuadratureDivider).toInt()));
    code.append(QString("TrajectoryFIRFilter.X=%1\n").arg(ptrIni->getValue(IniSettings::A3200_TrajectoryFIRFilter).toInt()));
    code.append(QString("TrajectoryFIRFilter.Y=%1\n").arg(ptrIni->getValue(IniSettings::A3200_TrajectoryFIRFilter).toInt()));
    code.append("ENABLE X Y\n");
    code.append("HOME X Y CONDITIONAL\n");
    code.append("PSOCONTROL X RESET\n");
    code.append("PSOTRACK X INPUT 2,1\n");
    //code.append("PSOTRACK X INPUT 0,1\n");
    code.append("PSOTRACK X RESET 32\n");
    code.append("PSOOUTPUT X CONTROL 0,1\n");
    code.append("PSOOUTPUT X PULSE\n");
    code.append("ABSOLUTE\n");
    code.append("VELOCITY ON\n");
    code.append("G17\n\n\n\n");


    code.append(MotionCompiler::A3200_Paras(HParaLibController::GetInstance()->getParaDatas(ptrIni->getValue(IniSettings::Edit_mode).toInt(),namePara)));


    for(qint32 k=0;k<num;++k){
        switch (type) {
        case 0:{
            code.append(A3200_G0_Z(valueStart+(k*valueStep)));
            //code.append("WAIT INPOS Z\n");
        }
            break;
        default:
            break;
        }

        switch (markType) {
        case 0:{
            auto posss=pos+QPointF(step*k,0.0);
            //auto posss=pos+QPointF(0.0,step*k);
            code.append(A3200_G0(posss));
            code.append(A3200_WAIT_INPOS());
            code.append(A3200_PSO_ON());
            code.append(A3200_G1(posss+QPointF(0.0,markSize)));
            //code.append(A3200_G1(posss+QPointF(markSize,0.0)));
            code.append(A3200_WAIT_INPOS());
            code.append(A3200_PSO_OFF());

            if(useMark)
                addNumber(posss/*-QPointF(0.0,markSize/qAbs(markSize))*/,QPointF(step/qAbs(step),-markSize/qAbs(markSize)),code,k,step*0.3);

        }
            break;
        case 1:{
            auto posss=pos+QPointF(step*k,0.0)+QPointF(markSize*0.5,0.0);
            code.append(A3200_G0(posss));
            code.append(A3200_WAIT_INPOS());
            code.append(A3200_PSO_ON());
            code.append(QString("G3 P%1  Q%2  R%3\n").arg(0.0,0,'f',7).arg(0.0,0,'f',7).arg(markSize*0.5,0,'f',7));
            code.append(A3200_WAIT_INPOS());
            code.append(A3200_PSO_OFF());

            if(useMark)
                addNumber(posss/*-QPointF(0.0,markSize/qAbs(markSize))*/,QPointF(step/qAbs(step),-markSize/qAbs(markSize)),code,k,step*0.3);
        }
            break;
        default:
            break;
        }
    }

    code.append("END PROGRAM");


    QFile file(QCoreApplication::applicationDirPath()+"/A3200/taskCode/findFocus.pgm");
    if(file.open(QFile::WriteOnly)){
        file.write(code.toLocal8Bit());
        file.close();
        return H_MotionController::GetInstance()->GetAxis(axisX)->RunProgramFile(QCoreApplication::applicationDirPath()+"/A3200/taskCode/findFocus.pgm");

    }
    else{
        return false;
    }

}

bool MotionCompiler::createMarkGcodeAndRun(const QString &axisX, const QString &axisY, const QString &axisZ,
                                           const QPointF &pos, qint32 markType, double size, const QString& paraName)
{
    auto ptrIni=IniSettings::GetInstance();
    QString code;

    code.append(QString("'Created time: %1\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd  hh-mm-ss-zzz")));
    code.append(QString("'User: %1\n\n").arg("INNO"));


    //code.append("#DEFINE CALLBACK_RETURNCODE $global[0]\n");
    code.append("MotionInterpolationMode = 1\n");
    //code.append("EncoderDivider.X = 0\n");
    //code.append("EncoderDivider.Y = 1\n");
    //code.append(QString("EmulatedQuadratureDivider.X=%1\n").arg(IniSettings::GetInstance()->getValue(IniSettings::A3200_EmulatedQuadratureDivider).toInt()));
    //code.append(QString("EmulatedQuadratureDivider.Y=%1\n").arg(IniSettings::GetInstance()->getValue(IniSettings::A3200_EmulatedQuadratureDivider).toInt()));
    code.append(QString("TrajectoryFIRFilter.X=%1\n").arg(ptrIni->getValue(IniSettings::A3200_TrajectoryFIRFilter).toInt()));
    code.append(QString("TrajectoryFIRFilter.Y=%1\n").arg(ptrIni->getValue(IniSettings::A3200_TrajectoryFIRFilter).toInt()));
    code.append("ENABLE X Y\n");
    code.append("HOME X Y CONDITIONAL\n");
    code.append("PSOCONTROL X RESET\n");
    code.append("PSOTRACK X INPUT 2,1\n");
    //code.append("PSOTRACK X INPUT 0,1\n");
    code.append("PSOTRACK X RESET 32\n");
    code.append("PSOOUTPUT X CONTROL 0,1\n");
    code.append("PSOOUTPUT X PULSE\n");
    code.append("ABSOLUTE\n");
    code.append("VELOCITY ON\n");
    code.append("G17\n\n\n\n");

    code.append(MotionCompiler::A3200_Paras(HParaLibController::GetInstance()->getParaDatas(ptrIni->getValue(IniSettings::Edit_mode).toInt(),paraName)));

    switch (markType) {
    case 0:{

        code.append(A3200_G0(pos+QPointF(size*0.5,0.0)));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());
        code.append(QString("G3 P%1  Q%2  R%3\n").arg(0.0,0,'f',7).arg(0.0,0,'f',7).arg(size*0.5,0,'f',7));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_OFF());


    }
        break;
    case 1:{
        code.append(A3200_G0(pos+QPointF(-size*0.5,0.0)));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());
        code.append(A3200_G1(pos+QPointF(size*0.5,0.0)));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_OFF());

        code.append(A3200_G0(pos+QPointF(0.0,-size*0.5)));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());
        code.append(A3200_G1(pos+QPointF(0.0,size*0.5)));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_OFF());
    }
        break;
    default:
        break;
    }

    code.append("END PROGRAM");


    QFile file(QCoreApplication::applicationDirPath()+"/A3200/taskCode/cutMark.pgm");
    if(file.open(QFile::WriteOnly)){
        file.write(code.toLocal8Bit());
        file.close();
        return H_MotionController::GetInstance()->GetAxis(axisX)->RunProgramFile(QCoreApplication::applicationDirPath()+"/A3200/taskCode/cutMark.pgm");

    }
    else{
        return false;
    }
}

qint32 MotionCompiler::createPointsAndRunWait(const QString &axisX, const QString &axisY, const QString &axisZ,
                                              const QVector<QPointF> &pts, qint32 markType, double size, const QString& paraName)
{
    auto ptrIni=IniSettings::GetInstance();
    QString code;

    code.append(QString("'Created time: %1\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd  hh-mm-ss-zzz")));
    code.append(QString("'User: %1\n\n").arg("INNO"));


    //code.append("#DEFINE CALLBACK_RETURNCODE $global[0]\n");
    code.append("MotionInterpolationMode = 1\n");
    //code.append("EncoderDivider.X = 0\n");
    //code.append("EncoderDivider.Y = 1\n");
    //code.append(QString("EmulatedQuadratureDivider.X=%1\n").arg(IniSettings::GetInstance()->getValue(IniSettings::A3200_EmulatedQuadratureDivider).toInt()));
    //code.append(QString("EmulatedQuadratureDivider.Y=%1\n").arg(IniSettings::GetInstance()->getValue(IniSettings::A3200_EmulatedQuadratureDivider).toInt()));
    code.append(QString("TrajectoryFIRFilter.X=%1\n").arg(ptrIni->getValue(IniSettings::A3200_TrajectoryFIRFilter).toInt()));
    code.append(QString("TrajectoryFIRFilter.Y=%1\n").arg(ptrIni->getValue(IniSettings::A3200_TrajectoryFIRFilter).toInt()));
    code.append("ENABLE X Y\n");
    code.append("HOME X Y CONDITIONAL\n");
    code.append("PSOCONTROL X RESET\n");
    code.append("PSOTRACK X INPUT 2,1\n");
    //code.append("PSOTRACK X INPUT 0,1\n");
    code.append("PSOTRACK X RESET 32\n");
    code.append("PSOOUTPUT X CONTROL 0,1\n");
    code.append("PSOOUTPUT X PULSE\n");
    code.append("ABSOLUTE\n");
    code.append("VELOCITY ON\n");
    code.append("G17\n\n\n\n");

    code.append(MotionCompiler::A3200_Paras(HParaLibController::GetInstance()->getParaDatas(ptrIni->getValue(IniSettings::Edit_mode).toInt(),paraName)));

    switch (markType) {
    case 0:{

        for(const auto&k:pts){
            code.append(A3200_G0(k+QPointF(size*0.5,0.0)));
            code.append(A3200_WAIT_INPOS());
            code.append(A3200_PSO_ON());
            code.append(QString("G3 P%1  Q%2  R%3\n").arg(0.0,0,'f',7).arg(0.0,0,'f',7).arg(size*0.5,0,'f',7));
            code.append(A3200_WAIT_INPOS());
            code.append(A3200_PSO_OFF());
        }
    }
        break;
    case 1:{
        for(const auto&k:pts){
            code.append(A3200_G0(k+QPointF(-size*0.5,0.0)));
            code.append(A3200_WAIT_INPOS());
            code.append(A3200_PSO_ON());
            code.append(A3200_G1(k+QPointF(size*0.5,0.0)));
            code.append(A3200_WAIT_INPOS());
            code.append(A3200_PSO_OFF());

            code.append(A3200_G0(k+QPointF(0.0,-size*0.5)));
            code.append(A3200_WAIT_INPOS());
            code.append(A3200_PSO_ON());
            code.append(A3200_G1(k+QPointF(0.0,size*0.5)));
            code.append(A3200_WAIT_INPOS());
            code.append(A3200_PSO_OFF());
        }
    }
        break;
    default:
        break;
    }

    code.append("END PROGRAM");


    QFile file(QCoreApplication::applicationDirPath()+"/A3200/taskCode/cutMarks.pgm");
    if(file.open(QFile::WriteOnly)){
        file.write(code.toLocal8Bit());
        file.close();
        return H_MotionController::GetInstance()->GetAxis(axisX)->RunProgramFileWait(QCoreApplication::applicationDirPath()+"/A3200/taskCode/cutMarks.pgm");

    }
    else{
        return -1;
    }
}




bool MotionCompiler::RunTask1()
{
    //return A3200Api::GetInstance()->RunProgram(QString(QCoreApplication::applicationDirPath()+"/A3200/taskCode/task1-1.pgm").toLocal8Bit());
    return true;
}

bool MotionCompiler::RunTask1AndWait()
{
    //return A3200Api::GetInstance()->RunProgramAndWait(QString(QCoreApplication::applicationDirPath()+"/A3200/taskCode/task1-1.pgm").toLocal8Bit());
    return true;
}

bool MotionCompiler::RunTask2(const QString &code)
{
    QFile file(QCoreApplication::applicationDirPath()+"/A3200/taskCode/task2-1.pgm");
    if(file.open(QFile::WriteOnly)){
        file.write(code.toLocal8Bit());
        file.close();
        return true;
        //return A3200Api::GetInstance()->RunProgram(QString(QCoreApplication::applicationDirPath()+"/A3200/taskCode/task2-1.pgm").toLocal8Bit());
    }
    else{
        return false;
    }


}



bool MotionCompiler::RunAdjust1()
{
    //return A3200Api::GetInstance()->RunProgram(QString(QCoreApplication::applicationDirPath()+"/A3200/taskCode/adjust-1.pgm").toLocal8Bit());
    return true;
}

bool MotionCompiler::RunAdjust1AndWait()
{
    // return A3200Api::GetInstance()->RunProgramAndWait(QString(QCoreApplication::applicationDirPath()+"/A3200/taskCode/adjust-1.pgm").toLocal8Bit());
    return true;
}

bool MotionCompiler::RegistrFirstCallBack()
{
    //return A3200Api::GetInstance()->RegistrFirstCall();
    return true;
}

bool MotionCompiler::CallbackWaitCancle()
{
    //return A3200Api::GetInstance()->CallBackWaitCancle();
    return true;
}


bool MotionCompiler::WaitCallback(QVariantList&args)
{
    // return  A3200Api::GetInstance()->WaitCallbackMessage(args);
    return true;
}



bool MotionCompiler::CallbackDone(double returnCode)
{
    // return  A3200Api::GetInstance()->CallbackDone(returnCode);
    return true;
}


bool MotionCompiler::StopProgram()
{
    return H_MotionController::GetInstance()->GetAxis(0)->StopProgramWait();
}

bool MotionCompiler::isProgramRunning()
{
    //    int st=A3200Api::GetInstance()->GetProgramStatus();
    //    if(st==4||st==6){
    //        return true;
    //    }else{
    //        return false;
    //    }
    return true;
}

QString MotionCompiler::A3200_DWELL(double time){
    if(time<0.001){
        return QString();
    }
    return QString("DWELL   %1\n").arg (time,0,'f',7);
}

QString MotionCompiler::A3200_WAIT_INPOS(){
    return "WAIT INPOS X Y\n";
}

QString MotionCompiler::A3200_PSO_FIRE_ONE(){
    return "PSOCONTROL X FIRE\n";
}

QString MotionCompiler::A3200_Paras(const QStringList &paras){
    QString code;


    QString value=paras.value(HParaLibController::Cutter_General_focus);
    if(!value.isEmpty()){
        //code.append(A3200_CALLBACK(0,value));
        code.append(A3200_AXIS_Z(value.toDouble()));
    }

    //        value=paras.value(HParaLibController::Laser2_power);
    //        if(!value.isEmpty()){
    //            code.append(A3200_CALLBACK(1,QString("MCS=%1").arg(int(value.toDouble()*10))));
    //        }

    //        value=paras.value(HParaLibController::Laser2_prf);
    //        if(!value.isEmpty()){
    //            code.append(A3200_CALLBACK(1,QString("PRF=%1").arg(int(value.toDouble()*100))));
    //        }

    //        value=paras.value(HParaLibController::Laser2_id1);
    //        if(!value.isEmpty()){
    //            code.append(A3200_CALLBACK(1,QString("ID1=%1").arg(int(value.toDouble()*10))));
    //        }

    //        value=paras.value(HParaLibController::Laser2_id2);
    //        if(!value.isEmpty()){
    //            code.append(A3200_CALLBACK(1,QString("ID2=%1").arg(int(value.toDouble()*10))));
    //        }

    //        value=paras.value(HParaLibController::Laser2_id3);
    //        if(!value.isEmpty()){
    //            code.append(A3200_CALLBACK(1,QString("ID3=%1").arg(int(value.toDouble()*10))));
    //        }

    //        value=paras.value(HParaLibController::Laser2_psn);
    //        if(!value.isEmpty()){
    //            code.append(A3200_CALLBACK(1,QString("PSN=%1").arg(value.toInt())));
    //        }

    //        value=paras.value(HParaLibController::Laser2_dpk);
    //        if(!value.isEmpty()){
    //            code.append(A3200_CALLBACK(1,QString("DPK=%1").arg(value.toInt())));
    //        }

    value=paras.value(HParaLibController::Cutter_Cam_speed);
    if(!value.isEmpty()){
        code.append(A3200_COMPLEX_SPEED(value.toDouble()));
    }

    value=paras.value(HParaLibController::Cutter_Cam_acc);
    if(!value.isEmpty()){
        code.append(A3200_COMPLEX_ACC(value.toDouble()));
    }

    value=paras.value(HParaLibController::Cutter_Cam_jumpSpeed);
    if(!value.isEmpty()){
        code.append(A3200_JUMP_SPEED(value.toDouble()));
    }

    value=paras.value(HParaLibController::Cutter_Cam_jumpAcc);
    if(!value.isEmpty()){
        code.append(A3200_JUMP_ACC(value.toDouble()));
    }

    value=paras.value(HParaLibController::Cutter_Cam_trigDistance);
    if(!value.isEmpty()){
        code.append(QString("IF (EmulatedQuadratureDivider.X >0)\n"
                            "PSODISTANCE X FIXED ABS (UNITSTOCOUNTS(X,%1/EmulatedQuadratureDivider.X))\n"
                            "ELSE\n"
                            "PSODISTANCE X FIXED ABS (UNITSTOCOUNTS(X,%2))\n"
                            "END IF\n").arg(value).arg(value));
    }

    QStringList values;
    values.append(paras.value(HParaLibController::Cutter_Cam_pulseWidth));
    values.append(paras.value(HParaLibController::Cutter_Cam_dutyPercent));
    values.append(paras.value(HParaLibController::Cutter_Cam_pulseCount));
    values.append(paras.value(HParaLibController::Cutter_Cam_pulseDelay));
    if(!values.at(0).isEmpty()&&!values.at(1).isEmpty()&&!values.at(2).isEmpty()&&!values.at(3).isEmpty()){
        code.append(QString("PSOPULSE X TIME %1,%2 CYCLES %3 DELAY %4 \n").arg(values.at(0))
                    .arg((1.0-values.at(1).toDouble()*0.01)*values.at(0).toDouble())
                    .arg(values.at(2))
                    .arg(values.at(3)));
    }


    return code;

}

QString MotionCompiler::A3200_CALLBACK(int type, const QString &var1){

    return  QString("CALLBACK_RETURNCODE = CALLBACK 0, %1, \"%2\" \n"
                    "IF (CALLBACK_RETURNCODE<0.5)\nPSOCONTROL X OFF\nPROGRAM STOP\nEND IF\n\n").arg(type).arg(var1);


}

QString MotionCompiler::A3200_PVT(const QPointF &xy, double speedX, double speedY, double time){
    return QString("PVT X %1,%2  Y %3,%4 TIME %5\n").arg (xy.x(),0,'f',7).arg (speedX,0,'f',7).arg (xy.y(),0,'f',7).arg (speedY,0,'f',7).arg (time*1000,0,'f',7);
}

QString MotionCompiler::A3200_G0_Z(double pos){
    return QString("G0 Z %1\n").arg (pos,0,'f',7);
}

QString MotionCompiler::A3200_G0(const QPointF &xy){
    return QString("G0 X%1   Y%2\n").arg (xy.x(),0,'f',7).arg (xy.y(),0,'f',7);
}

QString MotionCompiler::A3200_G1(const QPointF &xy){
    return QString("G1 X%1   Y%2\n").arg (xy.x(),0,'f',7).arg (xy.y(),0,'f',7);
}

QString MotionCompiler::A3200_G2_G3(bool isCCW, const QPointF &xy, double i, double j){
    return QString("G%1 X%2   Y%3   I%4   J%5\n").arg (isCCW?2:3).arg(xy.x(),0,'f',7).arg (xy.y(),0,'f',7).arg (i,0,'f',7).arg (j,0,'f',7);
}

QString MotionCompiler::A3200_PSO_ON(){
    return QString("PSOCONTROL X ARM\n");
}

QString MotionCompiler::A3200_PSO_OFF(){
    return QString("PSOCONTROL X OFF\n");
}

QString MotionCompiler::A3200_JUMP_ACC(double acc){
    return QString("RAMP RATE X   %1\nRAMP RATE Y   %2\n").arg(acc,0,'f',7).arg(acc,0,'f',7);
}

QString MotionCompiler::A3200_JUMP_SPEED(double vel){
    return QString("XF   %1\nYF   %2\n").arg(vel,0,'f',7).arg(vel,0,'f',7);
}

QString MotionCompiler::A3200_COMPLEX_ACC(double acc){
    return QString("RAMP RATE   %1\n").arg(acc,0,'f',7);
}

QString MotionCompiler::A3200_COMPLEX_SPEED(double vel){
    return QString("F   %1\n").arg(vel,0,'f',7);
}

QString MotionCompiler::A3200_AXIS_Z(double pos){
    return QString("G0 Z %1\n").arg(pos,0,'f',7);
}

MotionCompiler::MotionCompiler(QObject *parent) :QObject(parent)
{

}

MotionCompiler::~MotionCompiler()
{

}

void MotionCompiler::addNumber(const QPointF &pos,const QPointF&dir,QString &code, qint32 number, double size)
{
    QString numString=QString("%1").arg(number,2,10,QChar('0'));
    double width=-size;
    double height=size*2.0;
    double interval=-size*0.4;


    QPointF tl=pos+QPointF((-interval*0.5-width)*dir.x(),height*dir.y());
    QPointF tr=pos+QPointF((-interval*0.5)*dir.x(),height*dir.y());
    QPointF br=pos+QPointF((-interval*0.5)*dir.x(),0.0);
    QPointF bl=pos+QPointF((-interval*0.5-width)*dir.x(),0.0);

    QPointF lc=pos+QPointF((-interval*0.5-width)*dir.x(),height*0.5*dir.y());
    QPointF rc=pos+QPointF((-interval*0.5)*dir.x(),height*0.5*dir.y());

    mCodeNumber(code,QString(numString.at(0)).toInt(),tl,tr,br,bl,lc,rc);

    tr=pos+QPointF((interval*0.5+width)*dir.x(),height*dir.y());
    tl=pos+QPointF((interval*0.5)*dir.x(),height*dir.y());
    bl=pos+QPointF((interval*0.5)*dir.x(),0.0);
    br=pos+QPointF((interval*0.5+width)*dir.x(),0.0);

    rc=pos+QPointF((interval*0.5+width)*dir.x(),height*0.5*dir.y());
    lc=pos+QPointF((interval*0.5)*dir.x(),height*0.5*dir.y());

    mCodeNumber(code,QString(numString.at(1)).toInt(),tl,tr,br,bl,lc,rc);
}

void MotionCompiler::mCodeNumber(QString &code, qint32 number, const QPointF &tl, const QPointF &tr, const QPointF &br, const QPointF &bl, const QPointF &lc, const QPointF &rc)
{
    switch (number) {
    case 0:{
        code.append(A3200_G0(br));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());

        code.append(A3200_G1(tr));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(tl));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(bl));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(br));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_PSO_OFF());
    }
        break;
    case 1:{
        code.append(A3200_G0(br));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());

        code.append(A3200_G1(tr));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_PSO_OFF());
    }
        break;
    case 2:{
        code.append(A3200_G0(tl));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());

        code.append(A3200_G1(tr));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(rc));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(lc));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(bl));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(br));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_PSO_OFF());
    }
        break;
    case 3:{
        code.append(A3200_G0(tl));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());

        code.append(A3200_G1(tr));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(br));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(bl));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_OFF());

        code.append(A3200_G0(lc));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());

        code.append(A3200_G1(rc));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_OFF());


    }
        break;
    case 4:{
        code.append(A3200_G0(tl));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());

        code.append(A3200_G1(lc));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(rc));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(br));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_OFF());

        code.append(A3200_G0(rc));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());

        code.append(A3200_G1(tr));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_OFF());
    }
        break;
    case 5:{
        code.append(A3200_G0(tr));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());

        code.append(A3200_G1(tl));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(lc));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(rc));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(br));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(bl));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_PSO_OFF());
    }
        break;
    case 6:{
        code.append(A3200_G0(tr));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());

        code.append(A3200_G1(tl));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(bl));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(br));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(rc));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(lc));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_PSO_OFF());
    }
        break;
    case 7:{
        code.append(A3200_G0(tl));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());

        code.append(A3200_G1(tr));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(br));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_PSO_OFF());
    }
        break;
    case 8:{
        code.append(A3200_G0(br));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());

        code.append(A3200_G1(tr));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(tl));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(bl));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(br));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_PSO_OFF());

        code.append(A3200_G0(lc));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());

        code.append(A3200_G1(rc));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_OFF());
    }
        break;
    case 9:{
        code.append(A3200_G0(bl));
        code.append(A3200_WAIT_INPOS());
        code.append(A3200_PSO_ON());

        code.append(A3200_G1(br));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(tr));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(tl));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(lc));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_G1(rc));
        code.append(A3200_WAIT_INPOS());

        code.append(A3200_PSO_OFF());
    }
        break;
    default:
        break;
    }
}

