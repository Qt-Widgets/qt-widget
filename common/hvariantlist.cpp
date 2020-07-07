#include "hvariantlist.h"

HVariantList::HVariantList(): QVariantList()
{

}

HVariantList::HVariantList(const HVariantList &list) :QVariantList(list)
{

}

HVariantList::HVariantList(const QVariantList &list) :QVariantList(list)
{

}



HVariantList::HVariantList(const QStringList &list)
{
    for(const auto&k:list){
        append(k);
    }
}

HVariantList::HVariantList(const QVariant &data)
{
     append(data);
}

HVariantList::HVariantList(const QString &data)
{
    append(data);
}

HVariantList::HVariantList(const QString &data, const QString &spliter)
{
    for(const auto&k:data.split(spliter)){
      append(k);
    }
}

HVariantList::~HVariantList()
{
    clear();
}

QStringList HVariantList::toStringList_h() const
{
    QStringList list;
    for(const auto&k:*this){
       list.append(k.toString());
    }
    return list;
}

QVector<QVariant> HVariantList::toVariantVector_h() const
{
    QVector<QVariant> vs;
    for(const auto&k:*this){
        vs<<k;
    }
    return vs;
}
