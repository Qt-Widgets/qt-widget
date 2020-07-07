#include "dialogexport.h"
#include "ui_dialogexport.h"

DialogExport::DialogExport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogExport)
{
    setFont(MyFont1);
    ui->setupUi(this);

}

DialogExport::~DialogExport()
{
    writeToFile(QCoreApplication::applicationDirPath()+"/plugins/settings/settings_Adjust_export.bin");
    delete ui;
}

void DialogExport::setData(const QStringList &headers, const QVector<QStringList> &datas)
{
    ui->comboBox->addItems(headers);
    ui->comboBox_2->addItems(headers);
    m_data=datas;
    openFromFile(QCoreApplication::applicationDirPath()+"/plugins/settings/settings_Adjust_export.bin");
}

QDataStream &operator>>(QDataStream &input, DialogExport &obj){
    QStringList list;
    input>>list;
    obj.setControlData(list);
    return input;
}

QDataStream &operator<<(QDataStream &output, const DialogExport &obj){
    output<<obj.getControlData();
    return output;
}

void DialogExport::on_toolButton_clicked()
{
    QString filePath= QFileDialog::getSaveFileName(this, tr("txt file"),nullptr,"adjust file (*.smtIni)");

    if(ui->checkBox->checkState()){
        for(auto&k:m_data[ui->comboBox->currentIndex()]){
            if(k.at(0)=='-'){
                k.remove(0,1);
            }else{
                k.prepend('-');
            }
        }
    }

    if(ui->checkBox_2->checkState()){
        for(auto&k:m_data[ui->comboBox_2->currentIndex()]){
            if(k.at(0)=='-'){
                k.remove(0,1);
            }else{
                k.prepend('-');
            }
        }
    }

    QStringList strList;
    auto mkkk=m_data.value(ui->comboBox->currentIndex());
    for(qint32 k=mkkk.size()-1;k>-1;--k){
       strList.append(mkkk.value(k));
    }

    QString str;
    str.append(ui->lineEdit->text());
    str.append(m_data.value(ui->comboBox->currentIndex()).join(ui->lineEdit_2->text()));
    str.append(',');
    str.append(strList.join(ui->lineEdit_2->text()));
    str.append("\n");

    str.append(ui->lineEdit_3->text());
    str.append(m_data.value(ui->comboBox_2->currentIndex()).join(ui->lineEdit_4->text()));
    str.append("\n");

    QFile file(filePath);
    if(file.open(QFile::WriteOnly)){
        file.write(str.toLocal8Bit());
        file.close();
        QMessageBox::information(this,tr("message"),tr("write file succeed!"));
    }else{
        QMessageBox::critical(this,tr("error"),tr("write file failed!"));
    }
}

QStringList DialogExport::getControlData() const
{
    QStringList list;
    list<<ui->lineEdit->text()<<ui->lineEdit_2->text()<<ui->lineEdit_3->text()<<ui->lineEdit_4->text()<<QString::number(ui->comboBox->currentIndex())<<QString::number(ui->comboBox_2->currentIndex())
          <<QString::number(ui->checkBox->isChecked())<<QString::number(ui->checkBox_2->isChecked());
    return list;
}

void DialogExport::setControlData(const QStringList &data)
{
    qint32 i=-1;
    ui->lineEdit->setText(data.value(++i));
    ui->lineEdit_2->setText(data.value(++i));
    ui->lineEdit_3->setText(data.value(++i));
    ui->lineEdit_4->setText(data.value(++i));
    ui->comboBox->setCurrentIndex(data.value(++i).toInt());
    ui->comboBox_2->setCurrentIndex(data.value(++i).toInt());
    ui->checkBox->setChecked(data.value(++i).toInt());
    ui->checkBox_2->setChecked(data.value(++i).toInt());
}

bool DialogExport::writeToFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)){
        return false;
    }
    QDataStream ds(&file);
    ds << *this ;
    file.close();
    return true;
}

bool DialogExport::openFromFile(const QString &filePath)
{
    QFile f(filePath);
    if(!f.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QDataStream ds(&f);
    ds >> *this;
    f.close();
    return true;
}


