#ifndef GRADEGRAPH_H
#define GRADEGRAPH_H

#include "./src/jceData/Grades/gradePage.h"
#include "qcustomplot.h"
#include <QDialog>

namespace Ui {
class gradegraph;
}

class gradegraph : public QDialog
{
    Q_OBJECT

public:
    gradegraph(QWidget *parent = 0, GradePage *gpPTR = 0);
    ~gradegraph();

private:

    GradePage *gp;
    Ui::gradegraph *ui;
};

#endif // GRADEGRAPH_H
