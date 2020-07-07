#ifndef HSERIALPORT_H
#define HSERIALPORT_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

class MasterThread : public QThread
{
    Q_OBJECT

public:
    explicit MasterThread(QObject *parent = nullptr);
    ~MasterThread() override;

    void transaction(const QString &portName, int waitTimeout, const QString &request);
    void closePort();

    bool isConnected() const;

signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:

    void run() override;


    bool isClose;
    QString m_portName;
    QString m_request;
    int m_waitTimeout;
    QMutex m_mutex;
    QWaitCondition m_cond;
    bool m_quit = false;
};

class HSerialport: public QObject
{
    Q_OBJECT
public:
    static HSerialport* GetInstance(QObject *parent = nullptr);
    static void Release();

public slots:

    QStringList getComArray() const;

    void closePort();
    void requestData(const QString& requestData,long timeOut=1200);
    bool connectSerial(const QString & SerialportName, const int &timeout,const QString& firstRequest);

    bool requestDataWait(const QString& requestData,QString& resposeData,qint32 timeOut=12,qint32 retryTimes=3);

    bool isConnected() const;

signals:
    void responseData(QString s);
    void errorData(QString s);
    void timeOutData(QString s);

private:
    explicit HSerialport(QObject *parent = nullptr);
    HSerialport(const HSerialport& that);
    ~HSerialport();
    static HSerialport* m_pInstance;

    QMutex m_mutex;

    enum ComStatus {
        Ready,
        ReadySucceed,
        ReadyFail,
        WaitResponse

    } serialState;

    QString reusltString;

    QString ComName;
    int timeOut;
    MasterThread m_thread;

};

#endif // HSERIALPORT_H
