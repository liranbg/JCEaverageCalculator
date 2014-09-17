#include "mainscreen.h"
#include "ui_mainscreen.h"


MainScreen::MainScreen(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainScreen)
{
    ui->setupUi(this);
    //this->setFixedSize(this->size()); //main not resizeable

    //Login Tab
    QPixmap iconPix;
    iconPix.load(":/icons/iconX.png");
    ui->pswdLineEdit->setEchoMode((QLineEdit::Password));
    ui->labelUsrInputStatus->setVisible(false);
    ui->labelPswInputStatus->setVisible(false);
    ui->labelUsrInputStatus->setPixmap(iconPix);
    ui->labelPswInputStatus->setPixmap(iconPix);

    //Status Bar
    ui->statusBar->setStyleSheet("QStatusBar::item { border: 0px solid black };");
    ButtomStatusLabel = new QLabel(this);
    statusLabel = new QLabel(this);
    ui->statusBar->setMaximumSize(this->geometry().width(),STATUS_ICON_HEIGH);
    ui->statusBar->addPermanentWidget(ButtomStatusLabel,0);
    ui->statusBar->addPermanentWidget(statusLabel,1);
    setLabelConnectionStatus(jceLogin::jceStatus::JCE_NOT_CONNECTED);

    //Course, Setting, Calendar Tab
    calendarSchedule * calendarSchedulePtr = new calendarSchedule();
    ui->calendarGridLayoutMain->addWidget(calendarSchedulePtr);
    ui->avgLCD->setPalette(QPalette(QPalette::WindowText,Qt::blue));

    //Pointer allocating
    this->userLoginSetting = new user("","");
    this->courseTableMgr = new coursesTableManager(ui->coursesTable,userLoginSetting);
    this->loginHandel = new loginHandler(userLoginSetting);
    this->calendar = new CalendarManager(calendarSchedulePtr);
    this->data = new SaveData();

    //check login File
    if (data->isSaved())
    {
        ui->usrnmLineEdit->setText(data->getUsername());
        ui->pswdLineEdit->setText(data->getPassword());
        ui->keepLogin->setChecked(true);
    }

    //Local Check and ui setting.
    checkLocale();



}

