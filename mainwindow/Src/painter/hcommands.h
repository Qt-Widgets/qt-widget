
#ifndef HCOMMANDS_H
#define HCOMMANDS_H

#include <QUndoCommand>

#include "hgraphicsview.h"

class ProgramHandler;


//! [0]
class MoveCommand : public QUndoCommand
{
public:
    enum { Id = 1234 };

    MoveCommand(HGraphicsItem *diagramItem, const QPointF &oldPos,
                QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override { return Id; }

private:
    HGraphicsItem *myDiagramItem;
    QPointF myOldPos;
    QPointF newPos;
};
//! [0]

//! [1]
class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(HGraphicsScene *view,  ProgramHandler *ptrProgram,QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    QMap<qint32, QMap<qint32,HGraphicsItem*> > myItemIndexs;
    HGraphicsScene *myScene;
     ProgramHandler *ptrProgramHandler;
};
//! [1]

//! [2]
class AddCommand : public QUndoCommand
{
public:
    AddCommand(HGraphicsScene *scene, HGraphicsItem * addItem, ProgramHandler *ptrProgram,QUndoCommand *parent = nullptr);
    ~AddCommand() override;

    void undo() override;
    void redo() override;


private:
    HGraphicsItem *myAddItem;
    HGraphicsScene *myscene;
    ProgramHandler *ptrProgramHandler;
    qint32 layerIndex;
    qint32 itemIndex;
};
//! [2]

QString createCommandString(QGraphicsItem *item, const QPointF &point);

#endif
