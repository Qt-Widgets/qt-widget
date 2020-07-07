#include "htableviewdelegate.h"
#include <QComboBox>
#include <QLineEdit>
#include <QPainter>
#include <QStyle>
#include <QPushButton>
#include <QFileDialog>
#include <QHBoxLayout>


HTableViewDelegate::HTableViewDelegate(QObject *parent, const QVector<bool> &editables, const HDelegateTypes &types):QStyledItemDelegate(parent)
{
    m_types=types;
    m_editables=editables;
}

void HTableViewDelegate::SetAllColDelegateTypes(const HAllColDelegateTypes &types)
{
    m_types.clear();
    m_Coltypes=types;
}

void HTableViewDelegate::SetAllDelegateTypes(const HDelegateTypes &types)
{
    m_Coltypes.clear();
    m_types=types;
}

void HTableViewDelegate::setHighlightRowsUseFirstColumn(const QVariantList &names)
{
    highlightRows=names;
}




QWidget* HTableViewDelegate::createEditor(QWidget *parent,  const QStyleOptionViewItem &option,const  QModelIndex &index)  const
{
    Q_UNUSED(option);


    if(!m_editables.isEmpty()&&!m_editables.at(index.column())){
        return nullptr;
    }

    if(m_types.isEmpty()&&m_Coltypes.isEmpty()){
        return new QLineEdit(parent);
    }


    switch (m_Coltypes.isEmpty()?m_types[index.row()][index.column()]:m_Coltypes[index.column()]) {
    case TVDelegateType::TV_LineEdit:
    {
        return new  QLineEdit(parent);
    }
    case TVDelegateType::TV_Combbox:
    {
        return new  QComboBox(parent);
    }
    case TVDelegateType::TV_FileDialog:
    {

        QWidget *wid=new QWidget(parent);
        QHBoxLayout *layout=new QHBoxLayout(wid);
        layout->setMargin(0);
        layout->setContentsMargins(0,0,0,0);
        layout->setSpacing(0);
        QLineEdit *edit= new QLineEdit(wid);
        edit->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        //edit->setObjectName("fileDialogEdit");
        edit->setFixedHeight(29);
        QPushButton *button=new  QPushButton("...",wid);

        connect(button,&QPushButton::pressed,this,[=](){
            QString filePath= QFileDialog::getOpenFileName(button, tr("HConfiguration File"), nullptr,"config file (*)");
            if(!filePath.isEmpty()){
                edit->setText(filePath);
            }
        });
        button->setFixedSize(30,30);
        layout->addWidget(edit);
        layout->addWidget(button);
        return wid;
    }
    default:
        return QStyledItemDelegate::createEditor(parent, option, index);

    }



}



void  HTableViewDelegate::setEditorData(QWidget *editor,  const  QModelIndex &index)  const
{

    if(m_types.isEmpty()&&m_Coltypes.isEmpty()){
        QLineEdit *textEdit =  static_cast <QLineEdit*>(editor);
        textEdit->setAlignment(Qt::AlignCenter);
        textEdit->setText(index.model()->data(index).toString());
        return;
    }


    switch (m_Coltypes.isEmpty()?m_types[index.row()][index.column()]:m_Coltypes[index.column()]) {
    case TVDelegateType::TV_LineEdit:
    {
        QLineEdit *textEdit =  static_cast <QLineEdit*>(editor);
        textEdit->setAlignment(Qt::AlignCenter);
        textEdit->setText(index.model()->data(index,Qt::EditRole).toString());
        return;
    }

    case TVDelegateType::TV_Combbox:
    {
        QComboBox *combbox =  static_cast <QComboBox*>(editor);
        QStringList listc=index.model()->data(index,Qt::EditRole).toStringList();
        if(listc.isEmpty())
            return;
        QString cindex=listc.takeFirst();
        combbox->addItems(listc);
        combbox->setCurrentText(cindex);
        return;
    }

    case TVDelegateType::TV_FileDialog:
    {
        QLineEdit * linEditor=editor->findChild<QLineEdit *>();
        linEditor->setText(index.model()->data(index,Qt::EditRole).toString());
        return ;
    }

    }
    QStyledItemDelegate::setEditorData(editor, index);

}



void  HTableViewDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const  QModelIndex &index)  const
{

    if(m_types.isEmpty()&&m_Coltypes.isEmpty()){
        model->setData(index, static_cast <QLineEdit*>(editor)->displayText());

        return;
    }

    switch (m_Coltypes.isEmpty()?m_types[index.row()][index.column()]:m_Coltypes[index.column()]) {
    case TVDelegateType::TV_LineEdit:
    {
        model->setData(index, static_cast <QLineEdit*>(editor)->displayText());
    }
        break;
    case TVDelegateType::TV_Combbox:
    {
        QComboBox *combbox=static_cast <QComboBox*>(editor);


        QStringList data;
        data<<combbox->currentText();
        for(qint32 k=0;k<combbox->count();++k){
            data<<combbox->itemText(k);
        }

        model->setData(index, data);
    }
        break;
    case TVDelegateType::TV_FileDialog:
    {
        QLineEdit * linEditor=editor->findChild<QLineEdit *>();
        model->setData(index, linEditor->text());
    }
        break;
    default:
        QStyledItemDelegate::setModelData(editor, model, index);
    }




}

//QSize HTableViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
//{

//}

void HTableViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);

    if(!m_types.isEmpty()||!m_Coltypes.isEmpty()){
        switch (m_Coltypes.isEmpty()?m_types[index.row()][index.column()]:m_Coltypes[index.column()]) {
        case TVDelegateType::TV_LineEdit:
        {


        }
            break;
        case TVDelegateType::TV_Combbox:
        {


        }
            break;
        case TVDelegateType::TV_FileDialog:
        {


        }
            break;
        }
    }


    if(!m_editables.isEmpty()&&!m_editables.value(index.column())){

        //        if (option.state & QStyle::State_Selected){
        //            painter->fillRect(option.rect, option.palette.highlight());
        //        }
        //        else{
        //            painter->fillRect(option.rect, Qt::darkGreen);
        //        }

        painter->fillRect(option.rect, option.state & QStyle::State_Selected?option.palette.highlight():QColor(174,196,208,index.row()%2==0?100:233));
    }



    if(option.state & QStyle::State_Selected){
        painter->fillRect(option.rect, Qt::darkGreen);
    }

    if(highlightRows.contains(index.siblingAtColumn(0).data())){
        painter->fillRect(option.rect, Qt::cyan);
    }

    painter->restore();

    QStyledItemDelegate::paint(painter, option, index);
}

void  HTableViewDelegate::updateEditorGeometry(QWidget *editor,const  QStyleOptionViewItem &option,  const  QModelIndex &index) const
{

    Q_UNUSED(index);

    if(m_types.isEmpty()&&m_Coltypes.isEmpty()){
        editor->setGeometry(option.rect);
        return;
    }


    switch (m_Coltypes.isEmpty()?m_types[index.row()][index.column()]:m_Coltypes[index.column()]) {
    case TVDelegateType::TV_LineEdit:
    {
        editor->setGeometry(option.rect);
        return;
    }

    case TVDelegateType::TV_Combbox:
    {
        editor->setGeometry(option.rect);
        return;
    }

    case TVDelegateType::TV_FileDialog:
    {
        editor->setGeometry(option.rect);
        return ;
    }

    }

    editor->setGeometry(option.rect);
    return;

}



