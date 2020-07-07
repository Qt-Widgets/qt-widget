#include "hdatacollectsetting.h"

#include <QtSql>
#include <QPushButton>


#include <QtWidgets>
#include <QMessageBox>


#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

static QSqlDatabase db;
static QSqlQuery query;


HDataCollectSetting* HDataCollectSetting::m_pInstance = nullptr;

HDataCollectSetting *HDataCollectSetting::GetInstance(QWidget *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HDataCollectSetting(parent);
    }
    return m_pInstance;
}

QVariantList HDataCollectSetting::getNameList()
{
    return GetColumnValue(tableName,"name");
}

QVariantList HDataCollectSetting::getCommondList()
{
    return GetColumnValue(tableName,"instruction");
}

void HDataCollectSetting::submit()
{
    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();
    } else {
        model->database().rollback();
        QMessageBox::warning(this, tr("Cached Table"),
                             tr("The database reported an error: %1")
                                 .arg(model->lastError().text()));
        model->revertAll();

        if (model->submitAll()) {
            model->database().commit();
        }
    }
    emit SettingsChanged(GetColumnValue(tableName,"name"),GetColumnValue(tableName,"instruction"));
}

void HDataCollectSetting::add()
{
    //     query.clear();
    //     query.exec("insert into msettings values('Danny', 'Young')");
    //     submit();
    model->insertRow(model->rowCount());
}

void HDataCollectSetting::m_clear()
{
    //    query.clear();
    //    query.exec("RUNCATE TABLE msettings");

    model->removeRows(0,model->rowCount());
    submit();
}

void HDataCollectSetting::m_delete()
{
    model->removeRow(view->selectionModel()->selectedRows().first().row());
    submit();
}

HDataCollectSetting::HDataCollectSetting(QWidget *parent, Qt::WindowFlags f) : QDialog(parent,f)
{
    createConnection();

    model = new QSqlTableModel(this,db);
    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    QStringList columnNames=getAllColumnNames(tableName);
    for(qint32 k=0;k<columnNames.size();++k){
        model->setHeaderData(k, Qt::Horizontal, columnNames.at(k));
    }


    view = new QTableView;
    view->setModel(model);
    view->resizeColumnsToContents();
    view->verticalHeader()->hide();
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->setEditTriggers(QAbstractItemView::DoubleClicked/*| QAbstractItemView::SelectedClicked*/);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setAlternatingRowColors(true);
    view->setSelectionMode(QAbstractItemView::SingleSelection);

    addButton = new QPushButton(tr("Add"));
    deleteButton = new QPushButton(tr("&Delete"));
    clearButton = new QPushButton(tr("&Clear"));


    submitButton = new QPushButton(tr("Submit"));
    submitButton->setDefault(true);
    revertButton = new QPushButton(tr("&Revert"));
    revertButton->setEnabled(false);
    quitButton = new QPushButton(tr("Quit"));

    buttonBox = new QDialogButtonBox(Qt::Vertical);
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(clearButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(submitButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(revertButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(addButton, &QPushButton::clicked, this, &HDataCollectSetting::add);
    connect(deleteButton, &QPushButton::clicked, this, &HDataCollectSetting::m_delete);
    connect(clearButton, &QPushButton::clicked,  this, &HDataCollectSetting::m_clear);
    connect(submitButton, &QPushButton::clicked, this, &HDataCollectSetting::submit);
    connect(revertButton, &QPushButton::clicked,  model, &QSqlTableModel::revertAll);
    connect(quitButton, &QPushButton::clicked, this, &HDataCollectSetting::close);



    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(view);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    //setWindowTitle(tr("Cached Table"));

}

bool HDataCollectSetting::execSQL(const QString &str)
{
    if(!query.exec(str)){
        QMessageBox::warning(nullptr,"",tr("SQL database error:\n")+str+tr("\nexecute failed!\nreason: ")+query.lastError().text());
        return false;
    }
    else{
        query.next();
        return true;
    }
}

QStringList HDataCollectSetting::getAllColumnNames(const QString &tabelName)
{
    QStringList list;
    if(!query.exec(QString("PRAGMA table_info('%1')").arg(tabelName))){
        QMessageBox::warning(nullptr,"",tr("SQL database error:\n")+tr("Select Name execute failed!\nreason: ")+query.lastError().text());
        return list;
    }
    else{
        while (query.next()) {
            list<< query.value(1).toString();
        }
        return list;
    }
}

QVariantList HDataCollectSetting::GetColumnValue(const QString &tabelName, const QString &colName)
{
    query.clear();
    query.exec(QString("SELECT %1 FROM %2 ").arg(colName).arg(tabelName));
    QVariantList list;
    while (query.next()) {
        list<< query.value(0);
    }
    return list;
}

bool HDataCollectSetting::createConnection()
{
    //    db = QSqlDatabase::addDatabase("QSQLITE");
    //    db.setDatabaseName(":memory:");
    //    if (!db.open()) {
    //        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
    //                              QObject::tr("Unable to establish a database connection.\n"
    //                                          "This example needs SQLite support. Please read "
    //                                          "the Qt SQL driver documentation for information how "
    //                                          "to build it.\n\n"
    //                                          "Click Cancel to exit."), QMessageBox::Cancel);
    //        return false;
    //    }
    //    query=QSqlQuery(db);
    //    query.exec("create table msettings (name varchar(20), instruction varchar(20))");

    db = QSqlDatabase::addDatabase("QSQLITE","collection_data");
    db.setDatabaseName(QCoreApplication::applicationDirPath()+"/dataCollection.db");

    if (!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database!!"),
                              QObject::tr("Unable to establish a database connection.\n"
                                          "This Application needs SQLite support."
                                          "Click OK to exit."), QMessageBox::Ok);
        exit(-1);
    }

    query=QSqlQuery(db);

    if(db.tables().contains(tableName)){
        if(getAllColumnNames(tableName).size()!=2){
            QMessageBox::warning(this,"",tr("data error!! All settings has been refreshed!!"));
            execSQL("drop table msettings");
            execSQL("create table msettings (name varchar(20), instruction varchar(20))");
            return false;
        }

    }else{
        QMessageBox::warning(this,"",tr("data missed!! All settings has been newed!!"));
        execSQL("create table msettings (name varchar(20), instruction varchar(20))");
         return false;
    }

    return true;
}
