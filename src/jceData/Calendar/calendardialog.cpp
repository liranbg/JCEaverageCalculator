#include "calendardialog.h"
#include "ui_calendardialog.h"


CalendarDialog::CalendarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalendarDialog)
{
    ui->setupUi(this);
    ui->calEnd->showToday();
    ui->calStart->showToday();

    this->isOK = false;
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

bool CalendarDialog::ok()
{
    return this->isOK;
}

void CalendarDialog::on_calStart_clicked(const QDate &date)
{

}

void CalendarDialog::on_calStart_selectionChanged()
{
    if(ui->calStart->selectedDate() > ui->calEnd->selectedDate()){
        ui->lbl_status->setText("[ X ] The End of the semester is before it starts... ");
        this->isOK = false;
    }else if(ui->calStart->selectedDate() == ui->calEnd->selectedDate()){
        ui->lbl_status->setText("[ ! ] Semester Cannot start and end on the same date... Where are you studying?! :)");
        this->isOK = false;
    }else{
        ui->lbl_status->setText("[ V ] Looks ok, Press OK");
        this->isOK = true;
    }
}

void CalendarDialog::on_buttonBox_accepted()
{
    if(ui->calStart->selectedDate() > ui->calEnd->selectedDate())
        qDebug() << "start bigger than end!";
}

void CalendarDialog::on_calEnd_clicked(const QDate &date)
{

}

void CalendarDialog::on_calStart_activated(const QDate &date)
{
    //null
}

void CalendarDialog::on_calEnd_selectionChanged()
{
    if(ui->calStart->selectedDate() > ui->calEnd->selectedDate()){
        ui->lbl_status->setText("[ X ] The End of the semester is before it starts... ");
        this->isOK = false;
    }else if(ui->calStart->selectedDate() == ui->calEnd->selectedDate()){
        ui->lbl_status->setText("[ ! ] Semester Cannot start and end on the same date... Where are you studying?! :)");
        this->isOK = false;
    }else{
        ui->lbl_status->setText("[ V ] Looks ok, Press OK");
        this->isOK = true;
    }
}
