#include "hwsidgetdelegate.h"
#include <QSlider>
#include <QEvent>

HWsidgetDelegate::HWsidgetDelegate(QWidget *parent) : QWidget(parent)
{
     radioBtn = new QSlider(Qt::Horizontal,this);
     QPoint poi=this->rect().center();
     radioBtn->setGeometry(poi.x()-25,poi.y()-11,50,24);

     radioBtn->setMinimum(0);
     radioBtn->setMaximum(1);
     radioBtn->setFixedSize(50,24);
     radioBtn->installEventFilter(this);
}


bool HWsidgetDelegate::eventFilter(QObject *target, QEvent *event)
{
    if(target==radioBtn)
    {
        switch(event->type()) {

        case QEvent::Wheel:
        {
            return true;
        }
        default:
            break;
        }
    }
    return QWidget::eventFilter(target, event);
}
