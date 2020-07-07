#include "hlistwidget.h"

HListWidget::HListWidget(QWidget *parent) : QListWidget(parent)
{

}

void HListWidget::deleteSelectedItems()
{
    for(const auto &k:selectedItems()){
        takeItem(row(k));
    }
}


void HListWidget::newItem()
{
    qint32 size=count();
    QStringList items;
    for(qint32 k=0;k<size;++k){
        items.append(item(k)->text());
    }

    auto mk=new QListWidgetItem(getSaveName());
    // mk->setFlags( Qt::ItemIsEnabled|/*Qt::ItemIsEditable|*/Qt::ItemIsSelectable);

    //mk->setData(Qt::UserRole,data);

    addItem(mk);
}

void HListWidget::clearAllItem()
{
    clear();
}

QString HListWidget::getSaveName()
{
    auto names=getListWidItemNames();

    if(!names.contains(m_defaultItemName)){
        return m_defaultItemName;
    }

    qint32 index=0;
    do {
       ++index;
    }while (names.contains(m_defaultItemName+QString::number(index)));

    return m_defaultItemName+QString::number(index);
}

inline QStringList HListWidget::getListWidItemNames()
{
    QStringList list;
    qint32 size=count();
    for(qint32 k=0;k<size;++k){
        list<<item(k)->text();
    }
    return list;
}
