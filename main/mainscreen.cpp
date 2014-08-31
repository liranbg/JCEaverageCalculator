#include "mainscreen.h"
#include "ui_mainscreen.h"


MainScreen::MainScreen(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainScreen)
{
    ui->setupUi(this);

    //this->setFixedSize(this->size()); //main not resizeable


    //Login Tab
    ui->pswdLineEdit->setEchoMode((QLineEdit::Password));

    //Status Bar
    ui->statusBar->setStyleSheet("QStatusBar::item { border: 0px solid black };");
    ButtomStatusLabel = new QLabel(this);
    statusLabel = new QLabel(this);
    ui->statusBar->setMaximumSize(this->geometry().width(),StatusIconHeight);
    ui->statusBar->addPermanentWidget(ButtomStatusLabel,0);
    ui->statusBar->addPermanentWidget(statusLabel,1);
    setLabelConnectionStatus(jceLogin::jceStatus::JCE_NOT_CONNECTED);

    //Course, Setting, Calendar Tab
    ui->CoursesTab->setDisabled(true);
    ui->avgLCD->setPalette(QPalette(QPalette::WindowText,Qt::blue));



    //Pointer allocating
    this->userLoginSetting = new user("","");
    this->courseTableMgr = new coursesTableManager(ui->coursesTable,userLoginSetting);
    this->loginHandel = new loginHandler(userLoginSetting);
    this->calendar = new CalendarManager(ui->calendartext);

    //check login File
    SaveData::init();
    if (SaveData::isSaved())
    {
        ui->usrnmLineEdit->setText(SaveData::getUsername());
        ui->pswdLineEdit->setText(SaveData::getPassword());
        ui->keepLogin->setChecked(true);
    }
}

MainScreen::~MainScreen()
{
    delete userLoginSetting;
    delete loginHandel;
    delete ui;
}
void MainScreen::on_loginButton_clicked()
{
    if (loginHandel->isLoggedInFlag())
        uiSetDisconnectMode();

    else
        uiSetConnectMode();

}
void MainScreen::on_pushButton_clicked()
{
    int status = 0;
    if (loginHandel->isLoggedInFlag())
    {
        if ((status = loginHandel->makeCalendarRequest(ui->spinBoxYear->value(),ui->spinBoxSemester->value())) == jceLogin::JCE_GRADE_PAGE_PASSED)
        {
            //Use it for debug. add plain text and change the object name to 'plainTextEdit' so you will get the html request
            //ui->plainTextEdit->setPlainText(loginHandel->getCurrentPageContect());
            calendar->setCalendar(loginHandel->getCurrentPageContect().toStdString());
        }

        else if (status == jceLogin::JCE_NOT_CONNECTED)
        {
            QMessageBox::critical(this,tr("Error"),tr("Not Connected"));
        }
    }
}

