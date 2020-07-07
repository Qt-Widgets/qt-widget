
#ifdef Q_OS_WIN
// for _isnan:
#include <cfloat>
#endif

#include <complex>

#include <QRegExp>


#include "RMath.h"


/**
 * Last error encountered.
 */
QString RMath::lastError = "";
QString RMath::mathExt = QString();

/**
 * \return Absolute modulus.
 */
int RMath::absmod(int a, int b) {
    if (b == 0) {
        return a;
    }
    int m = a % b;
    if ((b < 0 && m > 0) || (b > 0 && m < 0)) {
        return b + m;
    }
    return m;
}

/**
 * \return v truncated (cut off) at decimal point.
 */
double RMath::trunc(double v) {
#if defined Q_OS_WIN
    if (v==0 || RMath::isNaN(v) || RMath::isInf(v)) {
        return v;
    }
    if (v >= 0.0) {
        return floor(v);
    }
    else {
        return ceil(v);
    }
#else
    return ::trunc(v);
#endif
}

/**
 * Safe pow function.
 *
 * \param x the base
 * \param y the exponent
 *
 * \return The value of x raised to the power of y.
 */
double RMath::pow(double x, double y) {
    errno = 0;
    double ret = ::pow(x, y);
    if (errno == EDOM) {
        qCritical("RMath::pow: EDOM in pow");
        ret = 0.0;
    } else if (errno == ERANGE) {
        qCritical("RMath::pow: ERANGE in pow");
        ret = 0.0;
    }
    return ret;
}

/**
 * \return true if v is non NaN and not Inf.
 */
bool RMath::isNormal(double v) {
    if (RMath::isNaN(v) || RMath::isInf(v)) {
        return false;
    }
    return true;
}

/**
 * \return true if v is NaN.
 */
bool RMath::isNaN(double v) {
#ifdef Q_OS_MAC
    return std::fpclassify(v)==FP_NAN;
#elif defined Q_OS_WIN
    return _isnan(v);
#else
    return std::isnan(v);
#endif
}

/**
 * \return true if v is Inf.
 */
bool RMath::isInf(double v) {
#ifdef Q_OS_MAC
    return std::fpclassify(v)==FP_INFINITE;
#elif defined Q_OS_WIN
    return !_finite(v);
#else
    return std::fpclassify(v)==FP_INFINITE;
#endif
}

bool RMath::isSane(double v) {
    return !isNaN(v) && !isInf(v) && v>-1e12 && v<1e12;
}

/**
 * Evaluates the given mathematical expression and returns the result.
 * Angles may be expressed in degrees (default), rad (#r), gon (#g)
 * or as surveyors angles (N#d#'#"E).
 */


/**
 * \return Last error encountered by a math function.
 */
QString RMath::getError() {
    return lastError;
}

/**
 * \return True if an error was encountered since the last check.
 */
bool RMath::hasError() {
    return !lastError.isEmpty();
}

/**
 * Converts the given angle in rad to the given angle format.
 * \todo Support different formats.
 */
QString RMath::angleToString(double a) {
    return QString("%1").arg(rad2deg(a));
}

QString RMath::trimTrailingZeroes(const QString& s) {
    if (!s.contains('.')) {
        return s;
    }

    QString ret = s;

    bool done = false;
    while (!done) {
        if (ret.size()>0) {
            if (ret.at(ret.size()-1)=='0') {
                ret = ret.left(ret.size()-1);
            }
            else if (ret.at(ret.size()-1)=='.') {
                ret = ret.left(ret.size()-1);
                done = true;
            }
            else {
                done = true;
            }
        }
        else {
            done = true;
        }
    }

    return ret;
}


/**
 * Converts radians to degrees.
 *
 * \param a angle in radians
 */
double RMath::rad2deg(double a) {
    return (a / (2.0 * M_PI) * 360.0);
}

/**
 * Converts grads to degrees.
 *
 * \param a angle in grad (gon)
 */
double RMath::gra2deg(double a) {
    return a / 400.0 * 360.0;
}

/**
 * \return True if the given value is between the given limits.
 * \param inclusive True to accept values close to the limits within the given tolerance.
 */
bool RMath::isBetween(double value, double limit1, double limit2, bool inclusive, double tolerance) {
    if (fuzzyCompare(value, limit1, tolerance) || fuzzyCompare(value, limit2, tolerance)) {
        return inclusive;
    }
    double min = qMin(limit1, limit2);
    double max = qMax(limit1, limit2);
    return (value>=min && value<=max);
}

