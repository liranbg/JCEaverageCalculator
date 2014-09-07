#ifndef CSV_EXPORTER_H
#define CSV_EXPORTER_H

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>

#include "../Calendar/calendarSchedule.h"

#define CSV_CALENDAR_HEADER "Subject,Start Date,Start Time,End Date,End Time,Description,Location"

class CSV_Exporter
{
public:
    CSV_Exporter();
    static bool exportCalendar(calendarSchedule* calSched);


private:
    static QString getFileFath();
    static QString makeLine(QString name,int day,int startH,int startM,int endH,int endM,QString lecturer,QString room,QString type);
};

#endif // CSV_EXPORTER_H
