#ifndef RTC4FUNC_H
#define RTC4FUNC_H

#include "hscanner.h"


class RTC4Func : public HScanner
{
public:
    RTC4Func();
    ~RTC4Func();

    qint32 GetSerialNumber();

    qint32 InitScanner(const QString &programFile, qint32 laserMode, const QString &correctFile);



    void SetLaserPrf(double data);
    void SetLaserFpk(double data);
    void SetLaserPulseWidth(double data);
    void SetLaserPowerByDig(double data);
    void SetLaserPowerByAna(double data);
    void SetLaserDelays(double on,double off);

    void SetScannerDelays(double jumpDelay,double markDelay,double polyDelay);

    void SetMarkVel(double data);
    void SetJumpVel(double data);

    void MarkPointsAndWait(const HMarkData&pts);

    void MarkLinesAndWait(const HLines &lines);
    void MarkShapesPtsAndWait(const HPainterPaths &paths,double step=0.001);
    void MarkFontsAndWait(const HPainterPaths &paths,double step=0.001);
    void MarkFillFontsAndWait(const HPainterPaths &paths, const HLines &fillLines, double step=0.001);
    void MarkGreyPictureAndWait(const QPointF &topLeft, double dotDistance, unsigned dotFrequency,
                                short minDuty,  short maxDuty, unsigned char *data, unsigned short width, unsigned short height);


private:


    void SetHomePosition(short valueX,short valueY);



    double laserFrenquency=20.0;
    double laserFKP=2.0;
    double laserPulse=5.0;
    double laserPower=0.5;




    struct image {
        short xLocus, yLocus;           // upper left corner of the image in bits
        double dotDistance;             // pixel distance in bits
        unsigned dotFrequency;          // pixel frequency in Hz
        unsigned short ppl;             // pixels per line
        unsigned short lpi;             // lines per image
        unsigned char *raster;          // pointer to the raster data
    };


    inline void markPoints(const HMarkData&data,bool isAbs=true);
    inline void markArcs(const HMarkArcs&data,bool isAbs=true);

    inline void markGreyPicture(const QPointF&topLeft, double dotDistance, unsigned dotFrequency, short minDuty,
                         short maxDuty, unsigned char *data, unsigned short width, unsigned short height);


    inline int imagePrint(image *picture, short minDuty, short maxDuty);

    unsigned short ListSpace = 0;
    inline int plotLineAbs(short ptx, short pty, int start);
    inline int plotLineRel(short ptx, short pty, int start);
    inline int plotArcAbs(short ptx, short pty, double angle);
    inline int plotArcRel(short ptx, short pty, double angle);
    inline int plotJumpAbs(short ptx, short pty);
    inline int plotJumpRel(short ptx, short pty);


    void plotFlush();

    void waitReady();
};

#endif // RTC4FUNC_H
