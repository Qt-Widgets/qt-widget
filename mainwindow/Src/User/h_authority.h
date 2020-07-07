#ifndef H_AUTHORITY_H
#define H_AUTHORITY_H


class H_Authority
{
public:
    static H_Authority* GetInstance();
    static void Release();

    int getAuthority() const {return rank;}
    void setAuthority(int data) { rank=data;}

private:
    explicit H_Authority();
    ~H_Authority();
    static H_Authority* m_pInstance;

    int rank;

};

#endif // H_AUTHORITY_H
