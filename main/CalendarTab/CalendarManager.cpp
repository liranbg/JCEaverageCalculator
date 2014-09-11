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
void CalendarManager::exportCalendarCSV()
{
    QMessageBox msgBox;
    int buttonClicked = caliDialog->exec();
    if (buttonClicked == 0) //cancel?
        return;
    //calDialog.getStartDate(),calDialog.getEndDate()
    if(caliDialog->ok())
    {
        if(CSV_Exporter::exportCalendar(caliSchedPtr, caliDialog))
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(QObject::tr("Exported Successfuly!"));
            msgBox.exec();
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
}


