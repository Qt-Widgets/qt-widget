#include "hgraphicsview.h"


#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif
#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#else
#include <QtWidgets>
#endif
#include <qmath.h>

#include <QMouseEvent>
#include <QtConcurrent>

#include "hpaintertoolbar.h"

#include "hgraphicsscene.h"
#include "Src/data/inisettings.h"


HGraphicsView::HGraphicsView(QWidget *parent):QGraphicsView(parent)
{

    setFrameStyle(Sunken | StyledPanel);

    setWindowFlags(Qt::FramelessWindowHint);
    //    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if(IniSettings::GetInstance()->getValue(IniSettings::painter_Antialiasing).toBool()){
        setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    }else{
        setRenderHint(QPainter::Antialiasing, false);
    }

    //setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setRubberBandSelectionMode(Qt::ContainsItemShape);
    //setAlignment(Qt::AlignCenter);

    scene = new HGraphicsScene(-10000,-10000,20000,20000,this);
    setScene(scene);

    setupMatrix();
    togglePointerMode(dragMode);
    // connect(scene,&QGraphicsScene::selectionChanged,this,&HGraphicsView::handlerSelectionChanged);

    setMouseTracking(true);

    setCursor(Qt::BlankCursor);

    //toggleOpenGL(true);

}

HGraphicsView::~HGraphicsView()
{
    scene->clear();
    delete scene;
}

void HGraphicsView::setModifyMode(bool isModify)
{
    for(const auto&k:scene->getLayer()){
        for(const auto&m:k.getItems()){
            auto flags=isModify?(m->flags()|HGraphicsPathItem::ItemIsMovable):(m->flags()&(~HGraphicsPathItem::ItemIsMovable));
            m->setFlags(flags);
        }
    }
}

void HGraphicsView::setMousePressMode(qint32 mode)
{
    scene->m_mousePressMode=mode;
}

QPointF HGraphicsView::getCameraPos()
{
    return scene->cameraPosItem->getPos();
}

void HGraphicsView::resetView()
{
    setupMatrix();
    ensureVisible(QRectF(0, 0, 0, 0));
}


void HGraphicsView::setupMatrix()
{

    QMatrix matrix;
    matrix.scale(zoomValue,-zoomValue);
    matrix.rotate(rotateValue);

    setMatrix(matrix);
}

void HGraphicsView::togglePointerMode(qint32 mode)
{
    setDragMode(mode?QGraphicsView::NoDrag: QGraphicsView::ScrollHandDrag);
    setInteractive(mode);
}

void HGraphicsView::toggleOpenGL(bool useOpengl)
{
#ifndef QT_NO_OPENGL

    if(useOpengl){
        QGLFormat fmt;
        fmt.setSampleBuffers(true);
        fmt.setDepthBufferSize(64);
        fmt.setStencilBufferSize(128);
        fmt.setSamples(16);
        auto glw=new QGLWidget(fmt,this);
        setViewport(glw);
    }else{
        setViewport(new QWidget(this));
    }
#endif
}

void HGraphicsView::toggleAntialiasing(bool useAntia)
{
    setRenderHint(QPainter::Antialiasing, useAntia);
}

void HGraphicsView::print()
{
#if QT_CONFIG(printdialog)
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        render(&painter);
    }
#endif
}

void HGraphicsView::scrollContentsBy(int dx, int dy)
{
    QGraphicsView::scrollContentsBy(dx,dy);
    viewport()->update();
}



void HGraphicsView::mousePressEvent(QMouseEvent *event)
{

    QGraphicsView::mousePressEvent(event);

    if(event->button()==Qt::MouseButton::MidButton){
        dragMode=!dragMode;
        togglePointerMode(dragMode);
        return;
    }
    //viewport()->update();
}

void HGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);

}

void HGraphicsView::enterEvent(QEvent *event)
{
    scene->setMMouseVisible(true);
    QWidget::enterEvent(event);
}

void HGraphicsView::leaveEvent(QEvent *event)
{
    scene->setMMouseVisible(false);
    QWidget::leaveEvent(event);
}



#if QT_CONFIG(wheelevent)
void HGraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->delta() > 0)
        zoomValue*=1.1;
    else
        zoomValue*=0.9;
    e->accept();

    setupMatrix();

    //    if (e->modifiers() & Qt::ControlModifier) {
    //        if (e->delta() > 0)
    //            zoomValue*=1.1;
    //        else
    //            zoomValue*=0.9;
    //        e->accept();

    //        setupMatrix();
    //    } else {
    //        QGraphicsView::wheelEvent(e);
    //    }

}
#endif







