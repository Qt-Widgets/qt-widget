#ifndef HMATHFUNCTIONS_H
#define HMATHFUNCTIONS_H

#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <stable.h>


template<typename _Tp>std::vector<_Tp> convertMat2Vector(const cv::Mat &mat){
    return static_cast<std::vector<_Tp>>(mat.reshape(1, 1));
}

template<typename _Tp>cv::Mat convertVector2Mat(std::vector<_Tp> v, int channels, int rows){
    cv::Mat mat = cv::Mat(v);
    cv::Mat dest = mat.reshape(channels, rows).clone();
    return dest;
}

typedef QVector<QPointF> HPoints;

using namespace  cv;


class   HMathFunc {

public:

    static  qreal get2PointDis(const QPointF &p1,const QPointF &p2){

        return qSqrt(qPow(p1.x()-p2.x(),2.0)+qPow(p1.y()-p2.y(),2.0));
    }

    static QString pointsToString(const QVector<QPointF> &pts){
        QStringList list;
        for(const auto&k:pts){
            list<<QString("%1,%2").arg(k.x()).arg(k.y());
        }
        return list.join(';');
    }

    static QString transToString(const QTransform &trans){
        return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9").arg(trans.m11()).arg(trans.m12()).arg(trans.m13())
                .arg(trans.m21()).arg(trans.m22()).arg(trans.m23())
                .arg(trans.m31()).arg(trans.m32()).arg(trans.m33());
    }

    static QTransform stringToTrans(const QString &trans){
        auto list=trans.split(',');
        if(list.size()!=9){
            return QTransform(1,0,0,0,1,0,0,0,1);
        }
        return QTransform(
                    list.at(0).toDouble(),list.at(1).toDouble(),list.at(2).toDouble()
                    ,list.at(3).toDouble(),list.at(4).toDouble(),list.at(5).toDouble()
                    ,list.at(6).toDouble(),list.at(7).toDouble(),list.at(8).toDouble()
                    );
    }

    static QVector<QPointF> stringToPoints(const QString &pts){
        QVector<QPointF> list;
        for(const auto&k:pts.split(';')){
            auto nnn=k.split(',');
            if(nnn.size()==2){
                list<<QPointF(nnn.first().toDouble(),nnn.at(1).toDouble());
            }else{
                list<<QPointF(0,0);
            }

        }
        return list;
    }

    static QTransform  GetPerspectiveTransform(const QVector<QPointF> &src,const QVector<QPointF> &dst) {

        const Point2f src_pt[4] = { Point2f(static_cast<float>(src.at(0).x()),static_cast<float>(src.at(0).y())),
                                    cv::Point2f(static_cast<float>(src.at(1).x()),static_cast<float>(src.at(1).y())),
                                    cv::Point2f(static_cast<float>(src.at(2).x()),static_cast<float>(src.at(2).y())),
                                    cv::Point2f(static_cast<float>(src.at(3).x()),static_cast<float>(src.at(3).y()))};

        const Point2f dst_pt[4] = { Point2f(static_cast<float>(dst.at(0).x()),static_cast<float>(dst.at(0).y())),
                                    Point2f(static_cast<float>(dst.at(1).x()),static_cast<float>(dst.at(1).y())),
                                    Point2f(static_cast<float>(dst.at(2).x()),static_cast<float>(dst.at(2).y())),
                                    Point2f(static_cast<float>(dst.at(3).x()),static_cast<float>(dst.at(3).y()))};

        const Mat affine_matrix = getPerspectiveTransform(src_pt,dst_pt);
        const std::vector<double> mk = convertMat2Vector<double>(affine_matrix);
        return QTransform(mk.at(0),mk.at(3),mk.at(6),mk.at(1),mk.at(4),mk.at(7),mk.at(2),mk.at(5),mk.at(8));
    }

    static QTransform  GetAffineTransform(const QVector<QPointF> &src,const QVector<QPointF> &dst) {

        const Point2f src_pt[3] = { Point2f(static_cast<float>(src.at(0).x()),static_cast<float>(src.at(0).y())),
                                    cv::Point2f(static_cast<float>(src.at(1).x()),static_cast<float>(src.at(1).y())),
                                    cv::Point2f(static_cast<float>(src.at(2).x()),static_cast<float>(src.at(2).y()))};

        const Point2f dst_pt[3] = { Point2f(static_cast<float>(dst.at(0).x()),static_cast<float>(dst.at(0).y())),
                                    Point2f(static_cast<float>(dst.at(1).x()),static_cast<float>(dst.at(1).y())),
                                    Point2f(static_cast<float>(dst.at(2).x()),static_cast<float>(dst.at(2).y()))};

        const Mat affine_matrix = getAffineTransform(src_pt,dst_pt);

        const std::vector<double> mk = convertMat2Vector<double>(affine_matrix);
        return QTransform(mk.at(0),mk.at(3),0,mk.at(1),mk.at(4),0,mk.at(2),mk.at(5),1);
    }

