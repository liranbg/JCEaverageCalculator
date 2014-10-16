#ifndef EXAMDIALOG_H
#define EXAMDIALOG_H

#include <QDialog>
#include <QTime>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QScrollBar>
#include "calendarExam.h"

namespace Ui {
class examDialog;
}

/**
 * @brief The examDialog class
 *
 * This class preseting a Dialog with dates, information and time of exams
 *
 * This dialog main goal is to let the user an option to edit and see the containing data of his exam schedule.
 * The user will be able to export the exam schedule into .CSV file through CalendarTab
 *
 * Made By liran ben gida, LiranBG@gmail.com On 08/10/2014
 */
class examDialog : public QDialog
{
    Q_OBJECT

public:
    explicit examDialog(QWidget *parent, calendarExam * calExamPtr);
    void initializingDataIntoTable();
    ~examDialog();

private slots:

    bool upgradeExamsTime(QTableWidgetItem* item);

    void on_pushButtonOk_clicked();

    void on_pushButtonRevert_clicked();

    void on_pushButtonCancel_clicked();

private:

    calendarExamCourse *getExamByRow(int row);
    void resetGeo();

    Ui::examDialog *ui;
    calendarExam * exams;
    calendarExam * examsCpy;
};

#endif // EXAMDIALOG_H
