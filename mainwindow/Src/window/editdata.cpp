#include "editdata.h"
#include <QtWidgets>
#include "programdata.h"





EditData::EditData(ItemType t,ProgramData *program, LayerInfo *layer, HGraphicsItem *item):m_type(t),
    m_pProgram(program),m_pLayer(layer),m_pItem(item)
{

    myPixmap1=QPixmap(":/resource/icons/eye.svg");
    myPixmap2=QPixmap(":/resource/icons/eye-disable.svg");

    myPixmap3=QPixmap(":/resource/icons/unlocked.svg");
    myPixmap4=QPixmap(":/resource/icons/locked.svg");

    button1=QRect(QPoint(15, 4),QPoint(55, 24));
    button2=QRect(QPoint(60, 0),QPoint(90, 30));
    button3=QRect(QPoint(95, 2),QPoint(121, 28));
}


QSize EditData::sizeHint() const
{
    return QSize(75, 30);
}

qint32 EditData::GetProgramIndex(const QModelIndex &index) const
{

    switch (m_type) {
    case ItemType::none:{
        return -1;
    }
    case ItemType::program:{
        return index.row();
    }
    case ItemType::layer:{
        return index.parent().row();
    }
    case ItemType::item:{
        return index.parent().parent().row();
    }
    }

}




void EditData::paint(const QModelIndex* index,QPainter *painter, const QRect &rect,const QPalette &/*palette*/, EditMode mode) const
{

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);

    if (mode == Editable) {
        painter->fillRect(rect, QColor("lightgreen")/*option.palette.highlight()*/);
        painter->restore();
        return;
    }

    if(index==nullptr){
        painter->restore();
        return;
    }

    switch (m_type)
    {
    case none:{

    }
        break;
    case program:{
        if(m_pProgram->isActived()){
            painter->translate(QPoint(rect.x(),rect.y()));
            painter->setPen(Qt::darkGreen);
            painter->setFont(QFont("Microsoft YaHei", 13, QFont::Bold));
            painter->drawText(QPointF(15,20),QObject::tr("Actived"));
        }
        painter->restore();
        return;
    }
    case layer:{
        painter->translate(QPoint(rect.x(),rect.y()));
        painter->fillRect(button1,m_pLayer->getColor());
        painter->drawPixmap(button2,m_pLayer->getVisible()?myPixmap1:myPixmap2);
        painter->drawPixmap(button3,m_pLayer->getisEnabled()?myPixmap3:myPixmap4);
        painter->restore();
        return;
    }
    case item:{
        painter->translate(QPoint(rect.x(),rect.y()));
        painter->fillRect(button1,m_pItem->getColor());
        painter->drawPixmap(button2,m_pItem->isVisible()?myPixmap1:myPixmap2);
        painter->drawPixmap(button3,m_pItem->isEnabled()?myPixmap3:myPixmap4);
        painter->restore();
        return;
    }
    }

}
