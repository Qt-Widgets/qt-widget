#include "h_treeview.h"
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QDebug>
#include <QHeaderView>
#include <QSettings>
#include<QMimeData>

H_TreeView::H_TreeView(QWidget *parent) : QTreeView(parent)
{
    header()->setSectionsClickable(false);
    header()->setFont(QFont("calibri", 12));
    header()->setDefaultAlignment(Qt::AlignCenter);
    header()->setSectionResizeMode(QHeaderView::Interactive);  // 定宽

    header()->setMinimumSectionSize(120);
    header()->setStretchLastSection(true);
    header()->setHighlightSections(false);  //去选中黑体
    header()->setSectionsMovable(false);

    QSettings settings("AOC", "inno");
    header()->restoreState(settings.value("inory/treeViewHeaderState").toByteArray()/*header()->saveState()*/);

    //ptrTreeView->setEditTriggers(QTreeView::NoEditTriggers);
    setEditTriggers(QAbstractItemView::DoubleClicked/*| QAbstractItemView::SelectedClicked*/);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    ////ptrTreeView->setFocusPolicy(Qt::NoFocus);
    setDragEnabled(true);

    setAcceptDrops(true);
    setExpandsOnDoubleClick(false);
    setDropIndicatorShown(true);
    setDefaultDropAction(Qt::DropAction::MoveAction);
  //  setDragDropMode(QAbstractItemView::DragDrop);
    setUniformRowHeights(true);
    //setDragDropOverwriteMode(true);
    setAnimated(true);
    setIconSize(QSize(20,20));

}

H_TreeView::~H_TreeView()
{
    QSettings settings("AOC", "inno");
    settings.setValue("inory/treeViewHeaderState", /*ptrTreeView->saveGeometry()*/header()->saveState());
}

