#include "hmathfunctions.h"


#include "hsystemadjustcontroller.h"
#include "Src/Motion/h_motioncontroller.h"
#include "Src/Motion/hcameracontroller.h"
#include "Src/Gui/h_cameracontrollerwidget.h"
#include "interface/hinterfacecontroller.h"



HSystemAdjustController* HSystemAdjustController::m_pInstance = nullptr;

HSystemAdjustController *HSystemAdjustController::GetInstance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HSystemAdjustController;
        atexit(Release);
    }
    return m_pInstance;
}
void HSystemAdjustController::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

bool HSystemAdjustController::Init()
{
    filePath=QCoreApplication::applicationDirPath()+"/config/adjust_data.bin";
    if(!openFromFile(filePath)){
        adjustDataArray.resize(6);
        return false;
    }
    if(adjustDataArray.size()!=6){
        adjustDataArray.resize(6);
        return false;
    }
    return true;
}


QVariantList HSystemAdjustController::getOutline(qint32 type) const
{
    QVariantList list;
    for (const auto&m:adjustDataArray.at(type).outlines) {
        list<<m.name<<m.status;
    }
    return list;
}

QVector<QString> HSystemAdjustController::getContent(ContentType type, const QString &name) const
{

    return adjustDataArray.at(type).contents.value(name);
}

QStringList HSystemAdjustController::getParas(HSystemAdjustController::ContentType type, const QString &name) const
{
    return adjustDataArray.at(type).paras.value(name);
}

QString HSystemAdjustController::getCurrentOutline(qint32 type)
{
    return adjustDataArray.at(type).currentOutline;
}

HAdjustOutline HSystemAdjustController::getOutline(HSystemAdjustController::ContentType type, const QString &name) const
{
    return adjustDataArray.at(type).outlines.value(adjustDataArray.at(type).outlines.indexOf(name));
}

qint32 HSystemAdjustController::deletePageData(qint32 type, const QString &name)
{
    qint32 index=adjustDataArray[type].outlines.indexOf(name);
    if(index!=-1){
        adjustDataArray[type].outlines.removeOne(HAdjustOutline(name));

        adjustDataArray[type].contents.remove(name);
        adjustDataArray[type].transforms.remove(name);

        if(name==adjustDataArray[type].currentOutline){
            qint32 size=adjustDataArray[type].outlines.size();
            if(size!=index){
                adjustDataArray[type].currentOutline=adjustDataArray[type].outlines.at(index).name;
            }else{
                adjustDataArray[type].currentOutline=size>0?adjustDataArray[type].outlines.last().name:nullptr;
            }
            return 1;
        }
    }
    return 0;
}

void HSystemAdjustController::clearPage(qint32 type)
{
    adjustDataArray.remove(type);
}

qint32 HSystemAdjustController::addNewData(qint32 type, const HAdjustOutline &outline)
{
    adjustDataArray[type].outlines.append(outline);
    QVector<QString> content;
    content<<outline.name;
    adjustDataArray[type].contents.insert(outline.name,content);
    adjustDataArray[type].transforms[outline.name][HAdjustTransQPolygonF()]=QTransform(1,0,0,0,1,0,0,0,1);
    if(adjustDataArray[type].outlines.size()==1){
        adjustDataArray[type].currentOutline=adjustDataArray[type].outlines.first().name;
        return 1;
    }
    return 0;
}

void HSystemAdjustController::savePageData(qint32 type, const QString &oname, const HAdjustOutline &outline, const QVector<QString> &content)
{
    qint32 index= adjustDataArray.at(type).outlines.indexOf(HAdjustOutline(oname));
    adjustDataArray[type].outlines[index]=outline;
    adjustDataArray[type].contents.remove(oname);
    adjustDataArray[type].contents[outline.name]=content;
}

void HSystemAdjustController::setDefautOutline(qint32 type, const QString &name)
{
    if(adjustDataArray.at(type).outlines.contains(name)){
        adjustDataArray[type].currentOutline= name;
    }
}

void HSystemAdjustController::saveParasData(HSystemAdjustController::ContentType type, const QString &name, const QStringList &para)
{
    adjustDataArray[type].paras[name]=para;
}

