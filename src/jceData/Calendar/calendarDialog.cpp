#include "calendarDialog.h"
#include "ui_calendardialog.h"


CalendarDialog::CalendarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalendarDialog)
{
    ui->setupUi(this);
    changeLabeStatusIcon(true);//be default the dates are ok, i Updated it according to jce official dates
    this->isOK = true;
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
void CalendarDialog::on_calStart_selectionChanged()
{
    if(ui->calStart->selectedDate() >= ui->calEnd->selectedDate())
    {
        changeLabeStatusIcon(false);
        ui->lbl_status->setText(tr("The end of the semester can NOT be equal or before the semester begin."));
        this->isOK = false;
    }
    else
    {
        changeLabeStatusIcon(true);
        ui->lbl_status->setText(tr("Looks ok, Press OK"));
        this->isOK = true;
    }
}

void CalendarDialog::on_buttonBox_accepted()
{
    if(ui->calStart->selectedDate() > ui->calEnd->selectedDate())
        qDebug() << "start is bigger than end!";
}

void CalendarDialog::on_calEnd_selectionChanged()
{
    if(ui->calStart->selectedDate() >= ui->calEnd->selectedDate())
    {
        changeLabeStatusIcon(false);
        ui->lbl_status->setText(tr("The end of the semester can NOT be equal or before the semester begin."));
        this->isOK = false;
    }
    else
    {
        changeLabeStatusIcon(true);
        ui->lbl_status->setText(tr("Looks ok, Press OK"));
        this->isOK = true;
    }

}

void CalendarDialog::changeLabeStatusIcon(bool goodOrBad)
{
    if (goodOrBad == true) //good date!
        iconPixStatus.load(":/icons/iconV.png");
    else
        iconPixStatus.load(":/icons/iconX.png");
    this->ui->labelIconStatus->setPixmap(iconPixStatus);
}
