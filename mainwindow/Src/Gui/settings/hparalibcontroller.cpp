#include "hparalibcontroller.h"


HParaLibController* HParaLibController::m_pInstance = nullptr;

HParaLibController *HParaLibController::GetInstance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HParaLibController;
        atexit(Release);
    }
    return m_pInstance;
}
void HParaLibController::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

bool HParaLibController::Init()
{
    filePath=QCoreApplication::applicationDirPath()+"/config/paraLib_data.bin";
    return openFromFile(filePath);
}

QStringList HParaLibController::getParaDatas(qint32 mode, const QString &name) const {
    switch (mode) {
    case 0:
        return getParas(mode,name,1,Cutter_Cam_pulseDelay+2);
    case 1:
        return getParas(mode,name,1,Scanner_Cam_closeLaserDelay+2);
    default:
        return QStringList();
    }
}

QStringList HParaLibController::getGeneralDatas(qint32 mode, const QString &name) const {
    switch (mode) {
    case 0:
        return getParas(mode,name,Cutter_General_focus+1,Cutter_General_count+2);
    case 1:
        return getParas(mode,name,Scanner_General_focus+1,Scanner_General_count+2);
    default:
        return QStringList();
    }
}

QStringList HParaLibController::getLaserDatas(qint32 mode, const QString &name) const {
    switch (mode) {
    case 0:
        return getParas(mode,name,Cutter_Laser_power+1,Cutter_Laser_dpk+2);
    case 1:
        return getParas(mode,name,Scanner_Laser_power+1,Scanner_Laser_fpk+2);
    default:
        return QStringList();
    }
}

QStringList HParaLibController::getCamDatas(qint32 mode, const QString &name) const {
    switch (mode) {
    case 0:
        return getParas(mode,name,Cutter_Cam_speed+1,Cutter_Cam_pulseDelay+2);
    case 1:
        return getParas(mode,name,Scanner_Cam_speed+1,Scanner_Cam_closeLaserDelay+2);
    default:
        return QStringList();
    }
}

QStringList HParaLibController::getDatas(qint32 mode, QString name) const
{
    switch (mode) {
    case 0:
        if(name.isNull()){
            name=m_defaultCutter;
        }
        return m_data_cutter.value(name);
    case 1:
        if(name.isNull()){
            name=m_defaultScanner;
        }
        return m_data_scanner.value(name);
    default:
        return QStringList();
    }

}


QStringList HParaLibController::getDatas(qint32 mode,qint32 index) const
{
    switch (mode) {
    case 0:
        return m_data_cutter.value(m_names_cutter.value(index));
    case 1:
        return m_data_scanner.value(m_names_scanner.value(index));
    default:
        return QStringList();
    }

}


void HParaLibController::addNew(qint32 mode,const QString &name, const QStringList &datas)
{
    switch (mode) {
    case 0:
        m_names_cutter.append(name);
        m_data_cutter[name]=datas;
        break;
    case 1:
        m_names_scanner.append(name);
        m_data_scanner[name]=datas;
        break;
    default:
        break;
    }


}

void HParaLibController::deletePara(qint32 mode,const QString &name)
{
    switch (mode) {
    case 0:
        m_names_cutter.removeOne(name);
        m_data_cutter.remove(name);
        break;
    case 1:
        m_names_scanner.removeOne(name);
        m_data_scanner.remove(name);
        break;
    default:
        break;
    }
}

void HParaLibController::deletePara(qint32 mode, qint32 index)
{
    switch (mode) {
    case 0:
        if(index>-1&&index<m_names_cutter.size())
            m_data_cutter.remove(m_names_cutter.takeAt(index));
        break;
    case 1:
        if(index>-1&&index<m_names_scanner.size())
            m_data_scanner.remove(m_names_scanner.takeAt(index));
        break;
    default:
        break;
    }
}

void HParaLibController::clearParas(qint32 mode)
{
    switch (mode) {
    case 0:
        m_names_cutter.clear();
        m_data_cutter.clear();
        m_defaultCutter.clear();
        break;
    case 1:
        m_names_scanner.clear();
        m_data_scanner.clear();
        m_defaultScanner.clear();
        break;
    default:
        break;
    }
}

void HParaLibController::setData(qint32 mode, const QString &name, qint32 index, const QString &data)
{
    switch (mode) {
    case 0:
        if(m_data_cutter.contains(name)){
            auto datas=m_data_cutter.value(name);
            datas.replace(index+1,data);
            m_data_cutter[name]=datas;
        }
        break;
    case 1:
        if(m_data_scanner.contains(name)){
            auto datas=m_data_scanner.value(name);
            datas.replace(index+1,data);
            m_data_scanner[name]=datas;
        }
        break;
    default:
        break;
    }
}

void HParaLibController::setDatas(qint32 mode, const QString &name, const QStringList &datas)
{
    switch (mode) {
    case 0:
        if(m_names_cutter.contains(name)){
            m_names_cutter[m_names_cutter.indexOf(name)]=datas.first();
            m_data_cutter[datas.first()]=datas;
        }
        break;
    case 1:
        if(m_names_scanner.contains(name)){
            m_names_scanner[m_names_scanner.indexOf(name)]=datas.first();
            m_data_scanner[datas.first()]=datas;
        }
        break;
    default:
        break;
    }
}

void HParaLibController::setDatas(qint32 mode, qint32 index, const QStringList &datas)
{
    switch (mode) {
    case 0:
        if(index>-1&&index<m_names_cutter.size()){
            m_names_cutter[index]=datas.first();
            m_data_cutter[datas.first()]=datas;
        }
        break;
    case 1:
        if(index>-1&&index<m_names_scanner.size()){
            m_names_scanner[index]=datas.first();
            m_data_scanner[datas.first()]=datas;
        }
        break;
    default:
        break;
    }
}


inline QStringList HParaLibController::getParas(qint32 mode, QString name, qint32 start, qint32 end) const
{

    QStringList list,datas;

    switch (mode) {
    case 0:
        if(name.isNull()){
            name=m_defaultCutter;
        }
        datas= m_data_cutter.value(name);
        for(;start<end;++start){
            list<<datas.value(start);
        }
        break;
    case 1:
        if(name.isNull()){
            name=m_defaultScanner;
        }
        datas= m_data_scanner.value(name);
        for(;start<end;++start){
            list<<datas.value(start);
        }
        break;
    default:
        break;
    }


    return list;
}


QDataStream &operator>>(QDataStream &input, HParaLibController &obj)
{

    input >> obj.m_data_cutter>>obj.m_data_scanner>>obj.m_names_cutter>>obj.m_names_scanner>>obj.m_defaultCutter>>obj.m_defaultScanner;
    return input;
}

QDataStream &operator<<(QDataStream &output, const HParaLibController &obj)
{

    output << obj.m_data_cutter<<obj.m_data_scanner<<obj.m_names_cutter<<obj.m_names_scanner<<obj.m_defaultCutter<<obj.m_defaultScanner;

    return output;
}


HParaLibController::HParaLibController()
{

}

HParaLibController::~HParaLibController()
{
    writeToFile(filePath);
}


bool HParaLibController::writeToFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)){
        return false;
    }
    QDataStream ds(&file);
    ds << *this ;
    file.close();
    return true;
}

bool HParaLibController::openFromFile(const QString &filePath)
{
    QFile f(filePath);
    if(!f.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QDataStream ds(&f);
    ds >> *this;
    f.close();
    return true;
}



