#ifndef H_CAMERACONTROLLERWIDGET_H
#define H_CAMERACONTROLLERWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPainterPath>
#include <QThread>


class HCameraController;

class H_CameraShow;
class H_Wid_Common;
class H_MotionController;

QT_BEGIN_NAMESPACE
class QTabWidget;
class QToolBar;
class QAction;
class QLabel;
class QStatusBar;
class QDialog;
QT_END_NAMESPACE



//struct HMouseInfo
//{
//    qint32 x=-1;
//    qint32 y=-1;
//    qint32 pixelRed=0;
//    qint32 pixelGreen=0;
//    qint32 pixelBlue=0;
//};


class CameraShowThread : public QThread
{
    Q_OBJECT

public:
    explicit CameraShowThread(QObject *parent = nullptr);
    ~CameraShowThread() override;


    qint32 cameraIndex=-1;
    HCameraController *ptrCamera;
    quint32 waitTime=30;

signals:

    void cameraCaptureDone(uchar *ptrRed,uchar *ptrGreen,uchar *ptrBlue);

private:

    //QMutex m_mutex;
    void run() override;
};



class H_CameraControllerWidget : public QWidget
{
    Q_OBJECT
public:
    static H_CameraControllerWidget* GetInstance(QWidget *parent = nullptr);

    //////////////////////////////////////
    uchar* getCurrentImage(qint32 &width,qint32 &height);
    QPointF getCurrentImageCpt();
    void setImage();
    qint32 showOneImg(qint32 index);
    qint32 showOneImg(const QString & name);

    void stopContinuesCapture();


    void addShowText(const QString& msg,const QColor &color,const QPointF& pos=QPointF(10,30));
    void addShowCross(const QPointF& pos);
    void addShowEllipse(const QPointF&center,double radius1,double radius2);



private:
    explicit H_CameraControllerWidget(QWidget *parent = nullptr);
    ~H_CameraControllerWidget() Q_DECL_OVERRIDE;
    static H_CameraControllerWidget* m_pInstance;


protected:
    //void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    // bool eventFilter(QObject *target, QEvent *event) Q_DECL_OVERRIDE;

private slots:

    void reCreatWindow();

    void currentCameraChange(qint32 index);

    void refreshStatusPosInfo(const QPointF &pos,const QColor &color);

    void connectCamera(bool ope);
    void showImgContinues(bool ope);
    void showCrossLine(bool ope);
    void showDrawCircle(bool ope);

    void showImg(uchar *ptrRed,uchar *ptrGreen,uchar *ptrBlue);

    qint32 showImgOne();
    void saveImg();
    void openImg();
    void showWindowNormal();
    void clearWindow();
    void openSettingDialog();

private:


    //GUI


    QWidget* creatStatusBar();
    QStatusBar*ptrStatusBar;
    QWidget* creatTabWidget();
    QTabWidget *ptrTabWid;
    QWidget* creatToolBar();
    QToolBar *ptrToolBar;


    qint32 m_currenCameraIndex=-1;

    CameraShowThread m_cameraShowThread;


    qint32 showContinuousImg();
    void stopContinuousImg();



    enum ToolButton{
        Connect=0,
        Shot_Continues,
        show_cross,



        Shot_Once,
        Save_img,
        Open_img,

        Show_normal,
        Clear_window,
        Open_setting_dialog,

        show_DrawCircle,

    };

    enum StatusLabel{
        Transport_info=0,
        ImgSize_info,
        fps_info,
        Pixel_info,
        Scale_info
    };

    QVector <H_CameraShow*> tabList;
    QVector <QAction*> actionList;
    QVector <QLabel*> labelList;

    H_MotionController *ptrMotion;


    H_Wid_Common* paraWidget;


    HCameraController *ptrCameraDevice;
    // H_HalconImgProcess *ptrProcesser;

    struct DeviceInfo{
        qint32 width=100;
        qint32 height=100;
        qreal fps=0.0;
    } device_info;

    bool isShowCross=true;

    qint32 getCameraInfo(qint32 index);
    void openCameraShow();

signals:

public slots:

};

#endif // H_CAMERACONTROLLERWIDGET_H
