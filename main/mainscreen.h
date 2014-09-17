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


#define STATUS_ICON_HEIGH 35
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

    void on_loginButton_clicked();

    void on_usrnmLineEdit_editingFinished();

    void on_actionCredits_triggered();

    void on_clearTableButton_clicked();

    void on_actionExit_triggered();

    void on_coursesTable_itemChanged(QTableWidgetItem *item);

    void on_keepLogin_clicked();

    void on_actionHow_To_triggered();

    void on_getCalendarBtn_clicked();

    void on_checkBoxCoursesInfluence_toggled(bool checked);

    void on_exportToCVSBtn_clicked();

    void on_actionHebrew_triggered();

    void on_actionEnglish_triggered();

    void on_actionOS_Default_triggered();

    void on_spinBoxCoursesFromSemester_valueChanged(int arg1);

    void on_spinBoxCoursesFromYear_valueChanged(int arg1);

    void on_spinBoxCoursesToYear_valueChanged(int arg1);

    void on_spinBoxCoursesToSemester_valueChanged(int arg1);

private:

    void uiSetDisconnectMode();
    void uiSetConnectMode();
    void setLabelConnectionStatus(jceLogin::jceStatus statusDescription);
    void checkLocale();

    bool checkIfValidDates();

    Ui::MainScreen *ui;

    QLabel *ButtomStatusLabel;
    QLabel *statusLabel;
    QPixmap iconPix;

    user *userLoginSetting;
    SaveData *data;

    CalendarManager * calendar;
    coursesTableManager *courseTableMgr;

    loginHandler *loginHandel;

};

#endif // MAINSCREEN_H
