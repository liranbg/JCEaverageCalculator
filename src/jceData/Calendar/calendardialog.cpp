#include "calendardialog.h"
#include "ui_calendardialog.h"


CalendarDialog::CalendarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalendarDialog)
{
    ui->setupUi(this);
    ui->calEnd->showToday();
    ui->calStart->showToday();
}

CalendarDialog::~CalendarDialog()
{
    delete ui;
}

QDate CalendarDialog::getStartDate()
{
    return ui->calStart->selectedDate();
}

QDate CalendarDialog::getEndDate()
{
    return ui->calEnd->selectedDate();
}





void CalendarDialog::on_calStart_clicked(const QDate &date)
{
    qDebug() << "FUCK!!!";
}

void CalendarDialog::on_calStart_selectionChanged()
{
    qDebug() << "selection **************************************";
}
