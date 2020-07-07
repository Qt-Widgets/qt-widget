#ifndef DIALOG_SUPERCHECK_H
#define DIALOG_SUPERCHECK_H

#include <QDialog>

namespace Ui {
class Dialog_SuperCheck;
}

class Dialog_SuperCheck : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_SuperCheck(QWidget *parent = 0);
    ~Dialog_SuperCheck();

protected slots:

void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;


private slots:
void on_pushButton_pressed();

void on_pushButton_2_pressed();

private:
    Ui::Dialog_SuperCheck *ui;

    bool flag;
};

#endif // DIALOG_SUPERCHECK_H
