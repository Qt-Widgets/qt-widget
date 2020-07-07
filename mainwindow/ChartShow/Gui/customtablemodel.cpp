

#include "customtablemodel.h"
#include <QtCore/QVector>
#include <QtCore/QRandomGenerator>
#include <QtCore/QRect>
#include <QtGui/QColor>

CustomTableModel::CustomTableModel(QVariantList headNames, QObject *parent) :QAbstractTableModel(parent)
{
    m_headNames=headNames;
    m_headNames.prepend(tr("Time(s)"));
    m_columnCount = headNames.size();


    //    m_rowCount = 100;

    //    // m_data
    //    for (int i = 0; i < m_rowCount; i++) {
    //        QVector<qreal>* dataVec = new QVector<qreal>(m_columnCount);
    //        for (int k = 0; k < dataVec->size(); k++) {
    //            if (k % 2 == 0)
    //                dataVec->replace(k, i * 50 + QRandomGenerator::global()->bounded(20));
    //            else
    //                dataVec->replace(k, QRandomGenerator::global()->bounded(100));
    //        }
    //        m_data.append(dataVec);
    //    }
}

int CustomTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.count();
}

int CustomTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columnCount;
}

QVariant CustomTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {

        return m_headNames.at(section);

    } else {
        return QString("%1").arg(section + 1);
    }
}

QVariant CustomTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return m_data[index.row()]->at(index.column());
    } else if (role == Qt::EditRole) {
        return m_data[index.row()]->at(index.column());
    } else if (role == Qt::BackgroundRole) {
        for (const QRect &rect : m_mapping) {
            if (rect.contains(index.column(), index.row()))
                return QColor(m_mapping.key(rect));
        }
        // cell not mapped return white color
        return QColor(Qt::white);
    }
    return QVariant();
}

bool CustomTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        m_data[index.row()]->replace(index.column(), value.toDouble());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags CustomTableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void CustomTableModel::addMapping(QString color, QRect area)
{
    m_mapping.insertMulti(color, area);
}

void CustomTableModel::appendData(const QList<QVariant> &data)
{
    beginResetModel();
    QVector<QVariant>* dataVec = new QVector<QVariant>(m_columnCount);
    for (int k = 0; k < dataVec->size(); k++) {

        dataVec->replace(k, data.at(k));

    }
    m_data.append(dataVec);
    endResetModel();

    for(qint32 k=0;k<columnCount();++k){
        addMapping(m_colors[k], QRect(k, rowCount()-1, 1, 1));
    }
}

void CustomTableModel::clearAllData()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

void CustomTableModel::setHeadNames(const QVariantList &names)
{
    beginResetModel();
    m_headNames=names;
    m_headNames.prepend(tr("Time(s)"));
    m_columnCount=m_headNames.size();
    endResetModel();
}


