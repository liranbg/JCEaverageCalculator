#include "CalendarManager.h"

CalendarManager::CalendarManager(calendarSchedule *ptr)
{
    this->caliSchedPtr = ptr;
}

void CalendarManager::exportCalendarCSV()
{
    if(CSV_Exporter::exportCalendar(this->caliSchedPtr))
    {
        QMessageBox msgBox;
        msgBox.setText("<center>Exported Successfuly!<br><b>HaazZaA!!");
        msgBox.exec();
    }else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("<center>Something went wrong...<br></center>Maybe: <ul><li>You Canceled</li><li>Unable to save the File - try again</li></ul><br><br>"
                       "<b><center>In case of a serious problem, please file a bug report.<br>thank you. OpenJCE teem");
        msgBox.exec();
    }
}

void CalendarManager::setCalendar(std::string html)
{
    caliSchedPtr->setPage(html);
}
