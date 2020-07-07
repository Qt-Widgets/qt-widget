#ifndef PROGRAMHANDLER_H
#define PROGRAMHANDLER_H

#include <QObject>
#include "programdata.h"

class PainterMainWidget;

class ProgramHandler : public QObject
{
    Q_OBJECT
public:
    static ProgramHandler* GetInstance(QObject *parent = nullptr);

    void Init();


    int getProgramCount() const;
    QStringList getProgramNames() const;


signals:
    void programActived(int pre,int current);
    void programRefreshed(int index);
    void programNewed(QString,ProgramData *);
    void programAdded(QString,ProgramData *pData);
    void programFileAdded();
    void programClosed(int index);
    void programSelectionChanged(int index);



public slots:
    bool openProgram();
    bool openProgramFile(const QString& filePath);
    void newProgram();
    bool importDxf();
    void saveProgram();
    void saveProgramAs();

    void refreshProgram(int index);
    void refreshProgram();

    void alignProgramCenter();
    void alignProgramImg();

public:
    ProgramData*  getActiveProgram();
    int  getActiveProgramIndex() const{return activeIndex;}
    ProgramData*  getProgram(int index);
    void setActiveProgram(int index);
    void closeProgram(int index);
    qint32 getProgramIndex(ProgramData* program) const;

private:
    static ProgramHandler* m_pInstance;
    explicit ProgramHandler(QObject *parent = nullptr);
    ~ProgramHandler() Q_DECL_OVERRIDE;

    QVector <ProgramData*> projectList;
    int activeIndex;
    QVector<QString> savePath;

};

#endif // PROGRAMHANDLER_H
