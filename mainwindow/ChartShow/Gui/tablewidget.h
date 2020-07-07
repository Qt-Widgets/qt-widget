

#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChart>

QT_CHARTS_USE_NAMESPACE

class CustomTableModel;
class HDataCollect;
class QTableView;

class TableWidget : public QWidget
{
    Q_OBJECT
public:
    TableWidget(QWidget *parent = nullptr);

    QString GetDataString() const;

    void appendData();

    void clearAllData();



private slots:

    void RefreshSettings();



    void createDataCollection(const QVariantList &namelist,const QVariantList &commondlist);
private:

    qreal rangeXmin;
    qreal rangeXmax;
    qreal rangeYmin;
    qreal rangeYmax;



    CustomTableModel *ptrModel;
    HDataCollect *ptrDataCollector;
    QChart *ptrChart;
    QTableView * ptrTableView;

};

#endif // TABLEWIDGET_H
