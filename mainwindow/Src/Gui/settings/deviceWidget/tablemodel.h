
#ifndef TableModel_H
#define TableModel_H

//#include <QAbstractTableModel>
//#include <QModelIndex>
//#include <QVariant>
#include <stable.h>

enum ColumnType {
    index=0,
    inputName,
    input,
    outputName,
    output,
    outState
};

class HIODeviceController;

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel(QObject *parent = nullptr);
    ~TableModel() Q_DECL_OVERRIDE;

public slots:


    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;



signals:
    void programChanged(int programIndex);


private:
   HIODeviceController *ptrIODevice;


    QVector<QVariant> headData;

    QList<QIcon> m_publicIconMap;

};

#endif // TableModel_H
