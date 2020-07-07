#ifndef HLISTWIDGET_H
#define HLISTWIDGET_H

#include <QWidget>
#include <QListWidget>

class HListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit HListWidget(QWidget *parent = nullptr);


    void setDefaultName(const QString &name){m_defaultItemName=name;}


signals:

    void currentItemChanged(const QString& name);

public slots:

    void deleteSelectedItems();
    void newItem();
    void clearAllItem();

private:

    QString m_defaultItemName;
    QString getSaveName();
    inline QStringList getListWidItemNames();
};

#endif // HLISTWIDGET_H
