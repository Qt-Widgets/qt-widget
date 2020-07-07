#ifndef H_TREEVIEW_H
#define H_TREEVIEW_H

#include <QTreeView>

class H_TreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit H_TreeView(QWidget *parent = nullptr);
    ~H_TreeView() override;



signals:

public slots:

};

#endif // H_TREEVIEW_H
