#include "calendarPage.h"

/**
 * @brief CalendarPage::setPage getting the html and stripping it into a courses schedule in a list
 * @param html
 */
void CalendarPage::setPage(QString html)
{
   courses.clear();
  tempHtml = getString(html);
  calendarListInit(tempHtml);

}
/**
 * @brief CalendarPage::calendarListInit - make an object from each line of course
 * @param linesTokinzedString - string contain lines of coureses. each string contain single course information
 */
void CalendarPage::calendarListInit(QString &linesTokinzedString)
{
    QString tempToken;

    QStringList holder = linesTokinzedString.split("\n");
    QStringList::iterator iterator;
    for (iterator = holder.begin(); iterator != holder.end(); ++iterator)
    {
        tempToken = (*iterator);

        if (!tempToken.isEmpty())
        {
            calendarCourse *cTemp = lineToCourse(tempToken);
            if (cTemp != NULL)
                this->courses.push_back(cTemp);
        }
    }
}

/**
 * @brief CalendarPage::lineToCourse
 * @param line -    line of course contain its data and information
 * @return calendarcourse object with its information
 */
calendarCourse *CalendarPage::lineToCourse(QString line)
{

  calendarCourse *tempC = NULL;
  QString templinearray[CALENDAR_COURSE_FIELDS];//[serial,name,type,lecturer,points,semesterhours,dayandhours,room]
  int serial;
  double points,semesterHours;
  QString name,type, lecturer,dayAndHour,room;

  QString tempToken;
  int i = 0;
  QStringList holder = line.split("\t");
  QStringList::iterator iterator;
  for (iterator = holder.begin(); iterator != holder.end(); ++iterator)
  {
      tempToken = (*iterator);

      if (i >= 1) //skips on semester character
        {
            templinearray[i-1] = tempToken.trimmed();
        }
      i++;
      if (i > CALENDAR_COURSE_FIELDS)
          break;
  }

  if (templinearray[0] == "") //empty parsing
      return NULL;


  serial = templinearray[calendarCourse::CourseScheme::SERIAL].toInt();
  name = templinearray[calendarCourse::CourseScheme::NAME];
  type = templinearray[calendarCourse::CourseScheme::TYPE];


  if (!templinearray[calendarCourse::CourseScheme::LECTURER].isEmpty())
    lecturer = templinearray[calendarCourse::CourseScheme::LECTURER];
  else
    lecturer = LECTURER_DEFAULT_STRING;

  if (!templinearray[calendarCourse::CourseScheme::POINTS].isEmpty())
    points = templinearray[calendarCourse::CourseScheme::POINTS].toDouble();
  else
    points = 0;
  if (!templinearray[calendarCourse::CourseScheme::SEM_HOURS].isEmpty())
    semesterHours = templinearray[calendarCourse::CourseScheme::SEM_HOURS].toDouble();
  else
    semesterHours = 0;

  dayAndHour = templinearray[calendarCourse::CourseScheme::DAY_AND_HOURS];

  if (!templinearray[calendarCourse::CourseScheme::ROOM].isEmpty())
    room = templinearray[calendarCourse::CourseScheme::ROOM];
  else
    room = ROOM_DEFAULT_STRING;


  tempC = new calendarCourse(serial,name,type,lecturer,points,semesterHours,dayAndHour,room);
//  qDebug() << "serial is: " << tempC->getSerialNum();
//  qDebug() << tempC->getName();
//  qDebug() << tempC->getType();
//  qDebug() << tempC->getLecturer();
//  qDebug() << tempC->getPoints();
//  qDebug() << tempC->getHourBegin() << ":" <<  tempC->getMinutesBegin();
//  qDebug() << tempC->getHourEnd() << ":" <<  tempC->getMinutesEnd();

//  qDebug() << tempC->getDay();
//  qDebug() << tempC->getRoom();

  return tempC;
}
