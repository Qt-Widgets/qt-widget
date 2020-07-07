#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QWidget>
#include "editdata.h"

QT_BEGIN_NAMESPACE
class QToolButton;
QT_END_NAMESPACE


class EditorWidget : public QWidget
{
    Q_OBJECT
public:
    EditorWidget(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    void setEditData(const QModelIndex &index,   const EditData &data);
    void editData();

    void editCompleted();

signals:
    void editingFinished();

protected:
    void paintEvent(QPaintEvent *event) override;

private:

    QList <QIcon> m_IconList;

    EditData myEditData;
    QModelIndex myIndex;

    QToolButton*button1;
    QToolButton*button2;
    QToolButton*button3;

    QColor colorSet;
    bool checkedSet;
    bool lockedSet;

    QColor originalColor;
    bool originalChecked;
    bool originallocked;
};

#endif // EDITORWIDGET_H
