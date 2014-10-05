#include "gradegraph.h"
#include "ui_gradegraph.h"

gradegraph::gradegraph(QWidget *parent, GradePage *gpPTR) :
    QDialog(parent),
    ui(new Ui::gradegraph)
{
    ui->setupUi(this);
    this->gp = gpPTR;
}

gradegraph::~gradegraph()
{
    delete ui;
}
