#include "examDialog.h"
#include "ui_examDialog.h"
/*
 *TODO: exam revert, add to csv exportation, dialog size
 */
/**
 * @brief examDialog::examDialog
 * @param parent
 * @param calSchedPtr - list of courses with information about each exam
 */
examDialog::examDialog(QWidget *parent, calendarExam *calSchedPtr) : QDialog(parent),
    ui(new Ui::examDialog)
{
    ui->setupUi(this);
    exams = calSchedPtr;
    QStringList headLine;
    //SERIAL, NAME, LECTURER, FIELD, TYPE, FIRST_DATE, FIRST_HOUR_BEGIN, SECOND_DATE, SECOND_HOUR_BEGIN

    headLine <<  tr("Serial") << tr("Course") << tr("Lecturer") << tr("Field") << tr("Type") << tr("Exam 1 Date")  << tr("Starting Hour") << tr("Exam 2 Date") << tr("Starting Hour");

    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->setColumnCount(EXAM_SCHEDULE_FIELDS);
    ui->tableWidget->setHorizontalHeaderLabels(headLine);
    ui->tableWidget->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    ui->tableWidget->setSortingEnabled(false);

    this->setModal(true); //always on top

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    connect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(upgradeExamsTime(QTableWidgetItem*)));
}

/**
 * @brief examDialog::initializingDataIntoTable
 *
 *  Inserting each object of exam into the table
 */
void examDialog::initializingDataIntoTable()
{
    disconnect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(upgradeExamsTime(QTableWidgetItem*)));
    ui->tableWidget->setRowCount(exams->getExamsCounter());
    int i=0,j=0;
    int year,month,day; //for constructin qdate by setdate
    QTableWidgetItem *lecturer,*name,*type;
    QTableWidgetItem *serial;
    QTableWidgetItem *field;
    QTableWidgetItem *firstDate;
    QTableWidgetItem *firstHourbegin;
    QTableWidgetItem *secondDate;
    QTableWidgetItem *secondHourbegin;
    for (calendarExamCourse * tempExam: exams->getExams())
    {
        j=0;

        lecturer = new QTableWidgetItem();
        lecturer->setData(Qt::EditRole, tempExam->getLecturer());
        lecturer->setFlags(lecturer->flags() & ~Qt::ItemIsEditable);

        name = new QTableWidgetItem();
        name->setData(Qt::EditRole, tempExam->getName());
        name->setFlags(name->flags() & ~Qt::ItemIsEditable);

        type = new QTableWidgetItem();
        type->setData(Qt::EditRole, tempExam->getType());
        type->setFlags(type->flags() & ~Qt::ItemIsEditable);

        serial = new QTableWidgetItem();
        serial->setData(Qt::EditRole, tempExam->getSerialNum());
        serial->setFlags(serial->flags() & ~Qt::ItemIsEditable);

        field = new QTableWidgetItem();
        field->setData(Qt::EditRole, tempExam->getField());
        field->setFlags(field->flags() & ~Qt::ItemIsEditable);

        firstDate = new QTableWidgetItem();
        tempExam->getFirstDate().getDate(&year,&month,&day);
        firstDate->setData(Qt::EditRole, QDate(year,month,day).toString("d/M/yy"));
        firstDate->setFlags(firstDate->flags() & ~Qt::ItemIsEditable);

        secondDate = new QTableWidgetItem();
        tempExam->getSecondDate().getDate(&year,&month,&day);
        secondDate->setData(Qt::EditRole, QDate(year,month,day).toString("d/M/yy"));
        secondDate->setFlags(secondDate->flags() & ~Qt::ItemIsEditable);

        firstHourbegin = new QTableWidgetItem();
        firstHourbegin->setData(Qt::EditRole, QTime(tempExam->getFirstHourbegin().hour(),tempExam->getFirstHourbegin().minute()).toString("hh:mm"));

        secondHourbegin = new QTableWidgetItem();
        secondHourbegin->setData(Qt::EditRole, QTime(tempExam->getSecondHourbegin().hour(),tempExam->getSecondHourbegin().minute()).toString("hh:mm"));


        ui->tableWidget->setItem(i,j++,serial);
        ui->tableWidget->setItem(i,j++,name);
        ui->tableWidget->setItem(i,j++,lecturer);
        ui->tableWidget->setItem(i,j++,field);
        ui->tableWidget->setItem(i,j++,type);
        ui->tableWidget->setItem(i,j++,firstDate);
        ui->tableWidget->setItem(i,j++,firstHourbegin);
        ui->tableWidget->setItem(i,j++,secondDate);
        ui->tableWidget->setItem(i,j++,secondHourbegin);
        i++;
    }

    resetGeo();
    connect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(upgradeExamsTime(QTableWidgetItem*)));
}
examDialog::~examDialog()
{
    delete ui;
}

