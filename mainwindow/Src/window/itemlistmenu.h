#ifndef ITEMLISTMENU_H
#define ITEMLISTMENU_H

#include <QMenu>

class H_TreeView;
class ProgramHandler;
class DialogLayerLoPts ;

class ItemListMenu : public QMenu
{
    Q_OBJECT
public:
    explicit ItemListMenu(H_TreeView * view,QWidget *parent = nullptr);

    void setLayerIndex(qint32 index);



private slots:



private:

    H_TreeView *ptrView;
    ProgramHandler *ptrProgramHandler;
    DialogLayerLoPts *ptrDialogs;

signals:

public slots:

};

#endif // ITEMLISTMENU_H
