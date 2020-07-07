
#include <QtWidgets>

#include "hcommands.h"

#include "Src/window/programhandler.h"


MoveCommand::MoveCommand(HGraphicsItem *diagramItem, const QPointF &oldPos,
                         QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myDiagramItem = diagramItem;
    newPos = diagramItem->pos();
    myOldPos = oldPos;
}

bool MoveCommand::mergeWith(const QUndoCommand *command)
{
    const MoveCommand *moveCommand = static_cast<const MoveCommand *>(command);
    HGraphicsItem *item = moveCommand->myDiagramItem;

    if (myDiagramItem != item)
        return false;

    newPos = item->pos();
    setText(QObject::tr("Move %1")
            .arg(createCommandString(myDiagramItem, newPos)));

    return true;
}

void MoveCommand::undo()
{
    myDiagramItem->setPos(myOldPos);
    myDiagramItem->scene()->update();
    setText(QObject::tr("Move %1")
            .arg(createCommandString(myDiagramItem, newPos)));
}

void MoveCommand::redo()
{
    myDiagramItem->setPos(newPos);
    setText(QObject::tr("Move %1")
            .arg(createCommandString(myDiagramItem, newPos)));
}

DeleteCommand::DeleteCommand(HGraphicsScene *view, ProgramHandler *ptrProgram,QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myScene = view;
    ptrProgramHandler=ptrProgram;
    myItemIndexs = myScene->getSelectedItemsIndexs();
    for(const auto&k:myScene->getSelectedItems()){
        k->setSelected(false);
    }
    //    setText(QObject::tr("Delete %1")
    //        .arg(createCommandString(myDiagramItem.first(), myDiagramItem.first()->pos())));
}

void DeleteCommand::undo()
{

    auto i = myItemIndexs.constBegin();
    while (i != myItemIndexs.constEnd()) {
        myScene->insertItems(i.key(),i.value());
        ++i;
    }
    ptrProgramHandler->refreshProgram();
}

void DeleteCommand::redo()
{
    auto i = myItemIndexs.constBegin();
    while (i != myItemIndexs.constEnd()) {
        myScene->deleteItems(i.key(),i.value());
        ++i;
    }
    ptrProgramHandler->refreshProgram();
}

AddCommand::AddCommand(HGraphicsScene *scene, HGraphicsItem * addItem, ProgramHandler *ptrProgram,QUndoCommand *parent): QUndoCommand(parent)
{
    myscene = scene;
    ptrProgramHandler=ptrProgram;
    myAddItem =addItem;
    layerIndex=scene->getActiveLayer();
    itemIndex=scene->getFirstHighlightItemIndex();
}


AddCommand::~AddCommand()
{

}


void AddCommand::undo()
{
    layerIndex=myscene->deleteItem(myAddItem);
    ptrProgramHandler->refreshProgram();
}

void AddCommand::redo()
{
    myscene->insertItem(layerIndex,itemIndex,myAddItem);
    ptrProgramHandler->refreshProgram();
    qDebug()<<1231231231231321311;
}

QString createCommandString(QGraphicsItem *item, const QPointF &pos)
{
    QString name;
    switch (item->type()) {
    case HGraphicsPathItem::Type:
        name="curve";
        break;
    default:
        break;
    }
    return QObject::tr("%1 at (%2, %3)").arg(name).arg(pos.x()).arg(pos.y());
}