void HSystemAdjustController::saveAllData(qint32 type, const QString &oname, const HAdjustOutline &outline, const QVector<QString> &content, const QStringList &para, const QMap<HAdjustTransQPolygonF, QTransform> &trans)
{
    qint32 index= adjustDataArray.at(type).outlines.indexOf(HAdjustOutline(oname));
    adjustDataArray[type].outlines[index]=outline;
    adjustDataArray[type].contents.remove(oname);
    adjustDataArray[type].contents[outline.name]=content;
    adjustDataArray[type].paras.remove(oname);
    adjustDataArray[type].paras[outline.name]=para;
    adjustDataArray[type].transforms.remove(oname);
    adjustDataArray[type].transforms[outline.name]=trans;
}


QPointF HSystemAdjustController::getAxisPosFromImageUseCamera(const QString &cordName, const QString &funcName) const
{

    auto ptrImgProcess=HInterfaceController::GetInstance()->imgInterface;
    auto ptrMotion=H_MotionController::GetInstance();
    auto ptrCameraShow=H_CameraControllerWidget::GetInstance();
    if(!ptrImgProcess||!ptrMotion||!ptrCameraShow){
        return QPointF();
    }

    auto axisX=getContent(HSystemAdjustController::camera,cordName).value(1);
    auto axisY=getContent(HSystemAdjustController::camera,cordName).value(2);
    auto axisZ=getContent(HSystemAdjustController::camera,cordName).value(3);
    auto cameraName=getContent(HSystemAdjustController::camera,cordName).value(4);

    auto paxisX=ptrMotion->GetAxis(axisX);
    auto paxisY=ptrMotion->GetAxis(axisY);
    auto paxisZ=ptrMotion->GetAxis(axisZ);
    if(!paxisX||!paxisY||!paxisZ){
        return QPointF();
    }


    auto focus=getContent(HSystemAdjustController::camera,cordName).value(5).toDouble();

    if(paxisZ->MoveToWait(focus)){
        return QPointF();
    }

    if(ptrCameraShow->showOneImg(cameraName)){
        return QPointF();
    }
    qint32 w,h;
    uchar *pData=ptrCameraShow->getCurrentImage(w,h);
    if(ptrImgProcess->showImage(pData,w,h)){
        return QPointF();
    }
    QPointF result;
    if(ptrImgProcess->processImage(funcName,result)){
        return QPointF();
    }
    ptrCameraShow->addShowCross(result);
    ptrCameraShow->addShowText("OK",Qt::darkGreen);

    return QPointF(paxisX->GetFeedbackPos(),paxisY->GetFeedbackPos())-adjustDataArray[ContentType::camera].transforms.value(cordName).first().map(result);

}

QPointF HSystemAdjustController::getImageCenterFromCamera(const QString &cordName, const QString &funcName) const
{
    auto ptrImgProcess=HInterfaceController::GetInstance()->imgInterface;
    auto ptrMotion=H_MotionController::GetInstance();
    auto ptrCameraShow=H_CameraControllerWidget::GetInstance();
    if(!ptrImgProcess||!ptrMotion||!ptrCameraShow){
        return QPointF();
    }
    auto axisX=getContent(HSystemAdjustController::camera,cordName).value(1);
    auto axisY=getContent(HSystemAdjustController::camera,cordName).value(2);
    auto axisZ=getContent(HSystemAdjustController::camera,cordName).value(3);
    auto cameraName=getContent(HSystemAdjustController::camera,cordName).value(4);

    auto paxisX=ptrMotion->GetAxis(axisX);
    auto paxisY=ptrMotion->GetAxis(axisY);
    auto paxisZ=ptrMotion->GetAxis(axisZ);
    if(!paxisX||!paxisY||!paxisZ){
        return QPointF();
    }


    auto focus=getContent(HSystemAdjustController::camera,cordName).value(5).toDouble();

    if(paxisZ->MoveToWait(focus)){
        return QPointF();
    }

    if(ptrCameraShow->showOneImg(cameraName)){
        return QPointF();
    }
    qint32 w,h;
    uchar *pData=ptrCameraShow->getCurrentImage(w,h);
    if(ptrImgProcess->showImage(pData,w,h)){
        return QPointF();
    }
    QPointF result;
    if(ptrImgProcess->processImage(funcName,result)){
        return QPointF();
    }
    ptrCameraShow->addShowCross(result);
    ptrCameraShow->addShowText("OK",Qt::darkGreen);

    return QPointF(paxisX->GetFeedbackPos(),paxisY->GetFeedbackPos())-adjustDataArray[ContentType::camera].transforms.value(cordName).first().map(result);
}

