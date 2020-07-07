#include "inisettings.h"
#include <QFileInfo>
#include <QCoreApplication>
#include <QSettings>



IniSettings* IniSettings::m_pInstance = nullptr;

IniSettings *IniSettings::GetInstance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new IniSettings();
        atexit(Release);
    }
    return m_pInstance;
}
void IniSettings::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}


void IniSettings::initData()
{
    QString iniPath=QCoreApplication::applicationDirPath()+"/Configuration/config.ini";
    QFileInfo fileInfo(iniPath);

    if(!fileInfo.exists())
    {
        configIni = new QSettings(iniPath, QSettings::IniFormat);

        for(const auto&k:iniName){
            switch (iniName.indexOf(k)) {
            case Config_microStep:
                configIni->setValue(k,0.05);
                break;
            case Config_optimizeAngle:
                configIni->setValue(k,0.5236);
                break;
            case Config_optimizeLength:
                configIni->setValue(k,1.0);
                break;
            case A3200_EmulatedQuadratureDivider:
                configIni->setValue(k,100);
                break;
            default:
                configIni->setValue(k,0);
                break;
            }
        }

    }
    else{
        configIni= new QSettings(iniPath, QSettings::IniFormat);

        for(const auto&k:iniName){
            if(!configIni->contains(k)){

                switch (iniName.indexOf(k)) {
                case Config_microStep:
                    configIni->setValue(k,0.05);
                    break;
                case Config_optimizeAngle:
                    configIni->setValue(k,0.5236);
                    break;
                case Config_optimizeLength:
                    configIni->setValue(k,1.0);
                    break;
                case A3200_EmulatedQuadratureDivider:
                    configIni->setValue(k,100);
                    break;
                default:
                    configIni->setValue(k,0);
                    break;
                }

            }
        }

    }

    qint32 count=iniName.size();
    for(qint32 k=0;k<count;++k){
        hashInfo.insert(k,configIni->value(iniName.at(k)));
    }

}

void IniSettings::writeData()
{
    qint32 count=hashInfo.count();
    for(qint32 k=0;k<count;++k)
        configIni->setValue(iniName.at(k), hashInfo.value(k));
}

QVariant IniSettings::getValue(IniType index)
{
    return hashInfo.value(index);
}


void IniSettings::setValue(IniType index, const QVariant& value)
{
    hashInfo.insert(index,value);
    //configIni->setValue(iniName.at(index),value);
}


IniSettings::IniSettings()
{
    initData();
}

IniSettings::~IniSettings()
{
    writeData();
    delete configIni;
}
