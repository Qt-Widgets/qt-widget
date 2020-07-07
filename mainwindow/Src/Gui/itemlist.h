#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <QWidget>
#include <QHash>
#include <QList>
#include <QMenu>



class TreeModel;
class ProgramHandler;
class ItemListMenu;
class H_ProcessWidget;
class PainterMainWidget;
class H_TreeView;


class ItemList : public QWidget
{
    Q_OBJECT
public:
    static ItemList* GetInstance(QWidget *parent = nullptr);

signals:


public slots:




private:
    explicit ItemList(QWidget *parent = nullptr);
    static ItemList* m_pInstance;
    ~ItemList() Q_DECL_OVERRIDE;

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    //bool eventFilter(QObject *target, QEvent *event) Q_DECL_OVERRIDE;

private:

    ProgramHandler *pProgramData;
    ItemListMenu *menu;
    H_ProcessWidget *ptrProcessWidget;
    PainterMainWidget *ptrPainterWidget;
    H_TreeView *ptrTreeView;



};

#endif // ITEMLIST_H