qint32 HSystemAdjustController::getAxisPosFromImageUseVision(QPointF &outPos, const QString &visionName, const QString &funcName, bool isUseVisionFocus, double focusd) const
{
    auto cameraContent=getContent(camera,getContent(vision,visionName).value(1));

    auto axisX=cameraContent.value(1);
    auto axisY=cameraContent.value(2);
    auto axisZ=cameraContent.value(3);
    auto cameraName=cameraContent.value(4);

    auto ptrImgProcess=HInterfaceController::GetInstance()->imgInterface;
    auto ptrMotion=H_MotionController::GetInstance();
    auto ptrCameraShow=H_CameraControllerWidget::GetInstance();

    if(!ptrImgProcess||!ptrMotion||!ptrCameraShow){
        return -2;
    }

    auto paxisX=ptrMotion->GetAxis(axisX);
    auto paxisY=ptrMotion->GetAxis(axisY);
    auto paxisZ=ptrMotion->GetAxis(axisZ);
    if(!paxisX||!paxisY||!paxisZ){
        return -1;
    }

    auto transs=adjustDataArray[ContentType::vision].transforms.value(visionName);
    auto pos=QPointF(paxisX->GetFeedbackPos(),paxisY->GetFeedbackPos());


    if(funcName.isEmpty()){


        if(transs.contains(pos)){
            outPos=pos-transs.value(pos).map(QPointF(0,0));
            return 0;
        }else{
            outPos=pos-adjustDataArray[ContentType::camera].transforms.value(adjustDataArray[vision].contents.value(visionName).value(1)).first().map(QPointF(0,0));
            return 0;
        }
    }

    if(isUseVisionFocus){
        auto focus=cameraContent.value(5).toDouble();
        if(paxisZ->MoveToWait(focus)){
            return 1;
        }
    }else{
        if(paxisZ->MoveToWait(focusd)){
            return 1;
        }
    }


    if(ptrCameraShow->showOneImg(cameraName)){
        return 2;
    }

    qint32 w,h;
    uchar *pData=ptrCameraShow->getCurrentImage(w,h);
    if(ptrImgProcess->showImage(pData,w,h)){
        return 2;
    }
    QPointF result;
    if(ptrImgProcess->processImage(funcName,result)){
        return 3;
    }
    ptrCameraShow->addShowCross(result);
    ptrCameraShow->addShowText("OK",Qt::darkGreen);

    if(transs.contains(pos)){
        outPos=pos-transs.value(pos).map(result);
    }else{
        outPos=pos-adjustDataArray[ContentType::camera].transforms.value(adjustDataArray[vision].contents.value(visionName).value(1)).first().map(result);
    }
    return 0;
}

qint32 HSystemAdjustController::moveToAxisPosAndGetPosFromImageUseVision(QPointF &outPos, const QString &visionName, const QString &funcName, bool isUseVisionFocus, double focusd) const
{
    auto cameraContent=getContent(camera,getContent(vision,visionName).value(1));

    auto axisX=cameraContent.value(1);
    auto axisY=cameraContent.value(2);
    auto axisZ=cameraContent.value(3);
    auto cameraName=cameraContent.value(4);

    auto ptrImgProcess=HInterfaceController::GetInstance()->imgInterface;
    auto ptrMotion=H_MotionController::GetInstance();
    auto ptrCameraShow=H_CameraControllerWidget::GetInstance();

    if(!ptrImgProcess||!ptrMotion||!ptrCameraShow){
        return -1;
    }

    auto paxisX=ptrMotion->GetAxis(axisX);
    auto paxisY=ptrMotion->GetAxis(axisY);
    auto paxisZ=ptrMotion->GetAxis(axisZ);
    if(!paxisX||!paxisY||!paxisZ){
        return -1;
    }

    if(paxisX->MoveTo(outPos.x())){
        return 1;
    }
    if(paxisY->MoveTo(outPos.y())){
        return 2;
    }
    if(paxisX->WaitAxisStop()||paxisY->WaitAxisStop()){
        return 3;
    }


    auto transs=adjustDataArray[ContentType::vision].transforms.value(visionName);
    auto pos=QPointF(paxisX->GetFeedbackPos(),paxisY->GetFeedbackPos());

    if(funcName.isEmpty()){

        if(transs.contains(pos)){
            outPos=pos-transs.value(pos).map(QPointF(0,0));
            return 0;
        }else{
            outPos=pos-adjustDataArray[ContentType::camera].transforms.value(adjustDataArray[vision].contents.value(visionName).value(1)).first().map(QPointF(0,0));
            return 0;
        }
    }

    if(isUseVisionFocus){
        auto focus=cameraContent.value(5).toDouble();
        if(paxisZ->MoveToWait(focus)){
            return 1;
        }
    }else{
        if(paxisZ->MoveToWait(focusd)){
            return 1;
        }
    }


    if(ptrCameraShow->showOneImg(cameraName)){
        return 2;
    }

    qint32 w,h;
    uchar *pData=ptrCameraShow->getCurrentImage(w,h);
    if(ptrImgProcess->showImage(pData,w,h)){
        return 2;
    }
    QPointF result;
    if(ptrImgProcess->processImage(funcName,result)){
        return 3;
    }
    ptrCameraShow->addShowCross(result);
    ptrCameraShow->addShowText("OK",Qt::darkGreen);

    if(transs.contains(pos)){
        outPos=pos-transs.value(pos).map(result);
    }else{
        outPos=pos-adjustDataArray[ContentType::camera].transforms.value(adjustDataArray[vision].contents.value(visionName).value(1)).first().map(result);
    }
    return 0;
}

