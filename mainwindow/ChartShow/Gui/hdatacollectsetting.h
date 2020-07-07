#ifndef HDATACOLLECTSETTING_H
#define HDATACOLLECTSETTING_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QPushButton;
class QSqlTableModel;
class QTableView;
QT_END_NAMESPACE


class HDataCollectSetting : public QDialog
{
    Q_OBJECT
public:

    static HDataCollectSetting* GetInstance(QWidget *parent = nullptr);


    QVariantList getNameList();
    QVariantList getCommondList();


signals:

    void SettingsChanged(const QVariantList &namelist,const QVariantList &commondlist);

public slots:

private slots:
    void submit();
    void add();
    void m_clear();
    void m_delete();

private:

    explicit HDataCollectSetting(QWidget *parent = nullptr,Qt::WindowFlags f =Qt::WindowCloseButtonHint);
    static HDataCollectSetting* m_pInstance;

    const QString tableName="msettings";

    bool createConnection();
    bool execSQL(const QString &str);
    QStringList getAllColumnNames(const QString &tabelName);
    QVariantList GetColumnValue(const QString &tabelName,const QString &colName);


    QTableView *view;

    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *clearButton;

    QPushButton *submitButton;
    QPushButton *revertButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QSqlTableModel *model;

};

#endif // HDATACOLLECTSETTING_H
