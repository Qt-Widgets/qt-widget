#include "hsqlcommon.h"

#include <QMessageBox>
#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QDebug>

#include <QSqlRecord>


#include "Src/Motion/h_motioncontroller.h"


//static bool createConnection()
//{
//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","connection_data");
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

//    QSqlQuery query;
//    query.exec("create table person (id int primary key, "
//               "firstname varchar(20), lastname varchar(20))");
//    query.exec("insert into person values(101, 'Danny', 'Young')");
//    query.exec("insert into person values(102, 'Christine', 'Holand')");
//    query.exec("insert into person values(103, 'Lars', 'Gordon')");
//    query.exec("insert into person values(104, 'Roberto', 'Robitaille')");
//    query.exec("insert into person values(105, 'Maria', 'Papadopoulos')");

//    query.exec("create table items (id int primary key,"
//               "imagefile int,"
//               "itemtype varchar(20),"
//               "description varchar(100))");
//    query.exec("insert into items "
//               "values(0, 0, 'Qt',"
//               "'Qt is a full development framework with tools designed to "
//               "streamline the creation of stunning applications and  "
//               "amazing user interfaces for desktop, embedded and mobile "
//               "platforms.')");
//    query.exec("insert into items "
//               "values(1, 1, 'Qt Quick',"
//               "'Qt Quick is a collection of techniques designed to help "
//               "developers create intuitive, modern-looking, and fluid "
//               "user interfaces using a CSS & JavaScript like language.')");
//    query.exec("insert into items "
//               "values(2, 2, 'Qt Creator',"
//               "'Qt Creator is a powerful cross-platform integrated "
//               "development environment (IDE), including UI design tools "
//               "and on-device debugging.')");
//    query.exec("insert into items "
//               "values(3, 3, 'Qt Project',"
//               "'The Qt Project governs the open source development of Qt, "
//               "allowing anyone wanting to contribute to join the effort "
//               "through a meritocratic structure of approvers and "
//               "maintainers.')");

//    query.exec("create table images (itemid int, file varchar(20))");
//    query.exec("insert into images values(0, 'images/qt-logo.png')");
//    query.exec("insert into images values(1, 'images/qt-quick.png')");
//    query.exec("insert into images values(2, 'images/qt-creator.png')");
//    query.exec("insert into images values(3, 'images/qt-project.png')");

//    return true;
//}


HSqlCommon* HSqlCommon::m_pInstance = nullptr;
HSqlCommon *HSqlCommon::GetInstance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HSqlCommon();
        atexit(Release);
    }
    return m_pInstance;
}

void HSqlCommon::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

bool HSqlCommon::execSQL(const QString &str, QSqlQuery &sqlDb)
{
    sqlDb.clear();
    if(!sqlDb.exec(str)){
        QMessageBox::warning(nullptr,"","SQL database error:\n"+str+"\nexecute failed!\nreason: "+sqlDb.lastError().text());
        return false;
    }
    else{
        return true;
    }
}



HSqlCommon::HSqlCommon()
{

}

HSqlCommon::~HSqlCommon()
{
    db.close();
}

bool HSqlCommon::connectDataBase(const QString &dataPath, const QString &connectionName){


    db = QSqlDatabase::addDatabase("QSQLITE",connectionName);
    db.setDatabaseName(dataPath);

    if (!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database!!"),
                              QObject::tr("Unable to establish a database connection.\n"
                                          "This Application needs SQLite support."
                                          "Click OK to exit."), QMessageBox::Ok);
        exit(-1);
        return false;
    }
    connectNa=connectionName;
    query=QSqlQuery(db);
    return true;
}

bool HSqlCommon::switchConnection(const QString &connectionName)
{
    auto newdb=QSqlDatabase::database(connectionName,true);

    if(newdb.isOpen()){
        db.close();
        db=newdb;
        query=QSqlQuery(newdb);
        return true;
    }
    return false;
}

void HSqlCommon::closeConnection(const QString &connectionName)
{
    if(connectionName.isEmpty()){
        db.close();
    }else{
        auto newdb=QSqlDatabase::database(connectionName,true);
        newdb.close();
    }
}

bool HSqlCommon::deleteTables(const QStringList &tableNames)
{

    for(const auto &k:tableNames){
        if(!execSQL(QString("DROP TABLE %1").arg(k),query)){
            return false;
        }
    }
    return true;
}



QStringList HSqlCommon::getAllTableNames() const
{
    return db.tables();
}


bool HSqlCommon::insertTable(const QString &tableName, const QStringList &headerNames)
{
    if(db.tables().contains(tableName)){
        QMessageBox::warning(nullptr,"database error","table name has existed!! ");
        return false;
    }

    QStringList list;
    for(const auto &k:headerNames){
        list.append(k+" varchar(1024)");
    }

    return execSQL(QString("CREATE TABLE %1(%2)").arg(tableName).arg(list.join(",")),query);
}

