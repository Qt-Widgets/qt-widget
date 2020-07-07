

#ifndef HVARIANTLIST_H
#define HVARIANTLIST_H

#include <QVariant>
#include <QList>

class HVariantList  : public QVariantList
{
public:
     HVariantList();
     HVariantList(const HVariantList &list);
     HVariantList(const QVariantList &list);
     HVariantList(const QStringList &list);
     HVariantList(const QVariant &data);
     HVariantList(const QString &data);
     HVariantList(const QString &data,const QString & spliter);

    ~HVariantList();

    QStringList toStringList_h() const;

    QVector<QVariant> toVariantVector_h() const;

//    HVariantList &operator=(const HVariantList &data){
//        if ( this == &data )
//        {
//            return *this;
//        }

//        *this = data;

//        return *this;
//    }

private:

};

#endif // HVARIANTLIST_H
