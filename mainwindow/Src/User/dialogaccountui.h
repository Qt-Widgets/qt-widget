#ifndef DIALOGACCOUNTUI_H
#define DIALOGACCOUNTUI_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class DialogAccountUI;
}

QT_BEGIN_NAMESPACE
class QSqlTableModel;
QT_END_NAMESPACE

class DialogAccountUI : public QDialog
{
    Q_OBJECT
public:
    explicit DialogAccountUI(bool isFirst=false,int deauthority=-1,QWidget *parent = nullptr);
     ~DialogAccountUI() Q_DECL_OVERRIDE;


protected slots:

    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:

    void checkSupperAccount(int);

    void on_button_login_clicked();

    void on_pushButton_new_pressed();

    void on_pushButton_delete_pressed();

private:




    Ui::DialogAccountUI *ui;

    bool isFirstLogin;
    int authority;

    QSqlDatabase db;

    //    typedef struct
    //    {
    //        QString password;
    //        int type;

    //    } Mydata;



    //    QHash<QString, Mydata> hash;

    void connectDatabase();

private:

    QSqlTableModel* model;
    void showDatabase();



};



#endif // DIALOGACCOUNTUI_H
