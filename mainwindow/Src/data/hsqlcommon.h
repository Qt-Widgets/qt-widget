#ifndef HSQLCOMMON_H
#define HSQLCOMMON_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <stable.h>




class HSqlCommon
{
public:
    static HSqlCommon* GetInstance();
    static void Release();

    bool connectDataBase(const QString &dataPath,const QString &connectionName);

    const QString &getCurrentConnectionName() const{return connectNa;}

    bool switchConnection(const QString &connectionName);
    void closeConnection(const QString &connectionName=nullptr);

    bool deleteTables(const QStringList&tableNames);




    QStringList getAllTableNames() const;

    bool insertTable(const QString&tableName,const QStringList& headerNames);
    bool insertRowValues(const QString&tableName,const QStringList& values,const QStringList& headerNames=QStringList());

    bool updateValue(const QString&tableName,const QString&headerName,const QString&data,const QString&conditionName,const QString&conditionValue);
    bool updateColumnValues(const QString&tableName, const QStringList&headerNames, const QString&data);
    bool updateRowValues(const QString&tableName, const QStringList&headerNames,const QString&conditionName,const QStringList&conditionValues,const QStringList& datas);

    bool deleteValue(const QString&tableName,const QString&conditionName,const QString&conditionValue);
    bool deleteAll(const QString&tableName);

    QVariantList getColumnValues(const QString&tableName,const QStringList&headerNames);
    QVariantList getColumnValues(const QString&tableName,const QStringList&headerNames,const QString&conditionName,const QString&conditionValue);
    QVariantList getRowValues(const QString&tableName,const QStringList &headerNames,const QString&conditionName,const QStringList&conditionValues);

    QVariant getValue(const QString&tableName,const QString&headerName,const QString&conditionName,const QString&conditionValue);


    qint32 getRowConut(const QString&tableName);
    qint32 getColCount(const QString&tableName);




private:
    explicit HSqlCommon();
    ~HSqlCommon();
    static HSqlCommon* m_pInstance;

    QSqlDatabase db;
    QSqlQuery query;

    QString connectNa;

    bool execSQL(const QString &str, QSqlQuery& sqlDb);

};


#endif // HSQLCOMMON_H
