#include "gradegraph.h"
#include "ui_gradegraph.h"
gradegraph::gradegraph(QWidget *parent) : QDialog(parent), ui(new Ui::gradegraph)
{
  ui->setupUi(this);
  this->setModal(true); //makes it on top of application
  this->gp = NULL;
  tableWidget = new QCustomPlot(this);
  ui->verticalLayoutTable->addWidget(tableWidget);

}

void gradegraph::showGraph(GradePage *gpPTR)
{
  qDebug() << Q_FUNC_INFO;
  this->gp = gpPTR;

  if (tableWidget->graphCount() > 0)
    clearGraph();

  setVisualization();
  setGraphsData();


  tableWidget->replot();

  this->show();
}

gradegraph::~gradegraph()
{
  delete ui;
}

void gradegraph::setGraphsData()
{
  int minYearInList = gp->getMinYearInList()-1;
  int maxYearInList = gp->getMaxYearInList()+1;
  int xRangeForYear = (maxYearInList - minYearInList+2)*3;
  QVector<double> SemesterialAvg(xRangeForYear),yearlyAvg(xRangeForYear),indicatorX(xRangeForYear);

  for (int yearCount=0,i=1; i<xRangeForYear; ++i) //filling data
    {

      indicatorX[i] = i;
      if (i%4==1) //years
        {
          QCPItemText *textLabel = new QCPItemText(tableWidget);
          tableWidget->addItem(textLabel);

          double lastAvg = gp->getAvg(minYearInList+yearCount);

          if (lastAvg > 0)
            {
              // add the text label at the top:
              if (lastAvg <= MIN_GRADE) //will be below the graph
                {
                  textLabel->position->setCoords(i, MIN_GRADE+5); // place position at center/top of axis rect
                  textLabel->setText(QString::number(lastAvg,'g',4) + "<= Min Grade (" + QString::number(MIN_GRADE) + ")");
                }
              else
                {
                  textLabel->position->setCoords(i, lastAvg+1.5); // place position at center/top of axis rect
                  textLabel->setText(QString::number(lastAvg,'g',4));
                }
            }
          textLabel->setFont(QFont(font().family(), 8)); // make font a bit larger
          textLabel->setPen(QPen(Qt::black)); // show black border around text

          yearlyAvg[i] = lastAvg;
          yearCount++;

        }
      else
        {
          if (i+4 < xRangeForYear) //semesters
            {
              QCPItemText *textLabel = new QCPItemText(tableWidget);
              tableWidget->addItem(textLabel);

              //qDebug() << "year: " << minYearInList+yearCount << "sem " << (i-1)%4;
              double avg = gp->getAvg(minYearInList+yearCount,(i-1)%4);
              //qDebug() << "avg: " << avg;

              if (avg > 0)
                {
                  // add the text label at the top:
                  if (avg <= MIN_GRADE) //will be below the graph
                    {
                      textLabel->position->setCoords(i+4, MIN_GRADE+1.5); // place position at center/top of axis rect
                      textLabel->setText(QString::number(avg,'g',4) + "<= Min Grade (" + QString::number(MIN_GRADE) + ")");
                    }
                  else
                    {
                      textLabel->position->setCoords(i+4, avg+1.5); // place position at center/top of axis rect
                      textLabel->setText(QString::number(avg,'g',4));
                    }

                  textLabel->setFont(QFont(font().family(), 8)); // make font a bit larger
                  textLabel->setPen(QPen(Qt::black)); // show black border around text

                  SemesterialAvg[i+4] = avg;
                }

            }

          yearlyAvg[i] = 0;
        }
    }
  //yearly
  tableWidget->graph(0)->setData(indicatorX,yearlyAvg);
  //yearly
  tableWidget->graph(1)->setData(indicatorX,SemesterialAvg);
}

/**
 * @brief gradegraph::setvisualization set graph borders text, range and looking
 */
void gradegraph::setVisualization()
{
  tableWidget->setBackground(QBrush(Qt::white));
  tableWidget->axisRect()->setupFullAxesBox(true); //make the graph looks like a box

  tableWidget->addGraph(); //yearly and semesterial graphs
  tableWidget->addGraph();

  tableWidget->graph(0)->setName(tr("Yearly Average"));
  tableWidget->graph(0)->setPen(QPen(Qt::GlobalColor::blue));
  tableWidget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue, 7));
  tableWidget->graph(0)->setLineStyle(QCPGraph::lsImpulse );

  tableWidget->graph(1)->setName(tr("Semesterial Average"));
  tableWidget->graph(1)->setLineStyle(QCPGraph::lsStepLeft);
  tableWidget->graph(1)->setPen(QPen( QColor(Qt::GlobalColor::red)));
  tableWidget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::red, 7));

  int minYearInList = gp->getMinYearInList()-1;
  int maxYearInList = gp->getMaxYearInList()+1;
  int xRangeForYear = (maxYearInList - minYearInList+2)*3;

  QVector<QString> xStrings(0);
  for (int year=minYearInList,i = 0; i < xRangeForYear-1; ++i)
    {
      //set year x axe label to be yyyy A B C yyyy+1 A B C yyyy+2....
      int semesterChar = i%4;
      QString tempString;
      switch (semesterChar)
        {
        case 1:
          tempString =  tr("A");
          xStrings << tempString;
          break;
        case 2:
          tempString =  tr("B");
          xStrings << tempString;
          break;
        case 3:
          tempString =  tr("C");
          xStrings << tempString;
          break;
        case 0:
          tempString = QString::number(year);
          xStrings << tempString;
          year++;
          break;

        }
    }
  tableWidget->yAxis->setLabel(tr("AVG Grade"));
  tableWidget->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
  tableWidget->yAxis->setRange(MIN_GRADE,MAX_GRADE);
  tableWidget->yAxis->setTickStep(2);
  tableWidget->yAxis->setAutoSubTicks(false);
  tableWidget->yAxis->setAutoTickStep(false);
  tableWidget->yAxis->setSubTickCount(5);


  tableWidget->xAxis->setLabel(tr("Years"));
  tableWidget->xAxis->setAutoTickLabels(false);
  tableWidget->xAxis->setTickLabelFont(QFont(QFont().family(), 7));
  tableWidget->xAxis->setAutoTickStep(false);
  tableWidget->xAxis->setTickStep(1);
  tableWidget->xAxis->setAutoSubTicks(false);
  tableWidget->xAxis->setSubTickCount(0);
  tableWidget->xAxis->setTickVectorLabels(xStrings);
  tableWidget->xAxis->setRange(1,xRangeForYear);

  tableWidget->legend->setVisible(true); //show graph name on top right
}
void gradegraph::clearGraph()
{
  int itemDeleted,graphs,plots;
  itemDeleted = tableWidget->clearItems();
  graphs = tableWidget->clearGraphs();
  plots = tableWidget->clearPlottables();
  qDebug() << Q_FUNC_INFO << "items:" << itemDeleted << "graphs" << graphs << "plots:" << plots;
}
void gradegraph::on_pushButton_clicked()
{
  qDebug() << Q_FUNC_INFO << " Closed Graph Dialog";
  this->done(0);
}