/**
 * Converts degrees to radians.
 *
 * \param a angle in degrees
 */
double RMath::deg2rad(double a) {
    return ((a / 360.0) * (2.0 * M_PI));
}

/**
 * Converts radians to gradians.
 *
 * \param a angle in radians
 */
double RMath::rad2gra(double a) {
    return (a / (2.0 * M_PI) * 400.0);
}

/**
 * Finds greatest common divider using Euclid's algorithm.
 * \sa http://en.wikipedia.org/wiki/Greatest_common_divisor
 *
 * \param a the first number
 *
 * \param b the second number
 *
 * \return The greatest common divisor of \c a and \c b.
 */
int RMath::getGcd(int a, int b) {
    int rem;

    while (b != 0) {
        rem = a % b;
        a = b;
        b = rem;
    }

    return a;
}

/**
 * Tests if angle a is between a1 and a2. a, a1 and a2 must be in the
 * range between 0 and 2*PI.
 * All angles in rad.
 *
 * \param a the test angle
 * \param a1 the lower limiting angle
 * \param a2 the upper limiting angle
 * \param reversed True for clockwise testing. False for ccw testing.
 * \return true if the angle a is between a1 and a2.
 */
bool RMath::isAngleBetween(double a, double a1, double a2, bool reversed) {

    a = RMath::getNormalizedAngle(a);
    a1 = RMath::getNormalizedAngle(a1);
    a2 = RMath::getNormalizedAngle(a2);

    bool ret = false;

    if (reversed) {
        double tmp = a1;
        a1 = a2;
        a2 = tmp;
    }

    if (a1 >= a2 - RS::AngleTolerance) {
        if (a >= a1 - RS::AngleTolerance || a <= a2 + RS::AngleTolerance) {
            ret = true;
        }
    } else {
        if (a >= a1 - RS::AngleTolerance && a <= a2 + RS::AngleTolerance) {
            ret = true;
        }
    }
    return ret;
}

/**
 * Gets the normalized angle from \c a.
 * Used to make sure that an angle is in the range between 0 and 2 pi.
 *
 * \param a the unnormalized angle, e.g. 8
 *
 * \return The angle \c a normalized to the range of \f$ 0 \ldots 2\pi \f$,
 * e.g. normalized angle from 8 is 1.716.
 */
double RMath::getNormalizedAngle(double a) {
    if (a >= 0.0) {
        int n = (int) floor(a / (2*M_PI));
        a -= 2*M_PI * n;
    } else {
        int n = (int) ceil(a / (-2*M_PI));
        a += 2*M_PI * n;
    }

    if (a>2*M_PI-RS::AngleTolerance) {
        a = 0.0;
    }

    return a;
}

/**
 * \return Angle a as angle relative to baseAngle.
 *         Result is in range -PI < result < PI.
 */
double RMath::getRelativeAngle(double a, double baseAngle) {
    double ret = a - baseAngle;
    if (ret>M_PI) {
        ret-=2*M_PI;
    }
    if (ret<-M_PI) {
        ret+=2*M_PI;
    }
    return ret;
}

/**
 * \param a1 first angle in rad
 * \param a2 second angle in rad
 *
 * \return The angle that needs to be added to a1 to reach a2.
 *         Always positive and less than 2*pi.
 */
double RMath::getAngleDifference(double a1, double a2) {
    double ret;

    if (a1 >= a2) {
        a2 += 2*M_PI;
    }
    ret = a2 - a1;

    if (ret >= 2*M_PI) {
        ret = 0.0;
    }

    return ret;
}

/**
 * \param a1 first angle in rad
 *
 * \param a2 s second angle in rad
 *
 * \return The angle that needs to be added to a1 to reach a2.
 *         Always between -pi and pi.
 */
double RMath::getAngleDifference180(double a1, double a2) {
    double ret;

    ret = a2 - a1;
    if (ret > M_PI) {
        ret = -(2*M_PI - ret);
    }
    if (ret < -M_PI) {
        ret = 2*M_PI + ret;
    }

    return ret;
}

/**
 * Adds 180° to the given angle if a text constructed with that angle
 * otherwise wouldn't be readable.
 * Used for dimension texts and for mirroring texts.
 *
 * \param angle the original angle
 *
 * \param readable true: make angle readable, false: unreadable
 *
 * \param corrected Pointer to boolean that will point to true if the given angle was
 *   corrected, false otherwise, or null.
 *
 * \return The given angle or the given \c angle + pi, depending which one
 * is readable from the bottom or right.
 */
