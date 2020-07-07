#ifndef HMATH_H
#define HMATH_H

#include <QPointF>
#include <QLineF>
#include <QtCore/qmath.h>
#include <QDebug>
#include <limits>
#include <qglobal.h>

#ifndef H_ANGLE_TO_RADIAN
#define H_ANGLE_TO_RADIAN (0.0174532925199433)
#endif

#ifndef H_ABS
#define H_ABS(x) ( (x)>0?(x):-(x) )
#endif

#ifndef H_EQUAL
#define H_EQUAL(x, y) qAbs(x-y)<0.0000001
#endif

#ifndef H_EQUAL2
#define H_EQUAL2(x, y) qAbs(x-y)<0.001
#endif

#ifndef H_ANGMLE_LINE
#define H_ANGMLE_LINE(x, y) ( H_EQUAL2(x,y)||H_EQUAL2(H_ABS(x-y),180.0)||H_EQUAL2(H_ABS(x-y),360.0) )
#endif

#ifndef H_NO_EQUAL
#define H_NO_EQUAL(x, y) qAbs(x-y)>0.001
#endif

#ifndef H_SMALLER_EQUAL
#define H_SMALLER_EQUAL(x, y) x<y||qAbs(x-y)<0.0001
#endif




#ifndef H_DOUBLE_MAX
#define H_DOUBLE_MAX 1.5e300
#endif


namespace H_Math {



template <typename T>
constexpr inline void QuickSort(QVector<T> & data,int low,int high){

    if(data.size()<2)
        return;
    int i = low;
    int j = high;
    T key = data.at(low);
    if (low >= high)
    {
        return ;
    }
    while (low < high)
    {
        while (low < high && key <= data.at(high))
        {
            --high;
        }
        if (key > data.at(high))
        {
            data[low] = data[high];
            ++low;
        }
        while (low < high && key >= data.at(low))
        {
            ++low;
        }
        if (key < data.at(low))
        {
            data[high] = data[low];
            --high;
        }
    }
    data[low] = key;
    QuickSort(data, i, low-1);
    QuickSort(data, low+1, j);
}

template <typename T>
constexpr inline void ShellSort(QVector<T> & data){
    qint32 gap=0;
    qint32 len=data.size();
    for(gap = 3; gap >0; gap--)
    {
        for(qint32 i=0; i<gap; i++)
        {
            for(qint32 j = i+gap; j<len; j=j+gap)
            {
                if(data[j]<data[j-gap])
                {
                    T temp =data[j];
                    qint32 k = j-gap;
                    while(k>=0&&data[k] > temp)
                    {
                        data[k+gap] = data[k];
                        k = k-gap;
                    }
                    data[k+gap] = temp;
                }
            }
        }
    }

}

template <typename T>
constexpr inline void ShellSort_R(QVector<T> & data){
    qint32 gap=0;
    qint32 len=data.size();
    for(gap = 3; gap >0; gap--)
    {
        for(qint32 i=0; i<gap; i++)
        {
            for(qint32 j = i+gap; j<len; j=j+gap)
            {
                if(data[j]>data[j-gap])
                {
                    T temp =data[j];
                    qint32 k = j-gap;
                    while(k>=0&&data[k] < temp)
                    {
                        data[k+gap] = data[k];
                        k = k-gap;
                    }
                    data[k+gap] = temp;
                }
            }
        }
    }

}


//convert

static qreal convertAngle(double angle){

    if(angle>2*M_PI){
        return angle-qint32(angle/(2*M_PI))*2*M_PI;
    }else if(angle>-2*M_PI){
        return angle+qint32(-angle/(2*M_PI))*2*M_PI;
    }else{
        return angle;
    }

}



static  qreal get2PointDis(const QPointF &p1,const QPointF &p2){

    return qSqrt(qPow(p1.x()-p2.x(),2.0)+qPow(p1.y()-p2.y(),2.0));
}


static   QPointF GetLineP2( QPointF P1,double length,double angle){

    if(H_EQUAL(length,0.0)){
        return P1;
    }
    if(angle>0){
        if(angle>360.0){
            angle=angle-qint64(angle/360.0)*360;
        }
    }
    else{
        if(angle<-360.0){
            angle=angle+qint64(angle/-360.0)*360.0;
        }
        else{
            angle+=360.0;
        }
    }

    QLineF line;
    line.setP1(P1);
    line.setAngle(angle);
    line.setLength(length);
    return line.p2();
}

static QPointF GetLineP1( QPointF P2,double length,double angle){

    if(H_EQUAL(length,0.0)){
        return P2;
    }

    if(angle>0){
        if(angle>360.0){
            angle=angle-qint64(angle/360.0)*360;
        }
    }
    else{
        if(angle<-360.0){
            angle=angle+qint64(angle/-360.0)*360.0;
        }
        else{
            angle+=360.0;
        }
    }



    QLineF line;

    line.setP1(P2);
    line.setLength(length);
    line.setAngle(angle+((angle<180.0)?180.0:-180.0));

    return line.p2();
}

static double GetPointToLineDis(const QPointF& pt,const QLineF &line){

    double angle=line.angle();
    double diss=get2PointDis(pt,line.p1())+0.1;

    QLineF line1;
    line1.setP1(pt);
    line1.setAngle(angle+90);
    line1.setLength(diss);

    QLineF line2;
    line2.setP1(pt);
    line2.setAngle(angle+270);
    line2.setLength(diss);

    QPointF crossPoint;
    if(line1.intersect(line,&crossPoint)==QLineF::BoundedIntersection
            ||line2.intersect(line,&crossPoint)==QLineF::BoundedIntersection){
        return get2PointDis(crossPoint,pt);
    }else{
        return qMin(get2PointDis(pt,line.p1()),get2PointDis(pt,line.p2()));
    }
}

class HLine
{
public:
    enum HIntersectType {SameLineCrossDiffDire,SameLineNoCrossDiffDire ,SameLineNoCrossSameDire, SameLineCrossSameDire,Parallel,CrossOnLine,CrossOutLine };
    HLine(){ }
    HLine(const QLineF &mlinef): line(mlinef){  }
    HLine(const HLine &mlinef): line(mlinef.line){  }
    HLine(const QPointF &apt1, const QPointF &apt2): line(apt1,apt2){  }
    HLine(qreal x1, qreal y1, qreal x2, qreal y2): line(x1, y1,x2, y2){  }
    HLine(const QLine &mline) : line(mline){ }

