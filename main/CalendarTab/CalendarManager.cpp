#include "CalendarManager.h"

CalendarManager::CalendarManager(calendarSchedule *ptr)
{
    this->caliSchedPtr = ptr;
}

void CalendarManager::setCalendar(std::string html)
{
    caliSchedPtr->setPage(html);
}
void CalendarManager::exportCalendarCSV(CalendarDialog *calDialog)
{
    if(CSV_Exporter::exportCalendar(this->caliSchedPtr, calDialog))
    {
        QMessageBox msgBox;
        msgBox.setText(QObject::tr("Exported Successfuly!"));
        msgBox.exec();
    }else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(QObject::tr("<center>Something went wrong...<br></center>Maybe: <ul><li>You Canceled</li><li>Unable to save the File - try again</li></ul><br><br>"
                       "<b><center>In case of a serious problem, please file a bug report.<br>thank you. OpenJCE teem"));
        msgBox.exec();
    }

}