double RMath::makeAngleReadable(double angle, bool readable, bool* corrected) {

    double ret;

    bool cor = isAngleReadable(angle) ^ readable;

    // quadrant 1 & 4
    if (!cor) {
        ret = angle;
    }
    // quadrant 2 & 3
    else {
        ret = angle + M_PI;
    }

    if (corrected != NULL) {
        *corrected = cor;
    }

    return ret;
}

/**
 * \param angle The text angle in rad
 *
 * \param tolerance The tolerance by which the angle still maybe
 * in the unreadable range.
 *
 * \return true: If the given angle is in a range that is readable
 * for texts created with that angle.
 */
bool RMath::isAngleReadable(double angle, double tolerance) {
    double angleCorrected = getNormalizedAngle(angle);
    if (angleCorrected > M_PI / 2.0 * 3.0 + tolerance || angleCorrected < M_PI / 2.0 + tolerance) {
        return true;
    } else {
        return false;
    }
}

/**
 * Tests if two angels point approximately in the same direction.
 *
 * \param dir1 first direction
 *
 * \param dir2 second direction
 *
 * \param tolerance Tolerance in rad.
 *
 * \retval true The two angles point in the same direction.
 *
 * \retval false The difference between the two angles is at
 * least \c tolerance radians.
 */
bool RMath::isSameDirection(double dir1, double dir2, double tolerance) {
    double diff = fabs(dir1 - dir2);
    if (diff < tolerance || diff > 2*M_PI - tolerance) {
        return true;
    } else {
        return false;
    }
}

/**
 * \return String representing the given number, expressed as fraction (# #/#).
 */
QString RMath::toFractionString(double v, int maxDenominator) {
    int number, numerator, denominator;
    toFraction(v, maxDenominator, number, numerator, denominator);

    if (number==0) {
        return QString("%1/%2").arg(numerator).arg(denominator);
    }
    else {
        if (numerator==0) {
            return QString("%1").arg(number);
        }
        else {
            return QString("%1 %2/%3").arg(number).arg(numerator).arg(denominator);
        }
    }
}

/**
 * \return Representation of the given number as fraction (number numerator/denominator).
 * Rounding occurs to satisfy the use of maxDenominator as maximum value for denominator.
 */
void RMath::toFraction(double v, int maxDenominator, int& number, int& numerator, int& denominator) {
    int in = (int)v;
    number = in;

    if (in==v) {
        number = in;
        numerator = 0;
        denominator = 1;
        return;
    }

    simplify(abs(mround((v-in)*maxDenominator)), maxDenominator, numerator, denominator);
}

/**
 * \return Simlified fraction for the given fraction (numerator/denomiator).
 */
void RMath::simplify(int numerator, int denominator, int& numeratorRes, int& denominatorRes) {
    int g = getGcd(numerator, denominator);
    numeratorRes = numerator/g;
    denominatorRes = denominator/g;
}

/**
 * \return True if the difference between v1 and v2 is less then the
 * given tolerance.
 */
bool RMath::fuzzyCompare(double v1, double v2, double tolerance) {
    return fabs(v1-v2) < tolerance;
}

bool RMath::fuzzyAngleCompare(double v1, double v2, double tolerance) {
    return fabs(getAngleDifference180(v1, v2)) < tolerance;
}

bool RMath::containsFuzzy(const QList<double>& values, double v, double tol) {
    for (int i=0; i<values.length(); i++) {
        if (RMath::fuzzyCompare(v, values[i], tol)) {
            return true;
        }
    }

    return false;
}




void RMath::getQuadRoots(double p[], double r[][5]) {
    /*
    Array r[3][5] p[5]
    Roots of poly p[0]*x^2 + p[1]*x + p[2]=0
    x=r[1][k] + i r[2][k] k=1,2
    */
    double b,c,d;
    b=-p[1]/(2.0*p[0]);
    c=p[2]/p[0];
    d=b*b-c;
    if(d>=0.0) {
        if(b>0.0)
            b=(r[1][2]=(sqrt(d)+b));
        else
            b=(r[1][2]=(-sqrt(d)+b));
        r[1][1]=c/b;
        r[2][1]=(r[2][2]=0.0);
    }
    else {
        d=(r[2][1]=sqrt(-d));
        r[2][2]=-d;
        r[1][1]=(r[1][2]=b);
    }
}

