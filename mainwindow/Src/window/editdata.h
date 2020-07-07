#ifndef EDITDATA_H
#define EDITDATA_H

#include "hcommon.h"
#include "programdata.h"
#include <QColor>




class ProgramHandler;

class EditData
{
public:
    enum EditMode { Editable, ReadOnly };

    enum ItemType{
        none=-1,
        program=0,
        layer=1,
        item,
    };


    explicit EditData(ItemType t=none,ProgramData *program=nullptr,LayerInfo *layer=nullptr,HGraphicsItem *item=nullptr);

    QSize sizeHint() const;


    qint32 GetProgramIndex(const QModelIndex&index) const;
    void paint(const QModelIndex *index, QPainter *painter, const QRect &rect, const QPalette &palette, EditMode mode) const;


    ItemType m_type;
    ProgramData *m_pProgram;
    LayerInfo *m_pLayer;
    HGraphicsItem *m_pItem;


private:

     QPixmap myPixmap1;
     QPixmap myPixmap2;

     QPixmap myPixmap3;
     QPixmap myPixmap4;

     QRect button1;
     QRect button2;
     QRect button3;

};


Q_DECLARE_METATYPE(EditData)

#endif // EDITDATA_H
