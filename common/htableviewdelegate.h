#ifndef HDELEGATECOMBBOX_H
#define HDELEGATECOMBBOX_H


#include <QStyledItemDelegate>

//QT_BEGIN_NAMESPACE
//class QIcon;
//QT_END_NAMESPACE



typedef  QVector<QVector<qint32>>  HDelegateTypes;
typedef  QVector<qint32>  HAllColDelegateTypes;


class HTableViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    HTableViewDelegate(QObject *parent = nullptr,const QVector<bool>& editables=QVector<bool>(),const HDelegateTypes&types=HDelegateTypes());

    enum TVDelegateType{
        TV_LineEdit,
        TV_Combbox,
        TV_FileDialog,
    };


    void SetAllColDelegateTypes(const HAllColDelegateTypes&types);
    void SetAllDelegateTypes(const HDelegateTypes&types);

    void setAllEditables(const QVector<bool>& editables){m_editables=editables;}

    void setHighlightRowsUseFirstColumn(const QVariantList &names);


    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const override;
    //    QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const override;




private:
    //    QList <QIcon> m_IconList;
    //    QList <QPixmap> m_PixmapList;


    QVector<bool> m_editables;

    HDelegateTypes m_types;
    QVariantList highlightRows;

    HAllColDelegateTypes m_Coltypes;
private slots:

    //void commitAndCloseEditor();


};

#endif // HDELEGATECOMBBOX_H