/**
 * @brief examDialog::upgradeExamsTime
 * gets an item and check if its value is valid by time.
 * currently we allow only modification of time
 *
 * @param item - tablewidet from ui->tablewidet.
 * @return true if has been changed. false otherwise
 */
bool examDialog::upgradeExamsTime(QTableWidgetItem *item)
{
    QString text;
    int row,col;
    text = item->text();
    row = item->row();
    col = item->column();
    if (ui->tableWidget->item(row,calendarExamCourse::CourseScheme::SERIAL) == NULL)
        return true;

    int serialCourse = ui->tableWidget->item(row,calendarExamCourse::CourseScheme::SERIAL)->text().toInt();
    for (calendarExamCourse * tempExam: exams->getExams())
    {
        if (tempExam->getSerialNum() == serialCourse)
        {
            if (QTime::fromString(text).isValid())
            {
                if (col == calendarExamCourse::CourseScheme::FIRST_HOUR_BEGIN)
                    tempExam->setFirstHourbegin(QTime::fromString(text));
                else if (col == calendarExamCourse::CourseScheme::SECOND_HOUR_BEGIN)
                    tempExam->setSecondHourbegin(QTime::fromString(text));
                else
                    qCritical() << Q_FUNC_INFO; //is doesnt need to get here. we allow modifications only at hours
                return true;
            }
            else
            {
                if (col == calendarExamCourse::CourseScheme::FIRST_HOUR_BEGIN)
                    ui->tableWidget->item(row,col)->setData(Qt::EditRole, QTime(tempExam->getFirstHourbegin().hour(),tempExam->getFirstHourbegin().minute()).toString("hh:mm"));
                else if (col == calendarExamCourse::CourseScheme::SECOND_HOUR_BEGIN)
                    ui->tableWidget->item(row,col)->setData(Qt::EditRole, QTime(tempExam->getSecondHourbegin().hour(),tempExam->getSecondHourbegin().minute()).toString("hh:mm"));
                else
                    qCritical() << Q_FUNC_INFO; //is doesnt need to get here. we allow modifications only at hours
                qWarning() << Q_FUNC_INFO << "missmatch data";
                QMessageBox::critical(this,tr("Error"),tr("Missmatching data.\nFormat: hh:mm\nIn Example: 08:25 or 12:05"));
                return false;
            }
        }
    }
    return false;
}

/**
 * @brief examDialog::resetGeo
 * Resizes Dialog according to widgets and table content
 */
void examDialog::resetGeo()
{

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();

    int mWidth=0,mHeight=0;
    mHeight += ui->tableWidget->horizontalHeader()->height() + 4;
    for (int i=0,j=0; i < ui->tableWidget->rowCount() && j < ui->tableWidget->columnCount();++i,++j)
    {
        mWidth += ui->tableWidget->columnWidth(j);
        mHeight += ui->tableWidget->rowHeight(i) + 4;

    }
    mHeight += ui->buttonBox->height() + 4;
    mHeight += ui->labelHeader->height() + 4;


    this->setMinimumHeight(mHeight);
//    this->setMaximumHeight(mHeight);

}
