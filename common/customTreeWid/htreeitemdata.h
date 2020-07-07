#ifndef HTREEITEMDATA_H
#define HTREEITEMDATA_H


#include <QVector>
//#include "stable.h"


class HTreeItemData
{
public:
    explicit HTreeItemData(const QVector<QVariant> &data, HTreeItemData *parent = nullptr);
    ~HTreeItemData();

    HTreeItemData *Child(int number);
    int ChildCount() const;
    int ColumnCount() const;
    QVariant Data(int column) const;
    bool InsertChildren(int position, int count, int columns);
    bool InsertColumns(int position, int columns);
    HTreeItemData *Parent();
    bool RemoveChildren(int position, int count);
    bool RemoveColumns(int position, int columns);
    int ChildNumber() const;
    bool SetData(int column, const QVariant &value);


private:
    QVector<HTreeItemData*> childItems;
    QVector<QVariant> itemData;
    HTreeItemData *parentItem;

};


#endif // HTREEITEMDATA_H
