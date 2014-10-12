#include "mainscreen.h"
#include "ui_mainscreen.h"

MainScreen::MainScreen(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainScreen)
{
    ui->setupUi(this);
    this->isBlocked = false;
    //Login Tab
    iconPix.load(":/icons/iconX.png");
    ui->pswdLineEdit->setEchoMode((QLineEdit::Password));
    ui->labelUsrInputStatus->setVisible(false);
    ui->labelPswInputStatus->setVisible(false);
    ui->labelUsrInputStatus->setPixmap(iconPix);
    ui->labelPswInputStatus->setPixmap(iconPix);

    //StatusBar
    statusBar = new jceStatusBar(this);
    this->setStatusBar(statusBar);

    //GPA Tab
    ui->avgLCD->setPalette(QPalette(QPalette::WindowText,Qt::blue));

    //Pointer allocating
    qDebug() << Q_FUNC_INFO << "Allocating pointers";
    this->userLoginSetting = new user("","");
    this->courseTableMgr = new coursesTableManager(ui->coursesTable,userLoginSetting);
    this->loginHandel = new loginHandler(userLoginSetting, ui->loginButton, statusBar);
    this->calendar = new CalendarManager(this,ui->calendarGridLayoutMain);
    this->data = new SaveData();

    //check login File
    if (data->isSaved())
    {
        qDebug() << Q_FUNC_INFO << "Loading data from file";
        ui->usrnmLineEdit->setText(data->getUsername());
        ui->pswdLineEdit->setText(data->getPassword());
        ui->keepLogin->setChecked(true);
    }

    //language
    qDebug() << Q_FUNC_INFO << "Checking locale";
    checkLocale();
    qDebug() << Q_FUNC_INFO << "Ready.";

}
MainScreen::~MainScreen()
{
    delete calendar;
    delete courseTableMgr;
    delete userLoginSetting;
    delete loginHandel;
    delete data;
    delete ui;
}

