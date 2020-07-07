#include "h_globalapplication.h"
#include <QMouseEvent>


H_GlobalApplication::H_GlobalApplication(int &argc,char **argv):
QApplication(argc,argv)
{

}

H_GlobalApplication::~H_GlobalApplication()
{

}

void H_GlobalApplication::setWindowInstance(QWidget *wnd)
{
     widget = wnd;
}

#include <QDebug>

bool H_GlobalApplication::notify(QObject *obj, QEvent *e)
{
    const QMetaObject* objMeta = obj->metaObject();
    QString clName = objMeta->className();

    if(e->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
        if(keyEvent->key() == Qt::Key_F1)
        {

        }
//      widget->keyPress(keyEvent);
     }
    else if(e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
        if(mouseEvent->buttons() == Qt::LeftButton)
             qDebug()<<123321;
    }
    return QApplication::notify(obj,e);
}