qint32 HSystemAdjustController::moveToImageCenterAndWaitFromVision(const QString &visionName, const QPointF &imagePos) const
{
    auto cameraContent=getContent(camera,getContent(vision,visionName).value(1));

    auto axisX=cameraContent.value(1);
    auto axisY=cameraContent.value(2);
    auto axisZ=cameraContent.value(3);
    auto cameraName=cameraContent.value(4);

    auto ptrImgProcess=HInterfaceController::GetInstance()->imgInterface;
    auto ptrMotion=H_MotionController::GetInstance();
    auto ptrCameraShow=H_CameraControllerWidget::GetInstance();

    if(!ptrImgProcess||!ptrMotion||!ptrCameraShow){
        return 1;
    }

    auto paxisX=ptrMotion->GetAxis(axisX);
    auto paxisY=ptrMotion->GetAxis(axisY);
    auto paxisZ=ptrMotion->GetAxis(axisZ);
    if(!paxisX||!paxisY||!paxisZ){
        return 2;
    }

    auto transs=adjustDataArray[ContentType::vision].transforms.value(visionName);
    auto pos=QPointF(paxisX->GetFeedbackPos(),paxisY->GetFeedbackPos());

    QPointF mpos;
    if(transs.contains(pos)){
        mpos=-transs.value(pos).map(imagePos);
    }else{
        mpos=-adjustDataArray[ContentType::camera].transforms.value(adjustDataArray[vision].contents.value(visionName).value(1)).first().map(imagePos);
    }

    if(paxisX->MoveToWait(mpos.x())){
        return 3;
    }
    if(paxisY->MoveToWait(mpos.y())){
        return 4;
    }
    return 0;
}

qint32 HSystemAdjustController::moveToAxisPosFromPainterPosUseCutter(const QPointF &pos, QString cutterName)
{
    if(adjustDataArray[ContentType::cutter].outlines.isEmpty()){
        return -1;
    }
    if(cutterName.isEmpty()){
        cutterName=adjustDataArray[ContentType::cutter].currentOutline;
        if(cutterName.isEmpty())
            return -2;
    }


    auto list=getParas(ContentType::cutter,cutterName);
    auto cutterContent=getContent(ContentType::cutter,cutterName);

    auto axisX=cutterContent.value(1);
    auto axisY=cutterContent.value(2);
    auto axisZ=cutterContent.value(3);
    auto vision=cutterContent.value(4);
    auto focus=cutterContent.value(5).toDouble();

    auto ptrMotion=H_MotionController::GetInstance();
    if(!ptrMotion){
        return 1;
    }

    auto paxisX=ptrMotion->GetAxis(axisX);
    auto paxisY=ptrMotion->GetAxis(axisY);
    auto paxisZ=ptrMotion->GetAxis(axisZ);
    if(!paxisX||!paxisY||!paxisZ){
        return 2;
    }

    auto trans=HMathFunc::stringToTrans(list.value(1));
    auto mpos=trans.map(pos);

    if(paxisZ->MoveTo(focus)){
        return 1;
    }

    if(paxisX->MoveTo(mpos.x())){
        return 2;
    }

    if(paxisY->MoveTo(mpos.y())){
        return 3;
    }

    return 0;
}

