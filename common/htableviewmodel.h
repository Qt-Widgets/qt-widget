#ifndef HTABLEVIEWMODEL_H
#define HTABLEVIEWMODEL_H
#include "hvariantlist.h"


class HTableViewModel  : public QAbstractTableModel
{
    Q_OBJECT
public:
    HTableViewModel(QObject *parent = nullptr,const QStringList & headNames=QStringList());
    ~HTableViewModel() Q_DECL_OVERRIDE;


    enum ColType{
        LineEdit,
        Combbox,
        FileDialog,
    };

    void setHeaderNames(const HVariantList & headNames);

    void setAllColModelTypes(const QVector<qint32>&types){m_colTypes=types;}

    void setUniqueColumn(qint32 index){uniqueCol=index;}
    bool isUniqueKeyExist(const QVariant&key) const;
    QVariant getUniqueKey(qint32 row) const;

    void clearAllRows();
    QVariant deleteRow(qint32 row);
    void setDefaultRowDatas(const HVariantList&data){
        qint32 km=0;
        for(auto&k:m_defaultAddData){
          k=data.value(km);
          ++km;
        }
    }

    void resizeDatas(qint32 rowCount);

    void setData(qint32 row,qint32 column,const QVariant &data);
    void setDatasAtRow(qint32 row,const HVariantList &datas);
    void setDatasAtCol(qint32 column,const HVariantList &datas);
    void setAllDatasUseRow(const HVariantList &datas);
    void setAllDatasUseCol(const HVariantList &data, qint32 rowCount=-1);


    QVariant getData(qint32 row,qint32 column) const;
    HVariantList getDatasAtCol(qint32 col) const;
    HVariantList getDatasAtRow(qint32 row) const;
    HVariantList getAllDatasUseRow() const;
    QVector<QStringList> getAllDatasUseCol() const;

    void insertRowDatas(qint32 row,const HVariantList&data);

signals:

    void itemDataChanged(qint32 row,qint32 column,const QVariant&data);


public slots:
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    bool removeRows(int position, int rows,const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

private:
    QStringList headData;

    QVector<QVariant>  m_defaultAddData;
    QVector<QVector<QVariant>> m_data;

    QVector<qint32> m_colTypes;

    qint32 uniqueCol=-2;


};

#endif // HTABLEVIEWMODEL_H
