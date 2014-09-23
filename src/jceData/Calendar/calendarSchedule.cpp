#include "calendarSchedule.h"

calendarSchedule::calendarSchedule()
{
    QStringList days,hours;
    QTextStream hourString;
    QString a;
    int startingHour = HOURS_BEGIN;
    int endingHour = HOURS_END;
    int academicHour = ACADEMIN_HOUR;
    hourString.setString(&a);
    for (int i = startingHour;i <= endingHour; ++i)
    {
        if (i < 10)
            hourString << "0" << i << ":" << "00" << " - " << "0" << i << ":" << academicHour;
        else
            hourString << i << ":" << "00" << " - " << i << ":" << academicHour;
        hours << QObject::tr(hourString.string()->toStdString().c_str());
        a = "";
    }

    days << QObject::tr("Sunday") << QObject::tr("Monday") << QObject::tr("Tuesday") << QObject::tr("Wednesday") << QObject::tr("Thursday") << QObject::tr("Friday");

    setRowCount(endingHour - startingHour + 1);
    setColumnCount(6);

    setLayoutDirection(Qt::LayoutDirection::RightToLeft);\

    setHorizontalHeaderLabels(days);
    setVerticalHeaderLabels(hours);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void calendarSchedule::setPage(QString html)
{
    CalendarPage::setPage(html);

    insertCourseIntoTable();
}

void calendarSchedule::clearTableItems()
{
    int i,j;
    for (i = 0; i < rowCount(); i++)
        for (j = 0; j < columnCount(); j++)
            if (this->takeItem(i,j) != NULL)
                delete this->takeItem(i,j);
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void calendarSchedule::insertCourseIntoTable()
{

    QTableWidgetItem *item;
    QString courseString;
    int currentHour,currentDay,blocksNumer;
    int row,col;
    for (calendarCourse *coursePtr: *getCourses())
    {
        courseString = "";
        currentHour = coursePtr->getHourBegin();
        currentDay = coursePtr->getDay();
        blocksNumer = coursePtr->getHourEnd() - coursePtr->getHourBegin(); //every hour is a block to fill!
        while (blocksNumer >= 0)
        {
            row = currentHour - HOURS_BEGIN;
            col = currentDay-1;
            courseString = QString(coursePtr->getName() + "\n");

            if (coursePtr->getLecturer() != LECTURER_DEFAULT_STRING)
                courseString += coursePtr->getLecturer() + "\n";
            else
                courseString += QString("טרם נקבע מרצה או מתרגל\n");

            if (coursePtr->getRoom() != ROOM_DEFAULT_STRING)
                courseString += coursePtr->getRoom();
            else
                courseString += QString("טרם נקבעה כיתה");


            item = new QTableWidgetItem(courseString);
            if (this->takeItem(row,col) != NULL)
                delete this->takeItem(row,col);
            this->setItem(row,col,item);

            currentHour++;
            --blocksNumer;
        }
        horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }

        horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);
}


