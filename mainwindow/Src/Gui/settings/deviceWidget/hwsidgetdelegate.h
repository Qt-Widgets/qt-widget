#ifndef HWSIDGETDELEGATE_H
#define HWSIDGETDELEGATE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSlider;
QT_END_NAMESPACE


class HWsidgetDelegate : public QWidget
{
    Q_OBJECT
public:
    explicit HWsidgetDelegate(QWidget *parent = nullptr);
    QSlider *radioBtn;

protected:
     bool eventFilter(QObject *target, QEvent *event) Q_DECL_OVERRIDE;
private:


signals:

public slots:
};

#endif // HWSIDGETDELEGATE_H