qint32 HSystemAdjustController::moveToAxisPosAndWaitFromPainterPosUseCutter(const QPointF &pos, QString cutterName)
{
    if(adjustDataArray[ContentType::cutter].outlines.isEmpty()){
        return -1;
    }
    if(cutterName.isEmpty()){
        cutterName=adjustDataArray[ContentType::cutter].currentOutline;
        if(cutterName.isEmpty())
            return -2;
    }


    auto list=getParas(ContentType::cutter,cutterName);
    auto cutterContent=getContent(ContentType::cutter,cutterName);

    auto axisX=cutterContent.value(1);
    auto axisY=cutterContent.value(2);
    auto axisZ=cutterContent.value(3);
    auto vision=cutterContent.value(4);
    auto focus=cutterContent.value(5).toDouble();

    auto ptrMotion=H_MotionController::GetInstance();
    if(!ptrMotion){
        return 1;
    }

    auto paxisX=ptrMotion->GetAxis(axisX);
    auto paxisY=ptrMotion->GetAxis(axisY);
    auto paxisZ=ptrMotion->GetAxis(axisZ);
    if(!paxisX||!paxisY||!paxisZ){
        return 2;
    }

    auto trans=HMathFunc::stringToTrans(list.value(1));
    auto mpos=trans.map(pos);

    if(paxisZ->MoveTo(focus)){
        return 1;
    }

    if(paxisX->MoveTo(mpos.x())){
        return 2;
    }

    if(paxisY->MoveTo(mpos.y())){
        return 3;
    }

    if(paxisX->WaitAxisStop()||paxisY->WaitAxisStop()||paxisZ->WaitAxisStop()){
        return 4;
    }

    return 0;

}

qint32 HSystemAdjustController::getPainterPosFromImageUseCutter(QPointF &pos, const QString &funcName, QString cutterName)
{
    if(adjustDataArray[ContentType::cutter].outlines.isEmpty()){
        return -1;
    }
    if(cutterName.isEmpty()){
        cutterName=adjustDataArray[ContentType::cutter].currentOutline;
        if(cutterName.isEmpty())
            return -2;
    }

    auto list=getParas(ContentType::cutter,cutterName);
    auto cutterContent=getContent(ContentType::cutter,cutterName);

    //    auto axisX=cutterContent.value(1);
    //    auto axisY=cutterContent.value(2);
    //    auto axisZ=cutterContent.value(3);
    auto vision=cutterContent.value(4);
    auto focus=cutterContent.value(5).toDouble();



    if(getAxisPosFromImageUseVision(pos,vision,funcName,false,focus)){
        return 1;
    }

    auto trans=HMathFunc::stringToTrans(list.value(1)).inverted();
    pos=trans.map(pos);
    return 0;
}

qint32 HSystemAdjustController::getTransFromPainterPossUseCutter(QTransform &outPutTrans, const QVector<QPointF> &poss,  QString funcName, QString cutterName)
{
    if(adjustDataArray[ContentType::cutter].outlines.isEmpty()){
        return -1;
    }
    if(cutterName.isEmpty()){
        cutterName=adjustDataArray[ContentType::cutter].currentOutline;
        if(cutterName.isEmpty())
            return -2;
    }

    auto list=getParas(ContentType::cutter,cutterName);
    auto cutterContent=getContent(ContentType::cutter,cutterName);
    auto ptrImgProcess=HInterfaceController::GetInstance()->imgInterface;

    if(funcName.isEmpty()){
        funcName=ptrImgProcess->getDefaultProcessFuncName();
    }

    auto axisX=cutterContent.value(1);
    auto axisY=cutterContent.value(2);
    auto axisZ=cutterContent.value(3);
    auto vision=cutterContent.value(4);
    auto focus=cutterContent.value(5).toDouble();

    auto trans=HMathFunc::stringToTrans(list.value(1));
    auto transT=trans.inverted();

    QVector<QPointF> dst;
    for(const auto &k:poss){
        auto mpos=trans.map(k);

        if(moveToAxisPosAndGetPosFromImageUseVision(mpos,vision,funcName,false,focus)){
            return 1;
        }
        dst<<transT.map(mpos);
    }

    auto trans1=QTransform(1,0,0,0,1,0,0,0,1);
    switch (poss.size()) {
    case 0:{

    }
        break;
    case 1:{
        auto pt=dst.at(0)-poss.at(0);
        trans1=QTransform::fromTranslate(pt.x(),pt.y());
    }
        break;
    case 2:{
        auto s1=poss.at(0);
        auto s2=poss.at(1);
        auto p1=dst.at(0);
        auto p2=dst.at(1);
        auto angle=-(QLineF(s1,s2).angleTo(QLineF(p1,p2)));
        auto pt=p1-s1;
        trans1=QTransform::fromTranslate(s1.x(),s1.y()).rotate(angle).translate(-s1.x(),-s1.y()).translate(pt.x(),pt.y());
    }
        break;
    case 3:{
        trans1=HMathFunc::GetAffineTransform(poss,dst);
    }
        break;
    case 4:{
        trans1=HMathFunc::GetPerspectiveTransform(poss,dst);
    }
        break;
    default:
        return 2;
    }
    outPutTrans=trans1*trans*HMathFunc::stringToTrans(list.value(0));
    return 0;
}