    static QTransform  GetPerspectiveTransform(const QRectF &src,const QRectF &dst) {

        const Point2f src_pt[4] = { Point2f(static_cast<float>(src.left()),static_cast<float>(src.top())),
                                    cv::Point2f(static_cast<float>(src.right()),static_cast<float>(src.top())),
                                    cv::Point2f(static_cast<float>(src.right()),static_cast<float>(src.bottom())),
                                    cv::Point2f(static_cast<float>(src.left()),static_cast<float>(src.bottom()))};

        const Point2f dst_pt[4] = { Point2f(static_cast<float>(dst.left()),static_cast<float>(dst.top())),
                                    Point2f(static_cast<float>(dst.right()),static_cast<float>(dst.top())),
                                    Point2f(static_cast<float>(dst.right()),static_cast<float>(dst.bottom())),
                                    Point2f(static_cast<float>(dst.left()),static_cast<float>(dst.bottom()))};

        const Mat affine_matrix = getPerspectiveTransform(src_pt,dst_pt);
        const std::vector<double> mk = convertMat2Vector<double>(affine_matrix);
        return QTransform(mk.at(0),mk.at(3),mk.at(6),mk.at(1),mk.at(4),mk.at(7),mk.at(2),mk.at(5),mk.at(8));
    }

    static QLineF GetComplexLine(const QLineF& line1,const QLineF& line2){

        QPointF pt=line1.p2()-line2.p1()+line2.p2();
        return QLineF(line1.p1(),pt);
    }

    static bool LineFitting(const QVector<QPointF>& inory,double &m_dfAngle,double &A,double &B,double &C)
    {
        QPointF pt;
        double x_mean = 0;
        double y_mean = 0;
        double Dxx = 0, Dxy = 0, Dyy = 0;
        int size=inory.size();


        if( size< 2)
        {
            A = 0;
            B = 0;
            C = 0;
            return false;
        }


        for(int i = 0; i < size;++i)
        {
            pt=inory.at(i);
            x_mean += pt.x();
            y_mean += pt.y();
        }

        x_mean /= size;
        y_mean /= size; //计算 x y 的均值

        for(int i = 0; i < size; i++)
        {
            pt=inory.at(i);
            Dxx += (pt.x()- x_mean) * (pt.x() - x_mean);
            Dxy += (pt.x()- x_mean) * (pt.y() - y_mean);
            Dyy += (pt.y()- y_mean) * (pt.y() - y_mean);
        }

        double lambda = ( (Dxx + Dyy) - sqrt( (Dxx - Dyy) * (Dxx - Dyy) + 4 * Dxy * Dxy) ) / 2.0;
        double den = sqrt( Dxy * Dxy + (lambda - Dxx) * (lambda - Dxx) );
        A = Dxy / den;
        B = (lambda - Dxx) / den;
        C = - A * x_mean - B * y_mean;

        m_dfAngle=atan(-A/B);
        return true;

    }

    static void GetGrayImage(const QString &file,int &width,int &height,Mat &srcGrey){
        Mat src;
        src = imread(file.toLocal8Bit().data(),IMREAD_COLOR);
        if(!src.data)
            return;

        width=src.cols;
        height=src.rows;

        cvtColor(src, srcGrey, COLOR_BGR2GRAY);

        cv::imshow("123",srcGrey);

        waitKey(0);
        return ;
    }

    static void TransImgToPts(const QString &file,const QPointF &topLeft,const QPointF &bottomRight,  HPoints& data){



        Mat src, src_gray,edge, dst;

        src = imread(file.toLocal8Bit().data(),IMREAD_COLOR);
        if(!src.data)
            return;



        int width=src.cols;
        int height=src.rows;

        const Point2f src_pt[4]={Point2f(0.0,0.0),Point2f(width,0.0),Point2f(0.0,height),Point2f(width,height)};
        const Point2f dst_pt[4]={Point2f(float(topLeft.x()),float(topLeft.y())),Point2f(float(bottomRight.x()),float(topLeft.y())),
                                 Point2f(float(topLeft.x()),float(bottomRight.y())),Point2f(float(bottomRight.x()),float(bottomRight.y()))};



        const std::vector<double> mk = convertMat2Vector<double>(getPerspectiveTransform(src_pt,dst_pt));

        QTransform trans=QTransform(mk.at(0),mk.at(3),mk.at(6),mk.at(1),mk.at(4),mk.at(7),mk.at(2),mk.at(5),mk.at(8));


        /// Remove noise by blurring with a Gaussian filter
        //GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

        /// Convert the image to grayscale
        cvtColor(src, src_gray, COLOR_BGR2GRAY);

        blur(src_gray, edge, Size(3, 3));

        /// Apply Laplace function

        Canny(edge, edge, 50, 150);



        for(int i=0;i<height;++i){
            for(int j=0;j<width;++j){

                if(edge.at<unsigned char>(i,j)!=0){
                    data.append(trans.map(QPointF(j,i)));

                }

            }

        }

        cv::imshow("123",edge);

        waitKey(0);

    }


};

#endif // HMATHFUNCTIONS_H
