
#include <QAction>
#include <QtEvents>
#include <QFrame>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QLabel>
#include <QPainterPath>
#include <QPushButton>
#include <QHBoxLayout>
#include <QBitmap>
#include <QDebug>
#include <QUrl>
#include <QMetaObject>

#include "hcommon.h"
#include "colorswatch.h"




ColorSwatch::ColorSwatch(const QString &colorName,QWidget*widget, QMainWindow *parent, Qt::WindowFlags flags)
    : QDockWidget(parent, flags), mainWindow(parent)
{
    setObjectName(colorName /*+ QLatin1String(" Dock Widget")*/);
    setWindowTitle(objectName());
    setWidget(widget);


    BlueTitleBar *titlebar = new BlueTitleBar(colorName,this);
    setTitleBarWidget(titlebar);
    connect(this, &QDockWidget::topLevelChanged, titlebar, &BlueTitleBar::updateMask);
    connect(this, &QDockWidget::featuresChanged, titlebar, &BlueTitleBar::updateMask, Qt::QueuedConnection);


    closableAction = new QAction(tr("Closable"), this);
    closableAction->setCheckable(true);
    connect(closableAction, &QAction::triggered, this, &ColorSwatch::changeClosable);

    movableAction = new QAction(tr("Movable"), this);
    movableAction->setCheckable(true);
    connect(movableAction, &QAction::triggered, this, &ColorSwatch::changeMovable);

    floatableAction = new QAction(tr("Floatable"), this);
    floatableAction->setCheckable(true);
    connect(floatableAction, &QAction::triggered, this, &ColorSwatch::changeFloatable);

    verticalTitleBarAction = new QAction(tr("Vertical title bar"), this);
    verticalTitleBarAction->setCheckable(true);
    connect(verticalTitleBarAction, &QAction::triggered,
            this, &ColorSwatch::changeVerticalTitleBar);

    floatingAction = new QAction(tr("Floating"), this);
    floatingAction->setCheckable(true);
    connect(floatingAction, &QAction::triggered, this, &ColorSwatch::changeFloating);

    allowedAreasActions = new QActionGroup(this);
    allowedAreasActions->setExclusive(false);

    allowLeftAction = new QAction(tr("Allow on Left"), this);
    allowLeftAction->setCheckable(true);
    connect(allowLeftAction, &QAction::triggered, this, &ColorSwatch::allowLeft);

    allowRightAction = new QAction(tr("Allow on Right"), this);
    allowRightAction->setCheckable(true);
    connect(allowRightAction, &QAction::triggered, this, &ColorSwatch::allowRight);

    allowTopAction = new QAction(tr("Allow on Top"), this);
    allowTopAction->setCheckable(true);
    connect(allowTopAction, &QAction::triggered, this, &ColorSwatch::allowTop);

    allowBottomAction = new QAction(tr("Allow on Bottom"), this);
    allowBottomAction->setCheckable(true);
    connect(allowBottomAction, &QAction::triggered, this, &ColorSwatch::allowBottom);

    allowedAreasActions->addAction(allowLeftAction);
    allowedAreasActions->addAction(allowRightAction);
    allowedAreasActions->addAction(allowTopAction);
    allowedAreasActions->addAction(allowBottomAction);

    areaActions = new QActionGroup(this);
    areaActions->setExclusive(true);

    leftAction = new QAction(tr("Place on Left") , this);
    leftAction->setCheckable(true);
    connect(leftAction, &QAction::triggered, this, &ColorSwatch::placeLeft);

    rightAction = new QAction(tr("Place on Right") , this);
    rightAction->setCheckable(true);
    connect(rightAction, &QAction::triggered, this, &ColorSwatch::placeRight);

    topAction = new QAction(tr("Place on Top") , this);
    topAction->setCheckable(true);
    connect(topAction, &QAction::triggered, this, &ColorSwatch::placeTop);

    bottomAction = new QAction(tr("Place on Bottom") , this);
    bottomAction->setCheckable(true);
    connect(bottomAction, &QAction::triggered, this, &ColorSwatch::placeBottom);

    areaActions->addAction(leftAction);
    areaActions->addAction(rightAction);
    areaActions->addAction(topAction);
    areaActions->addAction(bottomAction);

    connect(movableAction, &QAction::triggered, areaActions, &QActionGroup::setEnabled);

    connect(movableAction, &QAction::triggered, allowedAreasActions, &QActionGroup::setEnabled);

    connect(floatableAction, &QAction::triggered, floatingAction, &QAction::setEnabled);

    connect(floatingAction, &QAction::triggered, floatableAction, &QAction::setDisabled);
    connect(movableAction, &QAction::triggered, floatableAction, &QAction::setEnabled);

    tabMenu = new QMenu(this);
    tabMenu->setStyleSheet(HSheetStyle::H_MENU);
    tabMenu->setTitle(tr("Tab into"));
    connect(tabMenu, &QMenu::triggered, this, &ColorSwatch::tabInto);

    splitHMenu = new QMenu(this);
    splitHMenu->setStyleSheet(HSheetStyle::H_MENU);
    splitHMenu->setTitle(tr("Split horizontally into"));
    connect(splitHMenu, &QMenu::triggered, this, &ColorSwatch::splitInto);

    splitVMenu = new QMenu(this);
    splitVMenu->setStyleSheet(HSheetStyle::H_MENU);
    splitVMenu->setTitle(tr("Split vertically into"));
    connect(splitVMenu, &QMenu::triggered, this, &ColorSwatch::splitInto);

    QAction *windowModifiedAction = new QAction(tr("Modified"), this);
    windowModifiedAction->setCheckable(true);
    windowModifiedAction->setChecked(false);
    connect(windowModifiedAction, &QAction::toggled, this, &QWidget::setWindowModified);

    menu = new QMenu(colorName, this);
    menu->addAction(toggleViewAction());
    menu->addAction(tr("Raise"), this, &QWidget::raise);

    //menu->addAction(tr("Change Size Hints..."), swatch, &ColorDock::changeSizeHints);


    menu->addSeparator();
    menu->addAction(closableAction);
    menu->addAction(movableAction);
    menu->addAction(floatableAction);
    menu->addAction(floatingAction);
    menu->addAction(verticalTitleBarAction);
    menu->addSeparator();
    menu->addActions(allowedAreasActions->actions());
    menu->addSeparator();
    menu->addActions(areaActions->actions());
    menu->addSeparator();
    menu->addMenu(splitHMenu);
    menu->addMenu(splitVMenu);
    menu->addMenu(tabMenu);
    menu->addSeparator();
    menu->addAction(windowModifiedAction);

    connect(menu, &QMenu::aboutToShow, this, &ColorSwatch::updateContextMenu);

    if (colorName == QLatin1String("Code Editor")) {
        leftAction->setShortcut(Qt::CTRL | Qt::Key_W);
        rightAction->setShortcut(Qt::CTRL | Qt::Key_E);
        toggleViewAction()->setShortcut(Qt::CTRL | Qt::Key_R);
    }
}

