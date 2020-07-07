#include "delegate.h"
#include "treemodel.h"
#include "programdata.h"
#include "programhandler.h"
#include "editorwidget.h"
#include "editdata.h"

#include <QToolButton>
#include <QPainter>
#include <QIcon>




MyButtonDelegate::MyButtonDelegate(QObject *parent):QStyledItemDelegate(parent)
{



}

QWidget *MyButtonDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem & option ,const QModelIndex & index ) const
{
    //if (index.data().canConvert<EditData>()) {

        if(qvariant_cast<EditData>(index.data()).m_type){
            EditorWidget *editor = new EditorWidget(parent);
            connect(editor, &EditorWidget::editingFinished,this, &MyButtonDelegate::commitAndCloseEditor);
            return editor;
        }
        return nullptr;

//    } else {
//        return QStyledItemDelegate::createEditor(parent, option, index);
//    }
}

void MyButtonDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
  //  if (index.data().canConvert<EditData>()) {
        EditData data = qvariant_cast<EditData>(index.data());
        EditorWidget *starEditor = qobject_cast<EditorWidget *>(editor);
        starEditor->setEditData(index,data);
//    } else {
//        QStyledItemDelegate::setEditorData(editor, index);
//    }
}

void MyButtonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
  //  if (index.data().canConvert<EditData>()) {
        EditorWidget *starEditor = qobject_cast<EditorWidget *>(editor);
        starEditor->editData();
        starEditor->editCompleted();

//    } else {
//        QStyledItemDelegate::setModelData(editor, model, index);
//    }

}

QSize MyButtonDelegate::sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const
{
 //   if (index.data().canConvert<EditData>()) {
        EditData data = qvariant_cast<EditData>(index.data());
        return data.sizeHint();
//    } else {
//        return QStyledItemDelegate::sizeHint(option, index);
//    }
}

void MyButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
 //   if (index.data().canConvert<EditData>()) {
        EditData starRating = qvariant_cast<EditData>(index.data());

        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, Qt::lightGray/*option.palette.highlight()*/);

        starRating.paint(&index,painter, option.rect, option.palette,EditData::ReadOnly);
//    } else {
//        QStyledItemDelegate::paint(painter, option, index);
//    }

}


//void MyButtonDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
//{
//
//}

void MyButtonDelegate::commitAndCloseEditor()
{
    EditorWidget *editor = qobject_cast<EditorWidget *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

