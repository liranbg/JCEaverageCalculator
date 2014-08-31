#include "CalendarManager.h"

CalendarManager::CalendarManager(QPlainTextEdit *ptr)
{
    this->ptr = ptr;
}

void CalendarManager::setCalendar(std::string html)
{
    cali = new CalendarPage(html);
    QString text;
    for (calendarCourse *a: *cali->getCourses())
    {
        text += QString::fromStdString(a->courseToString());
    }
    ptr->setPlainText(text);
}
