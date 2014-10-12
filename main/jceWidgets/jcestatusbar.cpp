#include "jcestatusbar.h"

jceStatusBar::jceStatusBar(QWidget *parent) :
  QStatusBar(parent)
{

  this->setStyleSheet("QStatusBar::item { border: 0px solid black };");
  this->setFixedHeight(STATUS_ICON_HEIGH+5);
  this->showMessage(tr("Ready"));


  //Icon
  iconButtomStatusLabel = new QLabel(this);
  iconButtomStatusLabel->setAlignment(Qt::AlignHCenter);

  //ProgressBar
  progressBar = new QProgressBar(this);
  connect(progressBar,SIGNAL(valueChanged(int)),this,SLOT(setProgressValue(int)));
  connect(this,SIGNAL(progressHasPacket(int)),this,SLOT(addValueToProgressBar(int)));

  progressBar->setFixedWidth(120);
  progressBar->setFixedHeight(STATUS_ICON_HEIGH);
  progressBar->setStyleSheet("#progressBar::horizontal {"
                             "border: 1px solid gray;"
                             "border-radius: 3px;"
                             "background: transparent;"
                             "padding: 1px;"
                             "}"
                             "#progressBar::chunk:horizontal {"
                             "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 green, stop: 1 white);"
                             "}");
  progressBar->setRange(0,100);
  progressBar->setValue(0);
  progressBar->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  progressBar->setTextVisible(true);
  progressBar->setFormat("%p%");


  addPermanentWidget(progressBar,0);
  addPermanentWidget(iconButtomStatusLabel,0);

}

void jceStatusBar::setIconConnectionStatus(jceProgressStatus update)
{
  QPixmap iconPix;
  switch (update)
    {
    case jceProgressStatus::Disconnected:
      iconPix.load(":/icons/redStatusIcon.png");
      showMessage(tr("Disconnected"));
      break;
    case jceProgressStatus::Ready:
      iconPix.load(":/icons/redStatusIcon.png");
      showMessage(tr("Ready."));
      break;
    case jceProgressStatus::Connecting:
      iconPix.load(":/icons/blueStatusIcon.png");
      showMessage(tr("Connecting..."));
      break;
    case jceProgressStatus::Sending:
      iconPix.load(":/icons/blueStatusIcon.png");
      showMessage(tr("Sending."));
      break;
    case jceProgressStatus::Recieving:
      iconPix.load(":/icons/blueStatusIcon.png");
      showMessage(tr("Recieving."));
      break;
    case jceProgressStatus::Inserting:
      iconPix.load(":/icons/blueStatusIcon.png");
      showMessage(tr("Inserting."));
      break;
    case jceProgressStatus::LoggedIn:
      setProgressValue(100);
      iconPix.load(":/icons/greenStatusIcon.png");
      showMessage(tr("LoggedIn."));
      break;
    case jceProgressStatus::Done:
      setProgressValue(100);
      iconPix.load(":/icons/greenStatusIcon.png");
      showMessage(tr("Done."));
      break;
    case jceProgressStatus::Connected:
      setProgressValue(100);
      iconPix.load(":/icons/greenStatusIcon.png");
      showMessage(tr("Connected"));
      break;
    }
  iconButtomStatusLabel->setPixmap(iconPix);

  repaint();

}
/**
* @brief Every time the value changes this method will be called
* @param value = the value of the progress Bar
*/
void jceStatusBar::setProgressValue(int value)
{

  if (value == 0)
    {
      progressBar->setVisible(false);
      progressBar->setValue(0);
    }
  else
    {
      progressBar->setValue(value);
      progressBar->setVisible(true);
    }

}

void jceStatusBar::addValueToProgressBar(int value)
{
  progressBar->setValue(progressBar->value() + value);

}