void ColorSwatch::updateContextMenu()
{
    const Qt::DockWidgetArea area = mainWindow->dockWidgetArea(this);
    const Qt::DockWidgetAreas areas = allowedAreas();

    closableAction->setChecked(features() & QDockWidget::DockWidgetClosable);
    if (windowType() == Qt::Drawer) {
        floatableAction->setEnabled(false);
        floatingAction->setEnabled(false);
        movableAction->setEnabled(false);
        verticalTitleBarAction->setChecked(false);
    } else {
        floatableAction->setChecked(features() & QDockWidget::DockWidgetFloatable);
        floatingAction->setChecked(isWindow());
        // done after floating, to get 'floatable' correctly initialized
        movableAction->setChecked(features() & QDockWidget::DockWidgetMovable);
        verticalTitleBarAction
                ->setChecked(features() & QDockWidget::DockWidgetVerticalTitleBar);
    }

    allowLeftAction->setChecked(isAreaAllowed(Qt::LeftDockWidgetArea));
    allowRightAction->setChecked(isAreaAllowed(Qt::RightDockWidgetArea));
    allowTopAction->setChecked(isAreaAllowed(Qt::TopDockWidgetArea));
    allowBottomAction->setChecked(isAreaAllowed(Qt::BottomDockWidgetArea));

    if (allowedAreasActions->isEnabled()) {
        allowLeftAction->setEnabled(area != Qt::LeftDockWidgetArea);
        allowRightAction->setEnabled(area != Qt::RightDockWidgetArea);
        allowTopAction->setEnabled(area != Qt::TopDockWidgetArea);
        allowBottomAction->setEnabled(area != Qt::BottomDockWidgetArea);
    }

    {
        const QSignalBlocker blocker(leftAction);
        leftAction->setChecked(area == Qt::LeftDockWidgetArea);
    }
    {
        const QSignalBlocker blocker(rightAction);
        rightAction->setChecked(area == Qt::RightDockWidgetArea);
    }
    {
        const QSignalBlocker blocker(topAction);
        topAction->setChecked(area == Qt::TopDockWidgetArea);
    }
    {
        const QSignalBlocker blocker(bottomAction);
        bottomAction->setChecked(area == Qt::BottomDockWidgetArea);
    }

    if (areaActions->isEnabled()) {
        leftAction->setEnabled(areas & Qt::LeftDockWidgetArea);
        rightAction->setEnabled(areas & Qt::RightDockWidgetArea);
        topAction->setEnabled(areas & Qt::TopDockWidgetArea);
        bottomAction->setEnabled(areas & Qt::BottomDockWidgetArea);
    }

    tabMenu->clear();
    splitHMenu->clear();
    splitVMenu->clear();
    QList<ColorSwatch*> dock_list = mainWindow->findChildren<ColorSwatch*>();
    foreach (ColorSwatch *dock, dock_list) {
        tabMenu->addAction(dock->objectName());
        splitHMenu->addAction(dock->objectName());
        splitVMenu->addAction(dock->objectName());
    }
}

