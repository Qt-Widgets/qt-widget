
#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QIcon>


class TreeItem;
class ProgramData;
class ProgramHandler;
class H_TreeView;


class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TreeModel(H_TreeView*view, QObject *parent = nullptr);
    ~TreeModel() Q_DECL_OVERRIDE;

public slots:


    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
   // QMimeData *mimeData(const QModelIndexList &indexes) const override;
   // bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
 //   QStringList mimeTypes() const override;
    bool setData(const QModelIndex &index, const QVariant &value,int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int position, int rows,const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,const QModelIndex &parent = QModelIndex()) override;

   // bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;


    void newProgram(QString Programname,ProgramData *pprogram);
    void addProgram(QString Programname,ProgramData*pprogram);
    void closeProgram(int index);
    void refreshData(int index);

    void DeleteLayer(qint32 index);
    void DeleteItem(qint32 index);


signals:
    void programChanged(int programIndex);


private:

    void addProgramData(TreeItem *inory,ProgramData*pprogram);

    TreeItem *getItem(const QModelIndex &index) const;

     QMap<QString,QIcon> ItemTypeIcons;

     QMap<qint32,QString> ItemTypeNames;

    TreeItem *rootItem;

    ProgramHandler *pDataHandler;

    H_TreeView *ptrView;
};

#endif // TREEMODEL_H
