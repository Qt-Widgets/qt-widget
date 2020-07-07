#ifndef HPROPERTY_PARA_H
#define HPROPERTY_PARA_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "hcommon.h"

class QTableView;
class HTableViewModel;
class HTableViewDelegate;


class HProperty_para : public QWidget
{
    Q_OBJECT
public:
    explicit HProperty_para(const QStringList&typesCol, qint32 userType=0, QWidget *parent = nullptr,const QString &types=nullptr, const QString & editables=nullptr);
     ~HProperty_para();

    enum ColType{
        LineEdit,
        Combbox,
        FileDialog,
    };


    template<class Obj, typename Func1>
    inline typename std::enable_if<!std::is_same<const char*, Func1>::value && QtPrivate::IsPointerToTypeDerivedFromQObject<Obj*>::Value, QPushButton *>::type
    AddControlButton(const QString &str,const Obj *object, Func1 slot,bool checkable=false)
    {
        QPushButton *button=new QPushButton(str,this);
        button->setCheckable(checkable);
        button->setFocusPolicy(Qt::NoFocus);
        buttonLayout->addWidget(button);
        connect(button, &QPushButton::pressed, object, std::move(slot));
        return button;
    }
    void addCtrlSpace(qint32 value);
    QPushButton * addButton(const QString &str,const QIcon&icon=QIcon());


    void setColCtrTypes(const QVector<qint32> &types);
    void setUniqueColIndex(qint32 index);
    void setAutoColWidth(bool enable=true);
    void setHeaderColWidthFixed(bool enable=true);
    void setDefaultAddData(const HVariantList &data);

    void resizeDatas(qint32 rowCount);


    qint32 getRowCount() const;
    HVariantList getDataAtCol(qint32 col) const;
    HVariantList getAllDataUseRow() const;
    void insertRowData(const HVariantList &data);


    bool isUniqueDataExist(const QVariant&key);
    QVariant getCurrentUniqueKey() const;
    QVariant getDataAt(qint32 row,qint32 colum);
    qint32 getCurrentRow();




signals:

    void buttonHasPressed(const QString &name,qint32 type);
    void dataChanged(qint32 row,qint32 column,const QVariant&data);
    void dataChangedUseKeyName(const QVariant& key,qint32 col,const QVariant&data);
    void dataSaved(qint32 type,const QVariantList &data) const; //no contain first column
    void rowSelected(const QVector<qint32>& indexs);
    void currentRowChanged(qint32 current);

public slots:

    void addEmptyRow();
    void addRowData(const HVariantList &data);
    void addRowPointData(const QPointF&pt);
    void setDatasAtCol(qint32 col, const HVariantList &data);
    void setAllDatasUseRow(const HVariantList &data);
    void setAllDatasUseCol(const HVariantList &data,qint32 rowCount=-1);
    void setDatasAtCurrentRow(const HVariantList &data);



    QVariant deleteCurrentRow();
    HVariantList deleteSelectedRows();
    void clearAllRows();


private:


    QTableView *ptrTableView=nullptr;
    HTableViewModel *ptrTableModel=nullptr;
    HTableViewDelegate *ptrTableDele=nullptr;


    QVector<qint32> selectedRows;

    qint32 m_userType;

    QVBoxLayout *buttonLayout=nullptr;
};


#endif // HPROPERTY_PARA_H
