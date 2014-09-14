#include "CalendarManager.h"

CalendarManager::CalendarManager(calendarSchedule *ptr)
{
    this->caliSchedPtr = ptr;
    caliDialog = new CalendarDialog();
}

void CalendarManager::setCalendar(std::string html)
{
    caliSchedPtr->setPage(html);
}
bool CalendarManager::exportCalendarCSV() //need to add fix to the null pointer bug
{
    if (this->caliSchedPtr->getCourses()->empty())
        return false;
    QMessageBox msgBox;
    int buttonClicked = caliDialog->exec();
    if (buttonClicked == 0) //cancel?
        return false;
    //calDialog.getStartDate(),calDialog.getEndDate()
    if(caliDialog->ok())
    {
        if(CSV_Exporter::exportCalendar(caliSchedPtr, caliDialog))
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(QObject::tr("Exported Successfuly!"));
            msgBox.exec();
            return true;
        }else
        {
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText(QObject::tr("Error on exporting."));
            msgBox.exec();
        }
    }
    else
    {
        msgBox.setIcon(QMessageBox::Critical);
         msgBox.setText(QObject::tr("Dates not valid"));
        msgBox.exec();
    }
    return false;
}


