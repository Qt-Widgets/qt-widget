#ifndef CUSTOMPOSITIONDIALOG_H
#define CUSTOMPOSITIONDIALOG_H

#include <QDialog>

namespace Ui {
class CustomPositionDialog;
}

class CustomPositionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomPositionDialog(QWidget *parent = nullptr);
    ~CustomPositionDialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::CustomPositionDialog *ui;
};

#endif // CUSTOMPOSITIONDIALOG_H
