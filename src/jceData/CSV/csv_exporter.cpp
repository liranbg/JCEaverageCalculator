#include "csv_exporter.h"

CSV_Exporter::CSV_Exporter()
{

}

bool CSV_Exporter::exportCalendar(calendarSchedule *calSched)
{
    qDebug() << "Getting path for csv file from user...";
    QString filePath = getFileFath();
    if(filePath == NULL) //User canceled
    {
        qDebug() << "User pressed Cancel... returning false";
        return false;
    }
    qDebug() << "User Chose: " << filePath;
    qDebug() << "Atempting to export the Schedule...";

    QFile file(filePath);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "unable to open/create the file... maybe permissions error.";
        return false;
    }//else

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

        QString line = makeLine(name, day, startH, startM, endH, endM, lecturer, room, type);
        if(line != NULL)
        {
            #ifndef  Q_OS_WIN32
            out << line << char(0x0D) << " "; //Fucking M$ Special end line shit...
            #endif
            out << line << char(0x0A);
        }
    }


    out.flush();


    file.close();
    qDebug() << "Saved Successfuly! - HazZaA!";
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

QString CSV_Exporter::makeLine(QString name, int day, int startH, int startM, int endH, int endM, QString lecturer, QString room, QString type)
{
    //Creating a CSV text line for Google Calendar/iCal/Outlook
    // First day for semester 10/26/2014

    QString CSV_line = "";
    QString subject = "\"";
    subject.append(name);
    subject.append(" - ");
    subject.append(type);
    subject.append("\"");

    QString date;
    switch (day) {
    case 1:
        date = "10/26/2014";
        break;
    case 2:
        date = "10/27/2014";
        break;
    case 3:
        date = "10/28/2014";
        break;
    case 4:
        date = "10/29/2014";
        break;
    case 5:
        date = "10/30/2014";
        break;
    case 6:
        date = "10/31/2014";
        break;
    default:
        return NULL;
        break;
    }

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

    CSV_line.append(date);
    CSV_line.append(",");

    CSV_line.append(start);
    CSV_line.append(",");

    CSV_line.append(date);
    CSV_line.append(",");

    CSV_line.append(end);
    CSV_line.append(",");

    CSV_line.append(description);
    CSV_line.append(",");

    CSV_line.append("\"JCE Jerusalem\"");

    return CSV_line;
}
