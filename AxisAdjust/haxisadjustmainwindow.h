#ifndef HAXISADJUSTMAINWINDOW_H
#define HAXISADJUSTMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class HAxisAdjustMainWindow;
}

class HAxisAdjustPlugin;

class HAxisAdjustMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HAxisAdjustMainWindow(HAxisAdjustPlugin *ptrPlugin,QWidget *parent = nullptr);
    ~HAxisAdjustMainWindow();

    void initWindow();
    void refreshWindow(const QStringList&axisNames,const QStringList&funcNames);

    friend QDataStream &operator>>(QDataStream & input, HAxisAdjustMainWindow &obj);
    friend QDataStream &operator<<(QDataStream & output, const HAxisAdjustMainWindow &obj);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_toolButton_1_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_3_clicked();

private:
    QStringList getControlData() const;
    void setControlData(const QStringList& data);

    bool writeToFile(const QString &filePath);
    bool openFromFile(const QString &filePath);

    Ui::HAxisAdjustMainWindow *ui;

    bool isStop=false;

    QStringList dataHeader;

    HAxisAdjustPlugin * ptrPluginInterface=nullptr;
};

#endif // HAXISADJUSTMAINWINDOW_H
