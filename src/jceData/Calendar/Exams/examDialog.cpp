#include "examDialog.h"
#include "ui_examDialog.h"

examDialog::examDialog(QWidget *parent, calendarExam *calSchedPtr) : QDialog(parent),
    ui(new Ui::examDialog)
{
    ui->setupUi(this);
    exams = calSchedPtr;
    QStringList headLine;
    //SERIAL, NAME, LECTURER, FIELD, TYPE, FIRST_DATE, FIRST_HOUR_BEGIN, SECOND_DATE, SECOND_HOUR_BEGIN

    headLine << tr("Serial") << tr("Course") << tr("Lecturer") << tr("Field") << tr("Type") << tr("First")  << tr("Begin") << tr("Second") << tr("Begin");

    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->setColumnCount(EXAM_SCHEDULE_FIELDS);
    ui->tableWidget->setHorizontalHeaderLabels(headLine);
    ui->tableWidget->setLayoutDirection(Qt::LayoutDirection::RightToLeft);

    this->setModal(true);
}

void examDialog::initializingDataIntoTable()
{
    ui->tableWidget->setRowCount(exams->getExamsCounter());
    int i=0,j=0;
    QTableWidgetItem *lecturer,*name,*type;
    QTableWidgetItem *serial;
    QTableWidgetItem *field;
    QDateEdit *firstDate;
    QTimeEdit *firstHourbegin;
    QDateEdit *secondDate;
    QTimeEdit *secondHourbegin;
    for (calendarExamCourse * tempExam: *exams->getExams())
    {
        j=0;
        lecturer = new QTableWidgetItem();
        lecturer->setData(Qt::EditRole, tempExam->getLecturer());
        name = new QTableWidgetItem();
        name->setData(Qt::EditRole, tempExam->getName());
        type = new QTableWidgetItem();
        type->setData(Qt::EditRole, tempExam->getType());
        serial = new QTableWidgetItem();
        serial->setData(Qt::EditRole, tempExam->getSerialNum());
        field = new QTableWidgetItem();
        field->setData(Qt::EditRole, tempExam->getField());
        firstDate = new QDateEdit();
        firstDate->setDisplayFormat("d/M/yy");
        firstDate->setDate(tempExam->getFirstDate());
        secondDate = new QDateEdit();
        secondDate->setDisplayFormat("d/M/yy");
        secondDate->setDate(tempExam->getSecondDate());
        firstHourbegin = new QTimeEdit();
        firstHourbegin->setTime(tempExam->getFirstHourbegin());
        secondHourbegin = new QTimeEdit();
        secondHourbegin->setTime(tempExam->getSecondHourbegin());

        ui->tableWidget->setItem(i,j++,lecturer);
        ui->tableWidget->setItem(i,j++,name);
        ui->tableWidget->setItem(i,j++,type);
        ui->tableWidget->setItem(i,j++,serial);
        ui->tableWidget->setItem(i,j++,field);
        ui->tableWidget->setCellWidget(i,j++,firstDate);
        ui->tableWidget->setCellWidget(i,j++,firstHourbegin);
        ui->tableWidget->setCellWidget(i,j++,secondDate);
        ui->tableWidget->setCellWidget(i,j++,secondHourbegin);
        i++;
    }
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    this->setMinimumHeight(ui->tableWidget->height());
    this->setMinimumWidth(ui->tableWidget->width());

}
examDialog::~examDialog()
{
    delete ui;
}
