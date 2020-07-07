#ifndef DIALOGCAMERASETTING_H
#define DIALOGCAMERASETTING_H

#include <QDialog>

namespace Ui {
class DialogCameraSetting;
}
class HImgPlugin;

class DialogCameraSetting : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCameraSetting(HImgPlugin *ptrInterfance,QWidget *parent = nullptr);
    ~DialogCameraSetting();



    void setCurrentCamera(const QString &cname);


    double getFocus() const;

    void initData(const QStringList&data);
    QStringList getSaveData() const;

public slots:

    qint32 moveToFcous();

private:
    Ui::DialogCameraSetting *ui;

     HImgPlugin * ptrPlugin;
     QString cameraName;
};

#endif // DIALOGCAMERASETTING_H
