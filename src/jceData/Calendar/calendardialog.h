#ifndef CALENDARDIALOG_H
#define CALENDARDIALOG_H

#include <QDialog>
#include <QDate>
#include <QDebug>

namespace Ui {
class CalendarDialog;
}

class CalendarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalendarDialog(QWidget *parent = 0);
    ~CalendarDialog();
    QDate getStartDate();
    QDate getEndDate();

private slots:
    void on_calStart_activated(const QDate &date);

    void on_calStart_selectionChanged();

    void on_calStart_clicked(const QDate &date);

private:
    Ui::CalendarDialog *ui;
};

#endif // CALENDARDIALOG_H
