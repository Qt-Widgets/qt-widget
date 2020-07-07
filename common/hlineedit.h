#ifndef HLINEEDIT_H
#define HLINEEDIT_H

#include <QLineEdit>

class HLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit HLineEdit(QWidget *parent = nullptr);

    double getNumberF() const;
    qint32 getNumberI() const;

signals:

public slots:
};

#endif // HLINEEDIT_H
