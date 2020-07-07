#ifndef HCOMMON_H
#define HCOMMON_H
#include <stable.h>
#include "hvariantlist.h"

namespace HSheetStyle {

static const QString H_MENU="background-color:rgb(180,180,180);selection-background-color:rgb(123, 123, 255);font-family:calibri;font-size:14px;border:0px;";
static const QString H_TABLEVIEW1="QTableView QTableCornerButton::section{\
                                   color: red;\
                                   background-color: rgb(64, 64, 64);\
                                   border: 5px solid #f6f7fa;\
                                   border-radius:0px;\
                                   border-color: rgb(64, 64, 64);}\
                                   QTableView {\
                                   color: rgb(64, 64, 64);\
                                   gridline-color: rgb(64, 64, 64);\
                                   background-color:Cornsilk;\
                                   alternate-background-color: LightBlue;\
                                   selection-color: Cornsilk;\
                                   selection-background-color: grey41;\
                                   border: 2px groove gray;\
                                   border-radius: 0px;\
                                   outline:none;\
                                   padding: 2px 4px;}\
                                   QHeaderView {\
                                   color: white;\
                                   font: bold 10pt;\
                                   background-color: rgb(64, 64, 64);\
                                   border: 0px solid rgb(64, 64, 64);\
                                   border:0px solid rgb(64, 64, 64);\
                                   border-left-color: rgba(255, 255, 255);\
                                   border-top-color: rgba(255, 255, 255);\
                                   border-radius:0px;\
                                   min-height:29px;}\
                                   QHeaderView::section {\
                                   color: white;\
                                   background-color: rgb(64, 64, 64);\
                                   border: 5px solid #f6f7fa;\
                                   border-radius:0px;\
                                   border-color: rgb(64, 64, 64);}";

static const QString H_LISTVIEW="QListView::item:selected{\
                                  color: red;\
                                  background-color: rgb(84, 139, 84);\
                                  border: 1px solid #f6f7fa;\
                                  border-radius:0px;\
                                  border-color: rgb(64, 64, 64);}\
                                  QListView {\
                                  color: rgb(64, 64, 64);\
                                  gridline-color: rgb(64, 64, 64);\
                                  background-color:Cornsilk;\
                                  alternate-background-color: PaleTurquoise;\
                                  selection-color: Cornsilk;\
                                  selection-background-color: grey41;\
                                  border: 2px groove gray;\
                                  border-radius: 0px;\
                                  outline:none;\
                                  padding: 2px 4px;}";
static const QString H_LISTVIEW1="QListView::item:selected{\
                                  color: black;\
                                  background-color: rgb(58 ,95 ,205);\
                                  border: 1px solid #f6f7fa;\
                                  border-radius:0px;\
                                  border-color: rgb(64, 64, 64);}\
                                  QListView {\
                                  color: rgb(64, 64, 64);\
                                  gridline-color: rgb(64, 64, 64);\
                                  background-color:Cornsilk;\
                                  alternate-background-color: PaleTurquoise;\
                                  selection-color: Cornsilk;\
                                  selection-background-color: grey41;\
                                  border: 2px groove gray;\
                                  border-radius: 0px;\
                                  outline:none;\
                                  padding: 2px 4px;}";

static const QString H_TABLEVIEW2="QTreeView{background-color: transparent;color:black;font:calibri;font-size:10pt;outline:none;}"
                                  "QTreeView::item:hover {background-color:skyblue;}"
                                  "QTreeView::item:selected {background-color:rgb(8,52,127);font:calibri;font-size:11pt;border:1px solid rgb(8,52,127);}\
                                   \
                                   QHeaderView{\
                                   color: white;\
                                   font: bold 10pt;\
                                   background-color: Aquamarine1;\
                                   border: 0px solid rgb(144, 144, 144);\
                                   border:0px solid rgb(191,191,191);\
                                   border-left-color: rgba(255, 255, 255,0);\
                                   border-top-color: rgba(255, 255, 255,0);\
                                   border-radius:0px;\
                                   min-height:29px;}\
                                   \
                                   QHeaderView::highlight {\
                                   color: skyblue;\
                                   font: bold 10pt;\
                                   background-color: Aquamarine1;\
                                   border: 0px solid rgb(144, 144, 144);\
                                   border:0px solid rgb(191,191,191);\
                                   border-left-color: rgba(255, 255, 255,0);\
                                   border-top-color: rgba(255, 255, 255,0);\
                                   border-radius:0px;\
                                   min-height:29px;}\
                                   \
                                   QHeaderView::section {\
                                   color: white;\
                                   background-color: rgb(64, 64, 64);\
                                   border: 5px solid #f6f7fa;\
                                   border-radius:0px;\
                                   border-color: rgb(64, 64, 64);}";


static const QString H_TABLEVIEW3=
        "QTableView QTableCornerButton::section {"
        "background: red;"
        "border: 2px outset red;"
        "color: red;"
        "background-color: rgb(64, 64, 64);"
        "border: 5px solid #f6f7fa;"
        "border-radius:0px;"
        " border-color: rgb(64, 64, 64);"
        "}"
        "QTableView {"
        "color: black;" /*表格内文字颜色*/
        "gridline-color: black; "/*表格内框颜色*/
        "background-color: rgb(200, 200, 200); "/*表格内背景色*/
        "alternate-background-color: rgb(180, 180, 180);"
        "selection-color: red; "                            /*选中区域的文字颜色*/
        "selection-background-color: lightgreen; "       /*选中区域的背景色*/
        "border: 2px groove gray;"
        "border-radius: 0px;"
        "padding: 2px 2px 2px 2px;"
        "outline:none;"
        " }"
        "QTableView::item:hover {background-color:skyblue;}"
        "QHeaderView {"
        "color: white;"
        "font: bold 10pt;"
        "background-color: rgb(108, 108, 108);"
        "border: 0px solid rgb(144, 144, 144);"
        "border:0px solid rgb(191,191,191);"
        "border-left-color: rgba(255, 255, 255,0);"
        "border-top-color: rgba(255, 255, 255,0);"
        "border-radius:0px;"
        "min-height:29px;"
        "}"
        "QHeaderView::section {"
        "color: white;"
        "background-color: rgb(64, 64, 64);"
        "border: 5px solid #f6f7fa;"
        "border-radius:0px;"
        "border-color: rgb(64, 64, 64);"
        "} ";


static const QString H_TABWIDGET1="QTabWidget::pane{border: 1px;background:white;font-family:Microsoft YaHei UI;font-size:10pt;}"
                                  "QTabWidget::tab-bar{alignment:left;background:red;font-family:Microsoft YaHei UI;font-size:10pt;}"
                                  "QTabBar::tab{background:transparent;color:black; min-width:30ex; min-height:10ex;font-family:Microsoft YaHei UI;font-size:10pt;}"
                                  "QTabBar::tab:hover{background:rgb(106, 181, 255);font-family:Microsoft YaHei UI;font-size:10pt;}"
                                  "QTabBar::tab:selected{border-color: white;background:skyblue;color:green;font-family:Microsoft YaHei UI;font-size:10pt;}";

static const QString H_TABWIDGET2="QTabWidget::pane{border: 1px;background:white;}"
                                  "QTabWidget::tab-bar{alignment:left;background:red;}"
                                  "QTabBar::tab{background:transparent;color:black; min-width:20ex; min-height:10ex;font-family:Microsoft YaHei UI;font-size:12px;}"
                                  "QTabBar::tab:hover{background:rgb(106, 181, 255);}"
                                  "QTabBar::tab:selected{border-color: white;background:skyblue;color:green;}";


static const QString H_MENU1="QMenu {background-color:rgb(89,87,87); \
                                   border: 3px solid rgb(235,110,36);}\
                                   QMenu::item {\
                                   font-size: 10pt;\
                                   color: rgb(225,225,225);\
                                   border: 3px solid rgb(60,60,60);\
                                   background-color:rgb(89,87,87);\
                                   padding:4px 30px;\
                                   margin:1px 1px;}\
                                   QMenu::item:selected { \
                                   background-color:rgb(235,110,36);}\
                                   QMenu::item:pressed {\
                                   border: 1px solid rgb(60,60,61); \
                                   background-color: rgb(220,80,6); }";




static const QString H_BUTTON1="QPushButton{font-family:Calibri;font-size:20px;font-weight:10;border-radius:3px; background:rgb(110, 190, 10); color:black;}"
                               "QPushButton:hover{font-family:Calibri;font-size:25px;background:rgb(106, 181, 255);border-radius:2px; border:2px solid rgb(100, 100, 100);}"
                               "QPushButton:pressed{font-family:Calibri;font-size:25px;background:rgb(100, 180, 255);border-radius:2px; border:2px solid rgb(210, 225, 230);}";
static const QString H_BUTTON2="QPushButton{font-family:Calibri;font-size:25px;border-radius:5px; background:rgb(110, 190, 10); color:red;}"
                               "QPushButton:hover{font-family:Calibri;font-size:25px;background:rgb(106, 181, 255);border-radius:5px; border:1px solid rgb(210, 225, 230);}";


}


#endif // HCOMMON_H
