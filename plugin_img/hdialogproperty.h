#ifndef HDIALOGPROPERTY_H
#define HDIALOGPROPERTY_H

#include <QDialog>

namespace Ui {
class HDialogProperty;
}

class HDialogProperty : public QDialog
{
    Q_OBJECT

public:
    explicit HDialogProperty(const QString&title,QWidget *parent = nullptr);
    ~HDialogProperty();

    void setLabels(const QStringList&names);

    void setLabels(const QString&names);

    void setContents(const QString&types,QVariantList datas=QVariantList());

    void setUniqueKeys(qint32 key,const QVariantList &list){m_uniqueKey=key;m_UniqueDatas=list;}
     void setUniqueKeys(qint32 key,const QStringList &list){
         m_uniqueKey=key;
         m_UniqueDatas.clear();
         for(const auto&k:list){
           m_UniqueDatas<<k;
         }
     }

    void setFileDialogFilter(const QString& filter){m_fileDialogFilter=filter;}

    QVariantList getContentData() const;


private slots:
    void on_toolButton_ok_clicked();

    void on_toolButton_cancel_clicked();

private:
    Ui::HDialogProperty *ui;

    struct ContentCtrl
    {
        qint32 type;
        QWidget *ptrWid;
    };
    QVector<ContentCtrl> contentTypes;

    qint32 m_uniqueKey=-1;
    QVariantList m_UniqueDatas;
    QStringList labelNames;

    QString m_fileDialogFilter="file (*)";
};

#endif // HDIALOGPROPERTY_H
