#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "colorswatch.h"
#include "toolbar.h"



QT_FORWARD_DECLARE_CLASS(QMenu)

QT_FORWARD_DECLARE_CLASS(QSystemTrayIcon)


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);


    void setupToolBar();
    void setupMenuBar();
    void setupDockWidgets();

public slots:

    //user
    void openUserDialog();

    void changeStatus();


    void saveSettings();
    void saveSettingsAsFile();
    void loadSettings();
    void resetSettings();
    void loadSettingsFromFile();

protected:

    void closeEvent(QCloseEvent *event);

private:

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;    //窗口管理动作
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;


    QAction* addCornerAction(const QString &text, QMainWindow *mw, QMenu *menu, QActionGroup *group,
                                    Qt::Corner c, Qt::DockWidgetArea a);
    void setDockOptions();



    QList<ToolBar*> toolBars;
    QVector<ColorSwatch *> swatchArray;

    QVector<QAction*> ptrActions_view;

    QMenu *mainWindowMenu;


};

#endif // MAINWINDOW_H
