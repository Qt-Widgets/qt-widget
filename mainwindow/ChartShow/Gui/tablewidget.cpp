

#include "tablewidget.h"
#include "customtablemodel.h"
#include "hdatacollectsetting.h"

#include "hsamplewidget.h"
#include "Src/hdatacollect.h"

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTableView>
#include <QtWidgets/QHeaderView>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QVXYModelMapper>

#include <QDebug>


TableWidget::TableWidget(QWidget *parent)
    : QWidget(parent)
{
    // create simple model for storing data
    // user's table data model
    //! [1]

    HDataCollectSetting* ptrCollectionSetting=HDataCollectSetting::GetInstance();

    QVariantList inamelist=ptrCollectionSetting->getNameList();
    QVariantList icommondlist=ptrCollectionSetting->getCommondList();
    ptrModel = new CustomTableModel(inamelist,this);
    ptrDataCollector=HDataCollect::GetInstance(icommondlist,this);

    connect(HSampleWidget::GetInstance(),&HSampleWidget::ParaSaved,this,&TableWidget::RefreshSettings);

    connect(ptrCollectionSetting,&HDataCollectSetting::SettingsChanged,this,&TableWidget::createDataCollection);
    //! [2]
    // create table view and add model to it
    ptrTableView= new QTableView;

    ptrTableView->setMinimumSize(400, 480);

    ptrTableView->setModel(ptrModel);
    ptrTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ptrTableView->verticalHeader()->hide();
    //! [2]

    //! [3]
    ptrChart = new QChart;
    ptrChart->setAnimationOptions(QChart::AllAnimations);
    ptrChart->setFont(this->font());
    //! [3]

    ptrModel->addMapping("gray", QRect(0, 0, 1, ptrModel->rowCount()));

    createDataCollection(inamelist,icommondlist);

    ///////////////////////////////
    ///////////////////////////////

    //! [8]

    QChartView *chartView = new QChartView(ptrChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(640, 480);
    //! [8]

    //! [9]
    // create main layout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(ptrTableView, 1, 0);
    mainLayout->addWidget(chartView, 1, 1);
    mainLayout->setColumnStretch(1, 1);
    mainLayout->setColumnStretch(0, 0);
    setLayout(mainLayout);
    //! [9]

    if(!inamelist.isEmpty())
        RefreshSettings();
}

QString TableWidget::GetDataString() const
{
    auto data= ptrModel->GetMData();
    QString str;
    for(const auto&k:data){
        for(const auto&m:*k){
            str.append(QString::number(m.toDouble(),'f',4));
            str.append("         ");
        }
        str.append("\n");
    }
    return str;
}

void TableWidget::appendData()
{

    auto data=ptrDataCollector->GetData();
    ptrModel->appendData(data);
    ptrTableView->scrollToBottom();

    double fdata=data.first().toDouble();

    if(fdata>rangeXmax){
        double kmg=fdata-rangeXmax;
        ptrChart->scroll(ptrChart->plotArea().width()/(rangeXmax-rangeXmin)*kmg, 0);
        rangeXmin=rangeXmin+kmg;
        rangeXmax=rangeXmax+kmg;
    }


}

void TableWidget::clearAllData()
{
    ptrModel->clearAllData();
    for(auto k:ptrChart->series()){
        qobject_cast<QLineSeries*>(k)->clear();
    }
    RefreshSettings();
}


void TableWidget::RefreshSettings()
{
    auto ptrSettings=HSampleWidget::GetInstance();

    rangeXmin=ptrSettings->GetPara(SampleParaType::SAMPLE_X_MIN);
    rangeXmax=ptrSettings->GetPara(SampleParaType::SAMPLE_X_MAX);
    rangeYmin=ptrSettings->GetPara(SampleParaType::SAMPLE_Y_MIN);
    rangeYmax=ptrSettings->GetPara(SampleParaType::SAMPLE_Y_MAX);

    ptrChart->axes().first()->setRange(rangeXmin,rangeXmax);
    ptrChart->axes().at(1)->setRange(rangeYmin,rangeYmax);

}


void TableWidget::createDataCollection(const QVariantList &namelist,const QVariantList &commondlist)
{
    //qDebug()<<namelist<<commondlist;

    QStringList colors;
    QString seriesColorHex = "#A9A9A9";
    // series 1
    ////////////////////

    ptrDataCollector->setInstructions(commondlist);
    ptrChart->removeAllSeries();

    colors<<seriesColorHex;
    for(qint32 k=0;k<namelist.size();++k){

        QLineSeries *series = new QLineSeries;
        //series->setUseOpenGL(true);
        series->setName(namelist.at(k).toString());
        QVXYModelMapper *mapper = new QVXYModelMapper(this);
        mapper->setXColumn(0);
        mapper->setYColumn(k+1);
        mapper->setSeries(series);
        mapper->setModel(ptrModel);
        ptrChart->addSeries(series);
        //! [4]

        //! [5]
        // for storing color hex from the series


        // get the color of the series and use it for showing the mapped area
        seriesColorHex = "#" + QString::number(series->pen().color().rgb(), 16).right(6).toUpper();

        colors<<seriesColorHex;
    }

    //    QLineSeries *series = new QLineSeries;
    //    //series->setUseOpenGL(true);
    //    series->setName("Crystal");
    //    QVXYModelMapper *mapper = new QVXYModelMapper(this);
    //    mapper->setXColumn(0);
    //    mapper->setYColumn(1);
    //    mapper->setSeries(series);
    //    mapper->setModel(ptrModel);
    //    ptrChart->addSeries(series);
    //    //! [4]

    //    //! [5]
    //    // for storing color hex from the series

    //    colors<<seriesColorHex;
    //    // get the color of the series and use it for showing the mapped area
    //    seriesColorHex = "#" + QString::number(series->pen().color().rgb(), 16).right(6).toUpper();

    //    colors<<seriesColorHex;


    //    //! [5]

    //    //////////////////////////////
    //    // series 2
    //    //! [6]
    //    series = new QLineSeries;
    //    //series->setUseOpenGL(true);
    //    series->setName("TD1");

    //    mapper = new QVXYModelMapper(this);
    //    mapper->setXColumn(0);
    //    mapper->setYColumn(2);
    //    mapper->setSeries(series);
    //    mapper->setModel(ptrModel);
    //    ptrChart->addSeries(series);

    //    seriesColorHex = "#" + QString::number(series->pen().color().rgb(), 16).right(6).toUpper();
    //    colors<<seriesColorHex;

    //    ////////////////////////

    //    series = new QLineSeries;
    //    //series->setUseOpenGL(true);
    //    series->setName("Laser-head");

    //    mapper = new QVXYModelMapper(this);
    //    mapper->setXColumn(0);
    //    mapper->setYColumn(3);
    //    mapper->setSeries(series);
    //    mapper->setModel(ptrModel);
    //    ptrChart->addSeries(series);

    //    seriesColorHex = "#" + QString::number(series->pen().color().rgb(), 16).right(6).toUpper();
    //    colors<<seriesColorHex;

    //    ////////////////////////////

    //    series = new QLineSeries;
    //    //series->setUseOpenGL(true);
    //    series->setName("ENV");

    //    mapper = new QVXYModelMapper(this);
    //    mapper->setXColumn(0);
    //    mapper->setYColumn(4);
    //    mapper->setSeries(series);
    //    mapper->setModel(ptrModel);
    //    ptrChart->addSeries(series);

    //    seriesColorHex = "#" + QString::number(series->pen().color().rgb(), 16).right(6).toUpper();
    //    colors<<seriesColorHex;

    ptrModel->setColors(colors);
    ptrModel->setHeadNames(namelist);

    ptrChart->createDefaultAxes();
}



