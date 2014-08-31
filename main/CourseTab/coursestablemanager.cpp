#include "coursestablemanager.h"

coursesTableManager::coursesTableManager(QTableWidget *ptr, user *usrPtr)
{
    this->gp = NULL;
    this->us = usrPtr;
    this->courseTBL = ptr;

    /*
     *  Initilizing Table
    */
    courseTBL->setRowCount(0);
    courseTBL->setColumnCount(COURSE_FIELDS);
    QStringList mz;
    mz << QObject::tr("Code") << QObject::tr("Name") << QObject::tr("Type") << QObject::tr("Points") << QObject::tr("Hours") << QObject::tr("Grade") << QObject::tr("Additions");
    courseTBL->setHorizontalHeaderLabels(mz);
    courseTBL->verticalHeader()->setVisible(true);
    courseTBL->setSelectionMode(QAbstractItemView::SingleSelection);
    courseTBL->setShowGrid(true);
    courseTBL->setStyleSheet("QTableView {selection-background-color: red;}");
}

coursesTableManager::~coursesTableManager()
{
    courseTBL = NULL;
    delete gp;
    gp = NULL;
}
/**
 * @brief coursesTableManager::insertJceCoursesIntoTable    phrasing the course list to rows in table
 */
void coursesTableManager::insertJceCoursesIntoTable()
{
    for (gradeCourse *c: *gp->getCourses())
    {
        if (us->getInfluenceCourseOnly())
        {
            if (isCourseInfluence(c))
                addRow(c);
        }
        else
            addRow(c);
    }
}
/**
 * @brief coursesTableManager::setCoursesList   creating courses list with given html page
 * @param html
 */
void coursesTableManager::setCoursesList(std::string &html)
{
    gp = new GradePage(html);
}
/**
 * @brief coursesTableManager::changes  when user changes the table manually it updates it
 * @param change    string change
 * @param row       row index
 * @param col       col index
 * @return  if change has been done
 */
bool coursesTableManager::changes(QString change, int row, int col)
{

    bool isNumFlag = true;

    int serialCourse = courseTBL->item(row,gradeCourse::CourseScheme::SERIAL)->text().toInt();
    for (gradeCourse *c: *gp->getCourses())
    {
        if (c->getSerialNum() == serialCourse)
        {
            switch (col)
            {
            case (gradeCourse::CourseScheme::NAME):
                c->setName(change.toStdString());
                break;
            case (gradeCourse::CourseScheme::TYPE):
                c->setType(change.toStdString());
                break;
            case (gradeCourse::CourseScheme::POINTS):
            {
                change.toDouble(&isNumFlag);

                if (!isNumFlag)
                {
                    courseTBL->item(row,col)->setText(QString::number(c->getPoints()));
                }
                else
                    c->setPoints(change.toDouble());
                break;
            }
            case (gradeCourse::CourseScheme::HOURS):
            {
                change.toDouble(&isNumFlag);

                if (!isNumFlag)
                {
                    courseTBL->item(row,col)->setText(QString::number(c->getHours()));
                }
                else
                    c->setHours(change.toDouble());
                break;
            }
            case (gradeCourse::CourseScheme::GRADE):
            {
                change.toDouble(&isNumFlag);

                if (!isNumFlag)
                {
                    courseTBL->item(row,col)->setText(QString::number(c->getGrade()));
                }
                else
                {
                    if ((change.toDouble() >= 0) && (change.toDouble() <= 100))
                        c->setGrade(change.toDouble());
                    else
                        courseTBL->item(row,col)->setText(QString::number(c->getGrade()));
                }
                break;
            }
            case (gradeCourse::CourseScheme::ADDITION):
                c->setAdditions(change.toStdString());
                break;
            }
            break;
        }
    }
    return isNumFlag;

}
/**
 * @brief coursesTableManager::addRow   adds row with given information
 * @param courseToAdd                   if exists, add its information to table
 */
