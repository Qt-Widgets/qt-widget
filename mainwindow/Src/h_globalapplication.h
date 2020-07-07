#ifndef H_GLOBALAPPLICATION_H
#define H_GLOBALAPPLICATION_H

#include <QApplication>

class H_GlobalApplication : public QApplication
{
public:
     H_GlobalApplication(int&argc,char **argv);
     ~H_GlobalApplication();

     bool notify(QObject*, QEvent *);
     void setWindowInstance(QWidget*wnd);
private:
     QWidget *widget;
};


#endif // H_GLOBALAPPLICATION_H
