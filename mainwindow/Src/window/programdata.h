#ifndef PROGRAMDATA_H
#define PROGRAMDATA_H

#include "Src/painter/hgraphicsview.h"


#include <QDataStream>
#include <QMessageBox>


class PainterMainWidget;
class HGraphicsView;
class ProgramHandler;

class ProgramData
{
public:
    explicit ProgramData(ProgramHandler *parent=nullptr);
    ~ProgramData();

    void setCurentProject();

    bool openDxf(const QString&path);
    bool openProject(const QString&filePath);
    void newProjrct(const QString &name);
    bool writeProject(const QString&filePath);
    void translateProject(const QPointF &pt);

    bool isActived() const;


    bool GetGcode(QString &code, QLineF &pline,const QTransform & transRE,qint32 range) const;

    HGraphicsView * getViewPtr() const;
    HLayerVector * getDataPtr() const;
    const HLayerVector & getData() const;
    HLayerVector & getData() ;

    void setHighlight(bool highLight) const;
    void setSelected(bool selected) const;

    qint32 getSelectedState() const;

    friend QDataStream &operator>>(QDataStream & input, ProgramData &obj){
        QString header,version;
         input >>header>>version;
         if(header!="HLaser_project"){
             QMessageBox::critical(nullptr,"error","open project failed,unknown file!!");
             return input;
         }
         if(version!=MyAppVersion){
             QMessageBox::critical(nullptr,"error","project version not support!!");
             return input;
         }
        input >> *obj.ptrItemView;
        return input;
    }
    friend QDataStream &operator<<(QDataStream & output, const ProgramData &obj){
        output <<QString("HLaser_project")<<MyAppVersion<<*obj.ptrItemView;

        return output;
    }


private:

    bool writeToFile(const QString &filePath);
    bool openFromFile(const QString &filePath);


    PainterMainWidget *ptrPainterWid=nullptr;
    HGraphicsView * ptrItemView=nullptr;
    ProgramHandler *ptrProgramHandler;

};

#endif // PROGRAMDATA_H
