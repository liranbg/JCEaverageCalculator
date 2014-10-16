#ifndef GRADEGRAPH_H
#define GRADEGRAPH_H

#include "../gradePage.h"
#include "qcustomplot.h"
#include <QDialog>

namespace Ui {
class gradegraph;
}

#define MIN_GRADE 50
#define MAX_GRADE 100

class gradegraph : public QDialog
{
    Q_OBJECT

public:
    gradegraph(QWidget *parent = 0);
    void showGraph(GradePage *gpPTR);
    ~gradegraph();

private slots:
    void on_pushButton_clicked();

private:

    GradePage *gp;

    QCustomPlot *tableWidget;
    void setVisualization();
    void setGraphsData();
    void clearGraph();

    Ui::gradegraph *ui;
};

#endif // GRADEGRAPH_H
