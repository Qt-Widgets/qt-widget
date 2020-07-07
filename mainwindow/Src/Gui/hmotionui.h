#ifndef HMOTIONUI_H
#define HMOTIONUI_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QSlider;
class QLineEdit;
class QBoxLayout;
QT_END_NAMESPACE

class H_MotionController;
class HConfigCtr;
class HConfigWidget;

class HMotionUI : public QWidget
{
    Q_OBJECT
public:
    explicit HMotionUI(QWidget *parent = nullptr);

signals:

private slots:

    void refreshAxisStatus();

    void reCreatWids();

private:

    QBoxLayout *allLayout;

    QWidget * ptrWid1;
    QWidget * ptrWid2;
    QWidget * ptrWid3;

    QWidget *creatStatusWid(const QStringList &names);
    QWidget *creatSettingWid();
    QWidget *creatOperateWid(const QStringList &names);



    QVector<QLabel*> ptrLabels_status;
    QVector<QLabel*> ptrLabels_position;
    QVector<QLabel*> ptrLabels_velocity;


    QLineEdit *lineEdit;
    H_MotionController *ptrMotion;
    HConfigWidget *ptrConfgiWid;


   const  QIcon iconLeft=QIcon(":/resource/icons/left2.svg");
   const  QIcon iconRight=QIcon(":/resource/icons/right1.svg");


    enum MoveMode{
        move_continuous=0,
        move_step_10,
        move_step_100,
        move_step_1000,
        move_step_custom,
    };

    MoveMode enuMovemode;


};

#endif // HMOTIONUI_H
