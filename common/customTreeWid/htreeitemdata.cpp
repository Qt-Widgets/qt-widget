#include "htreeitemdata.h"



HTreeItemData::HTreeItemData(const QVector<QVariant> &data, HTreeItemData *parent)
{
    parentItem = parent;
    itemData = data;

}

HTreeItemData::~HTreeItemData()
{
    qDeleteAll(childItems);
}

HTreeItemData *HTreeItemData::Child(int number)
{
    return childItems.at(number);
}

int HTreeItemData::ChildCount() const
{
    return childItems.count();
}

int HTreeItemData::ChildNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<HTreeItemData*>(this));

    return 0;
}

int HTreeItemData::ColumnCount() const
{
    return itemData.count();
}

QVariant HTreeItemData::Data(int column) const
{
    return itemData.at(column);
}

bool HTreeItemData::InsertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        HTreeItemData *item = new HTreeItemData(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool HTreeItemData::InsertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    for(const auto &child:childItems){
      child->InsertColumns(position, columns);
    }


    return true;
}

HTreeItemData *HTreeItemData::Parent()
{
    return parentItem;
}

bool HTreeItemData::RemoveChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}


bool HTreeItemData::RemoveColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    for(const auto &child:childItems){
      child->RemoveColumns(position, columns);
    }


    return true;
}


bool HTreeItemData::SetData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}

