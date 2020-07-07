#include "programhandler.h"
#include <QFileDialog>
#include <QSettings>
#include "Src/main-menu/hmainmenubar.h"

#include "Src/Motion/h_motioncontroller.h"
#include "Src/painter/paintermainwidget.h"


ProgramHandler* ProgramHandler::m_pInstance = nullptr;
ProgramHandler *ProgramHandler::GetInstance(QObject *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new ProgramHandler(parent);
    }
    return m_pInstance;
}

void ProgramHandler::Init()
{
    foreach(ProgramData* dk,projectList){
        delete dk;
    }
    projectList.clear();
    activeIndex=-1;

    HMainMenuBar*inory=HMainMenuBar::GetInstance();

    connect(inory->getAction(HMainMenuBar::File_file),&QAction::hovered,this,[=](){
        bool k=activeIndex>-1;
        inory->getAction(HMainMenuBar::File_imdxf)->setEnabled(k);
        inory->getAction(HMainMenuBar::File_save)->setEnabled(k);
        inory->getAction(HMainMenuBar::File_saveAs)->setEnabled(k);
    });
    connect(inory->getAction(HMainMenuBar::File_new),&QAction::triggered,this,&ProgramHandler::newProgram);
    connect(inory->getAction(HMainMenuBar::File_open),&QAction::triggered,this,&ProgramHandler::openProgram);
    connect(inory->getAction(HMainMenuBar::File_save),&QAction::triggered,this,&ProgramHandler::saveProgram);
    connect(inory->getAction(HMainMenuBar::File_saveAs),&QAction::triggered,this,&ProgramHandler::saveProgramAs);
    connect(inory->getAction(HMainMenuBar::File_imdxf),&QAction::triggered,this,&ProgramHandler::importDxf);

    connect(PainterMainWidget::GetInstance(),&PainterMainWidget::activeWindowChanged,this,&ProgramHandler::setActiveProgram);
    connect(PainterMainWidget::GetInstance(),&PainterMainWidget::windowClosed,this,&ProgramHandler::closeProgram);
}


int ProgramHandler::getProgramCount() const
{
    return projectList.size();
}

QStringList ProgramHandler::getProgramNames() const
{
    QStringList list;
    for(const auto&k:savePath){
        list<<QStringList(k.split("/")).last();
    }
    return list;
}


ProgramHandler::ProgramHandler(QObject *parent) : QObject(parent)
{


}

ProgramHandler::~ProgramHandler()
{

    for(const auto &k:savePath){
        if(k.isEmpty()){
            saveProgramAs();
        }
        else{
            if(projectList.value(activeIndex))
                projectList.at(activeIndex)->writeProject(savePath.at(activeIndex));
        }
    }

}

bool ProgramHandler::openProgram()
{

    QSettings settings("QtApp","inno");
    QString filePath= QFileDialog::getOpenFileName(nullptr, tr("HPRO file"), settings.value("DefaultDir/programOpen").toString(),"hpro file (*.hpro)");
    return openProgramFile(filePath);

}

bool ProgramHandler::openProgramFile(const QString &filePath)
{
    if(!filePath.isEmpty()){
        ProgramData*ptr=new ProgramData(this);
        if(ptr->openProject(filePath)){
            QSettings settings("QtApp","inno");
            settings.setValue("DefaultDir/programOpen",filePath);
            projectList.append(ptr);
            savePath.append(filePath);
            setActiveProgram(projectList.count()-1);
            emit programAdded(QStringList(filePath.split("/")).last(),ptr);

            connect(ptr->getViewPtr(),&HGraphicsView::selectionChanged,this,[=](){
                emit programSelectionChanged(getProgramIndex(ptr));
            });

            return true;
        }
        delete ptr;
    }
    return false;
}

void ProgramHandler::newProgram()
{
    QString namekkk="untitled-"+QString::number(projectList.size())+".hpro";
    ProgramData*ptr=new ProgramData(this);
    ptr->newProjrct(namekkk);
    connect(ptr->getViewPtr(),&HGraphicsView::selectionChanged,this,[=](){
        emit programSelectionChanged(getProgramIndex(ptr));
    });
    projectList.append(ptr);
    setActiveProgram(projectList.count()-1);
    emit programNewed(namekkk,ptr);

    savePath.append(namekkk);

}

bool ProgramHandler::importDxf()
{
    QSettings settings("QtApp","inno");
    QString filePath= QFileDialog::getOpenFileName(nullptr, tr("DXF file"), settings.value("DefaultDir/dxf").toString(),"DXF file (*.dxf)");

    if(!filePath.isEmpty()){
        if(getActiveProgram()->openDxf(filePath)){
            settings.setValue("DefaultDir/dxf",filePath);
            emit programRefreshed(getActiveProgramIndex());
            return true;
        }
    }
    return false;
}

void ProgramHandler::saveProgram()
{
    if(savePath.at(activeIndex).isEmpty()||savePath.at(activeIndex).left(9)=="untitled-"){
        saveProgramAs();
    }
    else{
        projectList.at(activeIndex)->writeProject(savePath.at(activeIndex));
    }
}

void ProgramHandler::saveProgramAs()
{
    QSettings settings("QtApp","inno");
    QString filePath= QFileDialog::getSaveFileName(nullptr, tr("HPRO file"), settings.value("DefaultDir/programSave").toString(),"hpro file (*.hpro)");

    if(!filePath.isEmpty()){
        settings.setValue("DefaultDir/programSave",filePath);
        projectList.at(activeIndex)->writeProject(filePath);
    }
}


void ProgramHandler::refreshProgram(int index)
{
    emit programRefreshed(index);
}

void ProgramHandler::refreshProgram()
{
    emit programRefreshed(activeIndex);
}

void ProgramHandler::alignProgramCenter()
{
    //projectList[activeIndex]->translateProject(H_HalconImgProcess::GetInstance()->getImgCenterPt());

}

void ProgramHandler::alignProgramImg()
{

}


ProgramData *ProgramHandler::getActiveProgram()
{
    return activeIndex>-1?projectList[activeIndex]:nullptr;
}

ProgramData *ProgramHandler::getProgram(int index)
{
    return index>-1?projectList.at(index):nullptr;
}

void ProgramHandler::setActiveProgram(int index)
{
    if(index>-1&&index<projectList.size()){
        auto bgbgbg=activeIndex;
        activeIndex=index;
        projectList.at(activeIndex)->setCurentProject();

        emit programActived(bgbgbg,index);
    }
}

void ProgramHandler::closeProgram(int index)
{

    if(index>-1&&index<projectList.size()){

        projectList.at(index)->getViewPtr()->getScene()->clearSelectedItems();
        emit programClosed(index);

        if(index==activeIndex){
            if(activeIndex!=0){
                setActiveProgram(activeIndex-1);
            }
            else{
                activeIndex=-1;
            }
        }else{
            if(index<activeIndex){
                --activeIndex;
            }
        }

        delete projectList.at(index);
        projectList.removeAt(index);

    }
}

qint32 ProgramHandler::getProgramIndex(ProgramData* program) const
{
    return projectList.indexOf(program);
}