    HIntersectType GetIntersection(const HLine& line2, QPointF* crossPoint=nullptr) const {
        QPointF ptm;
        if(crossPoint==nullptr){
            crossPoint=&ptm;
        }

        switch (line.intersect(line2.line,crossPoint)) {
        case QLineF::IntersectType::NoIntersection:{
            //qDebug()<<line.angle()<<"ssss"<<QLineF(line.p1(),line2.p2()).angle();


            if(line.p1()==line2.line.p1()){
                return H_EQUAL2(line.angle(),line2.line.angle())?SameLineCrossSameDire:SameLineCrossDiffDire;
            }

            bool flag=get2PointDis(line.center(),line2.line.center())>(line.length()+line2.line.length())*0.5;

            if(H_ANGMLE_LINE(line.angle(),QLineF(line.p1(),line2.line.p1()).angle())){

                if(H_EQUAL2(line.angle(),line2.line.angle())){
                    if(flag){
                        return SameLineNoCrossSameDire;
                    }
                    else{
                        return  SameLineCrossSameDire;
                    }
                }else{
                    if(flag){
                        return SameLineNoCrossDiffDire;
                    }
                    else{
                        return  SameLineCrossDiffDire;
                    }
                }
            }
            return Parallel;
        }
        case QLineF::IntersectType::BoundedIntersection:{
            return CrossOnLine;
        }
        case QLineF::IntersectType::UnboundedIntersection:{
            return CrossOutLine;
        }
        }
    }

    HLine &swapDirection(){
        line.setPoints(line.p2(),line.p1());
        return *this;
    }

    HLine getReserveLine() const{
        return HLine(line.p2(),line.p1());
    }


    inline HLine& operator = (const HLine& data) {
        if(this==&data)
            return *this;
        line.setPoints(data.line.p1(),data.line.p2());
        return *this;
    }

