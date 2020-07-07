#ifndef HSTARTEXTAPP_H
#define HSTARTEXTAPP_H

#include <QObject>

class QProcess;

class HStartExtApp : public QObject
{
    Q_OBJECT
public:
    static HStartExtApp* GetInstance(QObject *parent = nullptr);


signals:

public slots:

    bool StartAOCLaser();


private:
    explicit HStartExtApp(QObject *parent = nullptr);
       ~HStartExtApp() override;
    static HStartExtApp* m_pInstance;

    QProcess*aocLaser;
    bool isFineshed=true;
    bool isExited=false;
};

#endif // HSTARTEXTAPP_H
