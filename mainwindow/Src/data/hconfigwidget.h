#ifndef HCONFIGWIDGET_H
#define HCONFIGWIDGET_H

#include "stable.h"

#include <QDialog>

class QVBoxLayout;
class HProperty_para;
class HSettings;

class HConfigWidget : public QDialog
{
    Q_OBJECT
public:
    static HConfigWidget* GetInstance( QWidget *parent = nullptr);
    static void Release();


private:
    explicit HConfigWidget(QWidget *parent = nullptr, Qt::WindowFlags f =Qt::WindowCloseButtonHint|Qt::WindowMinMaxButtonsHint);
    ~HConfigWidget() Q_DECL_OVERRIDE;
    static HConfigWidget* m_pInstance;

signals:

    void DataSaveAndChanged();



private slots:

    void openCustomDialog(const QString &name,qint32 index);


private:


    const QStringList customDialogctrlTypes={
        "1,0,0,0,0,0,1,0,2",
        "1,0,0,0,0,1,2",
        "1,0,0,1,2",
        "1,0,0,1,2",
        "1,0,0,0,0,0,2",
        "1,0"
    };


    const QStringList defaultValues={
        ",,,,,no reverse;reverse,,",
        ",,,,no reverse;reverse X;reverse Y;reverse XY,",
        ",,no reverse;reverse,",
        ",,no reverse;reverse,",
        ",,,,,",
        ""
    };



    void addButtonContent(QVBoxLayout *vlayout1, const QStringList &headNames, const QString &colTypes, const QString &editables,
                          const QString &text, qint32 index,  const QStringList &buttons);



    const qint32 uniqueKey=2;

    struct AddDialogParas{
        QString title;
        QStringList labels;
        QString ctrltypes;
        QVariantList datas;
        QVariantList uniqueKeys;
    };


    bool openAddDialog(AddDialogParas &paras);


    QVector<HProperty_para*> m_contentWids;
    QVector<QDialog*> m_addWids;


    HSettings *ptrSettings;

    QIcon iconUnfold;
    QIcon iconfold;

};

#endif // HCONFIGWIDGET_H