qint32 HSystemAdjustController::getCutterTransPt(QPointF &outPutPt, QPointF pt, QString cutterName)
{
    if(adjustDataArray[ContentType::cutter].outlines.isEmpty()){
        return -1;
    }
    if(cutterName.isEmpty()){
        cutterName=adjustDataArray[ContentType::cutter].currentOutline;
        if(cutterName.isEmpty())
            return -2;
    }

    pt=HMathFunc::stringToTrans(getParas(ContentType::cutter,cutterName).value(1)).map(pt);


    if(adjustDataArray[ContentType::cutter].transforms.value(cutterName).contains(pt)){
        outPutPt=adjustDataArray[ContentType::cutter].transforms.value(cutterName).value(pt).map(pt);

    }else{
        outPutPt=HMathFunc::stringToTrans(adjustDataArray[ContentType::cutter].paras.value(cutterName).value(0)).map(pt);
    }
    return 0;
}

qint32 HSystemAdjustController::getCutterTrans(QTransform &trans, QString cutterName)
{
    if(adjustDataArray[ContentType::cutter].outlines.isEmpty()){
        return -1;
    }
    if(cutterName.isEmpty()){
        cutterName=adjustDataArray[ContentType::cutter].currentOutline;
        if(cutterName.isEmpty())
            return -2;
    }
    trans=HMathFunc::stringToTrans(getParas(ContentType::cutter,cutterName).value(1))*HMathFunc::stringToTrans(getParas(ContentType::cutter,cutterName).value(0));
    return 0;
}


QDataStream &operator>>(QDataStream &input, HSystemAdjustController &obj)
{

    input >> obj.adjustDataArray;
    return input;
}

QDataStream &operator<<(QDataStream &output, const HSystemAdjustController &obj)
{

    output << obj.adjustDataArray;

    return output;
}



HSystemAdjustController::HSystemAdjustController()
{
    //    double angle=QLineF(0,0,0,2).angleTo(QLineF(1,0,3,0));
    //    auto trans1=QTransform::fromTranslate(1,0).rotate(-angle);
    //    QVector<QPointF> src;
    //    QVector<QPointF> dst;
    //    src<<QPointF(0,2)<<QPointF(0,0)<<QPointF(2,0)<<QPointF(2,2);
    //    dst<<QPointF(3,0)<<QPointF(1,0)<<QPointF(1,-2)<<QPointF(3,-2);
    //    auto trans=HMathFunc::GetAffineTransform(src,dst);
    //    auto trans1=QTransform::fromTranslate(0,0).rotate(-90).translate(0,1);
    //    auto trans2=HMathFunc::GetPerspectiveTransform(src,dst);
    //   qDebug()<<angle<<trans1.map(QPointF(0,2));
}

HSystemAdjustController::~HSystemAdjustController()
{
    writeToFile(filePath);
}

bool HSystemAdjustController::writeToFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)){
        return false;
    }
    QDataStream ds(&file);
    ds << *this ;
    file.close();
    return true;
}

bool HSystemAdjustController::openFromFile(const QString &filePath)
{
    QFile f(filePath);
    if(!f.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QDataStream ds(&f);
    ds >> *this;
    f.close();
    return true;
}
