#include "hdatacollect.h"

#include "SerialPort/hserialdialog.h"

#include <QTimer>
#include <QEventLoop>
#include <QRandomGenerator>

#include <QVariant>
#include <QDebug>
#include <QMessageBox>

#include "Src/Gui/h_cameracontrollerwidget.h"


HDataCollect* HDataCollect::m_pInstance = nullptr;

HDataCollect *HDataCollect::GetInstance(QVariantList commondList,QWidget *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HDataCollect(commondList,parent);
    }
    return m_pInstance;
}

void HDataCollect::setInstructions(const QVariantList &inss)
{
    m_inss=inss;

    m_data_one.clear();

    for(qint32 k=0;k<m_inss.size()+1;++k){
        m_data_one<<0;
    }

}



HDataCollect::HDataCollect(QVariantList commondList,QObject *parent) : QObject(parent)
{
    m_inss=commondList;
    ptrSerialDialog=HSerialDialog::GetInstance();

    for(qint32 k=0;k<m_inss.size()+1;++k){
        m_data_one<<0;
    }


    connect(ptrSerialDialog,&HSerialDialog::DataResponsed,this,&HDataCollect::handlerSerialRespose);



    //myLib.setFileName(HConfigCtr::GetInstance()->GetConfigVision(h_vision_Dlls).value(1).toString());
    myLib.load();
    GetVersion = (HV_GetVersion)myLib.resolve("HV_GetVersion");

    AddParameters = (HV_AddParameters)myLib.resolve("HV_AddParameters");
    ClearParameters = (HV_ClearParameters)myLib.resolve("HV_ClearParameters");

    ProcessGreyImage = (HV_ProcessGreyImage)myLib.resolve("HV_ProcessGreyImage");

    GetLastError = (HV_GetLastError)myLib.resolve("HV_GetLastError");

    GetResult1 = (HV_GetResult1)myLib.resolve("HV_GetResult1");

    GetResult2 = (HV_GetResult2)myLib.resolve("HV_GetResult2");

    GetResult3 = (HV_GetResult3)myLib.resolve("HV_GetResult3");
    GetResult4 = (HV_GetResult4)myLib.resolve("HV_GetResult4");
}

HDataCollect::~HDataCollect()
{
    myLib.unload();
}

bool HDataCollect::CollectDataOnce(qreal time)
{
    m_data_one.first()=time;

    /////////////////////////////////////
    //    //    for(qint32 k=0;k<m_inss.size();++k){
    //    //        m_data_one[k+1]=QString::number(/*QRandomGenerator::global()->bounded(5.0)*/15,'f',5);
    //    //        qDebug()<<m_inss[k];
    //    //    }

    //    //   return true;
    //    m_index=1;
    //    handler();

    //    int k=0,size=m_inss.size();

    //    while (k<100000) {
    //        ++k;
    //        QEventLoop loop;
    //        QTimer::singleShot(5, &loop, SLOT(quit()));
    //        loop.exec();
    //        if(m_index==-1){
    //            m_index=1;
    //            return false;
    //        }
    //        else if(m_index==size){
    //            m_index=1;
    //            return true;
    //        }else{
    //            continue;
    //        }
    //    }
    //    return false;
    //////////////////////////////////
    ptrSerialDialog->RequestData(m_inss.at(0).toString());

    QEventLoop loop;
    QTimer::singleShot(1100, &loop, SLOT(quit()));
    loop.exec();

    if(isSerialSucceed){
        if(GetVersion&&AddParameters&&ProcessGreyImage&&GetLastError&&GetResult1&&GetResult2&&GetResult3&&GetResult4){
            if(GetVersion()==MyAppVersionInt){

                if(H_CameraControllerWidget::GetInstance()->showOneImg(0)){

                    qint32 width,height;
                    uchar *pData=H_CameraControllerWidget::GetInstance()->getCurrentImage(width,height);

//                    auto list=HConfigCtr::GetInstance()->GetConfigVision(h_vision_ParaTypes).value(1).toList();
//                    auto value=HConfigCtr::GetInstance()->GetConfigVision(h_vision_ParaValues).value(1).toList();

//                    ClearParameters();
//                    for(qint32 k=0;k<list.size();++k){

//                        switch (list.at(k).toInt()) {
//                        case 0:{
//                            double datta=value.value(k).toDouble();
//                            AddParameters(0,&datta);
//                        }
//                            break;
//                        case 1:{
//                            QByteArray datta=value.value(k).toString().toLocal8Bit();
//                            AddParameters(1,datta.data());
//                        }
//                            break;
//                        case 2:{
//                            int datta=value.value(k).toInt();
//                            AddParameters(2,&datta);
//                        }
//                            break;
//                        default:
//                            break;
//                        }
//                    }

                    if(ProcessGreyImage(pData,width,height)){

                        H_CameraControllerWidget::GetInstance()->addShowEllipse(QPointF(GetResult1(0),GetResult2(0)),GetResult3(0),GetResult4(0));
                        //qDebug()<<GetResult1(0)<<GetResult2(0)<<GetResult3(0)<<GetResult4(0);

                        m_data_one[1]=GetResult1(0);
                        m_data_one[2]=GetResult2(0);
                        m_data_one[3]=GetResult3(0);
                        m_data_one[4]=GetResult4(0);

                        ptrSerialDialog->RequestData(m_inss.at(1).toString());
                        return true;

                    }else{
                        QMessageBox::warning(nullptr,tr("warning"),tr("image process failed !"));
                    }

                }else{
                    QMessageBox::warning(nullptr,tr("warning"),tr("get image failed !"));
                }
            }else{
                QMessageBox::warning(nullptr,tr("warning"),tr("vision function dll version is unsupported !"));
            }

        }else{
            QMessageBox::warning(nullptr,tr("warning"),tr("unknown vision function dll !"));
        }
    }else{
        QMessageBox::warning(nullptr,tr("warning"),tr("serial port read failed !"));
    }
    return false;
}

void HDataCollect::handler()
{
    /////////////////////////////////////
    //ptrSerialDialog->RequestData(m_inss.at(m_index-1).toString()+"\r\n");
    /////////////////////////////////////
    ptrSerialDialog->RequestData(m_inss.at(0).toString());
}

void HDataCollect::handlerSerialRespose(bool isSucceeded, const QString &str)
{
    isSerialSucceed=isSucceeded;

    ///////////////////////for com data get/////////////////////////////////
    //    if(m_index!=-2){
    //        if(isSucceeded){

    //            m_data_one[m_index]=str.toDouble();

    //            if(m_index<m_inss.size()){
    //                ++m_index;
    //                handler();
    //            }
    //        }else{
    //            m_index=-1;
    //        }
    //    }
    ////////////////////////////////////////////////////////////////////

}