bool HSqlCommon::insertRowValues(const QString &tableName, const QStringList &values, const QStringList &headerNames)
{

    QStringList list;
    for(const auto&k:values){
        list<<("'"+k+"'");
    }

    if(headerNames.isEmpty()){
        return execSQL(QString("INSERT INTO %1 VALUES(%2)").arg(tableName).arg(list.join(",")),query);
    }else{
        if(list.size()!=headerNames.size()){
            return false;
        }
        return execSQL(QString("INSERT INTO %1 [(%2)] VALUES(%3)").arg(tableName).arg(headerNames.join(",")).arg(list.join(",")),query);
    }
}

bool HSqlCommon::updateValue(const QString&tableName, const QString&headerName, const QString &data, const QString&conditionName, const QString &conditionValue)
{
    return  execSQL(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
                    .arg(tableName).arg(headerName).arg(data).arg(conditionName).arg(conditionValue),query);
}

bool HSqlCommon::updateColumnValues(const QString &tableName, const QStringList &headerNames, const QString &data)
{
    return  execSQL(QString("UPDATE %1 SET %2='%3'")
                    .arg(tableName).arg(headerNames.join(",")).arg(data),query);
}

bool HSqlCommon::updateRowValues(const QString &tableName, const QStringList &headerNames, const QString &conditionName, const QStringList &conditionValues, const QStringList &datas)
{
    QStringList temp=datas;
    for(const auto &k:conditionValues){
        QStringList mkl;
        for(const auto &m:headerNames){
            mkl<<(m+"='"+temp.takeFirst()+"'");
        }

        return  execSQL(QString("UPDATE %1 SET %2 WHERE %3='%4'")
                        .arg(tableName).arg(mkl.join(",").arg(conditionName).arg(k)),query);
    }
}

bool HSqlCommon::deleteValue(const QString &tableName, const QString &conditionName, const QString &conditionValue)
{
    return  execSQL(QString("DELETE FROM %1 WHERE %2 = '%3'")
                    .arg(tableName).arg(conditionName).arg(conditionValue),query);
}

bool HSqlCommon::deleteAll(const QString &tableName)
{
    return  execSQL(QString("DELETE FROM %1").arg(tableName),query);
}




QVariantList HSqlCommon::getColumnValues(const QString &tableName, const QStringList &headerNames)
{


    QVariantList list;
    qint32 size=headerNames.size();
    for(qint32 k=0;k<size;++k){
        execSQL(QString("SELECT %1 FROM %2 ").arg(headerNames.at(k)).arg(tableName),query);
        while (query.next()) {
            //auto va=query.value(0);
            //if(va.isValid()&&!va.toString().isEmpty()){
               list<< query.value(0);
            //}
        }

    }
    return list;
}

QVariantList HSqlCommon::getColumnValues(const QString &tableName, const QStringList &headerNames, const QString &conditionName, const QString &conditionValue)
{
    QVariantList list;
    qint32 size=headerNames.size();
    for(qint32 k=0;k<size;++k){
        execSQL(QString("SELECT %1 FROM %2 WHERE %3='%4'").arg(headerNames.at(k)).arg(tableName).arg(conditionName).arg(conditionValue),query);
        while (query.next()) {
            //auto va=query.value(0);
            //if(va.isValid()&&!va.toString().isEmpty()){
               list<< query.value(0);
            //}
        }

    }
    return list;
}

QVariantList HSqlCommon::getRowValues(const QString &tableName,const QStringList &headerNames, const QString &conditionName, const QStringList &conditionValues)
{
    QVariantList list;
    for(const auto &k:conditionValues){

        QVariantList list;
        qint32 size=headerNames.size();
        for(qint32 m=0;m<size;++m){
            execSQL(QString("SELECT %1 FROM %2 WHERE %3 = '%4'").arg(headerNames.at(m)).arg(tableName).arg(conditionName).arg(k),query);
            while (query.next()) {
                list<< query.value(0);
            }
        }

    }
    return list;
}

QVariant HSqlCommon::getValue(const QString &tableName, const QString &headerName, const QString&conditionName, const QString &conditionValue)
{
    execSQL(QString("SELECT %1 FROM %2 WHERE %3 = '%4'")
            .arg(headerName)
            .arg(tableName)
            .arg(conditionName)
            .arg(conditionValue),query);
    query.next();
    return query.value(0);

}

qint32 HSqlCommon::getRowConut(const QString&tableName)
{
    execSQL(QString("SELECT COUNT(*) AS num FROM %1").arg(tableName),query);
    query.next();
    return query.value("num").toInt();

}

qint32 HSqlCommon::getColCount(const QString &tableName)
{
    execSQL(QString("SELECT * FROM %1").arg(tableName),query);
    query.next();
    return  query.record().count();
}







