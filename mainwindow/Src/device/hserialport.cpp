#include "hserialport.h"

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>
#include <QTime>
#include <QTimer>

#include <QEventLoop>

MasterThread::MasterThread(QObject *parent) :
    QThread(parent)
{
    isClose=false;
}


MasterThread::~MasterThread()
{
    m_mutex.lock();
    m_quit = true;
    m_cond.wakeOne();
    m_mutex.unlock();
    wait();
}

void MasterThread::transaction(const QString &portName, int waitTimeout, const QString &request)
{

    const QMutexLocker locker(&m_mutex);
    m_portName = portName;
    m_waitTimeout = waitTimeout;
    m_request = request;
    isClose=false;

    if (!isRunning())
        start();
    else
        m_cond.wakeOne();
}

void MasterThread::closePort()
{
    const QMutexLocker locker(&m_mutex);
    isClose=true;

    if (!isRunning())
        start();
    else
        m_cond.wakeOne();
}

bool MasterThread::isConnected() const
{
    return !isClose;
}

void MasterThread::run()
{
    bool currentPortNameChanged = false;

    m_mutex.lock();
//! [4] //! [5]
    QString currentPortName;
    if (currentPortName != m_portName) {
        currentPortName = m_portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = m_waitTimeout;
    QString currentRequest = m_request;
    m_mutex.unlock();
//! [5] //! [6]
    QSerialPort serial;

    if (currentPortName.isEmpty()) {
        emit error(tr("No port name specified"));
        return;
    }

    while (!m_quit) {
//![6] //! [7]
        if (currentPortNameChanged) {
            serial.close();
            serial.setPortName(currentPortName);

            if (!serial.open(QIODevice::ReadWrite)) {
                emit error(tr("Can't open %1, error code %2")
                           .arg(m_portName).arg(serial.error()));
                return;
            }
        }
//! [7] //! [8]
        // write request
        const QByteArray requestData = currentRequest.toUtf8();
        serial.write(requestData);
        if (serial.waitForBytesWritten(m_waitTimeout)) {
//! [8] //! [10]
            // read response
            if (serial.waitForReadyRead(currentWaitTimeout)) {
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(10))
                    responseData += serial.readAll();

                const QString response = QString::fromUtf8(responseData);
//! [12]
                emit this->response(response);
//! [10] //! [11] //! [12]
            } else {
                emit timeout(tr("Wait read response timeout %1")
                             .arg(QTime::currentTime().toString()));
            }
//! [9] //! [11]
        } else {
            emit timeout(tr("Wait write request timeout %1")
                         .arg(QTime::currentTime().toString()));
        }
//! [9]  //! [13]
        m_mutex.lock();
        m_cond.wait(&m_mutex);

        if(isClose){
            emit error(tr("serialPort closed!"));
            serial.clear();
            if(serial.isOpen())
                serial.close();
            m_mutex.unlock();
            return;
        }

        if (currentPortName != m_portName) {
            currentPortName = m_portName;
            currentPortNameChanged = true;
        } else {
            currentPortNameChanged = false;
        }
        currentWaitTimeout = m_waitTimeout;
        currentRequest = m_request;
        m_mutex.unlock();
    }
//! [13]
}


HSerialport* HSerialport::m_pInstance = nullptr;

HSerialport *HSerialport::GetInstance(QObject *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HSerialport(parent);
        //atexit(Release);
    }
    return m_pInstance;
}
void HSerialport::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

QStringList HSerialport::getComArray() const
{
    QStringList str;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        str<<info.portName();
    return str;
}


void HSerialport::closePort()
{
    m_thread.closePort();
}

HSerialport::HSerialport(QObject *parent) : QObject(parent)
{
    serialState=Ready;
    ComName="COM1";
    timeOut=1000;
    connect(&m_thread, &MasterThread::response, this,[=](QString s){
        //QMutexLocker locker(&m_mutex);
        //qDebug()<<s;
        emit responseData(s);
        reusltString=s;
        serialState=ReadySucceed;

    });
    connect(&m_thread, &MasterThread::error, this, [=](QString s){emit errorData(s); serialState=ReadyFail; /*qDebug()<<s;*/});
    connect(&m_thread, &MasterThread::timeout, this,[=](QString s){emit timeOutData(s); serialState=ReadyFail;/*qDebug()<<s;*/});

}


HSerialport::~HSerialport()
{
    m_thread.exit();

}


bool HSerialport::connectSerial(const QString &SerialportName, const int &timeout, const QString &firstRequest)
{
    if(getComArray().contains(SerialportName)){
        ComName=SerialportName;
        timeOut=timeout;
        //m_thread.transaction(SerialportName,timeOut,firstRequest);
        QString df;
        if(requestDataWait(firstRequest,df)){
            return df=="REMOTE\r\n";
        }
        return false;
    }
    else{
        return false;
    }
}

bool HSerialport::requestDataWait(const QString &requestData, QString &resposeData, qint32 timeOut,qint32 retryTimes)
{
    QMutexLocker locker(&m_mutex);
    if(serialState!=WaitResponse){

        for(qint32 mt=0;mt<retryTimes;++mt){

            m_thread.transaction(ComName,timeOut,requestData);
            serialState=WaitResponse;
            for(qint32 k=0;k<timeOut;++k){
                QEventLoop loop;
                QTimer::singleShot(100, &loop, SLOT(quit()));
                loop.exec();

                if(serialState==ReadySucceed){
                    resposeData.swap(reusltString);
                    return true;
                }else if(serialState==WaitResponse){
                    continue;
                }else{
                    break;
                }
            }
        }
    }
    return false;
}

bool HSerialport::isConnected() const
{
    return m_thread.isConnected();
}



void HSerialport::requestData(const QString &requestData, long timeOut)
{
    QMutexLocker locker(&m_mutex);
    if(serialState!=WaitResponse){
        m_thread.transaction(ComName,timeOut,requestData);
        serialState=WaitResponse;
    }
}
