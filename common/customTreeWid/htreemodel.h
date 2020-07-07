#ifndef HTREEMODEL_H
#define HTREEMODEL_H

#include <QAbstractItemModel>

//class HTreeItemData;

//class HTreeModel : public QAbstractItemModel
//{
//    Q_OBJECT
//public:
//    explicit HTreeModel(const QStringList &colNames,QObject *parent = nullptr);
//    ~HTreeModel() Q_DECL_OVERRIDE;


//signals:

//protected:

//    QModelIndex buddy(const QModelIndex &index) const override;
//    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
//    bool canFetchMore(const QModelIndex &parent) const override;
//    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
//    QVariant data(const QModelIndex &index, int role) const override;
//    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
//    void fetchMore(const QModelIndex &parent) override;
//    Qt::ItemFlags flags(const QModelIndex &index) const override;
//    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
//    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const override;
//    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const override;
//    bool insertColumns(int position, int columns,const QModelIndex &parent = QModelIndex()) override;
//    bool insertRows(int position, int rows,const QModelIndex &parent = QModelIndex()) override;
//    QMap<int, QVariant> itemData(const QModelIndex &index) const override;
//    QModelIndexList match(const QModelIndex &start, int role, const QVariant &value, int hits = 1, Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap)) const override;
//    QMimeData *mimeData(const QModelIndexList &indexes) const override;
//    QStringList mimeTypes() const override;
//    bool moveColumns(const QModelIndex &sourceParent, int sourceColumn, int count, const QModelIndex &destinationParent, int destinationChild) override;
//    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;
//    QModelIndex parent(const QModelIndex &index) const override;
//    bool removeColumns(int position, int columns,const QModelIndex &parent = QModelIndex()) override;
//    bool removeRows(int position, int rows,const QModelIndex &parent = QModelIndex()) override;
//    QHash<int, QByteArray> roleNames() const override;
//    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
//    bool setData(const QModelIndex &index, const QVariant &value,int role = Qt::EditRole) override;
//    bool setHeaderData(int section, Qt::Orientation orientation,const QVariant &value, int role = Qt::EditRole) override;
//    bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) override;
//    QModelIndex sibling(int row, int column, const QModelIndex &index) const override;
//    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
//    QSize span(const QModelIndex &index) const override;
//    Qt::DropActions supportedDragActions() const override;
//    Qt::DropActions supportedDropActions() const override;



//private:

//    HTreeItemData *getItem(const QModelIndex &index) const;
//    HTreeItemData *m_rootItem;


//};

#endif // HTREEMODEL_H
