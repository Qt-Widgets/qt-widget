

#ifndef RCOLOR_H_
#define RCOLOR_H_


#include <QCoreApplication>
#include <QObject>
#include <QColor>
#include <QMetaType>
#include <QMap>
#include <QIcon>
#include <QDebug>

#ifndef RDEFAULT_QSIZE_ICON
#define RDEFAULT_QSIZE_ICON QSize(32,10)
#endif

/**
 * Color. Fixed RGBA or ByLayer or ByBlock.
 *
 * \ingroup core
 * \scriptable
 * \copyable
 */



class  RColor: public QColor {

    Q_DECLARE_TR_FUNCTIONS(RColor);


public:
    friend  QDataStream& operator<<(QDataStream& stream, const RColor& color);

    friend  QDataStream& operator>>(QDataStream& stream, RColor& color);

    enum Mode {
        ByLayer, ByBlock, Fixed
    };

public:
    RColor();
    RColor(int r, int g, int b, int a = 255, RColor::Mode mode = RColor::Fixed);
    RColor(RColor::Mode mode);
    RColor(const QString& name, RColor::Mode mode = RColor::Fixed);

    /**
     * \nonscriptable
     */
    RColor(Qt::GlobalColor color, RColor::Mode mode = RColor::Fixed);
    /**
     * \nonscriptable
     */
    RColor(const QColor& color, RColor::Mode mode = RColor::Fixed);

    int getCustomColorCode() const;
    int getColorIndex() const;

    static void removeColor(const QString& cn);
    static void addColor(const QString& cn, const RColor& c);
    static RColor createFromCadIndex(const QString& code) {
        return createFromCadIndex(code.toInt());
    }
    static RColor createFromCadIndex(int code);
    static RColor createFromCadCustom(const QString& code) {
        return createFromCadCustom(code.toInt());
    }
    static RColor createFromCadCustom(int code);
    static RColor createFromName(const QString& name);

    static QList<QPair<QString, RColor> > getList(bool onlyFixed = false);
    static QIcon getIcon(const RColor& color, const QSize& size = RDEFAULT_QSIZE_ICON);

    unsigned long long getHash() const;

    bool isValid() const;
    QString getName() const;
    bool isByLayer() const;
    bool isByBlock() const;
    bool isFixed() const;

    QColor toCompat() const;
    void setCompat(const QColor& col);

    static RColor getHighlighted(const RColor& color, const QColor& bgColor, int minDist = 75);

    //bool equalsCorrected(const RColor & color) const;

    bool operator==(const RColor & color) const;
    bool operator!=(const RColor & color) const;
    bool operator<(const RColor & color) const;

public:
    static QColor CompatByLayer;
    static QColor CompatByBlock;

private:
    static void init();
    static void init(const QString& cn, const RColor& c);

private:
    static bool isInitialized;
    static QList<QPair<QString, RColor> > list;
    static QMap<QPair<RColor, QPair<int, int> >, QIcon> iconMap;
    //static QVector<RColor> cadColors;
    Mode mode;

private:
    static const double cadColors[][3];
};

/**
 *\nonscriptable
 */
 QDataStream& operator<<(QDataStream& stream, const RColor& color);

/**
 *\nonscriptable
 */
 QDataStream& operator>>(QDataStream& stream, RColor& color);

 QDebug operator<<(QDebug dbg, const RColor& c);

Q_DECLARE_METATYPE(RColor)
Q_DECLARE_METATYPE(RColor*)
typedef QPair<QString, RColor> _RPairStringRColor;
Q_DECLARE_METATYPE(_RPairStringRColor)
Q_DECLARE_METATYPE(QList< _RPairStringRColor >)
Q_DECLARE_METATYPE(RColor::Mode)



#endif /* RCOLOR_H_ */
