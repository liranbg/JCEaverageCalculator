#include "calendarDialog.h"
#include "ui_calendarDialog.h"


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

/**
 * @breif This method will return the state of the dates aka User input.
 * @return true - if dates are valid, invalid false.
 */
bool CalendarDialog::ok()
{
    return this->isOK;
}

/**
 * @breif A Slot - triggers when the start calendar widget is selected (Date changed by user)
 * it will change the status label (ui->lbl_status) according to the users input.
 */
void CalendarDialog::on_calStart_selectionChanged()
{
    if(ui->calStart->selectedDate() >= ui->calEnd->selectedDate()) //User input is invalid
    {

        changeLabeStatusIcon(false);
        ui->lbl_status->setText(tr("Invalid dates interval"));
        this->isOK = false;
    }
    else // input is valid
    {
        this->resize(610,310);
        changeLabeStatusIcon(true);
        ui->lbl_status->setText(tr("Looks fine, Click \"OK\""));
        this->isOK = true;
    }
}

/**
 * @breif A Slot triggerd when OK clicked
 */
void CalendarDialog::on_buttonBox_accepted()
{
    if(this->isOK)
      qDebug() << Q_FUNC_INFO << "CalendarDialog: Valid input";
}

void CalendarDialog::on_calEnd_selectionChanged()
{
    if(ui->calStart->selectedDate() >= ui->calEnd->selectedDate())
    {
        changeLabeStatusIcon(false);
        ui->lbl_status->setText(tr("Invalid dates interval"));

        this->isOK = false;
    }
    else
    {
        changeLabeStatusIcon(true);
        ui->lbl_status->setText(tr("Looks ok, Press OK"));
        this->isOK = true;
    }

}

/**
 * @breif changes the status icon according to the... status... smart huh?
 * @param goodOrBad - self explained
 */
void CalendarDialog::changeLabeStatusIcon(bool goodOrBad)
{
    if (goodOrBad == true) //good date!
        iconPixStatus.load(":/icons/iconV.png");
    else
        iconPixStatus.load(":/icons/iconX.png");
    this->ui->labelIconStatus->setPixmap(iconPixStatus);
}

bool CalendarDialog::isExams(){
    return ui->isExam->isChecked();
}
