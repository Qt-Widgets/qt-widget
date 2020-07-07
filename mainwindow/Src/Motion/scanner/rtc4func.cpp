#include "rtc4func.h"



//#include "RTC4expl.h"
#include "RTC4impl.h"

#include <QDebug>


// Figure Parameters
// Definition of "pi"

// Spiral Parameters
#define     AMPLITUDE   500
#define     PERIOD      512.0       // amount of vectors per turn


// Desired Nd:YAG Laser Timing for YAG mode 1
#define     LASER       20000       // Laser pulse frequency in Hz
#define     PULSE       5           // pulse width in microseconds
#define     PULSE1ST    2           // amount of pulses the first
// pulse killer FPK
// should be active
// Frequency Base
#define     MEGA        (1000*1000)
#define     FREQBASE    (8*MEGA)            // corresponding to a time base
// of 1/8 microseconds
// Derived Laser Timing in microseconds
#define     FPK         ((100*PULSE1ST+50)*FREQBASE/100/LASER)
#define     HALF_PERIOD (FREQBASE/LASER/2)
#define     PULSE_WIDTH (PULSE*FREQBASE/MEGA)



#include <stdio.h>
#include <Windows.h>


RTC4Func::RTC4Func()
{

}

RTC4Func::~RTC4Func()
{

    plotFlush();
}

qint32 RTC4Func::GetSerialNumber()
{
    return get_serial_number_32();
}



qint32 RTC4Func::InitScanner(const QString &programFile, qint32 laserMode, const QString &correctFile)
{
    short   ErrorCode=0;



    if(!correctFile.isEmpty()){

        ErrorCode = load_correction_file(correctFile.toLocal8Bit(),1,1.0, 1.0,0.0, 0.0, 0.0);
        if(ErrorCode) {
            return ErrorCode;
        }
    }


    if(ErrorCode) {
         return ErrorCode;
    }

    ErrorCode = load_program_file(programFile.toLocal8Bit());
    if(ErrorCode) {
         return ErrorCode;
    }


    set_laser_mode(quint16(laserMode));   //0-CO2,1-YAG1,2-YAG2,3-YAG3,4-laser4
    set_firstpulse_killer(FPK);

    // Activate a home jump and specify the beam dump
    home_position(-32000, -32000);

    // Turn on the optical pump source and wait for 2 seconds
    // (The following assumes that signal ANALOG OUT1 of the laser connector
    //  controls the pump source)
    write_da_1(640);
    // Wait for 2 seconds

    // Timing, delay and speed preset
    // Following list commands are written to the circular queue
    set_laser_timing(
                HALF_PERIOD,    // half of the laser signal period
                PULSE_WIDTH, 2, // pulse widths of signal LASER1
                1);             // time base; 0 corresponds to 1 microsecond.
    // Otherwise, the time base is 1/8 microseconds.
    set_scanner_delays(25,  // jump delay in 10 microseconds
                       10,     // mark delay in 10 microseconds
                       5);     // polygon delay in 10 microseconds
    set_laser_delays(100,   // laser on delay in microseconds
                     100);   // laser off delay in microseconds
    set_jump_speed(1000.0); // jump speed in bits per milliseconds
    set_mark_speed(250.0);  // marking speed in bits per milliseconds

    plotFlush();

    waitReady();

    set_list_mode(1);       // from now on, the list buffers are configured
    // as a circular queue

    return 0;
}

void RTC4Func::SetLaserPrf(double data)
{
    data*=1000;
    unsigned short halfPeroid=quint16(FREQBASE/data/2);
    unsigned short pulseWidth=quint16(laserPulse*FREQBASE/MEGA);

    while(!get_list_space());
    set_laser_timing(halfPeroid,pulseWidth,2,1);

    laserFrenquency=data;

    plotFlush();
    waitReady();
}

