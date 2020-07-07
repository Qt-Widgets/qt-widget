#ifndef HSYSTEMADJUSTCONTROLLER_H
#define HSYSTEMADJUSTCONTROLLER_H

#include "hmath.h"

class HAdjustOutline{

public:
    HAdjustOutline(){

    }
    HAdjustOutline(const QString&n):name(n){

    }
    HAdjustOutline(const QString&n,const QString&s,qint32 sa):name(n),status(s),state(sa){

    }

    inline bool operator == (const HAdjustOutline& data) const{
        return name == data.name;
    }


    friend QDataStream &operator>>(QDataStream & input, HAdjustOutline &obj){
        input >> obj.name>>obj.status>>obj.state;
        return input;
    }
    friend QDataStream &operator<<(QDataStream & output, const HAdjustOutline &obj){
        output << obj.name<<obj.status<<obj.state;

        return output;
    }


    QString name=nullptr;
    QString status=nullptr;
    qint32 state=-1;
};

class HAdjustTransQPolygonF{
public:
    HAdjustTransQPolygonF(){

    }
    HAdjustTransQPolygonF(const QPolygonF& rec):polygon(rec){

    }
    HAdjustTransQPolygonF(const QPointF& p):pt(p){

    }

    HAdjustTransQPolygonF(const QPolygonF& rec,const QPointF& p):polygon(rec),pt(p){

    }

    inline bool operator < (const HAdjustTransQPolygonF& data) const{
       if(H_EQUAL(pt.y(),data.pt.y())){
           return pt.x()<data.pt.x();
       }else{
          return pt.y()<data.pt.y();
       }
    }

    inline bool operator > (const HAdjustTransQPolygonF& data) const{
        if(H_EQUAL(pt.y(),data.pt.y())){
            return pt.x()>data.pt.x();
        }else{
           return pt.y()<data.pt.y();
        }
    }

    inline bool operator == (const HAdjustTransQPolygonF& data) const{
        return polygon.containsPoint(data.pt,Qt::OddEvenFill);
    }

    friend QDataStream &operator>>(QDataStream & input, HAdjustTransQPolygonF &obj){
        input >> obj.polygon>>obj.pt;
        return input;
    }
    friend QDataStream &operator<<(QDataStream & output, const HAdjustTransQPolygonF &obj){
        output << obj.polygon<<obj.pt;

        return output;
    }

    QPolygonF polygon;
    QPointF pt;

};

class HSystemAdjustController{

public:
    static HSystemAdjustController* GetInstance();
    static void Release();

    bool Init();

    QVariantList getOutline(qint32 type) const;

    enum ContentType{
        camera,
        vision,
        cutter,
        scanner_header,
        scanner_lens,
        scanner_range
    };

    QVector<QString> getContent(ContentType type, const QString &name) const;
    QStringList getParas(ContentType type, const QString &name) const;
    QString getCurrentOutline(qint32 type);
    HAdjustOutline getOutline(ContentType type, const QString &name) const;

    qint32 deletePageData(qint32 type, const QString &name);

    void clearPage(qint32 type);

    qint32 addNewData(qint32 type, const HAdjustOutline &outline);
    void savePageData(qint32 type, const QString&oname, const HAdjustOutline &outline, const QVector<QString> &content);
    void setDefautOutline(qint32 type, const QString&name);

    void saveParasData(ContentType type,const QString &name,const QStringList&para);
    void saveAllData(qint32 type,const QString&oname,const HAdjustOutline &outline,const QVector<QString> &content,const QStringList&para,const QMap<HAdjustTransQPolygonF,QTransform>& trans);


    QPointF getAxisPosFromImageUseCamera(const QString &cordName,const QString&funcName) const;
    QPointF getImageCenterFromCamera(const QString &cordName,const QString&funcName) const;

    qint32 getAxisPosFromImageUseVision(QPointF& outPos,const QString &visionName,const QString&funcName=nullptr, bool isUseVisionFocus=true,double focusd=0) const;
    qint32 moveToAxisPosAndGetPosFromImageUseVision(QPointF& outPos,const QString &visionName,const QString&funcName=nullptr, bool isUseVisionFocus=true,double focusd=0) const;
    qint32 moveToImageCenterAndWaitFromVision(const QString &visionName,const QPointF&imagePos) const;

    qint32 moveToAxisPosFromPainterPosUseCutter(const QPointF&pos, QString cutterName=nullptr);
    qint32 moveToAxisPosAndWaitFromPainterPosUseCutter(const QPointF&pos, QString cutterName=nullptr);
    qint32 getPainterPosFromImageUseCutter(QPointF& pos,const QString&funcName=nullptr,QString cutterName=nullptr);
    qint32 getTransFromPainterPossUseCutter(QTransform&outPutTrans,const QVector<QPointF> &poss, QString funcName=nullptr,QString cutterName=nullptr);
    qint32 getCutterTransPt(QPointF&outPutPt, QPointF pt,QString cutterName=nullptr);
    qint32 getCutterTrans(QTransform &trans,QString cutterName=nullptr);


    friend QDataStream &operator>>(QDataStream & output, HSystemAdjustController &obj);
    friend QDataStream &operator<<(QDataStream & input, const HSystemAdjustController &obj);

private:
    explicit HSystemAdjustController();
    ~HSystemAdjustController();
    static HSystemAdjustController* m_pInstance;


    class HAdjustData{
    public:
        HAdjustData(){
        }

        friend QDataStream &operator>>(QDataStream & input, HAdjustData &obj){
            input >>obj.currentOutline>> obj.outlines>>obj.contents>>obj.paras>>obj.transforms;
            return input;
        }
        friend QDataStream &operator<<(QDataStream & output, const HAdjustData &obj){
            output <<obj.currentOutline<< obj.outlines<<obj.contents<<obj.paras<<obj.transforms;
            return output;
        }

        QString currentOutline=nullptr;
        QList<HAdjustOutline> outlines;
        QMap<QString,QVector<QString>> contents;
        QMap<QString,QStringList> paras;
        QMap<QString,QMap<HAdjustTransQPolygonF,QTransform>> transforms;
    };

    QVector<HAdjustData> adjustDataArray;


    bool writeToFile(const QString &filePath);
    bool openFromFile(const QString &filePath);

    QString filePath;

};

#endif // HSYSTEMADJUSTCONTROLLER_H
