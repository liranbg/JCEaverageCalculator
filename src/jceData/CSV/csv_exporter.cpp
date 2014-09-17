#include "csv_exporter.h"

/*
 *
 * Class doc can be bound in csv_exporter.h
 *
 */


CSV_Exporter::CSV_Exporter()
{
    /* EMPTY - NO NEED */
}

/**
 * @brief   This method will generate the CSV file (Targeting google calendar import)
 *          it will create a full Semester calendar based on the users input (@param cal)
 *          and the @calSched wich holdes all the courses in "this" semester.
 * @param calSched  - Holdes all the Courses and there info
 * @param cal       - The Calendar dialog witch holdes the starting date and the eand date.
 * @return          - True if *all* went well, false if something on the way went wrong.
 */
bool CSV_Exporter::exportCalendar(calendarSchedule *calSched, CalendarDialog *cal)
{
    if ((cal == NULL) || (calSched == NULL)) //pointers checking!
        return false;

    if (calSched->getCourses() == NULL)
        return false;

    qDebug() << "Getting path for csv file from user...";

    QString filePath = getFileFath();
    if(filePath == NULL) //User canceled from the file explorer popup
    {
        qDebug() << "CSV : User pressed Cancel... returning false";
        return false;
    }
    qDebug() << "CSV : User Chose: " << filePath;
    qDebug() << "CSV : Atempting to export the Schedule...";

    QFile file(filePath);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate)) //Incase unable to open the file (binary mode - \n will not be converted on "Windows")
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(QObject::tr("Unable to open or create the file.\nExporting Failed"));
        msgBox.exec();
        qCritical() << "unable to open/create the file... maybe permissions error.";
        return false;
    }

    QTextStream out(&file);
    out << CSV_CALENDAR_HEADER << "\n"; // macro in header file

    for (calendarCourse *coursePtr: *(calSched->getCourses())) //main loop - running though all courses
    {
        // Getting course info - store in vars for easy access
        int day = coursePtr->getDay();
        int startH = coursePtr->getHourBegin();
        int startM = coursePtr->getMinutesBegin();
        int endH = coursePtr->getHourEnd();
        int endM = coursePtr->getMinutesEnd();
        QString lecturer = coursePtr->getLecturer();
        QString type = coursePtr->getType();
        QString name = coursePtr->getName();
        QString room = coursePtr->getRoom();

        QDate currentDate = cal->getStartDate(); // currentDate will iterate thou the semester

        currentDate = currentDate.addDays(day-1); //selecting the REAL starting day of that course

        /*
         * secondary loop - We have course info and starting day.
         * evrey loop enterence we add the course and moving one week forward.
         */
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

/**
 * @brief Get the file path according to user via a file explorer dialog
 * @return - QString: the file path.
 */
QString CSV_Exporter::getFileFath()
{
    QString fileName = QFileDialog::getSaveFileName();
    if (fileName == "")
        return NULL;
    if (!fileName.contains(".csv", Qt::CaseInsensitive))
        fileName.append(".csv");
    return fileName;
}


/**
 * @brief Returning a CSV formated ling in QString.
 * @param name
 * @param date
 * @param startH
 * @param startM
 * @param endH
 * @param endM
 * @param lecturer
 * @param room
 * @param type
 * @return a CSV formated ling in QString.
 */
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
    description.append("\n Created with JCE Manager.\"");

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
