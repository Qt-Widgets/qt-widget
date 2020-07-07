#ifndef MYTHREADWORKER_H
#define MYTHREADWORKER_H

//#include <QObject>
//#include <QThread>
//#include <QMutex>
//#include <QRunnable>
//#include <QThreadPool>

#include <stable.h>

typedef  std::function<void()> HfuncPointer;


class MyThreadWorker :public QObject, public QRunnable
{
    Q_OBJECT
public:
    static MyThreadWorker* GetInstance(QObject *parent=nullptr);

    bool RunThreadFunc(const HfuncPointer &data);


private:
    explicit MyThreadWorker(QObject *parent);
    static MyThreadWorker* m_pInstance;
    ~MyThreadWorker() Q_DECL_OVERRIDE;

signals:


public slots:




protected:


private:
    mutable HfuncPointer ptrFunc;
    void run() override;
    QThreadPool *threadPool = QThreadPool::globalInstance();

};
#endif // MYTHREADWORKER_H
