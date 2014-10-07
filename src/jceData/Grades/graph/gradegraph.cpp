#include "gradegraph.h"
#include "ui_gradegraph.h"

gradegraph::gradegraph(QWidget *parent, GradePage *gpPTR) :
  QDialog(parent),
  ui(new Ui::gradegraph)
{
  ui->setupUi(this);
  this->gp = gpPTR;

}

void gradegraph::showGraph(GradePage *gpPTR)
{
  this->gp = gpPTR;
  setVisualization();
  setGraphsData();
  this->show();
}

gradegraph::~gradegraph()
{
  delete ui;
}

void gradegraph::setGraphsData()
{
  int minYearInList = gp->getMinYearInList(); //2012
  int maxYearInList = gp->getMaxYearInList()+1; //2016
  int xRangeForYear = (maxYearInList - minYearInList+2)*3; //6*3=18
  QVector<double> SemesterialAvg(xRangeForYear),yearlyAvg(xRangeForYear),sem(xRangeForYear);

  for (int yearCount=0,i=1; i<xRangeForYear; ++i)
    {
      double lastAvg = 0;
      sem[i] = i;
      if (i%4==1) //years
        {
          lastAvg = gp->getAvg(minYearInList+yearCount);
          yearlyAvg[i] = lastAvg;

          // add the text label at the top:
          QCPItemText *textLabel = new QCPItemText(ui->graphwidget);
          ui->graphwidget->addItem(textLabel);
          textLabel->position->setCoords(i, lastAvg+1.5); // place position at center/top of axis rect
          textLabel->setText(QString::number(lastAvg,'g',4));
          textLabel->setFont(QFont(font().family(), 8)); // make font a bit larger
          textLabel->setPen(QPen(Qt::black)); // show black border around text
          yearCount++;

        }
      else
        {
          if (i+4 < xRangeForYear) //semesters
            {
              double avg = gp->getAvg(minYearInList+yearCount,(i-1)%4);
              SemesterialAvg[i+4] = avg;
              // add the text label at the top:

              QCPItemText *textLabel = new QCPItemText(ui->graphwidget);
              ui->graphwidget->addItem(textLabel);
              textLabel->position->setCoords(i+4, avg+0.5); // place position at center/top of axis rect
              textLabel->setText(QString::number(avg,'g',4));
              textLabel->setFont(QFont(font().family(), 8)); // make font a bit larger
              textLabel->setPen(QPen(Qt::black)); // show black border around text


            }

          yearlyAvg[i] = 0;
        }
    }
  //yearly
  ui->graphwidget->graph(0)->setData(sem,yearlyAvg);
  //yearly
  ui->graphwidget->graph(1)->setData(sem,SemesterialAvg);
}

/**
 * @brief gradegraph::setvisualization set graph borders text, range and looking
 */
void gradegraph::setVisualization()
{
  ui->graphwidget->axisRect()->setupFullAxesBox(true); //make the graph looks like a box

  ui->graphwidget->addGraph(); //yearly and semesterial graphs
  ui->graphwidget->addGraph();

  ui->graphwidget->graph(0)->setName(tr("Yearly Average"));
  ui->graphwidget->graph(0)->setLineStyle(QCPGraph::lsLine);
  ui->graphwidget->graph(0)->setPen(QPen(Qt::GlobalColor::blue));

  ui->graphwidget->graph(1)->setName(tr("Semesterial Average"));
  ui->graphwidget->graph(1)->setLineStyle(QCPGraph::lsLine);
  ui->graphwidget->graph(1)->setPen(QPen( QColor(Qt::GlobalColor::red)));

  int minYearInList = gp->getMinYearInList();
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
  ui->graphwidget->yAxis->setLabel(tr("AVG Grade"));
  ui->graphwidget->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
  ui->graphwidget->yAxis->setRange(50,100);
  ui->graphwidget->yAxis->setTickStep(2);
  ui->graphwidget->yAxis->setAutoSubTicks(false);
  ui->graphwidget->yAxis->setAutoTickStep(false);
  ui->graphwidget->yAxis->setSubTickCount(5);


  ui->graphwidget->xAxis->setLabel(tr("Years"));
  ui->graphwidget->xAxis->setAutoTickLabels(false);
  ui->graphwidget->xAxis->setTickVectorLabels(xStrings);
  ui->graphwidget->xAxis->setTickLabelFont(QFont(QFont().family(), 7));
  ui->graphwidget->xAxis->setAutoTickStep(false);
  ui->graphwidget->xAxis->setTickStep(1);
  ui->graphwidget->xAxis->setAutoSubTicks(false);
  ui->graphwidget->xAxis->setSubTickCount(1);
  ui->graphwidget->xAxis->setRange(1,xRangeForYear);

  ui->graphwidget->legend->setVisible(true); //show graph name on top right
}

void gradegraph::on_pushButton_clicked()
{
    qDebug() << "Closed Graph Dialog";
    this->done(0);
}
