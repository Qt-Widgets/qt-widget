

#ifndef CUSTOMTABLEMODEL_H
#define CUSTOMTABLEMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QHash>
#include <QtCore/QRect>

typedef  QList<QVector<QVariant> * > MData;

class CustomTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CustomTableModel( QVariantList headNames=QVariantList(),QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void addMapping(QString color, QRect area);
    void clearMapping() { m_mapping.clear(); }

    void setColors(const QStringList &colors){m_colors=colors;}
    const MData & GetMData() const{return m_data;}

    void appendData(const QList<QVariant> &data);

    void clearAllData();
    void setHeadNames(const QVariantList &names);

private:
    MData m_data;

    QStringList m_colors;
    QVariantList m_headNames;
    QHash<QString, QRect> m_mapping;
    int m_columnCount;
    //int m_rowCount;
};

#endif // CUSTOMTABLEMODEL_H
