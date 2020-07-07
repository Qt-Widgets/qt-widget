#include "hcommon.h"
#include "paintermainwidget.h"
#include "hpaintertoolbar.h"

#include <QHBoxLayout>
#include <QStatusBar>
#include <QTabBar>


PainterMainWidget* PainterMainWidget::m_pInstance = nullptr;

PainterMainWidget *PainterMainWidget::GetInstance(QWidget *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new PainterMainWidget(parent);
    }
    return m_pInstance;
}

void PainterMainWidget::setCurrentShowWindow(qint32 index)
{
    ptrTabWidget->setCurrentIndex(index);
}

void PainterMainWidget::setCurrentShowWindow(HGraphicsView *pwid)
{
    ptrTabWidget->setCurrentWidget(pwid);
}

HGraphicsView *PainterMainWidget::getItemView(qint32 index) const
{
    return ptrTabWidget->widget(index)->findChild<HGraphicsView *>("View");
}

void PainterMainWidget::handlerDocumentChanged(qint32 index)
{
    activeView=ptrTabWidget->widget(index)->findChild<HGraphicsView *>("View");
    emit activeWindowChanged(index);
    // emit ItemHasSelected(tabList.at(index)->getSelectedItems(),index);
}

void PainterMainWidget::handlerDocumentClosed(qint32 index)
{
    ptrStatusBar->clearMessage();
    emit windowClosed(index);
}

HGraphicsView * PainterMainWidget::newShowWindow(QString name)
{
    QWidget *wid=new QWidget(this);
    QHBoxLayout *horizontallyLayout=new QHBoxLayout(wid);
    horizontallyLayout->setMargin(0);
    horizontallyLayout->setContentsMargins(0,0,0,0);
    activeView=new HGraphicsView(wid);
    activeView->setObjectName("View");
    HPainterToolBar *ptrToolBar=new HPainterToolBar(activeView,activeView->getScene()->getDrawCacheItem(),wid);
    toolBar<<ptrToolBar;

    horizontallyLayout->addWidget(ptrToolBar);
    horizontallyLayout->addWidget(activeView);


    connect(activeView,&HGraphicsView::selectionChanged,this,[=](){
        //QPointF pos=activeView->getScene()->getMousePos();
        qint32 num=activeView->getScene()->getSelectedItems().size();
        //ptrStatusBar->showMessage(QString("X= %1   Y=%2    selected items number:%3").arg(pos.x()).arg(pos.y()).arg(num));
    });

    connect(activeView,&HGraphicsView::mouseMoved,this,[=](const QPointF&pos){
        qint32 num=activeView->getScene()->getSelectedItems().size();
        ptrStatusBar->showMessage(QString("X= %1   Y=%2    selected items number:%3").arg(pos.x()).arg(pos.y()).arg(num));
    });



    ptrTabWidget->addTab(wid,name);
    ptrTabWidget->setCurrentWidget(wid);

    return activeView;

}

void PainterMainWidget::closeShowWindow(HGraphicsView *pwid)
{
    ptrStatusBar->clearMessage();
    qint32 index=ptrTabWidget->indexOf((QWidget*)pwid->parent());
    if(index<0)
        return;
    delete toolBar.at(index);
    toolBar.removeAt(index);
    auto dfdf= ptrTabWidget->widget(index);
    ptrTabWidget->removeTab(index);
    delete dfdf;

}

void PainterMainWidget::closeShowWindow(int index)
{
    ptrStatusBar->clearMessage();
    delete toolBar.at(index);
    toolBar.removeAt(index);

    auto dfdf=ptrTabWidget->widget(index);

    ptrTabWidget->removeTab(index);

    delete dfdf;
}

void PainterMainWidget::refreshShowWindow(int index)
{
    ptrTabWidget->widget(index)->update();
}

void PainterMainWidget::resetShowWindow(int index)
{
    ptrTabWidget->widget(index)->findChild<HGraphicsView *>("View")->resetView();
}


//void PainterMainWidget::resizeEvent(QResizeEvent *event)
//{
//   Q_UNUSED(event);
//   ptrTabWidget->setGeometry(rect());
//}

PainterMainWidget::PainterMainWidget(QWidget *parent): QWidget(parent)
{
    ptrTabWidget = new QTabWidget(this);
    ptrTabWidget->setTabsClosable(true);
    ptrTabWidget->tabBar()->setDocumentMode(true);
    ptrTabWidget->setStyleSheet(HSheetStyle::H_TABWIDGET2);


    QHBoxLayout *horizontallyLayout=new QHBoxLayout(this);
    horizontallyLayout->setMargin(0);
    horizontallyLayout->setContentsMargins(0,0,0,0);



    QVBoxLayout *vLayout=new QVBoxLayout;
    vLayout->setMargin(0);

    vLayout->setContentsMargins(0,0,0,0);
    vLayout->addWidget(ptrTabWidget);
    ptrStatusBar=new QStatusBar(this);
    ptrStatusBar->setStyleSheet("color: darkcyan;background-color: rgb(200, 200, 200);");
    vLayout->addWidget(ptrStatusBar);

    horizontallyLayout->addLayout(vLayout);


    connect(ptrTabWidget,&QTabWidget::currentChanged,this,&PainterMainWidget::handlerDocumentChanged);

    connect(ptrTabWidget,&QTabWidget::tabCloseRequested,this,&PainterMainWidget::handlerDocumentClosed);

}

PainterMainWidget::~PainterMainWidget()
{

}
