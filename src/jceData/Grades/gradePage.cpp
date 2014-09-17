#include "gradePage.h"

GradePage::GradePage(QString html) : Page()
{
    courses = new std::list<gradeCourse*>();
    tempHtml = getString(html);
    tempHtml = tokenToLines(tempHtml);
    coursesListInit(tempHtml);

}
GradePage::~GradePage()
{
    for(Course* c : *courses)
        delete c;
    delete courses;
}

void GradePage::removeCourse(QString courseSerialID)
{
    for(gradeCourse* c : *courses)
    {
        if (c->getSerialNum() == courseSerialID.toInt())
        {
            courses->remove(c);
            delete c;
            return;
        }
    }

}
void GradePage::coursesListInit(QString &linesTokinzedString)
{
    std::list<QString> stringHolder;
    QString temp;
    gradeCourse* cTemp = NULL;
    char* tok;
    char* textToTok = strdup(linesTokinzedString.toStdString().c_str());
    tok = strtok(textToTok,"\n");
    while (tok != NULL)
    {
        temp = tok;
        stringHolder.push_back(temp);
        tok = strtok(NULL, "\n");
    }
    for(QString temp: stringHolder)
    {
        cTemp = lineToCourse(temp);
        if (cTemp != NULL)
            courses->push_back(cTemp);
    }
}

QString GradePage::tokenToLines(QString &textToPhrase)
{
    int ctr = 0;
    QString temp = "";
    char *tok;
    char* textToTok = strdup(textToPhrase.toStdString().c_str());
    tok = strtok(textToTok, "\n");
    while(tok != NULL)
    {
        //amount of data before the actual needed data and no empty lines
        if (strcmp(tok," \t ") != 0)
        {
            temp += tok;
            temp += "\n";
        }
        ctr++;
        tok = strtok(NULL, "\n");
    }
    return temp;

}
gradeCourse* GradePage::lineToCourse(QString line)
{
    gradeCourse *tempC = NULL;
    QString templinearray[COURSE_FIELDS];//[serial,name,type,points,hours,grade,additions]
    int serial;
    double points,hours,grade;
    QString name,type, additions;
    QString tempS = "";
    int i = 0;
    char* tok;
    char* cLine = strdup(line.toStdString().c_str());
    tok = strtok(cLine, "\t");
    while(tok != NULL)
    {

        tempS = tok;
        if (i == 1) //skip the tokenizing loop just once
        {
            tempS = "";
            char *tokTemp;
            tokTemp = tok;
            while (!(isdigit((int)*tokTemp)))
                tokTemp++;

            while (isdigit((int)*tokTemp))
            {
                tempS += QString(*tokTemp);
                tokTemp++;
            }
            templinearray[i-1] = tempS;
            templinearray[i] = QString(tokTemp);

        }
        else if (i > 1)
        {
            templinearray[i] = tempS;
        }
        i++;
        tok=strtok(NULL, "\t");
    }
    if (templinearray[0] == "") //empty phrasing
        return NULL;

    serial = templinearray[gradeCourse::CourseScheme::SERIAL].toInt();

    name = templinearray[gradeCourse::CourseScheme::NAME];
    type = templinearray[gradeCourse::CourseScheme::TYPE];

    points = templinearray[gradeCourse::CourseScheme::POINTS].toDouble();
    hours = templinearray[gradeCourse::CourseScheme::HOURS].toDouble();

    if (isGradedYet(templinearray[gradeCourse::CourseScheme::GRADE]))
        grade = templinearray[gradeCourse::CourseScheme::GRADE].toDouble();
    else
        grade = NO_GRADE_YET;

    additions = templinearray[gradeCourse::CourseScheme::ADDITION];

    tempC = new gradeCourse(serial,name,type,points,hours,grade,additions);
    return tempC;
}

//checking if one of the chars inside grade is not a number
bool GradePage::isGradedYet(QString grade)
{
    if (strlen(grade.toStdString().c_str()) <= 1)
        return false;

    for (char c: grade.toStdString())
    {
        if (c == '\0')
            break;
        if (((!isdigit((int)c))  && (!isspace((int)c)))) //48 = 0, 57 = 9
            return false;

    }
    return true;
}
double GradePage::getAvg()
{
    double avg = 0;
    double points = 0;
    for(gradeCourse* c : *courses)
    {
        if ((c->getGrade() != 0))
        {
            avg += c->getGrade() * c->getPoints();
            points += c->getPoints();
        }
    }

    avg /= points;
    return avg;
}
