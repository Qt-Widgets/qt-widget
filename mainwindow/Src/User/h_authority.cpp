#include "h_authority.h"


H_Authority* H_Authority::m_pInstance = nullptr;

H_Authority *H_Authority::GetInstance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new H_Authority;
    }
    return m_pInstance;
}
void H_Authority::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}


H_Authority::H_Authority()
{
    rank=-1;
}

H_Authority::~H_Authority()
{

}