void coursesTableManager::addRow(const gradeCourse *courseToAdd)
{
    int i,j;
    i = courseTBL->rowCount();
    j = 0;
    QTableWidgetItem *serial,*name,*type,*points,*hours,*grade,*addition;
    const gradeCourse * c;
    if (courseToAdd != NULL)
    {
        c = courseToAdd;
        if (!isCourseAlreadyInserted(c->getSerialNum()))
        {
            courseTBL->setRowCount(courseTBL->rowCount()+1);
            serial = new QTableWidgetItem(QString::number(c->getSerialNum()));
            serial->setFlags(serial->flags() & ~Qt::ItemIsEditable);
            points = new QTableWidgetItem(QString::number(c->getPoints()));
            points->setFlags(serial->flags() & ~Qt::ItemIsEditable);
            hours = new QTableWidgetItem(QString::number(c->getHours()));
            hours->setFlags(serial->flags() & ~Qt::ItemIsEditable);
            grade = new QTableWidgetItem(QString::number(c->getGrade()));
            name = new QTableWidgetItem(QString::fromStdString(c->getName()));
            name->setFlags(serial->flags() & ~Qt::ItemIsEditable);
            type = new QTableWidgetItem(QString::fromStdString(c->getType()));
            type->setFlags(serial->flags() & ~Qt::ItemIsEditable);
            addition = new QTableWidgetItem(QString::fromStdString(c->getAddidtions()));

            courseTBL->setItem(i,j++,serial);
            courseTBL->setItem(i,j++,name);
            courseTBL->setItem(i,j++,type);
            courseTBL->setItem(i,j++,points);
            courseTBL->setItem(i,j++,hours);
            courseTBL->setItem(i,j++,grade);
            courseTBL->setItem(i,j,addition);

        }
    }
    else
    {
    }
    courseTBL->resizeColumnsToContents();

}
double coursesTableManager::getAvg()
{
    if (this->gp != NULL)
        return gp->getAvg();
    return 0;
}


void coursesTableManager::influnceCourseChanged(bool ignoreCourseStatus)
{
    if (ignoreCourseStatus)
    {
        int i = 0;
        while (i < courseTBL->rowCount())
        {
            if (courseTBL->item(i,gradeCourse::CourseScheme::POINTS)->text().compare("0") == 0)
                courseTBL->removeRow(i--);
            i++;
        }
    }
    else
    {
        for (gradeCourse *c: *gp->getCourses())
        {
            if (!(isCourseAlreadyInserted(c->getSerialNum())))
                if (c->getPoints() == 0)
                    addRow(c);
        }
    }

}

void coursesTableManager::clearTable()
{
    if (courseTBL->rowCount() == 0)
        return;

    int i = 0; //starting point
    while (courseTBL->rowCount() > i)
    {
        gp->removeCourse(courseTBL->item(i,gradeCourse::CourseScheme::SERIAL)->text().toStdString());
        courseTBL->removeRow(i);
    }
    gp = NULL;
    courseTBL->repaint();
}

gradeCourse *coursesTableManager::getCourseByRow(int row)
{
    QString courseSerial = courseTBL->item(row,gradeCourse::CourseScheme::SERIAL)->text();
    for (gradeCourse *c: *gp->getCourses())
    {
        if (c->getSerialNum() == courseSerial.toDouble())
            return c;
    }
    return NULL;
}

bool coursesTableManager::isCourseAlreadyInserted(double courseID)
{
    int i=0;
    for (i = 0; i < courseTBL->rowCount(); ++i)
    {
        QString courseSerial = courseTBL->item(i,gradeCourse::CourseScheme::SERIAL)->text();
        if (QString::number(courseID) == courseSerial)
            return true;
    }
    return false;
}

bool coursesTableManager::isCourseInfluence(const gradeCourse *courseToCheck)
{
    if (courseToCheck->getPoints() > 0)
        return true;
    return false;

}
