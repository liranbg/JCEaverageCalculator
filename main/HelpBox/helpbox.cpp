#include "helpbox.h"
#include "ui_helpbox.h"

helpBox::helpBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helpBox)
{
    ui->setupUi(this);
}

helpBox::~helpBox()
{
    delete ui;
}
