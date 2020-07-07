#include "hsettings.h"
#include "hsqlcommon.h"
#include <QMessageBox>
#include <QCoreApplication>


HSettings* HSettings::m_pInstance = nullptr;

HSettings *HSettings::GetInstance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HSettings();
        atexit(Release);
    }
    return m_pInstance;
}
void HSettings::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

bool HSettings::Init()
{
    if(ptrDatabase->connectDataBase(QCoreApplication::applicationDirPath()+"/config/system_settings.db","settings_connection")){
        auto tablesssss=ptrDatabase->getAllTableNames();

        for(qint32 k=0;k<tables.size();++k){
            if(!tablesssss.contains(tables.at(k))){
                ptrDatabase->insertTable(tables.at(k),headerNamesArray[k]);
            }

        }
        return true;

    }else{
        QMessageBox::warning(nullptr,"database setting error","can not connect to database ,please check filepath!!");
        return false;
    }
}

void HSettings::resetAllSetting() const
{
    ptrDatabase->deleteTables(tables);

    qint32 size=tables.size();
    for(qint32 k=0;k<size;++k){
        ptrDatabase->insertTable(tables.at(k),headerNamesArray[k]);
    }
}

bool HSettings::resetTable(qint32 table) const
{
    return ptrDatabase->deleteAll(tables.at(table));
}

QVariantList HSettings::getTableValues(qint32 table) const
{
    return ptrDatabase->getColumnValues(tables.at(table),headerNamesArray[table]);
}



QVariantList HSettings::getTableValues(qint32 table,const QVector<qint32>& headIndexs) const
{
    QStringList list;

    for(const auto &k:headIndexs){
        list<< headerNamesArray[table].at(k);
    }
    return ptrDatabase->getColumnValues(tables.at(table),list);

}

QVariantList HSettings::getTableValues(qint32 table, const QVector<qint32> &headIndexs, qint32 indexCondition, const QVariant &valueCondition) const
{
    QStringList list;

    for(const auto &k:headIndexs){
        list<< headerNamesArray[table].at(k);
    }
    return ptrDatabase->getColumnValues(tables.at(table),list,headerNamesArray[table].at(indexCondition),valueCondition.toString());
}

QVariant HSettings::getTableValue(qint32 table,qint32 headIndex,qint32 indexCondition,const QVariant& valueCondition) const
{
    return ptrDatabase->getValue(tables.at(table),headerNamesArray[table].at(headIndex),headerNamesArray[table].at(indexCondition),valueCondition.toString());
}

QVariantList HSettings::getTableValuesAtCol(qint32 table, qint32 headerIndex) const
{
    return ptrDatabase->getColumnValues(tables.at(table),QStringList(headerNamesArray[table].at(headerIndex)));
}

bool HSettings::deleteRowValue(qint32 table, qint32 valueConditionIndex, const QVariant &data) const
{
    return ptrDatabase->deleteValue(tables.at(table),headerNamesArray[table].at(valueConditionIndex),data.toString());
}

bool HSettings::updataValue(qint32 table, qint32 headerIndex, qint32 indexCondition, const QVariant &valueCondition, const QVariant &data) const
{
    return ptrDatabase->updateValue(tables.at(table),headerNamesArray[table].at(headerIndex),data.toString(),headerNamesArray[table].at(indexCondition),
                                    valueCondition.toString());
}

bool HSettings::updataOneValue(qint32 table, const QString &name,const QVariantList&datas) const
{
    QStringList list;
    for(const auto&k:datas){
        list<<k.toString();
    }
    return ptrDatabase->updateRowValues(tables.at(table),headerNamesArray[table],headerNamesArray[table].first(),QStringList(name),list);
}

bool HSettings::addOneValue(qint32 table, const QVariantList &datas) const
{
    QStringList list;
    for(const auto&k:datas){
        list<<k.toString();
    }

    return ptrDatabase->insertRowValues(tables.at(table),list);
}




HSettings::HSettings()
{
    ptrDatabase=HSqlCommon::GetInstance();

}

HSettings::~HSettings()
{

}
