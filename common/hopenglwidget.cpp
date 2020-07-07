#include "hopenglwidget.h"
#include <QPainter>

HOpenGLWidget::HOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{


    m_tile = QPixmap(128, 128);
    m_tile.fill(Qt::white);
    QPainter pt(&m_tile);
    QColor color1(33, 40, 48);
    QColor color2(50, 50, 50);
    pt.fillRect(0, 0, 64, 64, color1);
    pt.fillRect(0, 64, 64, 64, color2);
    pt.fillRect(64, 0, 64, 64, color2);
    pt.fillRect(64, 64, 64, 64, color1);
    pt.end();

    //setAutoFillBackground(true);
}



void HOpenGLWidget::paint(QPainter *painter)
{
    Q_UNUSED(painter);
}

void HOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glShadeModel(GL_SMOOTH);
    glClearColor(0,0,0,0);
    glClearDepth(1.0);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

}

void HOpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width/2,width/2,-height/2,height/2,-1,1);
    glMatrixMode(GL_MODELVIEW);

}

void HOpenGLWidget::paintGL()
{
    //glDisable(GL_DEPTH_TEST);
    makeCurrent();
    //paintGL();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QPainter painter(this);

    painter.drawTiledPixmap(rect(), m_tile);
    paint(&painter);
    //glEnable(GL_DEPTH_TEST);

    //update();
}

void HOpenGLWidget::paintEvent(QPaintEvent *)
{
    paintGL();
}

//void HOpenGLWidget::resizeEvent(QResizeEvent *event)
//{
//    QSize size=event->size();
//    glViewport(0,0,size.width(),size.height());
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(-size.width()/2,size.width()/2,-size.height()/2,size.height()/2,-1,1);
//    glMatrixMode(GL_MODELVIEW);
//}
