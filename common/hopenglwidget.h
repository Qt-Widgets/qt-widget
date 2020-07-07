#ifndef HOPENGLWIDGET_H
#define HOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <gl/GL.h>
#include <gl/GLU.h>


#include <stable.h>


class HOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    HOpenGLWidget(QWidget *parent=nullptr);


    virtual void paint(QPainter *painter);

protected:
     void initializeGL() override;
     void resizeGL(int width, int height) override;
     void paintGL() override;
     void paintEvent(QPaintEvent *) override;

    // void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

 private:
     QPixmap m_tile;
};

#endif // HOPENGLWIDGET_H
