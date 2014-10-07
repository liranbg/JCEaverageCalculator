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
    void showGraph(GradePage *gpPTR);
    ~gradegraph();

private slots:
    void on_pushButton_clicked();

private:

    GradePage *gp;

    void setVisualization();
    void setGraphsData();
    Ui::gradegraph *ui;
};

#endif // GRADEGRAPH_H
