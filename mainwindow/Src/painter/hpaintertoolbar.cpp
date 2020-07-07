#include "hpaintertoolbar.h"
#include <QDebug>
#include <QMenu>
#include <QActionGroup>

#include "Src/window/programhandler.h"

#include "hcommands.h"
#include "hmath.h"

#include "math/RCircle.h"
#include "math/RSpline.h"

#include "Src/Gui/h_cameracontrollerwidget.h"
#include "Src/data/inisettings.h"
#include "Src/Motion/h_motioncontroller.h"
#include "Src/Gui/settings/hsystemadjustcontroller.h"


HPainterToolBar::HPainterToolBar(HGraphicsView *view, HGraphicsDrawCacheItem *pdi, QWidget *parent) : QToolBar(parent),ptrView(view),ptrDrawItem(pdi)
{
    ptrProgramHandler=ProgramHandler::GetInstance();
    ptrTransController=HSystemAdjustController::GetInstance();

    undoStack = new QUndoStack(this);

    QAction *undoAction = undoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);
    undoAction->setIcon(QIcon(QStringLiteral(":/resource/icons/undo.svg")));

    QAction *redoAction = undoStack->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);
    redoAction->setIcon(QIcon(QStringLiteral(":/resource/icons/redo.svg")));


    //createUndoView();
    addAction(undoAction);
    m_clickActions.append(undoAction);
    addAction(redoAction);
    m_clickActions.append(redoAction);


    m_clickActions.append(addAction(QIcon(QStringLiteral(":/resource/icons/save.svg")),tr("save"),this,&HPainterToolBar::save));
    m_clickActions.last()->setShortcuts(QKeySequence::Save);
    addSeparator();
    m_clickActions.append(addAction(QIcon(QStringLiteral(":/resource/icons/delete1.svg")),tr("delete"),this,&HPainterToolBar::deleteItems));
    m_clickActions.last()->setShortcuts(QKeySequence::Delete);
    addSeparator();

    m_StateActions.append(addAction(QIcon(QStringLiteral(":/resource/icons/aim.svg")),tr("capture")));
    m_StateActions.append(addAction(QIcon(QStringLiteral(":/resource/icons/node.svg")),tr("show node")));

    addSeparator();


    QActionGroup *modeActionGroup=new QActionGroup(this);
    QAction *action;
    action=addAction(QIcon(QStringLiteral(":/resource/icons/cursor.svg")),tr("single select items"));
    action->setChecked(true);
    modeActionGroup->addAction(action);
    addSeparator();

    action=addAction(QIcon(QStringLiteral(":/resource/icons/location.svg")),tr("mouse click"));
    modeActionGroup->addAction(action);

    QMenu *menu = new QMenu(tr("location offset"), this);
    menu->setIcon(QIcon(QStringLiteral(":/resource/icons/location2.svg")));
    QActionGroup *location2ActionGroup=new QActionGroup(this);
    location2ActionGroup->addAction(menu->addAction(tr("use cutter")));
    location2ActionGroup->addAction(menu->addAction(tr("use cutter(two point)")));
    location2ActionGroup->addAction(menu->addAction(tr("use vision")));
    location2ActionGroup->addAction(menu->addAction(tr("use vision(two point)")));

    for(const auto&k:location2ActionGroup->actions()){
        k->setCheckable(true);
    }

    QSettings settings("QtApp", "inno");
    location2ActionGroup->actions().value(settings.value("UserSettings/location2Style").toInt())->setChecked(true);
    connect(location2ActionGroup,&QActionGroup::triggered,this,[=](QAction *action){
        qint32 style=location2ActionGroup->actions().indexOf(action);
        QSettings settings("QtApp", "inno");
        settings.setValue("UserSettings/location2Style",style);
    });

    addAction(menu->menuAction());
    modeActionGroup->addAction(menu->menuAction());

    addSeparator();

    action=addAction(QIcon(QStringLiteral(":/resource/icons/line.svg")),tr("draw line"));
    modeActionGroup->addAction(action);
    action=addAction(QIcon(QStringLiteral(":/resource/icons/circle.svg")),tr("draw circle"));
    modeActionGroup->addAction(action);
    //action=addAction(QIcon(QStringLiteral(":/resource/icons/ellipse.svg")),tr("draw ellipse"));
    //modeActionGroup->addAction(action);
    action=addAction(QIcon(QStringLiteral(":/resource/icons/arc1.svg")),tr("draw arc"));
    modeActionGroup->addAction(action);
    action=addAction(QIcon(QStringLiteral(":/resource/icons/rectangle.svg")),tr("draw rect"));
    modeActionGroup->addAction(action);
    action=addAction(QIcon(QStringLiteral(":/resource/icons/spline.svg")),tr("draw spline"));
    modeActionGroup->addAction(action);
    addSeparator();
    action=addAction(QIcon(QStringLiteral(":/resource/icons/add.svg")),tr("draw text"));
    modeActionGroup->addAction(action);

    for(const auto&k:modeActionGroup->actions()){
        k->setCheckable(true);
    }

    modeActionGroup->actions().at(action_singleSelect)->setChecked(true);
    connect(modeActionGroup,&QActionGroup::triggered,this,[=](QAction *action){

        windowMode=modeActionGroup->actions().indexOf(action);

        //ptrView->setDragMode(windowMode==action_singleSelect?QGraphicsView::RubberBandDrag:QGraphicsView::NoDrag);
        ptrView->setModifyMode(windowMode==action_singleSelect);
    });

    setIconSize(QSize(25,25));
    setOrientation(Qt::Vertical);

    connect(ptrView->getScene(),&HGraphicsScene::sceneMouseMoved,this,&HPainterToolBar::handlerMouseMoved);
    connect(ptrView->getScene(),&HGraphicsScene::sceneMousePressed,this,&HPainterToolBar::handlerMousePressed);
    connect(ptrView->getScene(),&HGraphicsScene::sceneMouseReleased,this,&HPainterToolBar::handlerMouseReleased);

    action=new QAction(this);
    action->setShortcuts(QKeySequence::Quit);
    //connect(action,&QAction::triggered,this,&HPainterToolBar::resetDrawItem);

    //setEnabled(false);
}


