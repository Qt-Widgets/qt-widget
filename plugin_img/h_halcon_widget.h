#ifndef H_HALCON_WIDGET_H
#define H_HALCON_WIDGET_H

#include <QWidget>

#include "HalconCpp.h"

QT_FORWARD_DECLARE_CLASS(QFrame)
class HImgProcessMWindow;

class H_Halcon_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit H_Halcon_Widget(QWidget *parent = nullptr);

    const OpencvCpp::HTuple *  getHWindowHandlerPtr() const {return &h_WindowHandle;}
    const OpencvCpp::HTuple &  getHWindowHandler() const {return h_WindowHandle;}


    const OpencvCpp::HObject *getImagePtr() const{return &hImage;}
    const OpencvCpp::HObject &getImage() const{return hImage;}
    OpencvCpp::HObject &getImage() {return hImage;}



    void setImgSize(qint32 w, qint32 h);
    bool showImage(uchar *pData,qint32 width,qint32 height);


    bool showRect(double x1,double y1,double x2,double y2);
    bool showCircle(double x,double y,double radius);

    bool drawCircle(QPointF &center,double & radius);
    bool drawRect(QPointF &topLeft,QPointF &bottomRight);
    bool drawRect1(QPointF &rowColumn,QPointF &length1_2,double &phi);

    bool createCircleXLDTemplate(const QString &filePath,const QStringList&paras,double cx,double cy, double radius);
    bool createRectRegion(double x1,double y1,double x2,double y2,bool isOn=true);

    bool templateProcess(const QString &filePath,const QStringList&paras,double &cx,double &cy,double &angle,double value=100,bool isShow=true);

    bool callipersCircleProcess(const QStringList&paras,double &cx,double &cy,double &radius);
    bool callipersLineProcess(const QStringList&paras,QLineF&line);
    bool noneReProcess(double &cx,double &cy,double &radius);

protected:
    void resizeEvent(QResizeEvent*) override;
    bool eventFilter (QObject * obj, QEvent * event ) override;
    //void paintEvent(QPaintEvent *event) override;


public slots:

    bool clearWindow();
    bool resetWindow();

    bool showImageFromFile();


private:

    qint32 windowMode=0;

    HImgProcessMWindow *parentWindow;

    QFrame*viewPort=nullptr;

    QVector<OpencvCpp::HObject> hShowObjs;
    QMap<QString,OpencvCpp::HTuple> hTemplates;
    OpencvCpp::HObject hImage;
    OpencvCpp::HObject hImageRegion;
    OpencvCpp::HObject hCross;
    qint32 imgWidth=0,imgHeight=0;
    //QTransform viewToPortTrans=QTransform::fromTranslate(0,0);
    double imgSizeRate;

    void GetPartFloat(double *row1, double *col1, double *row2, double *col2);
    void SetPartFloat(double row1, double col1, double row2, double col2);

    QPoint lastMousePos;
    double lastRow1, lastCol1, lastRow2, lastCol2;


    OpencvCpp::HTuple h_WindowHandle;
};

#endif // H_HALCON_WIDGET_H
