#include "mainscreen.h"
#include "ui_mainscreen.h"

#define VERSION "1.1.0"

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
    delete statusBar;
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

            unlock();
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
                statusBar->setIconConnectionStatus(jceStatusBar::Error);
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
    if (!courseTableMgr->showGraph())
        QMessageBox::critical(this,tr("Error"),tr("You must to load GPA first\nClick on  'Get GPA'"));
}
void MainScreen::on_revertBtn_clicked()
{
    qDebug() << Q_FUNC_INFO;
    if (!isBusy())
    {
        lock();
        courseTableMgr->revertChanges();
        unlock();
    }
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


void MainScreen::on_langButton_clicked()
{
    qDebug() << Q_FUNC_INFO;
    if (data->getLocale() == "en")
    {
        qDebug() << Q_FUNC_INFO << "Changed lang to he";
        data->setLocale("he");

    }else if(data->getLocale() == "he"){
        qDebug() << Q_FUNC_INFO << "Changed lang to en";
        data->setLocale("en");
    }else{
        qCritical() << Q_FUNC_INFO << "currupted data. reset to en";
        data->reset();
        data->setLocale("en");
    }
    checkLocale();


    QMessageBox::information(this,tr("Settings"),tr("Your settings will take effect next time you start the program"),QMessageBox::Ok);

}

void MainScreen::on_creditButton_clicked()
{
    qDebug() << Q_FUNC_INFO;
    QMessageBox::about(this, "About",
                       "<a href='http://liranbg.github.io/JceManager/'>Jce Manager</a> "
                       + QString("Version: " + QString(VERSION)) +"<br><br>"
                       +tr("License: ")+ "LGPL v2.1<br>"
                                         "<br><a href='https://github.com/liranbg/JceManager'>Our GitHub Page</a>"
                       +"<br><br>" + tr("Developed By: ")+
                       "<ul>"+
                       "<li><a href='mailto:liranbg@gmail.com'>"+tr("Liran Ben Gida")+"</a></li>"+
                       "<li><a href='mailto:sagidayan@gmail.com'>"+tr("Sagi Dayan")+"</a></li></ul>"
                       );
}

void MainScreen::on_howtoButton_clicked()
{
    helpBox help(this);
    help.exec();
}

void MainScreen::checkLocale()
{
    qDebug() << Q_FUNC_INFO;
    if (data->getLocale() == "en")
    {
        ui->langButton->setIcon(QIcon(":/icons/us.png"));

    }else if(data->getLocale() == "he"){
        ui->langButton->setIcon(QIcon(":/icons/il.png"));
    }else{
        qCritical() << Q_FUNC_INFO << "currupted data. reset eng";
        data->reset();
        ui->langButton->setIcon(QIcon(":/icons/us.png"));

    }
}