void HPainterToolBar::save()
{
    undoStack->clear();
}

void HPainterToolBar::deleteItems()
{
    auto scene=ptrView->getScene();
    if (scene->getSelectedItems().isEmpty())
        return;

    QUndoCommand *deleteCommand = new DeleteCommand(scene,ptrProgramHandler);
    undoStack->push(deleteCommand);
    ptrView->getScene()->handlerSelectionChange();

}

void HPainterToolBar::addItem(HGraphicsItem *addItem)
{
    QUndoCommand *addCommand = new AddCommand(ptrView->getScene(),addItem,ptrProgramHandler);
    undoStack->push(addCommand);
}

void HPainterToolBar::groupItems()
{

}



void HPainterToolBar::handlerMouseMoved(QGraphicsSceneMouseEvent *mouseEvent, const QPointF &mmpos)
{

    switch (windowMode) {
    case action_singleSelect:{

        if(buttonPressed){
            auto mkg=ptrView->getScene();
            QPainterPath path;
            auto rectd=QRectF(mouseEvent->buttonDownScenePos(Qt::LeftButton),mouseEvent->scenePos());
            mkg->selectedRectItem->setRect(rectd);
            path.addRect(rectd);
            mkg->setSelectionArea(path,Qt::ContainsItemShape);

        }
    }
        break;
    case action_location:{

    }
        break;
    case action_alignmentOffset:{


        QSettings settings("QtApp", "inno");
        auto style=settings.value("UserSettings/location2Style").toInt();

        switch (style) {
        case 0:{

        }
            break;
        case 1:{


        }
            break;
        case 2:{

        }
            break;
        case 3:{

        }
            break;
        default:
            break;
        }

    }
        break;
    case action_drawLine:{
        qint32 size=mousePts.size();
        if(size==1){
            QPainterPath path;
            path.moveTo(mousePts.first());
            path.lineTo(mmpos);
            ptrDrawItem->setPainterPath(path);
            ptrView->getScene()->update();
        }
    }
        break;
    case action_drawCircle:{
        qint32 size=mousePts.size();
        if(size==1){
            QPainterPath path;
            QRectF rect;
            double r=H_Math::get2PointDis(mmpos,mousePts.first())*2.0;
            rect.setWidth(r);
            rect.setHeight(r);
            rect.moveCenter(mousePts.first());
            path.addEllipse(rect);
            ptrDrawItem->setPainterPath(path);
            ptrView->getScene()->update();
        }
    }
        break;
        //    case action_drawEllipse:{

        //    }
        //        break;
    case action_drawArc:{
        qint32 size=mousePts.size();
        if(size==1){
            QPainterPath path;
            path.moveTo(mousePts.first());
            path.lineTo(mmpos);
            ptrDrawItem->setPainterPath(path);
            ptrView->getScene()->update();
        }else if(size==2){
            auto arc=RArc(mousePts.first(),H_Math::get2PointDis(mousePts.first(),mousePts.at(1))
                          ,RLine(mousePts.at(0),mousePts.at(1)).getAngle(),
                          RLine(mousePts.at(0),mmpos).getAngle());
            QPainterPath path;
            path.moveTo(mousePts.first());
            path.lineTo(mousePts.at(1));
            QList<RSpline> splines = RSpline::createSplinesFromArc(arc);
            for (int i=0; i<splines.length(); i++) {
                RSpline spline = splines[i];
                path.cubicTo(spline.getControlPointAt(1).get2DPoint(), spline.getControlPointAt(2).get2DPoint(), spline.getControlPointAt(3).get2DPoint());
            }
            ptrDrawItem->setPainterPath(path);
            ptrView->getScene()->update();
        }
    }
        break;
    case action_drawRectangle:{
        qint32 size=mousePts.size();
        if(size==1){
            QPainterPath path;
            path.addRect(QRectF(mousePts.first(),mmpos));
            ptrDrawItem->setPainterPath(path);
            ptrView->getScene()->update();
        }
    }
        break;
    case action_drawSpline:{
        qint32 size=mousePts.size();
        if(size>0){
            QPainterPath path;
            path.moveTo(mousePts.first());
            path.lineTo(mmpos);
            ptrDrawItem->setPainterPath(path);
            ptrView->getScene()->update();
        }
        if(size>1){
            mousePts.append(mmpos);
            RPainterPath path(QSharedPointer<RShape>(new RSpline(mousePts,2)));
            ptrDrawItem->setPainterPath(path);
            ptrView->getScene()->update();
            mousePts.removeLast();
        }
    }
        break;
    case action_drawText:{

    }
        break;
    default:
        break;
    }
}

