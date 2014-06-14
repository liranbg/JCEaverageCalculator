#include "mainscreen.h"
#include "ui_mainscreen.h"


MainScreen::MainScreen(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainScreen)
{
    ui->setupUi(this);
    //    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    this->setFixedSize(this->size()); //main not resizeable
    ui->pswdLineEdit->setEchoMode((QLineEdit::Password));
    ui->statusBar->setStyleSheet("QStatusBar::item { border: 0px solid black };");
    ButtomStatusLabel = new QLabel(this);
    statusLabel = new QLabel(this);



    ui->CoursesTab->setDisabled(true);
    ui->SettingsTab->setDisabled(true);
    ui->statusBar->setMaximumSize(this->geometry().width(),StatusIconHeight);
    ui->statusBar->addPermanentWidget(ButtomStatusLabel,0);
    ui->statusBar->addPermanentWidget(statusLabel,1);

    this->jceLog = NULL;
    this->userLoginSetting = new user("","");

    this->courseTableMgr = new coursesTableManager(ui->coursesTable,userLoginSetting);

    updateDates();
    setLabelConnectionStatus(jceLogin::jceStatus::JCE_NOT_CONNECTED);

}

MainScreen::~MainScreen()
{
    delete userLoginSetting;
    delete jceLog;
    delete ui;
}
void MainScreen::on_ratesButton_clicked()
{
    QTextEdit phrase;
    std::string pageString;
    if (this->jceLog != NULL)
    {
        if (jceLog->getGrades())
        {
            phrase.setText(QString::fromStdString(jceLog->getPage()));
            pageString = phrase.toPlainText().toStdString();
            courseTableMgr->setCoursesList(pageString);
            courseTableMgr->insertJceCoursesIntoTable();
        }
    }

}

void MainScreen::on_spinBoxFromYear_editingFinished()
{
    if (ui->spinBoxFromYear->value() > ui->spinBoxToYear->value())
    {
        ui->spinBoxFromYear->setValue(ui->spinBoxToYear->value());
        ui->spinBoxFromYear->setFocus();
        updateDates();
    }
    else
        updateDates();
}
void MainScreen::on_spinBoxToYear_editingFinished()
{
    if (ui->spinBoxFromYear->value() > ui->spinBoxToYear->value())
    {
        ui->spinBoxToYear->setValue(ui->spinBoxFromYear->value());
        ui->spinBoxToYear->setFocus();
        updateDates();
    }
    else
        updateDates();
}
void MainScreen::on_spinBoxFromSem_editingFinished()
{
    if (ui->spinBoxFromYear->value() == ui->spinBoxToYear->value())
    {
        if (ui->spinBoxFromSem->value() > ui->spinBoxToSemester->value())
        {
            ui->spinBoxFromSem->setValue(ui->spinBoxToSemester->value());
            ui->spinBoxFromSem->setFocus();
        }
    }
    updateDates();
}
void MainScreen::on_spinBoxToSemester_editingFinished()
{
    if (ui->spinBoxFromYear->value() == ui->spinBoxToYear->value())
    {
        if (ui->spinBoxFromSem->value() > ui->spinBoxToSemester->value())
        {
            ui->spinBoxToSemester->setValue(ui->spinBoxFromSem->value());
            ui->spinBoxToSemester->setFocus();
        }
    }
    updateDates();
}

void MainScreen::on_coursesTable_itemChanged(QTableWidgetItem *item)
{
    this->courseTableMgr->changes(item->text().toStdString(),item->row(),item->column());
    ui->avgLCD->display(courseTableMgr->getAvg());
}

void MainScreen::on_loginButton_clicked()
{
    setLabelConnectionStatus(jceLogin::jceStatus::JCE_START_VALIDATING_PROGRESS);


    if (this->jceLog == NULL)
        uiSetConnectMode();
    else
    {
        if (jceLog->isLoginFlag() == true)
            uiSetDisconnectMode();
        else
            uiSetConnectMode();
    }

}

void MainScreen::on_checkBox_toggled(bool checked)
{
    this->userLoginSetting->setInfluenceCourseOnly(checked);
    this->courseTableMgr->influnceCourseChanged(checked);
}
void MainScreen::on_usrnmLineEdit_editingFinished()
{
    ui->usrnmLineEdit->setText(ui->usrnmLineEdit->text().toLower());
}
void MainScreen::updateDates()
{
    std::string fy,ty,fs,ts;
    fy = std::to_string(ui->spinBoxFromYear->value());
    ty = std::to_string(ui->spinBoxToYear->value());
    fs = std::to_string(ui->spinBoxFromSem->value());
    ts = std::to_string(ui->spinBoxToSemester->value());
    userLoginSetting->setDate(fy,fs,ty,ts);
}

void MainScreen::uiSetDisconnectMode()
{
    setLabelConnectionStatus(jceLogin::jceStatus::JCE_NOT_CONNECTED);
    ui->usrnmLineEdit->setText("");
    ui->pswdLineEdit->setText("");
    ui->usrnmLineEdit->setEnabled(true);
    ui->pswdLineEdit->setEnabled(true);

    delete jceLog;
    jceLog = NULL;
    ui->loginButton->setText("&Login");
    this->ui->ratesButton->setDisabled(true);
    return;
}

void MainScreen::uiSetConnectMode()
{
    string username;
    string password;
    if ((ui->usrnmLineEdit->text().isEmpty()) || (ui->pswdLineEdit->text().isEmpty()))
    {
        //add icon near to username and password to mark it
        return;
    }
    username = ui->usrnmLineEdit->text().toStdString();
    password = ui->pswdLineEdit->text().toStdString();

    ui->usrnmLineEdit->setDisabled(true);
    ui->pswdLineEdit->setDisabled(true);

    userLoginSetting->setUsername(username);
    userLoginSetting->setPassword(password);

    this->repaint();

    jceLog = new jceLogin(userLoginSetting);
    this->loginHandel = new loginHandler(jceLog,statusLabel,ui->pswdLineEdit,ui->usrnmLineEdit);

    if (loginHandel->makeConnection() == true)
    {
        setLabelConnectionStatus(jceLogin::jceStatus::JCE_YOU_ARE_IN);
        ui->loginButton->setText("&Logout");
        this->ui->ratesButton->setEnabled(true);
        ui->CoursesTab->setEnabled(true);
        ui->SettingsTab->setEnabled(true);
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
        statusLabel->setText("Connecting");
        break;
    case jceLogin::jceStatus::JCE_YOU_ARE_IN:
        iconPix.load(":/icons/greenStatusIcon.png");
        statusLabel->setText("Connected");
        break;
    default:
        iconPix.load(":/icons/redStatusIcon.png");
        statusLabel->setText("Disconnected");
        break;
    }
    ButtomStatusLabel->setPixmap(iconPix);

    this->repaint();
}
void MainScreen::on_actionCredits_triggered()
{
    QMessageBox credits;
    credits.setText(" A tiny application to calculate your grades average. \n Made by Liran Ben Gida \n Thanks to: Sagi Dayan, Nadav Luzzato");
    credits.exec();
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