void RTC4Func::SetLaserFpk(double data)
{
    unsigned short  firstPK=quint16((100*data+50)*FREQBASE/100/laserFrenquency);

    while(!get_list_space());
    set_firstpulse_killer(firstPK);
    laserFKP=data;

    plotFlush();
    waitReady();
}

void RTC4Func::SetLaserPulseWidth(double data)
{
    unsigned short halfPeroid=quint16(FREQBASE/laserFrenquency/2);
    unsigned short pulseW=quint16(data*FREQBASE/MEGA);

    while(!get_list_space());
    set_laser_timing(halfPeroid,pulseW,2,1);
    laserPulse=data;

    plotFlush();
    waitReady();
}

void RTC4Func::SetLaserPowerByDig(double data)
{
    while(!get_list_space());
    write_8bit_port(quint16(data*255));

    long_delay(50000);
    long_delay(50000);
    long_delay(50000);
    long_delay(50000);

    plotFlush();
    waitReady();
}

void RTC4Func::SetLaserPowerByAna(double data)
{
    while(!get_list_space());
    write_da_1(quint16(data*1023));

    long_delay(50000);
    long_delay(50000);
    long_delay(50000);
    long_delay(50000);

    plotFlush();
    waitReady();
}

void RTC4Func::SetLaserDelays(double on, double off)
{
    while(!get_list_space());
    set_laser_delays(qint16(on*1000),qint16(off*1000));

    plotFlush();
    waitReady();
}

void RTC4Func::SetScannerDelays(double jumpDelay, double markDelay, double polyDelay)
{
    while(!get_list_space());
    set_scanner_delays(quint16(jumpDelay*1000),quint16(markDelay*1000),quint16(polyDelay*1000));

    plotFlush();
    waitReady();
}

void RTC4Func::SetMarkVel(double data)
{
    while(!get_list_space());

    set_mark_speed(data*AMPLITUDE);

    plotFlush();
    waitReady();
}

void RTC4Func::SetJumpVel(double data)
{
    while(!get_list_space());
    set_jump_speed(data*AMPLITUDE);

    plotFlush();
    waitReady();
}

void RTC4Func::MarkPointsAndWait(const HMarkData &pts)
{
    markPoints(pts);
}

void RTC4Func::MarkLinesAndWait(const HLines &lines)
{
    HMarkData data;

    QPointF firstPt=lines.first().p1()+QPointF(1.0,1.0);
    for(const auto&line:lines){
        QPointF p1=line.p1();
        data<<HIntPoint(p1==firstPt?0:1,p1);
        QPointF p2=line.p2();
        data<<HIntPoint(0,p2);
        firstPt=p2;
    }

    markPoints(data);
}

void RTC4Func::MarkShapesPtsAndWait(const HPainterPaths &paths, double step)
{
    HMarkData data;

    for(const auto&path:paths){

        double st=step/path.length();

        for(double k=0.0,start=1;k<1;k+=st,start=0){
            data<<HIntPoint(int(start),path.pointAtPercent(k));
        }

        data<<HIntPoint(0,path.pointAtPercent(1.0));
    }

    markPoints(data);
}

void RTC4Func::MarkFontsAndWait(const HPainterPaths &paths, double step)
{
    HMarkData data;

    for(const auto&path:paths){

        double st=step/path.length();


        QPointF firstPt=path.pointAtPercent(0.0);

        for(double k=0.0,start=1;k<1;k+=st,start=0){
            QPointF pt=path.pointAtPercent(k);
            data<<HIntPoint(int(HMathFunc::get2PointDis(firstPt,pt)>2*step?1:0),pt);
            firstPt=pt;
        }
        data<<HIntPoint(0,path.pointAtPercent(1.0));

    }

    markPoints(data);
}

