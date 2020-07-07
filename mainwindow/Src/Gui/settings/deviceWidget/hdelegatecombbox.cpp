#include "hdelegatecombbox.h"
#include <QComboBox>
#include <QLineEdit>
#include <QPainter>
#include <QStyle>
#include "tablemodel.h"


HDelegateCombbox::HDelegateCombbox(QObject *parent):QStyledItemDelegate(parent)
{


}



QWidget* HDelegateCombbox::createEditor(QWidget *parent,  const QStyleOptionViewItem &option,const  QModelIndex &index)  const
{
    Q_UNUSED(option);


    switch (index.column()) {
    case ColumnType::index:
    {
        return nullptr;
    }
    case ColumnType::inputName:
    {
        QLineEdit *lineedit=new QLineEdit(parent);
        return lineedit;
    }
    case ColumnType::input:
    {
        return nullptr;
    }
    case ColumnType::outputName:
    {
        QLineEdit *lineedit=new QLineEdit(parent);
        return lineedit;
    }
    case ColumnType::output:
    {
        return nullptr;
    }
    case ColumnType::outState:
    {
        return nullptr;
    }

    }

    return QStyledItemDelegate::createEditor(parent, option, index);

}



void  HDelegateCombbox::setEditorData(QWidget *editor,  const  QModelIndex &index)  const
{
    switch (index.column()) {
    case ColumnType::index:
    {

    }
        return;
    case ColumnType::inputName:
    {

        QString text = index.model()->data(index).toString();
        QLineEdit *textEdit =  static_cast <QLineEdit*>(editor);
        textEdit->setAlignment(Qt::AlignCenter);
        textEdit->setText(text);
    }
        return;
    case ColumnType::input:
    {

    }
        return;
    case ColumnType::outputName:
    {
        QString text = index.model()->data(index).toString();
        QLineEdit *textEdit =  static_cast <QLineEdit*>(editor);
        textEdit->setText(text);
        textEdit->setAlignment(Qt::AlignCenter);
    }
        return;
    case ColumnType::output:
    {

    }
        return;
    }
    QStyledItemDelegate::setEditorData(editor, index);

}



void  HDelegateCombbox::setModelData(QWidget *editor, QAbstractItemModel *model,const  QModelIndex &index)  const
{

    switch (index.column()) {
    case ColumnType::index:
    {

    }
        break;
    case ColumnType::inputName:
    {
        QLineEdit *textEdit =  static_cast <QLineEdit*>(editor);
        model->setData(index, textEdit->displayText());

    }
        return;
    case ColumnType::input:
    {

    }
        break;
    case ColumnType::outputName:
    {
        QLineEdit *textEdit =  static_cast <QLineEdit*>(editor);
        model->setData(index, textEdit->displayText());

    }
        return;
    case ColumnType::output:
    {

    }
        break;
    }

    QStyledItemDelegate::setModelData(editor, model, index);


}

//QSize HDelegateCombbox::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
//{

//}

void HDelegateCombbox::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);

    switch (index.column())
    {
    case ColumnType::index:
    {

    }
        break;
    case ColumnType::inputName:
    {

    }
        break;
    case ColumnType::input:
    {
        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect,option.palette.highlight());


        QColor color=Qt::blue;
        int data=index.data().toInt();
        if(data==0){
            color=Qt::green;
        }
        else if(data==-1){
            color=Qt::gray;
        }
        else if(data==1){
            color=Qt::red;
        }

        painter->setBrush(color);
        painter->setPen(color);
        painter->drawEllipse(option.rect.center(),10,10);
        painter->restore();
    }
        return;
    case ColumnType::outputName:
    {

    }
        break;

    case ColumnType::output:
    {

    }
        break;


    case ColumnType::outState:
    {
        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect,option.palette.highlight());


        QColor color=Qt::blue;
        int data=index.data().toInt();
        if(data==0){
            color=Qt::green;
        }
        else if(data==-1){
            color=Qt::gray;
        }
        else if(data==1){
            color=Qt::red;
        }

        painter->setBrush(color);
        painter->setPen(color);
        painter->drawEllipse(option.rect.center(),10,10);
        painter->restore();
    }

        return;

    }
    painter->restore();

    QStyledItemDelegate::paint(painter, option, index);
}

void  HDelegateCombbox::updateEditorGeometry(QWidget *editor,const  QStyleOptionViewItem &option,  const  QModelIndex &index) const
{

    Q_UNUSED(index);

    editor->setGeometry(option.rect);

}

