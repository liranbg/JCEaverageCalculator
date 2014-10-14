#ifndef JCESTATUSBAR_H
#define JCESTATUSBAR_H


#include <QDebug>
#include <QLayout>
#include <QStatusBar>
#include <QProgressBar>
#include <QLabel>

#define STATUS_ICON_HEIGH 48

class jceStatusBar : public QStatusBar
{
  Q_OBJECT

public:

  enum jceProgressStatus
  {
    Ready,
    Disconnected,
    Connecting,
    Connected,
    LoggedIn,
    Sending,
    Recieving,
    Inserting,
    Done,
    Error
  };

  jceStatusBar(QWidget *parent = 0);

  void setIconConnectionStatus(jceProgressStatus update);


signals:
  void progressHasPacket(int num);

public slots:
  void setProgressValue(int val);

private slots:
  void addValueToProgressBar(int value);

private:


  QProgressBar * progressBar;
  QLabel *iconButtomStatusLabel;
};

#endif // JCESTATUSBAR_H
