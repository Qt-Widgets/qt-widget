
#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>

//QT_BEGIN_NAMESPACE
//class QIcon;
//QT_END_NAMESPACE



class MyButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    MyButtonDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    //void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
//    QList <QIcon> m_IconList;
//    QList <QPixmap> m_PixmapList;

signals:
      void programChanged(int programIndex);
private slots:

    void commitAndCloseEditor();


};


#endif
