#ifndef PAINTERMAINWIDGET_H
#define PAINTERMAINWIDGET_H

#include <QWidget>
#include <QTabWidget>


#include "hgraphicsview.h"

class HGraphicsView;
class HPainterToolBar;
class QStatusBar;


class PainterMainWidget : public QWidget
{
    Q_OBJECT
public:

    static PainterMainWidget* GetInstance(QWidget *parent = nullptr);


    void setCurrentShowWindow(qint32 index);
    void setCurrentShowWindow(HGraphicsView * pwid);

    HGraphicsView * getItemView(qint32 index) const;
    HGraphicsView * getActiveView() const {return activeView;}


    HGraphicsView * newShowWindow(QString name);
    void closeShowWindow(qint32 index);
    void closeShowWindow(HGraphicsView *pwid);
    void refreshShowWindow(qint32 index);
    void resetShowWindow(qint32 index);

protected:
    //void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

signals:
    void activeWindowChanged(qint32 index);

    void windowClosed(qint32 index);



private slots:
    void handlerDocumentChanged(qint32 index);
    void handlerDocumentClosed(qint32 index);

private:
    explicit PainterMainWidget(QWidget *parent = nullptr);
    static PainterMainWidget* m_pInstance;
    ~PainterMainWidget() Q_DECL_OVERRIDE;


    HGraphicsView *activeView=nullptr;

    QVector<QWidget *> toolBar;

    QTabWidget *ptrTabWidget;
    QStatusBar*  ptrStatusBar;

};

#endif // PAINTERMAINWIDGET_H
