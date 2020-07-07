#ifndef HDEVICEWIDGET_H
#define HDEVICEWIDGET_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QTableView;
QT_END_NAMESPACE

class HDeviceWidget : public QDialog
{
    Q_OBJECT
public:
    explicit HDeviceWidget(QWidget *parent = nullptr, Qt::WindowFlags f =Qt::WindowCloseButtonHint|Qt::WindowMinMaxButtonsHint);
    ~HDeviceWidget() Q_DECL_OVERRIDE;
protected:
    //void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
private:
    QTableView *ptrTableView;


signals:

public slots:
};

#endif // HDEVICEWIDGET_H
