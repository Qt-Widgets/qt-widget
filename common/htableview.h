#ifndef HTABLEVIEW_H
#define HTABLEVIEW_H

#include <QObject>
#include <QTableView>

#include "hvariantlist.h"

class HTableViewModel;
class HTableViewDelegate;

class HTableView : public QTableView
{
    Q_OBJECT
public:
    explicit HTableView(QWidget *parent = nullptr);

    void setHeaderNames(const HVariantList &names);
    void setColCtrTypes(const QVector<qint32> &types);
    void setColCtrlEditables(const QString& editables);
    void setUniqueColIndex(qint32 index);
    void setAutoColWidth(bool enable=true);
    void setDefaultAddData(const HVariantList &data);
    void setUserIndex(qint32 index);
    void setHighlightRowsUseFirstColumn(const HVariantList&names);


    qint32 getRowCount() const;
    HVariantList getDataAtCol(qint32 col) const;
    HVariantList getDataAtRow(qint32 row) const;
    HVariantList getDataAtCurrentRow() const;
    HVariantList getAllDataUseRow() const;
    QVector<QStringList> getAllDataUseCol() const;
    void insertRowData(const HVariantList &data);
    void addRowData(const HVariantList &data);

    bool isUniqueDataExist(const QVariant&key);
    QVariant getCurrentUniqueKey() const;
    QVariant getDataAt(qint32 row,qint32 colum);
    qint32 getCurrentRow();
    void setCurrentRow(qint32 row,qint32 column=0);



signals:

    void currentRowChanged(qint32 current,qint32 userIndex);

public slots:

    void setDatasAtCol(qint32 col, const HVariantList &data);
    void setAllDatasUseRow(const HVariantList &data);
    void setAllDatasUseCol(const HVariantList &data,qint32 rowCount=-1);
    void setDatasAtCurrentRow(const HVariantList &data);

    QVariant deleteCurrentRow();
    HVariantList deleteSelectedRows();
    void clearAllRows();



private:

    HTableViewModel *ptrTableModel=nullptr;
    HTableViewDelegate *ptrTableDele=nullptr;

    QVector<qint32> selectedRows;

    qint32 userIndex=0;
};

#endif // HTABLEVIEW_H