    inline HLine& operator = (const QLineF& data) {
        if(this->line==data)
            return *this;
        line.setPoints(data.p1(),data.p2());
        return *this;
    }

    inline bool operator < (const QLineF& data) const{

        double ang=line.angle();
        if(H_EQUAL2(ang,0.0)||H_EQUAL2(ang,360.0)){
            return line.x1()<data.x1();
        } else if(H_EQUAL2(ang,180.0)) {
            return line.x1()>data.x1();
        }else{
            if(ang<180.0){
                return line.y1()>data.y1();
            }else{
                return line.y1()<data.y1();
            }
        }
    }

    inline bool operator > (const QLineF& data) const{

        double ang=line.angle();
        if(H_EQUAL2(ang,0.0)||H_EQUAL2(ang,360.0)){
            return line.x1()>data.x1();
        } else if(H_EQUAL2(ang,180.0)) {
            return line.x1()<data.x1();
        }else{
            if(ang<180.0){
                return line.y1()<data.y1();
            }else{
                return line.y1()>data.y1();
            }
        }

    }

    inline bool operator < (const HLine& data) const{

        double ang=line.angle();

        //qDebug()<<ang<<1<<line<<data.line<<(line.y1()>data.line.y1());
        if(H_EQUAL2(ang,0.0)||H_EQUAL2(ang,360.0)){
            return line.x1()<data.line.x1();
        } else if(H_EQUAL2(ang,180.0)) {
            return line.x1()>data.line.x1();
        }else{
            if(ang<180.0){
                return line.y1()>data.line.y1();
            }else{
                return line.y1()<data.line.y1();
            }
        }
    }

    inline bool operator > (const HLine& data) const{

        double ang=line.angle();
        //qDebug()<<ang<<2;
        if(H_EQUAL2(ang,0.0)||H_EQUAL2(ang,360.0)){
            return line.x1()>data.line.x1();
        } else if(H_EQUAL2(ang,180.0)) {
            return line.x1()<data.line.x1();
        }else{
            if(ang<180.0){
                return line.y1()<data.line.y1();
            }else{
                return line.y1()>data.line.y1();
            }
        }
    }

    inline bool operator==(const HLine &d) const { return line==d.line; }
    inline bool operator!=(const HLine &d) const { return !(line== d.line); }

    QLineF line;
};

class LineEquation
{
public:
    explicit LineEquation(double m_A,double m_B,double m_C):A(m_A),B(m_B),C(m_C){

    }

    LineEquation(){

    }


    LineEquation(const LineEquation&data){
        A=data.A;
        B=data.B;
        C=data.C;
    }

    inline bool operator == (const LineEquation& data) const{
        if(qAbs(A-data.A)<0.00000001)
            if(qAbs(B-data.B)<0.00000001)
                if(qAbs(C-data.C)<0.00000001)
                    return true;
        return false;
    }

    inline LineEquation& operator = (const LineEquation& data) {
        if(this==&data)
            return *this;
        A=data.A;
        B=data.B;
        C=data.C;
        return *this;
    }

    //    QPointF Cross2LinePt(const LineEquation& data) const{
    //     double m1=A*B-data.A*B-A;
    //    }

    double GetLine_k() const{
        if(qAbs(B)<0.0000001){
            return -qAbs(B)/B*qAbs(A)/A*LLONG_MAX;
        }
        return -A/B;
    }

    double LineToPtDis(double x,double y) const{
        return qAbs((A*x+B*y+C)/qSqrt(A*A+B*B));
    }

    double LineToPtDis(const QPointF&pt) const{
        return qAbs((A*pt.x()+B*pt.y()+C)/qSqrt(A*A+B*B));
    }

    double GetLine_b() const{
        if(qAbs(B)<0.0000001){
            return -qAbs(B)/B*qAbs(C)/C*LLONG_MAX;
        }
        return -C/B;
    }

    double  GetA() const { return A;}
    void  SetA(double data) { A=data;}
    double  GetB() const {return B;}
    void  SetB(double data) { B=data;}
    double  GetC() const {return C;}
    void  SetC(double data) { C=data;}

private:
    double A=0.0;
    double B=0.0;
    double C=0.0;
};

}

#endif // HMATH_H



