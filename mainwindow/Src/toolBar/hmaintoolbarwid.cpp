#include "hmaintoolbarwid.h"
#include "ui_hmaintoolbarwid.h"

HMainToolBarWid::HMainToolBarWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HMainToolBarWid)
{
    ui->setupUi(this);
}

HMainToolBarWid::~HMainToolBarWid()
{
    delete ui;
}
