#ifndef CALENDARMANAGER_H
#define CALENDARMANAGER_H

#include "./src/jceData/Calendar/calendarPage.h"
#include "./src/jceData/Calendar/calendarSchedule.h"
#include "./src/jceData/CSV/csv_exporter.h"
#include "./src/jceData/Calendar/calendarDialog.h"

#include <QObject>
#include <QGridLayout>
#include <QMessageBox>

class CalendarManager : public QObject
{
    Q_OBJECT

public:
    CalendarManager(QGridLayout  *ptr);
    ~CalendarManager()
    {
        delete caliSchedPtr;
        delete caliDialog;
    }
    void exportCalendarCSV();
    void setCalendar(QString html);
    void resetTable() { if (caliSchedPtr != NULL) caliSchedPtr->clearTableItems(); }

private:
    calendarSchedule * caliSchedPtr;
    CalendarDialog * caliDialog;

};

#endif // CALENDARMANAGER_H
