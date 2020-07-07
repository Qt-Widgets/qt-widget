#ifndef H2LEVELTREEWID_H
#define H2LEVELTREEWID_H

#include <QTreeWidget>

struct HTextIcon
{

    qint32 type;
    QString text;
    QString text1;
    QIcon icon;
    QIcon icon1;
    QVariant userData;
    bool checkable=true;
    qint32 checkedState=false;
    bool editable=false;
    bool enable=true;
    bool selectable=true;

    qint32 mm_Flag=0;

    bool operator < (const HTextIcon &o) const
    {
        return type < o.type;
    }
};





typedef  QVector<HTextIcon> HTextIcons;

typedef  QVector <QPair<int,QString>> HTreeItem;

class H2LevelTreeWid : public QTreeWidget
{
    Q_OBJECT
public:
    explicit H2LevelTreeWid(QWidget *parent = nullptr);

    ~H2LevelTreeWid() Q_DECL_OVERRIDE;

    void AddAllItems(const HTextIcons&data);
    HTextIcons GetAllItems();


    void AddRootItem(const HTextIcon&data);
    void AddChildItem(const HTextIcon&data);
    void AddChildItems(const HTextIcons&data);

    void EndAddChildItem();


    void InsertItemAtSelection(const HTextIcon&data);
    void InsertItemsAtSelection(const HTextIcons&data);

    void InsertChildItemAtSelection(const HTextIcon&data);
    void InsertChildItemsAtSelection(const HTextIcons&data);

    void RemoveSelectedItems();
    void RemovechildItems(QTreeWidgetItem * parent,qint32 start,qint32 count);



    void SetColNames(const QStringList &names);


    HTextIcons GetSelectedItems();

    HTreeItem GetAllCheckedData();

    HTreeItem GetAll2ChildCheckedData();

    qint32 GetSelectedItemIndex();



signals:

    QVariant HTreeItemDoubleClicked(qint32 type,const QVariant&data);

public slots:


private slots:

    void treeItemChanged(QTreeWidgetItem *item, int col);
    void treeItemDoubleClick(QTreeWidgetItem *item, int column);

private:



    QVector<QTreeWidgetItem*> m_cache;
    QTreeWidgetItem* m_parItem;


    void updateParentItem(QTreeWidgetItem* item);

    qint32 colCount=1;

    QTreeWidgetItem* creatListTreeItem(const HTextIcon &data, void *parent, bool isRoot);
};

#endif // H2LEVELTREEWID_H
