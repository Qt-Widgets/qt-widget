#ifndef HDIALOGSYSTEMADJUST_H
#define HDIALOGSYSTEMADJUST_H

#include <QDialog>

namespace Ui {
class HDialogSystemAdjust;
}

class QListWidget;
class QListWidgetItem;
class H_MotionController;
class HCameraController;
class HSettings;
class HTableView;
class QToolButton;
class HSystemAdjustController;
class HParaLibController;
class QComboBox;
class H_CameraControllerWidget;
class IniSettings;

class HDialogSystemAdjust : public QDialog
{
    Q_OBJECT

public:
    explicit HDialogSystemAdjust(QWidget *parent = nullptr, Qt::WindowFlags f =Qt::WindowCloseButtonHint|Qt::WindowMinMaxButtonsHint);
    ~HDialogSystemAdjust() override;

private slots:

    void handlerCurrentItemChanged(qint32 rowIndex,qint32 itemIndex);
    void on_tabWidget_currentChanged(int index);
    void on_toolButton_camera_location_run_clicked();
    void on_toolButton_vision_run_clicked();

    void on_pushButton_first_cut_clicked();
    void on_pushButton_first_cal_clicked();

    void on_toolButton_cutter_run_clicked();



protected:
    virtual void showEvent(QShowEvent *event) override;

private:

    void showMsg(const QColor&color,const QString&msg);

    QVector<QString> getCurrentContents(qint32 index) const;
    void setCurrentContents(qint32 index, const QVector<QString> &datas);


    QVector<HTableView*> tableViewArray;
    QVector<QToolButton*> tableViewClearButtonArray;
    QVector<QToolButton*> tableViewAddButtonArray;
    QVector<QToolButton*> tableViewDeleteButtonArray;
    QVector<QToolButton*> tableViewSaveButtonArray;
    QVector<QToolButton*> tableViewSetButtonArray;


    QVector<QVector<QWidget*>> contentWidArray;

    QVector<QVector<QWidget*>> enableWidArray;

    QVector<QVector<QComboBox*>> combboxWidArray;


    H_MotionController *ptrMotion;
    HCameraController *ptrCameraDevice;
    HSettings*ptrSettings;
    IniSettings *ptrIniSetings;
    HSystemAdjustController *ptrAdjustController;
    HParaLibController *ptrParaLibController;

    H_CameraControllerWidget *ptrCameraShow;

    Ui::HDialogSystemAdjust *ui;
};

#endif // HDIALOGSYSTEMADJUST_H
