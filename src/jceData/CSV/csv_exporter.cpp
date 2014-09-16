#include "csv_exporter.h"

CSV_Exporter::CSV_Exporter()
{

}

bool CSV_Exporter::exportCalendar(calendarSchedule *calSched, CalendarDialog *cal)
{
    if ((cal == NULL) || (calSched == NULL) || (calSched->getCourses() == NULL)) //pointers checking!
    {
        qWarning() << "CSV : User trying to export to csv but no calendar was loaded. aborting.";
        return false;
    }
    qDebug() << "CSV : Getting path for csv file from user...";
    QString filePath = getFileFath();
    if(filePath == NULL) //User canceled
    {
        qDebug() << "CSV : User pressed Cancel... returning false";
        return false;
    }
    qDebug() << "CSV : User Chose: " << filePath;
    qDebug() << "CSV : Atempting to export the Schedule...";

    QFile file(filePath);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        qCritical() << "CSV : unable to open/create the file... maybe permissions error.";
        return false;
    }//else
    //Delete the file
    QTextStream out(&file);
    out << CSV_CALENDAR_HEADER << "\n";
    for (calendarCourse *coursePtr: *(calSched->getCourses()))
    {
        // Subject,Start Date,Start Time,End Date,End Time,Description,Location
        int day = coursePtr->getDay();
        int startH = coursePtr->getHourBegin();
        int startM = coursePtr->getMinutesBegin();
        int endH = coursePtr->getHourEnd();
        int endM = coursePtr->getMinutesEnd();
        QString lecturer = QString(coursePtr->getLecturer().c_str()); //WHY YOU USED STD STRING?!
        QString type = QString(coursePtr->getType().c_str());
        QString name = QString(coursePtr->getName().c_str());
        QString room = QString(coursePtr->getRoom().c_str());

        QDate currentDate = cal->getStartDate();

        currentDate = currentDate.addDays(day-1);

        for(;currentDate <= cal->getEndDate(); currentDate = currentDate.addDays(7))
        {
            QString line = makeLine(name, &currentDate, startH, startM, endH, endM, lecturer, room, type);
            if(line != NULL)
                out << line << char(0x0A);
            else
                qWarning() << "CSV : Got A NULL in Line! in function: " << Q_FUNC_INFO;
        }
        out.flush();
    }


    file.close();
    qDebug() << "CSV : Exported Successfully";
    return true;

}

QString CSV_Exporter::getFileFath()
{
    QString fileName = QFileDialog::getSaveFileName();
    if(fileName == "")
        return NULL;
    if(!fileName.contains(".csv", Qt::CaseInsensitive))
        fileName.append(".csv");
    return fileName;
}

QString CSV_Exporter::makeLine(QString name, QDate *date, int startH, int startM, int endH, int endM, QString lecturer, QString room, QString type)
{
    //Creating a CSV text line for Google Calendar/iCal/Outlook

    QString CSV_line = "";
    QString subject = "\"";
    subject.append(name);
    subject.append(" - ");
    subject.append(type);
    subject.append("\"");


    QString dateStr = date->toString("MM/dd/yyyy");


    QString start;
    start.append(QString::number(startH));
    start.append(":00");
    //start.append(QString::number(startM));
    start.append(":00");

    QString end;
    end.append(QString::number(endH));
    end.append(":");
    end.append(QString::number(endM));
    end.append(":00");

    QString description = "\"מרצה ";
    description.append(lecturer);
    description.append("\n");
    description.append(" ב");
    description.append(room);
    description.append("\"");

    //Create the Fucking Line
    //Header: Subject,Start Date,Start Time,End Date,End Time,Description,Location
    CSV_line.append(subject);
    CSV_line.append(",");

    CSV_line.append(dateStr);
    CSV_line.append(",");

    CSV_line.append(start);
    CSV_line.append(",");

    CSV_line.append(dateStr);
    CSV_line.append(",");

    CSV_line.append(end);
    CSV_line.append(",");

    CSV_line.append(description);
    CSV_line.append(",");

    CSV_line.append("\"JCE Jerusalem\"");

    return CSV_line;
}
