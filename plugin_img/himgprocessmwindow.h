#ifndef HIMGPROCESSMWINDOW_H
#define HIMGPROCESSMWINDOW_H

#include <QMainWindow>
class H_Halcon_Widget ;
class HImgPlugin;

QT_FORWARD_DECLARE_CLASS(QToolBar)
QT_FORWARD_DECLARE_CLASS(QActionGroup)

namespace Ui {
class HImgProcessMWindow;
}

class DialogCameraSetting;

class HImgProcessMWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HImgProcessMWindow(HImgPlugin * pInterface,QWidget *parent = nullptr);
    ~HImgProcessMWindow() override;

    bool showImage(uchar *pImage,qint32 width,qint32 height);
    bool processImage(const QString &name,QPointF &result);
    bool processImageLine(const QString &name,QLineF &result);
    const QStringList & getFuncNames() const {return funcNames;}
    const QString & getDefaultFuncName() const {return currentParas;}
    qint32 moveToFocus() const;

    void resetCameras(const QStringList &names);
    void resetAxises(const QStringList &names);

    void showStatusMessage(const QString &msg);


    friend QDataStream &operator>>(QDataStream & input, HImgProcessMWindow &obj);
    friend QDataStream &operator<<(QDataStream & output, const HImgProcessMWindow &obj);

    QString getAxisXName() const {return axisXName;}
    QString getAxisYName() const {return axisYName;}
    QString getAxisZName() const {return axisZName;}

protected:
    void closeEvent(QCloseEvent *event) override;

signals:

    void axisMoveTo(const QString &name,double pos);

private slots:

    bool getImage();
    void getImageContinues(bool isChecked);
    void saveParas();
    bool processImageTest();

    void regionDrawRect();
    void regionShowRect();

    void templateDrawCircle();
    void templateShowCircle();
    bool templateCircle(double& cx,double &cy,double &angle,double radius,bool isShow=true);
    void templateTestCircle();

    bool reprocessCircleCallipers(double& cx, double &cy, double &radius);
    void reprocessCircleCallipersTest();


    void on_toolButton_new_clicked();

    void on_toolButton_delete_clicked();

    void on_toolButton_clear_clicked();

    void on_toolButton_set_default_clicked();

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_reprocess_line_callipers_draw_clicked();

    void on_reprocess_line_callipers_test_clicked();

private:

    bool writeToFile(const QString &filePath);
    bool openFromFile(const QString &filePath);

    /////////////
    /// \brief setupToolBar
    void setupCenterWid();
    void setupToolBar();
    void setupMenuBar();
    void setupImageProcessDockWid();

    ///////////////

    void initControl();
    void refreshControl();

    ////////
    /// \brief m_ActionSave
    QAction *m_ActionShotContinues;
    QAction *m_ActionSave;
    QAction *m_ActionTest;
    QAction *m_ActionMoveFocus;
    void setControlEnabled(bool checked);

    void newItem(const QString& name,bool isNew=true,bool isActived=false);
    void getSaveName(const QStringList &names,const QString &oname, QString &name, qint32 index=0);
    bool checkSameName(const QString & text,QListWidget *wid);

    const QIcon defaultIcon=QIcon(":/resource/img/whiteBlank.png");

    /////////////

    QString currentParas;
    QStringList funcNames;
    QMap<QString,QStringList> imgProcessParas;

    double axisFocus=0.0;

    HImgPlugin * ptrPlugin;

    Ui::HImgProcessMWindow *ui;
    H_Halcon_Widget *camShow;
    QToolBar *ptrToolBar;
    QTimer *ptrTimer1;

    QString cameraName;
    QString axisXName;
    QString axisYName;
    QString axisZName;


    QActionGroup *ptrCameraGroup;
    QActionGroup *ptrAxisXGroup;
    QActionGroup *ptrAxisYGroup;
    QActionGroup *ptrAxisZGroup;


    ///////

    DialogCameraSetting* ptrCameraSetting;
};

#endif // HIMGPROCESSMWINDOW_H