void HPainterToolBar::handlerMousePressed(QGraphicsSceneMouseEvent *mouseEvent, const QPointF &mmpos)
{
    buttonPressed=mouseEvent->button()==Qt::LeftButton;

    switch (windowMode) {
    case action_singleSelect:{

        auto mkg=ptrView->getScene();

        QList<QGraphicsItem *> sitems;
        if(mouseEvent->modifiers()&Qt::ControlModifier){
            sitems= mkg->selectedItems();
        }

        mkg->setSelectionArea(mkg->mouseItem->getContainSahpe(),Qt::IntersectsItemShape);
        QGraphicsItem *item=nullptr;
        double dis=LLONG_MAX;
        for(const auto&k:mkg->selectedItems()){
            k->setSelected(false);
            auto dddd=((HGraphicsItem*)k)->getDistanceToShape(mkg->mouseItem->pos());
            if(dddd<dis){
                dis= dddd;
                item=k;
            }
        }
        if(item){
            item->setSelected(true);
        }
        for(const auto&k:sitems){
            k->setSelected(true);
        }
    }
        break;
    case action_location:{
        if(ptrTransController->moveToAxisPosFromPainterPosUseCutter(mmpos))
            break;
        ptrView->getScene()->cameraPosItem->setMPos(mmpos);
        //ptrView->update();
    }
        break;
    case action_alignmentOffset:{


        QSettings settings("QtApp", "inno");
        auto style=settings.value("UserSettings/location2Style").toInt();

        switch (style) {
        case 0:{


            //            QPointF pos=QPointF(H_MotionController::GetInstance()->GetAxis(0)->GetFeedbackPos(),
            //                                H_MotionController::GetInstance()->GetAxis(1)->GetFeedbackPos());
            //            QTransform transt1=QTransform::fromTranslate(-mmpos.x()+pos.x(),-mmpos.y()+pos.y());

            //            ptrView->getScene()->transAllItems(transt1);

        }
            break;
        case 1:{


        }
            break;
        case 2:{
             QPointF ppos;
            if(ptrTransController->getPainterPosFromImageUseCutter(ppos)){
                break;
            }
            ppos=ppos-mmpos;
            ptrView->getScene()->transAllItems(QTransform::fromTranslate(ppos.x(),ppos.y()));
        }
            break;
        case 3:{

        }
            break;
        default:
            break;
        }

    }
        break;
    case action_drawLine:{
        qint32 size=mousePts.size();
        if(size==0){
            if(mouseEvent->button()==Qt::LeftButton){
                mousePts<<mmpos;
            }

        }else if(size==1){
            if(mouseEvent->button()==Qt::LeftButton){
                addItem(new HGraphicsPathItem(QSharedPointer<RShape>(new RLine(mousePts.first(),mmpos)),nullptr,false));
                mousePts.clear();
                ptrDrawItem->resetData();
                ptrView->getScene()->update();
            }else if(mouseEvent->button()==Qt::RightButton){
                mousePts.removeLast();
            }

        }else{

        }

    }
        break;
    case action_drawCircle:{
        qint32 size=mousePts.size();
        if(size==0){
            if(mouseEvent->button()==Qt::LeftButton){
                mousePts<<mmpos;
            }

        }else if(size==1){
            if(mouseEvent->button()==Qt::LeftButton){

                addItem(new HGraphicsPathItem(QSharedPointer<RShape>(new RCircle(mousePts.first(),H_Math::get2PointDis(mousePts.first(),mmpos))),nullptr,false));
                mousePts.clear();
                ptrDrawItem->resetData();
                ptrView->getScene()->update();

            }else if(mouseEvent->button()==Qt::RightButton){
                mousePts.removeLast();
            }


        }else{

        }
    }
        break;
        //    case action_drawEllipse:{

        //    }
        //        break;
    case action_drawArc:{
        qint32 size=mousePts.size();
        if(size==0){
            if(mouseEvent->button()==Qt::LeftButton){
                mousePts<<mmpos;
            }

        }else if(size==1){
            if(mouseEvent->button()==Qt::LeftButton){
                mousePts<<mmpos;
            }else if(mouseEvent->button()==Qt::RightButton){
                mousePts.removeLast();
            }
        }else if(size==2){
            if(mouseEvent->button()==Qt::LeftButton){
                addItem(new HGraphicsPathItem(QSharedPointer<RShape>(new RArc(mousePts.first(),H_Math::get2PointDis(mousePts.first(),mousePts.at(1))
                                                                              ,RLine(mousePts.at(0),mousePts.at(1)).getAngle(),
                                                                              RLine(mousePts.at(0),mmpos).getAngle())),nullptr,false));
                mousePts.clear();
                ptrDrawItem->resetData();
                ptrView->getScene()->update();

            }else if(mouseEvent->button()==Qt::RightButton){
                mousePts.removeLast();
            }

        }else{

        }
    }
        break;
    case action_drawRectangle:{
        qint32 size=mousePts.size();
        if(size==0){
            if(mouseEvent->button()==Qt::LeftButton){
                mousePts<<mmpos;
            }

        }else if(size==1){
            if(mouseEvent->button()==Qt::LeftButton){

                addItem(new HGraphicsRectItem(QRectF(mousePts.first(),mmpos).normalized(),nullptr,false));
                mousePts.clear();
                ptrDrawItem->resetData();
                ptrView->getScene()->update();
            }else if(mouseEvent->button()==Qt::RightButton){
                mousePts.removeLast();
            }

        }else{

        }
    }
        break;
    case action_drawSpline:{

        if(mouseEvent->button()==Qt::LeftButton){
            mousePts<<mmpos;
        }else if(mouseEvent->button()==Qt::RightButton){
            addItem(new HGraphicsPathItem(QSharedPointer<RShape>(new RSpline(mousePts,2)),nullptr,false));
            mousePts.clear();
            ptrDrawItem->resetData();
            ptrView->getScene()->update();
        }
    }
        break;
    case action_drawText:{

    }
        break;
    default:
        break;
    }

}

void HPainterToolBar::handlerMouseReleased(QGraphicsSceneMouseEvent *mouseEvent, const QPointF &mmpos)
{
    buttonPressed=false;
    ptrView->getScene()->selectedRectItem->setRect(QRect());
}



void HPainterToolBar::createUndoView()
{
    undoView = new QUndoView(undoStack);
    undoView->setWindowTitle(tr("Command List"));
    undoView->show();
    undoView->setAttribute(Qt::WA_QuitOnClose, false);
}

