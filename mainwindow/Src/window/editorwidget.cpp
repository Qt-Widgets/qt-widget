#include "hcommon.h"
#include "editorwidget.h"
#include <QToolButton>
#include <QColorDialog>
#include "programdata.h"
#include "programhandler.h"
#include "treemodel.h"

#include "Src/painter/paintermainwidget.h"


static QPixmap genColorIcon(QSize size,const QColor& color)
{
    QImage image(size,QImage::Format_ARGB32_Premultiplied);

    for(int k=0;k<size.height();++k){
        for(int j=0;j<size.width();++j){
            image.setPixelColor(j,k,color);
        }
    }
    return QPixmap::fromImage(image, Qt::DiffuseDither|Qt::DiffuseAlphaDither);
}


EditorWidget::EditorWidget(QWidget *parent):QWidget(parent)
{

    setMouseTracking(true);
    setAutoFillBackground(true);
    button1=new QToolButton(this);
    button2=new QToolButton(this);
    button3=new QToolButton(this);
    //    button1->setCheckable(true);
    button2->setCheckable(true);
    button3->setCheckable(true);

    m_IconList.append(QIcon(QStringLiteral(":/resource/icons/eye-disable.svg")));
    m_IconList.append(QIcon(QStringLiteral(":/resource/icons/eye.svg")));
    m_IconList.append(QIcon(QStringLiteral(":/resource/icons/unlocked.svg")));
    m_IconList.append(QIcon(QStringLiteral(":/resource/icons/locked.svg")));

    button1->setIconSize(QSize(40,24));
    button2->setIconSize(QSize(30,30));
    button3->setIconSize(QSize(30,30));

    connect(button1,&QToolButton::clicked,this,[=](){
        QColor color= QColorDialog::getColor(Qt::red,this,tr("change color"));
        if(color.isValid()){
            button1->setIcon(genColorIcon(QSize(40,24),color));
            colorSet=color;
        }
    });
    connect(button2,&QToolButton::clicked,this,[=](){
        checkedSet=button2->isChecked();
        button2->setIcon(m_IconList.at(checkedSet?1:0));

    });

    connect(button3,&QToolButton::clicked,this,[=](){
        lockedSet=button3->isChecked();
        button3->setIcon(m_IconList.at(lockedSet?2:3));

    });

    button1->setGeometry(QRect(rect().topLeft()+QPoint(15,4),QSize(40,24)));
    button2->setGeometry(QRect(rect().topLeft()+QPoint(60,0),QSize(30,30)));
    button3->setGeometry(QRect(rect().topLeft()+QPoint(95,0),QSize(30,30)));
}


QSize EditorWidget::sizeHint() const
{
    return myEditData.sizeHint();
}

void EditorWidget::setEditData(const QModelIndex &index, const EditData &data) {


    myEditData=data;
    myIndex=index;

    // auto inory=myEditData.getIndex(myIndex);

    // auto pData=ProgramHandler::GetInstance()->getProgram(inory.programIndex)->getLayer()->at(inory.layerIndex);

    bool isChecked;
    bool isChecked2;
    QColor color;

    if(data.m_pItem){
        isChecked=data.m_pItem->isVisible();
        isChecked2=data.m_pItem->isEnabled();
        color=data.m_pItem->getColor();
        //data.m_pItem->update();
    }else if(data.m_pLayer){
        isChecked=data.m_pLayer->getVisible();
        isChecked2=data.m_pLayer->getisEnabled();
        color=data.m_pLayer->getColor();
//        for(const auto&k:data.m_pLayer->getItems()){
//            k->update();
//        }
    }else{
        return;
    }



    originalColor=color;
    originalChecked=isChecked;
    originallocked=isChecked2;

    colorSet=color;
    checkedSet=isChecked;
    lockedSet=isChecked2;


    button1->setIcon(genColorIcon(QSize(40,24),color));
    button2->setChecked(isChecked);
    button2->setIcon(m_IconList.at(checkedSet?1:0));

    button3->setChecked(isChecked2);
    button3->setIcon(m_IconList.at(lockedSet?2:3));
}

void EditorWidget::editData()
{

    if(myEditData.m_pItem){
        myEditData.m_pItem->setVisible(checkedSet);
        myEditData.m_pItem->setEnabled(lockedSet);
        myEditData.m_pItem->setColor(colorSet);
    }else if(myEditData.m_pLayer){
        myEditData.m_pLayer->setVisible(checkedSet);
        myEditData.m_pLayer->setisEnabled(lockedSet);
        myEditData.m_pLayer->setColor(colorSet);
    }else{
        return;
    }
}

void EditorWidget::editCompleted()
{

    emit editingFinished();
}


void EditorWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    myEditData.paint(nullptr,&painter, rect(), this->palette(),EditData::Editable);
}


