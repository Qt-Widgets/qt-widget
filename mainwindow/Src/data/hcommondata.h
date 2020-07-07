#ifndef HCOMMONDATA_H
#define HCOMMONDATA_H


class HCommonData
{
public:
    static HCommonData* GetInstance();
    static void Release();
private:
    static HCommonData* m_pInstance;
    explicit HCommonData();
};

#endif // HCOMMONDATA_H
