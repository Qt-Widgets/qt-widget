#ifndef HPAINTERTOOLBAR_H
#define HPAINTERTOOLBAR_H

#include <stable.h>
#include <QToolBar>

QT_BEGIN_NAMESPACE
class QAction;
class QUndoStack;
class QUndoView;
QT_END_NAMESPACE




class ProgramHandler;
class HGraphicsView;
class HGraphicsDrawCacheItem;
class HGraphicsItem;
class HSystemAdjustController;

class HPainterToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit HPainterToolBar(HGraphicsView *view,HGraphicsDrawCacheItem *pdi,QWidget *parent = nullptr);

    enum ToolStateMode{
        action_singleSelect,
        action_location,
        action_alignmentOffset,
        action_drawLine,
        action_drawCircle,
        //action_drawEllipse,
        action_drawArc,
        action_drawRectangle,
        action_drawSpline,
        action_drawText,

    };



protected:

private slots:

    void save();

    void deleteItems();

    void addItem(HGraphicsItem *addItem);

    void groupItems();



    void handlerMouseMoved(QGraphicsSceneMouseEvent *mouseEvent,const QPointF& mmpos);
    void handlerMousePressed(QGraphicsSceneMouseEvent *mouseEvent,const QPointF& mmpos);
    void handlerMouseReleased(QGraphicsSceneMouseEvent *mouseEvent,const QPointF& mmpos);


private:
    void createUndoView();

private:
    QList< QPointF> mousePts;


    qint32 windowMode=0;


    bool buttonPressed=false;



    ProgramHandler *ptrProgramHandler=nullptr;
    HGraphicsView *ptrView=nullptr;
    HSystemAdjustController *ptrTransController=nullptr;

    HGraphicsDrawCacheItem * ptrDrawItem;


    QVector<QAction*> m_StateActions;
    QVector<QAction*> m_clickActions;


    QUndoStack *undoStack;
    QUndoView *undoView;

};

#endif // HPAINTERTOOLBAR_H
