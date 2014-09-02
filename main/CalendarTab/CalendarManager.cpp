#include "CalendarManager.h"

CalendarManager::CalendarManager(calendarSchedule *ptr)
{
    this->caliSchedPtr = ptr;
}

void CalendarManager::setCalendar(std::string html)
{
    caliSchedPtr->setPage(html);
}
