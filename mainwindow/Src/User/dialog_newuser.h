#ifndef DIALOG_NEWUSER_H
#define DIALOG_NEWUSER_H

#include <QDialog>
#include "dialogaccountui.h"

namespace Ui {
class Dialog_newuser;
}

class Dialog_newuser : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_newuser(QWidget *parent = nullptr);
    ~Dialog_newuser() Q_DECL_OVERRIDE;

protected slots:

void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void on_pushButton_ok_pressed();

    void on_pushButton_cancle_pressed();

private:
    Ui::Dialog_newuser *ui;
    bool flag;
};

#endif // DIALOG_NEWUSER_H