void RMath::getCubicRoots(double p[], double r[][5]) {
    /*
    Array r[3][5] p[5]
    Roots of poly p[0]*x^3 + p[1]*x^2 + p[2]*x + p[3] = 0
    x=r[1][k] + i r[2][k] k=1,...,3
    Assumes 0<arctan(x)<pi/2 for x>0
    */
    double s,t,b,c,d;
    int k;
    if(p[0]!=1.0)
    {
        for(k=1;k<4;k++)
            p[k]=p[k]/p[0];
        p[0]=1.0;
    }
    s=p[1]/3.0;
    t=s*p[1];
    b=0.5*(s*(t/1.5-p[2])+p[3]);
    t=(t-p[2])/3.0;
    c=t*t*t;
    d=b*b-c;
    if(d>=0.0) {
        d=pow((sqrt(d)+fabs(b)),1.0/3.0);
        if(d!=0.0) {
            if(b>0.0)
                b=-d;
            else
                b=d;
            c=t/b;
        }
        d=r[2][2]=sqrt(0.75)*(b-c);
        b=b+c;
        c=r[1][2]=-0.5*b-s;
        if((b>0.0 && s<=0.0) || (b<0.0 && s>0.0)) {
            r[1][1]=c;
            r[2][1]=-d;
            r[1][3]=b-s;
            r[2][3]=0.0;
        }
        else {
            r[1][1]=b-s;
            r[2][1]=0.0;
            r[1][3]=c;
            r[2][3]=-d;
        }

    } /* end 2 equal or complex roots */
    else {
        if(b==0.0)
            d=atan(1.0)/1.5;
        else
            d=atan(sqrt(-d)/fabs(b))/3.0;
        if(b<0.0)
            b=2.0*sqrt(t);
        else
            b=-2.0*sqrt(t);
        c=cos(d)*b;
        t=-sqrt(0.75)*sin(d)*b-0.5*c;
        d=-t-c-s;
        c=c-s;
        t=t-s;
        if(fabs(c)>fabs(t)) {
            r[1][3]=c;
        }
        else {
            r[1][3]=t;
            t=c;
        }
        if(fabs(d)>fabs(t)) {
            r[1][2]=d;
        }
        else {
            r[1][2]=t;
            t=d;
        }
        r[1][1]=t;
        for(k=1;k<4;k++)
            r[2][k]=0.0;
    }
    return;
}

void RMath::getBiQuadRoots(double p[],double r[][5]) {
    /*
    Array r[3][5] p[5]
    Roots of poly p[0]*x^4 + p[1]*x^3 + p[2]*x^2 + p[3]*x + p[4] = 0
    x=r[1][k] + i r[2][k] k=1,...,4
    */
    double a,b,c,d,e;
    int k,j;
    if(p[0] != 1.0)
    {
        for(k=1;k<5;k++)
            p[k]=p[k]/p[0];
        p[0]=1.0;
    }
    e=0.25*p[1];
    b=2.0*e;
    c=b*b;
    d=0.75*c;
    b=p[3]+b*(c-p[2]);
    a=p[2]-d;
    c=p[4]+e*(e*a-p[3]);
    a=a-d;
    p[1]=0.5*a;
    p[2]=(p[1]*p[1]-c)*0.25;
    p[3]=b*b/(-64.0);
    if(p[3]<0.0)
    {
        getCubicRoots(p,r);
        for(k=1;k<4;k++)
        {
            if(r[2][k]==0.0 && r[1][k]>0.0)
            {
                d=r[1][k]*4.0;
                a=a+d;
                if(a>=0.0 && b>=0.0)
                    p[1]=sqrt(d);
                else if(a<=0.0 && b<=0.0)
                    p[1]=sqrt(d);
                else
                    p[1]=-sqrt(d);
                b=0.5*(a+b/p[1]);
                goto QUAD;
            }

        }

    }
    if(p[2]<0.0)
    {
        b=sqrt(c);
        d=b+b-a;
        p[1]=0.0;
        if(d>0.0)
            p[1]=sqrt(d);
    }
    else
    {
        if(p[1]>0.0)
            b=sqrt(p[2])*2.0+p[1];
        else
            b=-sqrt(p[2])*2.0+p[1];
        if(b!=0.0)
        {
            p[1]=0.0;
        }
        else
        {
            for(k=1;k<5;k++)
            {
                r[1][k]=-e;
                r[2][k]=0.0;
            }
            goto END;
        }
    }
QUAD:
    p[2]=c/b;
    getQuadRoots(p,r);
    for(k=1;k<3;k++)
        for(j=1;j<3;j++)
            r[j][k+2]=r[j][k];
    p[1]=-p[1];
    p[2]=b;
    getQuadRoots(p,r);
    for(k=1;k<5;k++)
        r[1][k]=r[1][k]-e;
END:
    return;
}
