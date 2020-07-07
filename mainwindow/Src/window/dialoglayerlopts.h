#ifndef DIALOGLAYERLOPTS_H
#define DIALOGLAYERLOPTS_H

#include <QDialog>

namespace Ui {
class DialogLayerLoPts;
}

class DialogLayerLoPts : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLayerLoPts(QWidget *parent = nullptr);
    ~DialogLayerLoPts() override;

    void setLayerIndex(qint32 index);

protected:
     void showEvent(QShowEvent *event) override;

private slots:
     void on_pushButton_clicked();

     void on_pushButton_2_clicked();

     void on_pushButton_3_clicked();

     void on_pushButton_4_clicked();

     void on_pushButton_save_clicked();

private:
    Ui::DialogLayerLoPts *ui;
    qint32 lIndex=-1;
};

#endif // DIALOGLAYERLOPTS_H
