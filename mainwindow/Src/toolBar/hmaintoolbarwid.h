#ifndef HMAINTOOLBARWID_H
#define HMAINTOOLBARWID_H

#include <QWidget>

namespace Ui {
class HMainToolBarWid;
}

class HMainToolBarWid : public QWidget
{
    Q_OBJECT

public:
    explicit HMainToolBarWid(QWidget *parent = nullptr);
    ~HMainToolBarWid();

private:
    Ui::HMainToolBarWid *ui;
};

#endif // HMAINTOOLBARWID_H
