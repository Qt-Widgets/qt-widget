#include "mythreadworker.h"
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include <QCoreApplication>
#include <QTime>

//class RunnableTask :public QObject, public QRunnable
//{
//    Q_OBJECT
//public:
//    explicit RunnableTask(const HfuncPointer& func,QObject* parent=nullptr):QObject(parent),ptrFunc(func){
//           // setAutoDelete(true);
//    }
//private:
//    HfuncPointer ptrFunc;
//    void run() override
//    {
//        ptrFunc();
//    }
//};


MyThreadWorker* MyThreadWorker::m_pInstance = nullptr;

MyThreadWorker *MyThreadWorker::GetInstance(QObject *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new MyThreadWorker(parent);
    }
    return m_pInstance;
}

bool MyThreadWorker::RunThreadFunc(const HfuncPointer &data)
{
    ptrFunc=data;
    return threadPool-> tryStart(this);
}

MyThreadWorker::MyThreadWorker(QObject *parent):QObject(parent)
{
    setAutoDelete(false);
    threadPool-> setMaxThreadCount(200);
}

MyThreadWorker::~MyThreadWorker()
{
    threadPool-> releaseThread();
}

void MyThreadWorker::run()
{
    ptrFunc();
}






