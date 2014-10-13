#include "CalendarManager.h"

CalendarManager::CalendarManager(QWidget *parent, QGridLayout *ptr) : QWidget(parent)
{
    caliSchedPtr = new calendarSchedule(this);
    examSchePtr = new calendarExam();
    ptr->addWidget(caliSchedPtr);
    caliDialog = new CalendarDialog(this);
    examDialogPtr = new examDialog(this,examSchePtr);
}

void CalendarManager::setCalendar(QString html)
{
    caliSchedPtr->setPage(html);

}

void CalendarManager::setExamsSchedule(QString html)
{
    examSchePtr->setPage(html);
    examDialogPtr->initializingDataIntoTable();
}

void CalendarManager::showExamDialog()
{
    examDialogPtr->show();
}
void CalendarManager::exportCalendarCSV()
{
    if (this->caliSchedPtr->getCourses().isEmpty())
        return;
    QMessageBox msgBox;
    int buttonClicked = caliDialog->exec();
    if (buttonClicked == 0) //cancel?
        return;
    //calDialog.getStartDate(),calDialog.getEndDate()
    if (caliDialog->ok())
    {
        if (CSV_Exporter::exportCalendar(caliSchedPtr, caliDialog, examSchePtr))
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(QObject::tr("Exported Successfuly!"));
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


