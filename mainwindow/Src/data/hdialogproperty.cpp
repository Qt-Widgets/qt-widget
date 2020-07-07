#include "hdialogproperty.h"
#include "ui_hdialogproperty.h"
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QToolButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>

HDialogProperty::HDialogProperty(const QString &title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HDialogProperty)
{
    setFont(MyFont1);
    ui->setupUi(this);
    setWindowTitle(title);
    setWindowFlags(Qt::WindowCloseButtonHint|Qt::WindowMinMaxButtonsHint);


    //   ui->layout_label->setAlignment(Qt::AlignTop);
    //   ui->layout_content->setAlignment(Qt::AlignTop);
}

HDialogProperty::~HDialogProperty()
{
    delete ui;
}

void HDialogProperty::setLabels(const QStringList &names)
{
    labelNames=names;
    for(const auto &k:names){
        QLabel *label=new QLabel(k,this);
        //label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        label->setFixedHeight(25);
        ui->layout_label->addWidget(label);
    }

    //  ui->layout_label->addStretch();

}

void HDialogProperty::setLabels(const QString &names)
{
    labelNames=names.split(",");
    for(const auto &k:labelNames){
        QLabel *label=new QLabel(k,this);
        //label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        label->setFixedHeight(25);
        ui->layout_label->addWidget(label);
    }
}

void HDialogProperty::setContents(const QString &types,  QVariantList datas)
{
    contentTypes.clear();
    for(const auto&k:types.split(',')){

        ContentCtrl ttye;
        ttye.type=k.toInt();
        switch (ttye.type) {
        case 0:{
            QLineEdit *lineEdit=new QLineEdit(datas.takeFirst().toString(),this);
            lineEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
            lineEdit->setFixedHeight(25);
            ttye.ptrWid=lineEdit;
            ui->layout_content->addWidget(ttye.ptrWid);
        }
            break;
        case 1:{
            QComboBox *comb=new QComboBox(this);
            comb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
            comb->setFixedHeight(25);
            comb->addItems(datas.takeFirst().toStringList());
            ttye.ptrWid=comb;
            ui->layout_content->addWidget(comb);
        }
            break;
        case 2:{

            QHBoxLayout *hlayout=new QHBoxLayout;
            QLineEdit *lineEdit=new QLineEdit(datas.takeFirst().toString(),this);
            lineEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
            lineEdit->setFixedHeight(25);
            ttye.ptrWid=lineEdit;
            hlayout->addWidget(lineEdit);
            QToolButton *button=new  QToolButton(this);
            button->setText("...");
            connect(button,&QToolButton::pressed,this,[=](){
                QString filePath= QFileDialog::getOpenFileName(button, tr("choose File"), nullptr,m_fileDialogFilter);
                if(!filePath.isEmpty()){
                    lineEdit->setText(filePath);
                }
            });
            button->setFixedSize(25,25);
            hlayout->addWidget(button);
            ui->layout_content->addLayout(hlayout);


        }
            break;
        default:
            break;
        }
        contentTypes<<ttye;
    }
    // ui->layout_content->addStretch();

}

QVariantList HDialogProperty::getContentData() const
{
    QVariantList list;
    for(const auto&k:contentTypes){
        switch (k.type) {
        case 0:{
            list<<qobject_cast<QLineEdit*>(k.ptrWid)->text();
        }
            break;
        case 1:{
            list<<qobject_cast<QComboBox*>(k.ptrWid)->currentText();
        }
            break;
        case 2:{
            list<<qobject_cast<QLineEdit*>(k.ptrWid)->text();
        }
            break;
        default:
            break;
        }
    }
    return list;
}

void HDialogProperty::on_toolButton_ok_clicked()
{
    if(m_uniqueKey!=-1){
        if(m_UniqueDatas.contains(getContentData().at(m_uniqueKey))){
            QMessageBox::warning(this,tr("parameter error"),QString("%1 of %2 is illegal!!").arg(labelNames.at(m_uniqueKey)).arg(windowTitle()));
            return;
        }
    }

    qint32 size=contentTypes.size();
    for(qint32 k=0;k<size;++k){
        switch (contentTypes.at(k).type) {
        case 0:{
            if(qobject_cast<QLineEdit*>(contentTypes.at(k).ptrWid)->text().isEmpty()){
                QMessageBox::warning(this,tr("parameter error"),QString("%1 of %2 is empty!!").arg(labelNames.at(k)).arg(windowTitle()));
                return;
            }
        }
            break;
        case 1:{
            if(qobject_cast<QComboBox*>(contentTypes.at(k).ptrWid)->currentText().isEmpty()){
                QMessageBox::warning(this,tr("parameter error"),QString("%1 of %2 is empty!!").arg(labelNames.at(k)).arg(windowTitle()));
                return;
            }
        }
            break;
        case 2:{
            if(qobject_cast<QLineEdit*>(contentTypes.at(k).ptrWid)->text().isEmpty()){
                QMessageBox::warning(this,tr("parameter error"),QString("%1 of %2 is empty!!").arg(labelNames.at(k)).arg(windowTitle()));
                return;
            }
        }
            break;
        default:
            break;
        }
    }
    done(1);
}

void HDialogProperty::on_toolButton_cancel_clicked()
{
    done(0);
}
