#ifndef HSCANNER_H
#define HSCANNER_H


#include "stable.h"
#include "hmathfunctions.h"

typedef QVector<QPainterPath> HPainterPaths;
typedef QVector<QLineF> HLines;

typedef  QPair<qint32,QPointF> HIntPoint;
typedef QVector<HIntPoint> HMarkData;

struct HRTCArc
{
    double startX;
    double startY;
    double centerX;
    double centerY;
    double angle;
};
typedef QVector<HRTCArc> HMarkArcs;

class HScanner
{
public:

    HScanner();
    virtual ~HScanner();


    virtual qint32 InitScanner(const QString& programFile,qint32 laserMode,const QString& correctFile=nullptr);

    virtual void SetLaserPrf(double data);
    virtual void SetLaserFpk(double data);
    virtual void SetLaserPulseWidth(double data);
    virtual void SetLaserPowerByDig(double data);
    virtual void SetLaserPowerByAna(double data);
    virtual void SetLaserDelays(double on,double off);

    virtual void SetScannerDelays(double jumpDelay,double markDelay,double polyDelay);

    virtual void SetMarkVel(double data);
    virtual void SetJumpVel(double data);

    virtual void MarkPointsAndWait(const HMarkData&pts);

    virtual void MarkLinesAndWait(const HLines &lines);
    virtual void MarkShapesPtsAndWait(const HPainterPaths &paths,double step=0.001);
    virtual void MarkFontsAndWait(const HPainterPaths &paths,double step=0.001);
    virtual void MarkFillFontsAndWait(const HPainterPaths &paths, const HLines &fillLines, double step=0.001);
    virtual void MarkGreyPictureAndWait(const QPointF &topLeft, double dotDistance, unsigned dotFrequency,
                                        short minDuty,  short maxDuty, unsigned char *data, unsigned short width, unsigned short height);

protected:


private:


};

#endif // HSCANNER_H
