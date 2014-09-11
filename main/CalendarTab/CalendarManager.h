#ifndef CALENDARMANAGER_H
#define CALENDARMANAGER_H

#include "./src/jceData/Calendar/calendarPage.h"
#include "./src/jceData/Calendar/calendarSchedule.h"
#include "./src/jceData/CSV/csv_exporter.h"
#include "./src/jceData/Calendar/calendardialog.h"
#include <iostream>
#include <QMessageBox>

class CalendarManager
{
public:
    CalendarManager(calendarSchedule *ptr);
    ~CalendarManager()
    {
        delete caliSchedPtr;
        delete caliDialog;
    }
    void exportCalendarCSV();
    void setCalendar(std::string html);
    void resetTable() { if (caliSchedPtr != NULL) caliSchedPtr->clearTableItems(); }

private:
    calendarSchedule * caliSchedPtr;
    CalendarDialog * caliDialog;

};

#endif // CALENDARMANAGER_H