MainScreen::~MainScreen()
{
    delete ButtomStatusLabel;
    delete statusLabel;
    delete calendar;
    delete courseTableMgr;
    delete userLoginSetting;
    delete loginHandel;
    delete ui;
    delete data;
}
//EVENTS ON STATUS BAR
void MainScreen::setLabelConnectionStatus(jceLogin::jceStatus statusDescription)
{
    QPixmap iconPix;
    switch (statusDescription)
    {
    case jceLogin::jceStatus::JCE_START_VALIDATING_PROGRESS:
        iconPix.load(":/icons/blueStatusIcon.png");
        statusLabel->setText(tr("Connecting"));
        break;
    case jceLogin::jceStatus::JCE_YOU_ARE_IN:
        iconPix.load(":/icons/greenStatusIcon.png");
        statusLabel->setText(tr("Connected"));
        break;
    default:
        iconPix.load(":/icons/redStatusIcon.png");
        statusLabel->setText(tr("Disconnected"));
        break;
    }
    ButtomStatusLabel->setPixmap(iconPix);

    this->repaint();
}
//EVENTS ON LOGIN TAB
void MainScreen::on_loginButton_clicked()
{
    if (loginHandel->isLoggedInFlag())
        uiSetDisconnectMode();
    else
        uiSetConnectMode();
}
void MainScreen::on_keepLogin_clicked()
{
    if (ui->keepLogin->isChecked())
    {
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
void MainScreen::uiSetDisconnectMode()
{
    setLabelConnectionStatus(jceLogin::jceStatus::JCE_NOT_CONNECTED);
    ui->usrnmLineEdit->setText("");
    ui->pswdLineEdit->setText("");
    ui->usrnmLineEdit->setEnabled(true);
    ui->pswdLineEdit->setEnabled(true);

    loginHandel->makeDisconnectionRequest();
    ui->loginButton->setText(tr("&Login"));
    ui->getCalendarBtn->setDisabled(true);
    ui->exportToCVSBtn->setDisabled(true);
    ui->ratesButton->setDisabled(true);
    return;
}
void MainScreen::uiSetConnectMode()
{
    QString username;
    QString password;
    if ((ui->usrnmLineEdit->text().isEmpty()) || (ui->pswdLineEdit->text().isEmpty()))
    {
        if (ui->usrnmLineEdit->text().isEmpty())
        {
            ui->labelUsrInputStatus->setVisible(true);
            qDebug() << "error, username input is empty";
        }
        else
            ui->labelUsrInputStatus->setVisible(false);
        if (ui->pswdLineEdit->text().isEmpty())
        {
            ui->labelPswInputStatus->setVisible(true);
            qDebug() << "error, password input is empty";
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
    setLabelConnectionStatus(jceLogin::jceStatus::JCE_START_VALIDATING_PROGRESS);

    username = ui->usrnmLineEdit->text();
    password = ui->pswdLineEdit->text();

    ui->usrnmLineEdit->setDisabled(true);
    ui->pswdLineEdit->setDisabled(true);

    userLoginSetting->setUsername(username);
    userLoginSetting->setPassword(password);

    this->loginHandel->setPointers(statusLabel,ui->pswdLineEdit,ui->usrnmLineEdit);
    if (loginHandel->makeConnection() == true)
    {
        setLabelConnectionStatus(jceLogin::jceStatus::JCE_YOU_ARE_IN);
        ui->loginButton->setText(tr("&Logout"));
        ui->ratesButton->setEnabled(true);
        ui->CoursesTab->setEnabled(true);
        ui->exportToCVSBtn->setEnabled(true);
        ui->getCalendarBtn->setEnabled(true);

    }
    else
    {
        uiSetDisconnectMode();
    }
}
//EVENTS ON GPA TAB
void MainScreen::on_ratesButton_clicked()
{
    if (!checkIfValidDates())
    {
        qWarning() << "MainScreen::on_ratesButton_clicked; Invalid dates! ";
        QMessageBox::critical(this,tr("Error"),tr("Invalid Dates.\nMake Sure everything is correct and try again"));
        return;
    }
    QString pageString;
    int status = 0;
    if (loginHandel->isLoggedInFlag())
    {
        if ((status = loginHandel->makeGradeRequest(ui->spinBoxCoursesFromYear->value(),ui->spinBoxCoursesToYear->value(),ui->spinBoxCoursesFromSemester->value(),ui->spinBoxCoursesToSemester->value())) == jceLogin::JCE_GRADE_PAGE_PASSED)
        {
            pageString = loginHandel->getCurrentPageContect();
            courseTableMgr->setCoursesList(pageString);
            courseTableMgr->insertJceCoursesIntoTable();
        }
        else if (status == jceLogin::JCE_NOT_CONNECTED)
        {
            QMessageBox::critical(this,tr("Error"),tr("Not Connected"));
        }
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
        QMessageBox::critical(this,tr("Error"),tr("Missmatching data"));
}
void MainScreen::on_clearTableButton_clicked()
{
    courseTableMgr->clearTable();
    ui->avgLCD->display(courseTableMgr->getAvg());
}
//EVENTS ON CALENDAR TAB
void MainScreen::on_getCalendarBtn_clicked()
{
    int status = 0;
    if (loginHandel->isLoggedInFlag())
    {
        if ((status = loginHandel->makeCalendarRequest(ui->spinBoxYear->value(),ui->spinBoxSemester->value())) == jceLogin::JCE_GRADE_PAGE_PASSED)
        {
            //Use it for debug. add plain text and change the object name to 'plainTextEdit' so you will get the html request
            //ui->plainTextEdit->setPlainText(loginHandel->getCurrentPageContect());
            calendar->resetTable();
            calendar->setCalendar(loginHandel->getCurrentPageContect());
        }

        else if (status == jceLogin::JCE_NOT_CONNECTED)
        {
            QMessageBox::critical(this,tr("Error"),tr("Not Connected"));
        }
    }
}
void MainScreen::on_exportToCVSBtn_clicked()
{
    if (loginHandel->isLoggedInFlag())
    {
        if(!this->calendar->exportCalendarCSV())
        {
            QMessageBox::critical(this,tr("Error"),"No Calendar was loaded."); //Need Translation
        }
    }
}

//EVENTS ON MENU BAR
void MainScreen::on_actionCredits_triggered()
{
    QMessageBox::about(this, "About", tr("CREDITS-ROOL-UP1")  + " v1.0<br><br>"
                       + tr("CREDITS-ROOL-UP2")+"<br>GNU LESSER GENERAL PUBLIC LICENSE V2<br>"
                       + tr("CREDITS-ROOL-UP3")+"<br>"
                                                "<a href='https://github.com/liranbg/jceAverageCalculator'>jceAverageCalculator Repository</a>"
                                                "<br><br>"+tr("CREDITS-ROOL-UP4")+"<a href='https://github.com/liranbg/jceConnection'> Jce Connection</a><br><br>"
                       +tr("DevBy")+":"
                                    "<ul>"
                                    "<li><a href='mailto:liranbg@gmail.com'>"+tr("Liran")+"</a></li>"
                                                                                          "<li><a href='mailto:sagidayan@gmail.com'>"+tr("Sagi")+"</a></li>"
                                                                                                                                                 "</ul>");
}
void MainScreen::on_actionExit_triggered()
{
    exit(0);
}
void MainScreen::on_actionHow_To_triggered()
{
    QMessageBox::information(this,"How To",
                             "<b>How To..</b>"
                             "<ul>"
                             "<br><li>"+tr("HELP1")+"</li>"
                                                    "<br><li>"+tr("HELP2")+"</li>"
                                                                           "<br><li>"+tr("HELP3")+"</li>"
                                                                                                  "<br><li>"+tr("HELP4")+"</li>"
                                                                                                                         "<br><li>"+tr("HELP5")+"</li>"
                                                                                                                                                "<br><br>"+tr("HELP6")+
                             "</ul>");

}




void MainScreen::on_actionHebrew_triggered()
{
    if (ui->actionEnglish->isChecked() || ui->actionOS_Default->isChecked())
    {
        ui->actionEnglish->setChecked(false);
        ui->actionOS_Default->setChecked(false);
        qDebug() << "Changed Language to hebrew";
        data->setLocal("he");
        QMessageBox::information(this,tr("Settings"),tr("Your settings will take effect next time you start the program"),QMessageBox::Ok);
    }
    else
        ui->actionHebrew->setChecked(true);
}

void MainScreen::on_actionEnglish_triggered()
{
    if (ui->actionHebrew->isChecked() || ui->actionOS_Default->isChecked())
    {
        ui->actionHebrew->setChecked(false);
        ui->actionOS_Default->setChecked(false);
        qDebug() << "Changed Language to English";
        data->setLocal("en");
        QMessageBox::information(this,"Settings",tr("Your settings will take effect next time you start the program"),QMessageBox::Ok);
    }
    else
        ui->actionEnglish->setChecked(true);
}


void MainScreen::on_actionOS_Default_triggered()
{
    if (ui->actionHebrew->isChecked() || ui->actionEnglish->isChecked())
    {
        ui->actionHebrew->setChecked(false);
        ui->actionEnglish->setChecked(false);
        qDebug() << "Changed Language to OS Default";
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
