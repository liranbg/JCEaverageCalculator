#ifndef EXAMDIALOG_H
#define EXAMDIALOG_H

#include <QDialog>
#include <QDateEdit>

#include "calendarExam.h"

namespace Ui {
class examDialog;
}

class examDialog : public QDialog
{
    Q_OBJECT

public:
    explicit examDialog(QWidget *parent,calendarExam * calSchedPtr);
    void initializingDataIntoTable();
    ~examDialog();

private:
    Ui::examDialog *ui;
    calendarExam * exams;
};

#endif // EXAMDIALOG_H