static ColorSwatch *findByName(const QMainWindow *mainWindow, const QString &name)
{
    foreach (ColorSwatch *dock, mainWindow->findChildren<ColorSwatch*>()) {
        if (name == dock->objectName())
            return dock;
    }
    return nullptr;
}

void ColorSwatch::splitInto(QAction *action)
{
    ColorSwatch *target = findByName(mainWindow, action->text());
    if (!target)
        return;

    const Qt::Orientation o = action->parent() == splitHMenu
            ? Qt::Horizontal : Qt::Vertical;
    mainWindow->splitDockWidget(target, this, o);
}

void ColorSwatch::tabInto(QAction *action)
{
    if (ColorSwatch *target = findByName(mainWindow, action->text()))
        mainWindow->tabifyDockWidget(target, this);
}

#ifndef QT_NO_CONTEXTMENU
void ColorSwatch::contextMenuEvent(QContextMenuEvent *event)
{
    event->accept();
    //for rightbutton menu
    //    menu->exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU

void ColorSwatch::resizeEvent(QResizeEvent *e)
{
    if (BlueTitleBar *btb = qobject_cast<BlueTitleBar*>(titleBarWidget()))
        btb->updateMask();

    QDockWidget::resizeEvent(e);

}

void ColorSwatch::allow(Qt::DockWidgetArea area, bool a)
{
    Qt::DockWidgetAreas areas = allowedAreas();
    areas = a ? areas | area : areas & ~area;
    setAllowedAreas(areas);

    if (areaActions->isEnabled()) {
        leftAction->setEnabled(areas & Qt::LeftDockWidgetArea);
        rightAction->setEnabled(areas & Qt::RightDockWidgetArea);
        topAction->setEnabled(areas & Qt::TopDockWidgetArea);
        bottomAction->setEnabled(areas & Qt::BottomDockWidgetArea);
    }
}

void ColorSwatch::place(Qt::DockWidgetArea area, bool p)
{
    if (!p)
        return;

    mainWindow->addDockWidget(area, this);

    if (allowedAreasActions->isEnabled()) {
        allowLeftAction->setEnabled(area != Qt::LeftDockWidgetArea);
        allowRightAction->setEnabled(area != Qt::RightDockWidgetArea);
        allowTopAction->setEnabled(area != Qt::TopDockWidgetArea);
        allowBottomAction->setEnabled(area != Qt::BottomDockWidgetArea);
    }
}



void ColorSwatch::changeClosable(bool on)
{ setFeatures(on ? features() | DockWidgetClosable : features() & ~DockWidgetClosable); }

void ColorSwatch::changeMovable(bool on)
{ setFeatures(on ? features() | DockWidgetMovable : features() & ~DockWidgetMovable); }

void ColorSwatch::changeFloatable(bool on)
{ setFeatures(on ? features() | DockWidgetFloatable : features() & ~DockWidgetFloatable); }

void ColorSwatch::changeFloating(bool floating)
{ setFloating(floating); }

void ColorSwatch::allowLeft(bool a)
{ allow(Qt::LeftDockWidgetArea, a); }

void ColorSwatch::allowRight(bool a)
{ allow(Qt::RightDockWidgetArea, a); }

void ColorSwatch::allowTop(bool a)
{ allow(Qt::TopDockWidgetArea, a); }

void ColorSwatch::allowBottom(bool a)
{ allow(Qt::BottomDockWidgetArea, a); }

void ColorSwatch::placeLeft(bool p)
{ place(Qt::LeftDockWidgetArea, p); }

void ColorSwatch::placeRight(bool p)
{ place(Qt::RightDockWidgetArea, p); }

void ColorSwatch::placeTop(bool p)
{ place(Qt::TopDockWidgetArea, p); }

void ColorSwatch::placeBottom(bool p)
{ place(Qt::BottomDockWidgetArea, p); }

void ColorSwatch::changeVerticalTitleBar(bool on)
{
    setFeatures(on ? features() | DockWidgetVerticalTitleBar
                   : features() & ~DockWidgetVerticalTitleBar);
}


QSize BlueTitleBar::minimumSizeHint() const
{
    QDockWidget *dw = qobject_cast<QDockWidget*>(parentWidget());
    Q_ASSERT(dw != nullptr);
    QSize result(leftPm.width() + rightPm.width(), centerPm.height());
    if (dw->features() & QDockWidget::DockWidgetVerticalTitleBar)
        result.transpose();
    return result;
}

BlueTitleBar::BlueTitleBar(const QString &title, QWidget *parent)
    : QWidget(parent)
    , leftPm(QPixmap())
    , centerPm(QPixmap(":/resource/img/titlebarCenter.png"))
    , rightPm(QPixmap(":/resource/img/titlebarRight.gif"))
{
    titleText=title;

}

void BlueTitleBar::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QRect rect = this->rect();

    QDockWidget *dw = qobject_cast<QDockWidget*>(parentWidget());
    Q_ASSERT(dw != nullptr);



    if (dw->features() & QDockWidget::DockWidgetVerticalTitleBar) {
        QSize s = rect.size();
        s.transpose();
        rect.setSize(s);

        painter.translate(rect.left(), rect.top() + rect.width());
        painter.rotate(-90);
        painter.translate(-rect.left(), -rect.top());
    }

    QRect textRect(rect.topLeft().x(),rect.topLeft().y(),200,24);

    painter.drawPixmap(rect.topRight() - QPoint(rightPm.width() - 1, 0), rightPm);

    QBrush brush(centerPm);
    painter.fillRect(rect.left() + leftPm.width(), rect.top(),
                     rect.width() - leftPm.width() - rightPm.width(),
                     centerPm.height(), centerPm);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Microsoft YaHei UI",10));
    painter.drawText(textRect, Qt::AlignLeft,"  "+titleText);

}

void BlueTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();

    QRect rect = this->rect();

    QDockWidget *dw = qobject_cast<QDockWidget*>(parentWidget());
    Q_ASSERT(dw != nullptr);

    if (dw->features() & QDockWidget::DockWidgetVerticalTitleBar) {
        QPoint p = pos;
        pos.setX(rect.left() + rect.bottom() - p.y());
        pos.setY(rect.top() + p.x() - rect.left());

        QSize s = rect.size();
        s.transpose();
        rect.setSize(s);
    }

    const int buttonRight = 8;
    const int buttonWidth = 23;
    int right = rect.right() - pos.x();
    int button = (right - buttonRight)/buttonWidth;
    switch (button) {
        case 0:
            event->accept();
            dw->close();
            break;
        case 1:
            event->accept();
            dw->setFloating(!dw->isFloating());
            break;
        case 2: {
            event->accept();
            QDockWidget::DockWidgetFeatures features = dw->features();
            if (features & QDockWidget::DockWidgetVerticalTitleBar)
                features &= ~QDockWidget::DockWidgetVerticalTitleBar;
            else
                features |= QDockWidget::DockWidgetVerticalTitleBar;
            dw->setFeatures(features);
            break;
        }
        default:
            event->ignore();
            break;
    }
}

void BlueTitleBar::updateMask()
{
//    QDockWidget *dw = qobject_cast<QDockWidget*>(parent());
//    Q_ASSERT(dw != nullptr);

//    QRect rect = dw->rect();
//    QPixmap bitmap(dw->size());

//    {
//        QPainter painter(&bitmap);

//        // initialize to transparent
//        painter.fillRect(rect, Qt::color0);

//        QRect contents = rect;
//        contents.setTopLeft(geometry().bottomLeft());
//        contents.setRight(geometry().right());
//        contents.setBottom(contents.bottom()-y());
//        painter.fillRect(contents, Qt::color1);

//        // let's paint the titlebar
//        QRect titleRect = this->geometry();

//        if (dw->features() & QDockWidget::DockWidgetVerticalTitleBar) {
//            QSize s = rect.size();
//            s.transpose();
//            rect.setSize(s);

//            QSize s2 = size();
//            s2.transpose();
//            titleRect.setSize(s2);

//            painter.translate(rect.left(), rect.top() + rect.width());
//            painter.rotate(-90);
//            painter.translate(-rect.left(), -rect.top());
//        }

//        contents.setTopLeft(titleRect.bottomLeft());
//        contents.setRight(titleRect.right());
//        contents.setBottom(rect.bottom()-y());

//        QRect rect = titleRect;

//        painter.drawPixmap(rect.topLeft(), leftPm.mask());
//        painter.fillRect(rect.left() + leftPm.width(), rect.top(),
//            rect.width() - leftPm.width() - rightPm.width(),
//            centerPm.height(), Qt::color1);
//        painter.drawPixmap(rect.topRight() - QPoint(rightPm.width() - 1, 0), rightPm.mask());

//        painter.fillRect(contents, Qt::color1);
//    }

//    dw->setMask(bitmap);
}

//#include "colorswatch.moc"



