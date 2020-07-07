#include "hcommondata.h"

HCommonData* HCommonData::m_pInstance = nullptr;

HCommonData *HCommonData::GetInstance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HCommonData();
       // atexit(Release);
    }
    return m_pInstance;
}
void HCommonData::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

HCommonData::HCommonData()
{

}
