#include "hscanner.h"



HScanner::HScanner()
{

}

HScanner::~HScanner()
{

}


qint32 HScanner::InitScanner(const QString &programFile, qint32 laserMode, const QString &correctFile)
{

    //return ptrRTC4->Init("D2_1.ctb","RTC4D2.hex");
    return 0;
}

void HScanner::SetLaserPrf(double data)
{

}

void HScanner::SetLaserFpk(double data)
{

}

void HScanner::SetLaserPulseWidth(double data)
{

}

void HScanner::SetLaserPowerByDig(double data)
{

}

void HScanner::SetLaserPowerByAna(double data)
{

}

void HScanner::SetLaserDelays(double on, double off)
{

}

void HScanner::SetScannerDelays(double jumpDelay, double markDelay, double polyDelay)
{

}

void HScanner::SetMarkVel(double data)
{

}

void HScanner::SetJumpVel(double data)
{

}

void HScanner::MarkPointsAndWait(const HMarkData &pts)
{

}

void HScanner::MarkLinesAndWait(const HLines &lines)
{

}

void HScanner::MarkShapesPtsAndWait(const HPainterPaths &paths, double step)
{

}

void HScanner::MarkFontsAndWait(const HPainterPaths &paths, double step)
{

}

void HScanner::MarkFillFontsAndWait(const HPainterPaths &paths, const HLines &fillLines, double step)
{

}

void HScanner::MarkGreyPictureAndWait(const QPointF &topLeft, double dotDistance, unsigned dotFrequency, short minDuty, short maxDuty, unsigned char *data, unsigned short width, unsigned short height)
{

}





