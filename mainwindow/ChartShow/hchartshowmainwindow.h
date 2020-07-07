#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class HChartShowMainWindow;
}

class  TableWidget;
class QTimer;
class HDataCollect;
class  HSerialDialog;

class HChartShowMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HChartShowMainWindow(QWidget *parent = nullptr);
    ~HChartShowMainWindow();

public slots:


private slots:

    void collectData();

    void startCollect();
    void stopCollect();
    void ClearData();
private:

    QVector<QAction*> listAction;
    Ui::HChartShowMainWindow *ui;


    TableWidget *ptrTableWidget;
    HSerialDialog *ptrSerialDialog;
    HDataCollect   *ptrDataCollector;
    QTimer *ptrTimer;

    qint32 timeCount=0;

};

#endif // MAINWINDOW_H