void RTC4Func::MarkFillFontsAndWait(const HPainterPaths &paths, const HLines &fillLines, double step)
{
    HMarkData data;

    for(const auto&path:paths){

        double st=step/path.length();


        QPointF firstPt=path.pointAtPercent(0.0);

        for(double k=0.0,start=1;k<1;k+=st,start=0){
            QPointF pt=path.pointAtPercent(k);
            data<<HIntPoint(int(HMathFunc::get2PointDis(firstPt,pt)>2*step?1:0),pt);
            firstPt=pt;
        }
        data<<HIntPoint(0,path.pointAtPercent(1.0));

    }

    for(const auto&line:fillLines){

        double st=step/line.length();

        for(double k=0.0,start=1;k<1;k+=st,start=0){
            data<<HIntPoint(int(start),line.pointAt(k));
        }
        data<<HIntPoint(0,line.pointAt(1.0));
    }


    markPoints(data);
}

void RTC4Func::MarkGreyPictureAndWait(const QPointF &topLeft, double dotDistance, unsigned dotFrequency, short minDuty, short maxDuty, unsigned char *data, unsigned short width, unsigned short height)
{
    markGreyPicture(topLeft,dotDistance,dotFrequency,minDuty,maxDuty,data,width,height);
}






void RTC4Func::SetHomePosition(short valueX, short valueY)
{
    home_position(valueX, valueY);
}






inline void RTC4Func::markPoints(const HMarkData &data,bool isAbs)
{



    if(isAbs){
        for(auto i=data.constBegin(); i!=data.end(); ++i) {

            while(!plotLineAbs(short(AMPLITUDE*i->second.x()),short(AMPLITUDE*i->second.y()),i->first)) {

            }

        }
    }else{
        for(auto i=data.constBegin(); i!=data.end(); ++i) {

            while(!plotLineRel(short(AMPLITUDE*i->second.x()),short(AMPLITUDE*i->second.y()),i->first)) {

            }

        }
    }

    plotFlush();
    waitReady();
}



inline void RTC4Func::markArcs(const HMarkArcs &data,bool isAbs)
{


    if(isAbs){
        for(const auto &k:data ){

            while(!plotJumpAbs(short(AMPLITUDE*k.startX),short(AMPLITUDE*k.startY))) {

            }

            while(!plotArcAbs(short(AMPLITUDE*k.centerX),short(AMPLITUDE*k.centerY),k.angle)) {

            }

        }
    }else{
        for(const auto &k:data ){
            while(!plotJumpAbs(short(AMPLITUDE*k.startX),short(AMPLITUDE*k.startY))) {

            }

            while(!plotArcRel(short(AMPLITUDE*k.centerX),short(AMPLITUDE*k.centerY),k.angle)) {

            }
        }
    }


    plotFlush();
    waitReady();

}



inline void RTC4Func::markGreyPicture(const QPointF &topLeft, double dotDistance, unsigned dotFrequency,
                                      short minDuty,  short maxDuty, unsigned char *data, unsigned short width, unsigned short height)
{

    image grayStairs={
        short(topLeft.x()*AMPLITUDE), short(topLeft.y()*AMPLITUDE),
        dotDistance,
        dotFrequency,
        width,
        height,
        data
    };

    // Print the Image
    while(!imagePrint(&grayStairs,minDuty,maxDuty)) {
        // Do something else while the RTC4 is working. For example:
        // Samsara - turning the wheels

    }

    plotFlush();
    waitReady();
}


