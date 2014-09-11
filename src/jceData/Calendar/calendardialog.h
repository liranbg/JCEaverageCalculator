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
    bool ok();

private slots:
    void on_calStart_activated(const QDate &date);
    void on_calStart_selectionChanged();
    void on_calStart_clicked(const QDate &date);
    void on_buttonBox_accepted();
    void on_calEnd_clicked(const QDate &date);
    void on_calEnd_selectionChanged();

private:
    Ui::CalendarDialog *ui;
    bool isOK;
};

#endif // CALENDARDIALOG_H
