#ifndef CALENDARMANAGER_H
#define CALENDARMANAGER_H

#include "./src/jceData/Calendar/calendarPage.h"
#include <iostream>

#include <QPlainTextEdit>

class CalendarManager
{
public:
    CalendarManager(QPlainTextEdit *ptr);
    ~CalendarManager()
    {
        delete cali;
    }

    void setCalendar(std::string html);
private:
    QPlainTextEdit * ptr;
    CalendarPage * cali;
};

#endif // CALENDARMANAGER_H
