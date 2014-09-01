#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QMainWindow>
#include <QApplication>
#include <QString>
#include <QMessageBox>
#include <QTextEdit>

#include <string>
#include <iostream>

#include "./CourseTab/coursestablemanager.h"
#include "./LoginTab/loginhandler.h"
#include "./CalendarTab/CalendarManager.h"

#define StatusIconHeight 35
namespace Ui {
class MainScreen;
}

class MainScreen : public QMainWindow
{
    Q_OBJECT

public:
    MainScreen(QWidget *parent = 0);
    ~MainScreen();

private slots:

    void on_ratesButton_clicked();

    void on_spinBoxCoursesFromYear_editingFinished();

    void on_spinBoxCoursesFromSemester_editingFinished();

    void on_spinBoxCoursesToYear_editingFinished();

    void on_spinBoxCoursesToSemester_editingFinished();

    void on_loginButton_clicked();

    void on_usrnmLineEdit_editingFinished();

    void on_actionCredits_triggered();

    void on_clearTableButton_clicked();

    void on_actionExit_triggered();

    void on_coursesTable_itemChanged(QTableWidgetItem *item);

    void on_keepLogin_clicked();

    void on_actionHow_To_triggered();

    void on_pushButton_clicked();

    void on_checkBoxCoursesInfluence_toggled(bool checked);

private:

    void uiSetDisconnectMode();
    void uiSetConnectMode();
    void setLabelConnectionStatus(jceLogin::jceStatus statusDescription);

    Ui::MainScreen *ui;

    user *userLoginSetting;
    SaveData *data;
    CalendarManager * calendar;
    coursesTableManager *courseTableMgr;
    loginHandler *loginHandel;

    QLabel *ButtomStatusLabel;
    QLabel *statusLabel;

};

#endif // MAINSCREEN_H
