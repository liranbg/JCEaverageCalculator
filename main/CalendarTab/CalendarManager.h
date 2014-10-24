#ifndef CALENDARMANAGER_H
#define CALENDARMANAGER_H

#include "../../src/jceData/Calendar/coursesSchedule/calendarPage.h"
#include "../../src/jceData/Calendar/coursesSchedule/calendarSchedule.h"
#include "../../src/jceData/Calendar/coursesSchedule/calendarDialog.h"
#include "../../src/jceData/CSV/csv_exporter.h"

#include "../../src/jceData/Calendar/Exams/calendarExam.h"
#include "../../src/jceData/Calendar/Exams/examDialog.h"

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QMessageBox>

class CalendarManager : public QWidget
{
    Q_OBJECT

public:
    CalendarManager(QWidget *parent = 0, QGridLayout  *ptr = 0);
    ~CalendarManager()
    {
        delete caliSchedPtr;
        delete examSchePtr;
        delete caliDialog;
        delete examDialogPtr;
    }
    void exportCalendarCSV();
    void setCalendar(QString html);
    void setExamsSchedule(QString html);
    void showExamDialog();
    void resetTable() { if (caliSchedPtr != NULL) caliSchedPtr->clearTableItems(); }

private:
    calendarExam * examSchePtr;
    examDialog * examDialogPtr;


    calendarSchedule * caliSchedPtr;
    CalendarDialog * caliDialog;

};

#endif // CALENDARMANAGER_H