inline int RTC4Func::imagePrint(image *picture,  short minDuty,  short maxDuty) {
    static unsigned line = 0;
    static unsigned short pixel_period;
    static unsigned char *pPixel;       // pointer to the current pixel
    static short xCounterbalance;
    static unsigned preDots;


    unsigned i;
    unsigned short busy, position;



    double PERIOD1=(FREQBASE)/picture->dotFrequency;

    double BLACK=PERIOD1*maxDuty;

    double GRAYGAIN=(PERIOD1*(maxDuty-minDuty))/255;
    double DRAGDELAY=360;
    double AXELPERIOD=3*DRAGDELAY;

    // Determine the pixel period and so on at the beginning of an image
    if(!line) {
        if(picture->dotFrequency < 2)
            pixel_period = 0xffff;
        else
            pixel_period = quint16(100000/picture->dotFrequency);

        pPixel = picture->raster;   // 1st pixel of the 1st line

        // Determine the amount of idle dots for the pre-scan period
        // and the corresponding counterbalance
        preDots = quint16(AXELPERIOD/pixel_period/10);
        xCounterbalance = qint16(double((AXELPERIOD - DRAGDELAY)/
                                        double(pixel_period)/10.0) * picture->dotDistance);
    }

    // Check whether the corresponding list is free to be loaded
    get_status(&busy, &position);
    if(busy){
        if(line & 1) {
            if(position >= 4000) return(0);     // list 2 is busy
        }
        else {
            if(position < 4000) return(0);      // list 1 is busy
        }
    }

    // Open the alternate list
    set_start_list(quint16((line & 1) + 1));

    // A counterbalanced jump to the beginning of the next line
    jump_abs(qint16(picture->xLocus - xCounterbalance),
             qint16(double(picture->yLocus) - double(line)*picture->dotDistance));

    set_pixel_line(1, pixel_period, picture->dotDistance, 0.0);
    // Pre-scan
    for(i = 0; i < preDots; i++)
        set_pixel(quint16(((BLACK - GRAYGAIN*(255)))/100), 0, 0);
    // Scan
    for(i = 0; i < picture->ppl; i++, pPixel++)
        set_pixel(quint16((BLACK - GRAYGAIN*(*pPixel))/100), 0, 0);


    // Post-scan
    set_pixel(0, 0, 0);

    set_end_of_list();

    // Only, apply execute_list for the first list. Otherwise, use auto_change.
    line ? auto_change() : execute_list(1);

    line++;
    if(line == picture->lpi) {
        line = 0;
        return(1);              // Success - image printing finished
    }
    return(0);                  // Image printing not yet finished
}


inline int RTC4Func::plotLineAbs(short ptx, short pty, int start) {

    if(!ListSpace) {
        ListSpace = get_list_space();


        if(!ListSpace)
            return(0);
    }
    ListSpace--;

    start?jump_abs(ptx,pty):mark_abs(ptx, pty);
    return(1);

}
inline int RTC4Func::plotLineRel(short ptx, short pty, int start) {

    if(!ListSpace) {
        ListSpace = get_list_space();


        if(!ListSpace)
            return(0);
    }
    ListSpace--;

    start?jump_rel(ptx,pty):mark_rel(ptx, pty);
    return(1);

}

inline int RTC4Func::plotArcAbs(short ptx, short pty, double angle)
{
    if(!ListSpace) {
        ListSpace = get_list_space();

        if(!ListSpace)
            return(0);
    }
    ListSpace--;

    arc_abs(ptx,pty,angle);
    return(1);
}

inline int RTC4Func::plotArcRel(short ptx, short pty, double angle)
{
    if(!ListSpace) {
        ListSpace = get_list_space();

        if(!ListSpace)
            return(0);
    }
    ListSpace--;

    arc_rel(ptx,pty,angle);
    return(1);
}

inline int RTC4Func::plotJumpAbs(short ptx, short pty)
{
    if(!ListSpace) {
        ListSpace = get_list_space();

        if(!ListSpace)
            return(0);
    }
    ListSpace--;

    jump_abs(ptx,pty);
    return(1);
}

inline int RTC4Func::plotJumpRel(short ptx, short pty)
{
    if(!ListSpace) {
        ListSpace = get_list_space();

        if(!ListSpace)
            return(0);
    }
    ListSpace--;

    jump_rel(ptx,pty);
    return(1);
}


inline void RTC4Func::plotFlush(){


    while(!get_list_space());

    set_end_of_list();
    ListSpace = 0;

}

inline void RTC4Func::waitReady()
{
    unsigned short  busy, position;
    do {
        get_status(&busy, &position);
    } while(busy);
}


