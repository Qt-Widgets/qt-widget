#ifndef HTREEVIEW_H
#define HTREEVIEW_H

#include <QTreeView>

class HTreeView :  public QTreeView
{
    Q_OBJECT
public:
    explicit HTreeView(QWidget *parent = nullptr);
    ~HTreeView() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HTREEVIEW_H
