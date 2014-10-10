/**
 * JCE Manager (C)
 *
 * This QDialog widget will hold the dates of a Semester, Start and End.
 *
 * this dialog will help the csv_exporter class to export a CSV file that will contain
 * all the courses within that period of time.
 */

#ifndef CALENDARDIALOG_H
#define CALENDARDIALOG_H

#include <QDialog>
#include <QDate>
#include <QDebug>

namespace Ui {
class CalendarDialog;
}

/**
 * @brief The CalendarDialog class
 *
 * This class preseting a Dialog with selection of dates
 * The user has to choose between a starting point of semester to the end.
 *
 * This dialog main goal is to let the user an option to export his CSV
 * containing data of his schedule.
 *
 * Made By Sagi Dayan, sagidayan@gmail.com On 22/09/2014
 */
class CalendarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalendarDialog(QWidget *parent = 0);
    ~CalendarDialog();
    QDate getStartDate();
    QDate getEndDate();
    bool ok();

private slots:
    void on_calStart_selectionChanged();
    void on_calEnd_selectionChanged();
    void on_buttonBox_accepted();

private:
    void changeLabeStatusIcon(bool goodOrBad);
    Ui::CalendarDialog *ui;
    bool isOK;
    QPixmap iconPixStatus;
};

#endif // CALENDARDIALOG_H