//EVENTS ON LOGIN TAB
void MainScreen::on_loginButton_clicked()
{
    if (!isBusy())
    {
        lock();
        qDebug() << Q_FUNC_INFO <<  "in: " << ui->tabWidget->currentWidget()->objectName();
        if ((ui->usrnmLineEdit->text().isEmpty()) || (ui->pswdLineEdit->text().isEmpty()))
        {
            if (ui->usrnmLineEdit->text().isEmpty())
            {
                ui->labelUsrInputStatus->setVisible(true);
                qDebug() << Q_FUNC_INFO << "username input is empty";
            }
            else
                ui->labelUsrInputStatus->setVisible(false);
            if (ui->pswdLineEdit->text().isEmpty())
            {
                ui->labelPswInputStatus->setVisible(true);
                qDebug() << Q_FUNC_INFO << "password input is empty";
            }
            else
                ui->labelPswInputStatus->setVisible(false);
            return;
        }
        else
        {
            ui->labelUsrInputStatus->setVisible(false);
            ui->labelPswInputStatus->setVisible(false);
        }
        qDebug() << Q_FUNC_INFO <<  "login session start";
        if (this->loginHandel->login(ui->usrnmLineEdit->text(),ui->pswdLineEdit->text()) == true)
        {
            qDebug() << Q_FUNC_INFO <<  "login session end with true";
            ui->pswdLineEdit->setDisabled(true);
            ui->usrnmLineEdit->setDisabled(true);
            if (ui->keepLogin->isChecked())
            {
                qDebug() << Q_FUNC_INFO <<  "saving data";
                data->setUsername(ui->usrnmLineEdit->text());
                data->setPassword(ui->pswdLineEdit->text());
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO <<  "login session end with false";
            ui->pswdLineEdit->setDisabled(false);
            ui->usrnmLineEdit->setDisabled(false);

        }
        unlock();
    }


}
void MainScreen::on_keepLogin_clicked()
{
    qDebug() << Q_FUNC_INFO <<  "in: " << ui->tabWidget->currentWidget()->objectName();
    if (ui->keepLogin->isChecked())
    {
        qDebug() << Q_FUNC_INFO <<  "saving data";
        data->setUsername(ui->usrnmLineEdit->text());
        data->setPassword(ui->pswdLineEdit->text());
    }
    else
        data->reset();
}
void MainScreen::on_usrnmLineEdit_editingFinished()
{
    qDebug() << Q_FUNC_INFO <<  "in: " << ui->tabWidget->currentWidget()->objectName();
    ui->usrnmLineEdit->setText(ui->usrnmLineEdit->text().toLower());
}

//EVENTS ON GPA TAB
void MainScreen::on_ratesButton_clicked()
{
    qDebug() << Q_FUNC_INFO <<  "in: " << ui->tabWidget->currentWidget()->objectName();
    if (!isBusy())
    {
        lock();
        if (!checkIfValidDates())
        {
            qWarning() << Q_FUNC_INFO << "Invalid dates! return";
            QMessageBox::critical(this,tr("Error"),tr("Invalid Dates.\nMake Sure everything is correct and try again"));
            return;
        }
        QString pageString;
        int status = 0;
        if (loginHandel->isLoggedInFlag())
        {
            statusBar->setIconConnectionStatus(jceStatusBar::Ready);
            if ((status = loginHandel->makeGradeRequest(ui->spinBoxCoursesFromYear->value(),
                                                        ui->spinBoxCoursesToYear->value(),ui->spinBoxCoursesFromSemester->value(),
                                                        ui->spinBoxCoursesToSemester->value())) == jceLogin::JCE_PAGE_PASSED)
            {
                qDebug() << Q_FUNC_INFO <<  "grade page is ready";
                statusBar->setIconConnectionStatus(jceStatusBar::Inserting);
                pageString = loginHandel->getCurrentPageContect();
                courseTableMgr->setCoursesList(pageString);
                courseTableMgr->insertJceCoursesIntoTable();
                statusBar->setIconConnectionStatus(jceStatusBar::Done);
            }
            else if (status == jceLogin::JCE_NOT_CONNECTED)
            {
                qWarning() << Q_FUNC_INFO << "not connected";
                QApplication::restoreOverrideCursor();
                QMessageBox::critical(this,tr("Error"),tr("Not Connected"));
                statusBar->setIconConnectionStatus(jceStatusBar::Disconnected);
            }
            else
            {
                statusBar->setIconConnectionStatus(jceStatusBar::ERROR);
                qCritical() << Q_FUNC_INFO << "grade get ended with" << status;
            }
        }
        unlock();
    }
}
bool MainScreen::checkIfValidDates()
{
    bool flag = false;
    if (ui->spinBoxCoursesFromYear->value() < ui->spinBoxCoursesToYear->value())
    {
        //doesnt matter what is the semester, its valid!
        flag = true;
    }
    else if ((ui->spinBoxCoursesFromYear->value() == ui->spinBoxCoursesToYear->value()))
    {
        //semester from must be equal or less than to semester
        if (ui->spinBoxCoursesFromSemester->value() <= ui->spinBoxCoursesToSemester->value())
            flag = true;
    }
    return flag;
}

void MainScreen::on_checkBoxCoursesInfluence_toggled(bool checked)
{
    qDebug() << Q_FUNC_INFO <<  "only main courses toggeled" << checked;
    this->userLoginSetting->setInfluenceCourseOnly(checked);
    this->courseTableMgr->influnceCourseChanged(checked);
}
void MainScreen::on_spinBoxCoursesFromYear_valueChanged(int arg1)
{
    ui->spinBoxCoursesFromYear->setValue(arg1);
}
void MainScreen::on_spinBoxCoursesToYear_valueChanged(int arg1)
{
    ui->spinBoxCoursesToYear->setValue(arg1);

}
void MainScreen::on_spinBoxCoursesFromSemester_valueChanged(int arg1)
{
    ui->spinBoxCoursesFromSemester->setValue(arg1%4);
}
void MainScreen::on_spinBoxCoursesToSemester_valueChanged(int arg1)
{
    ui->spinBoxCoursesToSemester->setValue(arg1%4);
}
void MainScreen::on_coursesTable_itemChanged(QTableWidgetItem *item)
{
    if (this->courseTableMgr->changes(item->text(),item->row(),item->column()))
        ui->avgLCD->display(courseTableMgr->getAvg());
    else
    {
        qWarning() << Q_FUNC_INFO << "missmatch data";
        QMessageBox::critical(this,tr("Error"),tr("Missmatching data"));
    }
}
void MainScreen::on_clearTableButton_clicked()
{
    qDebug() << Q_FUNC_INFO <<  "in: " << ui->tabWidget->currentWidget()->objectName();
    courseTableMgr->clearTable();
    ui->avgLCD->display(courseTableMgr->getAvg());
}

void MainScreen::on_graphButton_clicked()
{
    qDebug() << Q_FUNC_INFO;
    courseTableMgr->showGraph();
}


//EVENTS ON CALENDAR TAB
void MainScreen::on_examsBtn_clicked()
{
    calendar->showExamDialog();
}
void MainScreen::on_getCalendarBtn_clicked()
{
    qDebug() << Q_FUNC_INFO <<  "in: " << ui->tabWidget->currentWidget()->objectName();
    if (!isBusy())
    {
        lock();
        int status = 0;
        QString page;
        if (loginHandel->isLoggedInFlag())
        {
            statusBar->setIconConnectionStatus(jceStatusBar::Ready);
            if ((status = loginHandel->makeCalendarRequest(ui->spinBoxYear->value(),ui->spinBoxSemester->value())) == jceLogin::JCE_PAGE_PASSED)
            {
                statusBar->setIconConnectionStatus(jceStatusBar::Inserting);
                calendar->resetTable();
                page = loginHandel->getCurrentPageContect();
                calendar->setCalendar(page);

                qDebug() << Q_FUNC_INFO <<  "calendar is loaded";

                //auto getting exam
                if (loginHandel->isLoggedInFlag())
                {
                    if ((status = loginHandel->makeExamsScheduleRequest(ui->spinBoxYear->value(),ui->spinBoxSemester->value())) == jceLogin::JCE_PAGE_PASSED)
                    {
                        page = loginHandel->getCurrentPageContect();
                        calendar->setExamsSchedule(page);
                        qDebug() << Q_FUNC_INFO <<  "exams schedule is loaded";
                        statusBar->setIconConnectionStatus(jceStatusBar::Done);
                    }
                    else if (status == jceLogin::JCE_NOT_CONNECTED)
                    {
                        qWarning() << Q_FUNC_INFO <<  "not connected";
                        QApplication::restoreOverrideCursor();
                        QMessageBox::critical(this,tr("Error"),tr("Not Connected"));
                        statusBar->setIconConnectionStatus(jceStatusBar::Disconnected);
                    }
                    else
                        qCritical() << Q_FUNC_INFO << "exams request get ended with" << status;
                }
                else if (status == jceLogin::JCE_NOT_CONNECTED)
                {
                    qWarning() << Q_FUNC_INFO <<  "not connected";
                    QApplication::restoreOverrideCursor();
                    QMessageBox::critical(this,tr("Error"),tr("Not Connected"));
                    statusBar->setIconConnectionStatus(jceStatusBar::Disconnected);
                }
                else
                    qCritical() << Q_FUNC_INFO << "calendar get ended with" << status;
            }
        }
        unlock();
    }
}
void MainScreen::on_exportToCVSBtn_clicked()
{
    qDebug() << Q_FUNC_INFO <<  "in: " << ui->tabWidget->currentWidget()->objectName();
    if (loginHandel->isLoggedInFlag())
    {
        this->calendar->exportCalendarCSV();

    }
}
//EVENTS ON MENU BAR
void MainScreen::on_actionCredits_triggered()
{
    qDebug() << Q_FUNC_INFO;
    QMessageBox::about(this, "About",
                       "<a href='http://liranbg.github.io/JceManager/'>Jce Manager</a> v1.0.0<br><br>"
                       +tr("License:")+
                       "<br>GNU LESSER GENERAL PUBLIC LICENSE V2.1<br>"
                       +"<br>"+
                       "<a href='https://github.com/liranbg/JceManager'>JceManager Repository</a>"+
                       "<br><br>"
                       +tr("Powered By: ")+
                       "<a href='https://github.com/liranbg/jceConnection'> Jce Connection</a><br><br>"
                       +tr("Developed By")+
                       ":<ul>"+
                       "<li><a href='mailto:liranbg@gmail.com'>"+tr("Liran")+"</a></li>"+
                       "<li><a href='mailto:sagidayan@gmail.com'>"+tr("Sagi")+"</a></li></ul>"
                       );
}
void MainScreen::on_actionExit_triggered()
{
    qDebug() << Q_FUNC_INFO;
    exit(0);
}
void MainScreen::on_actionHow_To_triggered()
{
    qDebug() << Q_FUNC_INFO;
    QMessageBox::information(this,"How To",
                             "<b>"
                             +tr("Help Guide")+
                             "</b><ul>"
                             +tr("<br><li>Login: <ul><li>Type your username and password and click Login.</li><li>Once you are connected, you will see a green ball in the right buttom panel.</li></ul></li>")
                             +tr("<br><li>Getting GPA sheet<ul><li>Click on GPA Tab</li><li> Select your dates and click on Add</li></ul></li>")
                             +tr("<br><li>Average Changing<ul><li>Change one of your grade and see the average in the buttom panel changing.</li></ul></li>")
                             +tr("<br><li>Getting Calendar<ul><li>Click on Calendar Tab</li><li> Select your dates and click on Get Calendar</li></ul></li>")
                             +tr("<br><li>For exporting your calendar to a .CSV file:<ul><li>Do previous step and continue to next step</li><li> Click on Export to CSV</li><li>Select your dates and click OK</li><li>Once you're Done, go on your calendar and import your csv file</li></li>")+
                             "<br><br>"
                             +tr("<b>For more information, please visit us at: <a href='http://liranbg.github.io/JceManager/'>Jce Manager site</a></b>"));
}
void MainScreen::on_actionHebrew_triggered()
{
    qDebug() << Q_FUNC_INFO;
    if (ui->actionEnglish->isChecked() || ui->actionOS_Default->isChecked())
    {
        ui->actionEnglish->setChecked(false);
        ui->actionOS_Default->setChecked(false);
        qDebug() << Q_FUNC_INFO << "Changed Language to hebrew";
        data->setLocal("he");
        QMessageBox::information(this,tr("Settings"),tr("Your settings will take effect next time you start the program"),QMessageBox::Ok);
    }
    else
        ui->actionHebrew->setChecked(true);
}
void MainScreen::on_actionEnglish_triggered()
{
    qDebug() << Q_FUNC_INFO;
    if (ui->actionHebrew->isChecked() || ui->actionOS_Default->isChecked())
    {
        ui->actionHebrew->setChecked(false);
        ui->actionOS_Default->setChecked(false);
        qDebug() << Q_FUNC_INFO << "Changed Language to English";
        data->setLocal("en");
        QMessageBox::information(this,"Settings",tr("Your settings will take effect next time you start the program"),QMessageBox::Ok);
    }
    else
        ui->actionEnglish->setChecked(true);
}
void MainScreen::on_actionOS_Default_triggered()
{
    qDebug() << Q_FUNC_INFO;
    if (ui->actionHebrew->isChecked() || ui->actionEnglish->isChecked())
    {
        ui->actionHebrew->setChecked(false);
        ui->actionEnglish->setChecked(false);
        qDebug() << Q_FUNC_INFO << "Changed Language to OS Default";
        data->setLocal("default");
        QMessageBox::information(this,tr("Settings"),tr("Your settings will take effect next time you start the program"),QMessageBox::Ok);
    }
    else
        ui->actionOS_Default->setChecked(true);
}
void MainScreen::checkLocale()
{
    if(data->getLocal() == "en")
    {
        ui->actionHebrew->setChecked(false);
        ui->actionOS_Default->setChecked(false);
        ui->actionEnglish->setChecked(true);
    }else if(data->getLocal() == "he"){
        ui->actionHebrew->setChecked(true);
        ui->actionOS_Default->setChecked(false);
        ui->actionEnglish->setChecked(false);
    }else{
        ui->actionHebrew->setChecked(false);
        ui->actionOS_Default->setChecked(true);
        ui->actionEnglish->setChecked(false);
    }
}

//MAIN SCREEN
void MainScreen::on_labelMadeBy_linkActivated(const QString &link)
{
    qDebug() << Q_FUNC_INFO << "link: " << link;
}
/**
 * @brief MainScreen::lock
 * turn isblocked into true to avoid multiple requests and to cause errors
 */
void MainScreen::lock()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    this->isBlocked = true;
}
void MainScreen::unlock()
{
    QApplication::restoreOverrideCursor();
    this->isBlocked = false;
}
bool MainScreen::isBusy()
{
    return this->isBlocked;
}
