#include "CalendarManager.h"

CalendarManager::CalendarManager(calendarSchedule *ptr)
{
    this->caliSchedPtr = ptr;
    caliDialog = new CalendarDialog();
}

void CalendarManager::setCalendar(QString html)
{
    caliSchedPtr->setPage(html);
}
void CalendarManager::exportCalendarCSV() //need to add fix to the null pointer bug
{
    qDebug() << this->caliSchedPtr->getCourses();
    if (this->caliSchedPtr->getCourses() == NULL)
        return;
    QMessageBox msgBox;
    int buttonClicked = caliDialog->exec();
    if (buttonClicked == 0) //cancel?
        return;
    //calDialog.getStartDate(),calDialog.getEndDate()
    if (caliDialog->ok())
    {
        if (CSV_Exporter::exportCalendar(caliSchedPtr, caliDialog))
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(QObject::tr("Exported Successfuly!"));
        }
    }
    else
    {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(QObject::tr("Dates not valid"));
    }
    msgBox.exec();
}


