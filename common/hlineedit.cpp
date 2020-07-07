#include "hlineedit.h"

HLineEdit::HLineEdit(QWidget *parent) : QLineEdit(parent)
{

}

double HLineEdit::getNumberF() const
{
    return text().toDouble();
}

qint32 HLineEdit::getNumberI() const
{
    return text().toInt();
}
