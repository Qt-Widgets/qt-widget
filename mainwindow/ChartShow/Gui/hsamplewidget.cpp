
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>

#include "hsamplewidget.h"

HSampleWidget* HSampleWidget::m_pInstance = nullptr;

HSampleWidget *HSampleWidget::GetInstance(QWidget *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HSampleWidget(parent);
    }
    return m_pInstance;
}

HSampleWidget::HSampleWidget(QWidget *parent, Qt::WindowFlags f) : QDialog(parent,f)
{
    setFont(QFont("Microsoft YaHei UI",9));
    QVBoxLayout *parLayout=new QVBoxLayout(this);
    CreatLineEditor1(lineEditors,this,parLayout,tr("Sample interval(ms)"));
    CreatLineEditor2(lineEditors,this,parLayout,tr("X Axis Range"));
    CreatLineEditor2(lineEditors,this,parLayout,tr("Y Axis Range"));

    lineEditors[SAMPLE_INTERVAL]->setText("1000");

    lineEditors[SAMPLE_X_MIN]->setText("0");
    lineEditors[SAMPLE_X_MAX]->setText("30");
    lineEditors[SAMPLE_Y_MIN]->setText("10");
    lineEditors[SAMPLE_Y_MAX]->setText("60");


    QPushButton *buttonSave=new QPushButton(this);
    buttonSave->setText(tr("Apply"));
    connect(buttonSave,&QPushButton::pressed,this,&HSampleWidget::ButtonSaveHandler);

    parLayout->addSpacing(20);
    parLayout->addWidget(buttonSave);
}

qreal HSampleWidget::GetPara(SampleParaType type)
{
    return lineEditors.at(type)->text().toDouble();
}

void HSampleWidget::ButtonSaveHandler()
{
    emit ParaSaved();
}

void HSampleWidget::CreatLineEditor1(QVector<QLineEdit *> &lineVector, QWidget *parentWid, QBoxLayout *parentLay, QString name)
{
    QHBoxLayout *layout=new QHBoxLayout;

    QLabel *label=new QLabel(name,parentWid);
    layout->addWidget(label);

    layout->addSpacing(6);

    QLineEdit *lineEditor=new  QLineEdit(parentWid);
    lineVector<<lineEditor;
    // lineEditor->setAlignment(Qt::AlignCenter);
    lineEditor->setMaxLength(12);
    // lineEditor->setMaximumWidth(100);
    layout->addWidget(lineEditor);

    layout->addStretch();
    //    layout->addWidget(new QLabel("Z",parentWid));
    //    lineEditor=new  QLineEdit(parentWid);
    //    lineVector<<lineEditor;
    //    // lineEditor->setAlignment(Qt::AlignCenter);
    //    lineEditor->setMaxLength(10);
    //    // lineEditor->setMaximumWidth(100);
    //    layout->addWidget(lineEditor);

    layout->addSpacing(6);

    parentLay->addLayout(layout);
}

void HSampleWidget::CreatLineEditor2(QVector<QLineEdit *> &lineVector, QWidget *parentWid, QBoxLayout *parentLay, QString name)
{
    QHBoxLayout *layout=new QHBoxLayout;

    QLabel *label=new QLabel(name,parentWid);
    layout->addWidget(label);

    layout->addSpacing(6);

    QLineEdit *lineEditor=new  QLineEdit(parentWid);

    lineVector<<lineEditor;
    // lineEditor->setAlignment(Qt::AlignCenter);
    lineEditor->setMaxLength(12);
    // lineEditor->setMaximumWidth(100);
    layout->addWidget(lineEditor);

    layout->addSpacing(10);
    lineEditor=new  QLineEdit(parentWid);
    lineVector<<lineEditor;
    lineEditor->setMaxLength(12);
    layout->addWidget(lineEditor);

    //    layout->addWidget(new QLabel("Z",parentWid));
    //    lineEditor=new  QLineEdit(parentWid);
    //    lineVector<<lineEditor;
    //    // lineEditor->setAlignment(Qt::AlignCenter);
    //    lineEditor->setMaxLength(10);
    //    // lineEditor->setMaximumWidth(100);
    //    layout->addWidget(lineEditor);

    layout->addSpacing(6);

    parentLay->addLayout(layout);
}


