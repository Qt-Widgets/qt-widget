#ifndef HDIALOGPARAMETERLIBRARY_H
#define HDIALOGPARAMETERLIBRARY_H

#include <QDialog>

namespace Ui {
class HDialogParameterLibrary;
}

class QLineEdit;
class QListWidget;
class QListWidgetItem;
class HParaLibController;

class HDialogParameterLibrary : public QDialog
{
    Q_OBJECT

public:
    explicit HDialogParameterLibrary(QWidget *parent = nullptr);
    ~HDialogParameterLibrary() override;



private slots:

    void on_toolButton_new_clicked();

    void on_toolButton_delete_clicked();

    void on_toolButton_clear_clicked();

    void currentItemHasChanged(QListWidgetItem *current);

    void on_toolButton_save_clicked();

    void on_toolButton_set_default_clicked();

protected:
    void showEvent(QShowEvent *event) override;

private:

   inline QVector<QLineEdit *> & getLineEdits();

    void refreshAllControl(qint32 mode);

    void setControlEnable(bool enable);

    void newItem(QListWidget *ptrWid, const QString& defaultName);
    void getSaveName(const QStringList &names,const QString &oname, QString &name, qint32 index=0);
    bool checkSameName(const QString & text,QListWidget *wid);

    const QIcon defaultIcon=QIcon(":/resource/img/whiteBlank.png");


    Ui::HDialogParameterLibrary *ui;

    HParaLibController * ptrParaController;

    qint32 data_mode=0;

    QVector<QLineEdit *> lineEdits_cutter;
    QVector<QLineEdit *> lineEdits_scanner;
};

#endif // HDIALOGPARAMETERLIBRARY_H
