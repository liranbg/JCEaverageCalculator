/**
 * This simple static class will generate a csv file of a given semester.
 *
 * the class wat written for the JCE Manager and exporting a calendar to Goolgle Calendar.
 *
 * this CSV file should also work for iCal and Outlook. but have not been tested.
 *
 */
#ifndef CSV_EXPORTER_H
#define CSV_EXPORTER_H

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QMessageBox>

#include "../Calendar/calendarSchedule.h"
#include "../Calendar/calendarDialog.h"


#define CSV_CALENDAR_HEADER "Subject,Start Date,Start Time,End Date,End Time,Description,Location"

class CSV_Exporter
{
public:
    CSV_Exporter();
    static bool exportCalendar(calendarSchedule* calSched, CalendarDialog *cal);


private:
    static QString getFileFath();
    static QString makeLine(QString name,QDate *date,int startH,int startM,int endH,int endM,QString lecturer,QString room,QString type);
    static void changeDayNumberFromQtToNormal(int *);
    enum DAYS{SUNDAY=1, MONDAY=2, TUESDAY=3, WENDSDAY=4, THURSDAY=5, FRIDAY=6};
};

#endif // CSV_EXPORTER_H
