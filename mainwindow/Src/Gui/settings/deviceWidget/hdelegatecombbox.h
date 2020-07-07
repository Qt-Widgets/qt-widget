#ifndef HDELEGATECOMBBOX_H
#define HDELEGATECOMBBOX_H


#include <QStyledItemDelegate>

//QT_BEGIN_NAMESPACE
//class QIcon;
//QT_END_NAMESPACE

class HDelegateCombbox : public QStyledItemDelegate
{
    Q_OBJECT

public:
    HDelegateCombbox(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const override;
//    QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const override;
   void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
//    QList <QIcon> m_IconList;
//    QList <QPixmap> m_PixmapList;
private slots:

    //void commitAndCloseEditor();


};

#endif // HDELEGATECOMBBOX_H
