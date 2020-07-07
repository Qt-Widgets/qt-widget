#ifndef MOTIONCOMPILER_H
#define MOTIONCOMPILER_H
#include <QObject>

#include "Src/Gui/settings/hparalibcontroller.h"


class MotionCompiler : public QObject
{
    Q_OBJECT
public:
    static MotionCompiler* GetInstance(QObject *parent = nullptr);


    bool createGcodeAndRun(const QString&axisX,const QString&axisY,const QString&axisZ,
            qint32 range=1);

    bool createFocusGcodeAndRun(const QString&axisX,const QString&axisY,const QString&axisZ,
            const QString &namePara,qint32 type,double valueStart,double valueStep,const QPointF& pos,qint32 num,bool useMark,qint32 markType,double markSize,double step);

    bool createMarkGcodeAndRun(const QString&axisX,const QString&axisY,const QString&axisZ,
            const QPointF& pos,qint32 markType,double size,const QString& paraName=nullptr);

    qint32 createPointsAndRunWait(const QString&axisX,const QString&axisY,const QString&axisZ,
                                  const QVector<QPointF> & pts,qint32 markType,double size,const QString& paraName=nullptr);

    bool RunTask1();
    bool RunTask1AndWait();

    bool RunTask2(const QString &code );

    bool RunAdjust1();
    bool RunAdjust1AndWait();


    bool RegistrFirstCallBack();
    bool CallbackWaitCancle();
    bool WaitCallback(QVariantList &args);
    bool CallbackDone(double returnCode);


    bool StopProgram();

    bool isProgramRunning();

    static QString A3200_DWELL(double time);
    static QString A3200_WAIT_INPOS();
    static QString A3200_PSO_FIRE_ONE();

    static QString A3200_Paras(const QStringList &paras);
    static QString A3200_CALLBACK(int type, const QString& var1);
    static QString A3200_PVT(const QPointF& xy,double speedX,double speedY,double time);
    static QString A3200_G0_Z(double pos);
    static QString A3200_G0(const QPointF& xy);
    static QString A3200_G1(const QPointF& xy);
    static QString A3200_G2_G3(bool isCCW,const QPointF& xy,double i,double j);

    static QString A3200_PSO_ON();
    static QString A3200_PSO_OFF();

    static QString A3200_JUMP_ACC(double acc);
    static QString A3200_JUMP_SPEED(double vel);
    static QString A3200_COMPLEX_ACC(double acc);
    static QString A3200_COMPLEX_SPEED(double vel);
    static QString A3200_AXIS_Z(double pos);



private:

    static MotionCompiler* m_pInstance;
    explicit MotionCompiler(QObject *parent = nullptr);
    ~MotionCompiler() Q_DECL_OVERRIDE;


    void addNumber(const QPointF &pos,const QPointF&dir,QString &code, qint32 number, double size);

    void mCodeNumber(QString &code,qint32 number,const QPointF&tl,const QPointF&tr,const QPointF&br,const QPointF&bl,const QPointF&lc,const QPointF&rc);

};

#endif // MOTIONCOMPILER_H