void MainScreen::on_ratesButton_clicked()
{
    std::string pageString;
    int status = 0;
    if (loginHandel->isLoggedInFlag())
    {
        if ((status = loginHandel->makeGradeRequest(ui->spinBoxCoursesFromYear->value(),ui->spinBoxCoursesToYear->value(),ui->spinBoxCoursesFromSemester->value(),ui->spinBoxCoursesToSemester->value())) == jceLogin::JCE_GRADE_PAGE_PASSED)
        {
            pageString = loginHandel->getCurrentPageContect().toStdString();
            courseTableMgr->setCoursesList(pageString);
            courseTableMgr->insertJceCoursesIntoTable();
        }
        else if (status == jceLogin::JCE_NOT_CONNECTED)
        {
            QMessageBox::critical(this,tr("Error"),tr("Not Connected"));
        }
    }



}
void MainScreen::on_checkBoxCoursesInfluence_toggled(bool checked)
{
    this->userLoginSetting->setInfluenceCourseOnly(checked);
    this->courseTableMgr->influnceCourseChanged(checked);
}
void MainScreen::on_spinBoxCoursesFromYear_editingFinished()
{
    if (ui->spinBoxCoursesFromYear->value() > ui->spinBoxCoursesToYear->value())
    {
        ui->spinBoxCoursesFromYear->setValue(ui->spinBoxCoursesToYear->value());
        ui->spinBoxCoursesFromYear->setFocus();
    }

}
void MainScreen::on_spinBoxCoursesToYear_editingFinished()
{
    if (ui->spinBoxCoursesFromYear->value() > ui->spinBoxCoursesToYear->value())
    {
        ui->spinBoxCoursesToYear->setValue(ui->spinBoxCoursesFromYear->value());
        ui->spinBoxCoursesToYear->setFocus();

    }
}
void MainScreen::on_spinBoxCoursesFromSemester_editingFinished()
{
    if (ui->spinBoxCoursesFromYear->value() == ui->spinBoxCoursesToYear->value())
    {
        if (ui->spinBoxCoursesFromSemester->value() > ui->spinBoxCoursesToSemester->value())
        {
            ui->spinBoxCoursesFromSemester->setValue(ui->spinBoxCoursesToSemester->value());
            ui->spinBoxCoursesFromSemester->setFocus();
        }
    }
}
void MainScreen::on_spinBoxCoursesToSemester_editingFinished()
{
    if (ui->spinBoxCoursesFromYear->value() == ui->spinBoxCoursesToYear->value())
    {
        if (ui->spinBoxCoursesFromSemester->value() > ui->spinBoxCoursesToSemester->value())
        {
            ui->spinBoxCoursesToSemester->setValue(ui->spinBoxCoursesFromSemester->value());
            ui->spinBoxCoursesToSemester->setFocus();
        }
    }
}

void MainScreen::on_coursesTable_itemChanged(QTableWidgetItem *item)
{
    if (this->courseTableMgr->changes(item->text(),item->row(),item->column()))
        ui->avgLCD->display(courseTableMgr->getAvg());
    else
        QMessageBox::critical(this,"Error","Missmatching data");
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
    ui->loginButton->setText("&Login");
    this->ui->ratesButton->setDisabled(true);
    return;
}

void MainScreen::uiSetConnectMode() //fix before distrbute
{
    string username;
    string password;
    if ((ui->usrnmLineEdit->text().isEmpty()) || (ui->pswdLineEdit->text().isEmpty()))
    {
        //add icon near to username and password to mark it
        return;
    }
    setLabelConnectionStatus(jceLogin::jceStatus::JCE_START_VALIDATING_PROGRESS);

    username = ui->usrnmLineEdit->text().toStdString();
    password = ui->pswdLineEdit->text().toStdString();

    ui->usrnmLineEdit->setDisabled(true);
    ui->pswdLineEdit->setDisabled(true);

    userLoginSetting->setUsername(username);
    userLoginSetting->setPassword(password);

    this->loginHandel->setPointers(statusLabel,ui->pswdLineEdit,ui->usrnmLineEdit);
    if (loginHandel->makeConnection() == true)
    {
        setLabelConnectionStatus(jceLogin::jceStatus::JCE_YOU_ARE_IN);
        ui->loginButton->setText("&Logout");
        this->ui->ratesButton->setEnabled(true);
        ui->CoursesTab->setEnabled(true);

    }
    else
    {
        uiSetDisconnectMode();
    }
}
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
                       "</ul>"
                       +tr("Tnks")+ "... :"
                       "<ul>"
                       "<li><a href='mailto:nadav2051@gmail.com'>"+tr("Nadav")+"</a></li>"
                       "</ul>");
}

void MainScreen::on_clearTableButton_clicked()
{

    courseTableMgr->clearTable();
    ui->avgLCD->display(courseTableMgr->getAvg());
}

void MainScreen::on_actionExit_triggered()
{
    exit(0);
}


void MainScreen::on_keepLogin_clicked()
{
    if (ui->keepLogin->isChecked())
        SaveData::save(ui->usrnmLineEdit->text(),ui->pswdLineEdit->text());

    else
        SaveData::deleteData();
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


