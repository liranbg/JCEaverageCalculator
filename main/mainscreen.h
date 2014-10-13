#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QMainWindow>
#include <QApplication>
#include <QString>
#include <QMessageBox>
#include <QTextEdit>

#include "./CourseTab/coursestablemanager.h"
#include "./LoginTab/loginhandler.h"
#include "./CalendarTab/CalendarManager.h"
#include "./jceWidgets/jcestatusbar.h"



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

    //Login Tab slots
    void on_loginButton_clicked();
    void on_keepLogin_clicked();
    void on_usrnmLineEdit_editingFinished();

    //GPA Tab slots
    void on_ratesButton_clicked();
    void on_graphButton_clicked();
    void on_revertBtn_clicked();
    void on_clearTableButton_clicked();
    void on_coursesTable_itemChanged(QTableWidgetItem *item);
    void on_checkBoxCoursesInfluence_toggled(bool checked);

    //Schedule Tab slots
    void on_getCalendarBtn_clicked();
    void on_examsBtn_clicked();
    void on_exportToCVSBtn_clicked();

    //Main screen general slots
    void on_spinBoxCoursesFromSemester_valueChanged(int arg1);
    void on_spinBoxCoursesFromYear_valueChanged(int arg1);
    void on_spinBoxCoursesToYear_valueChanged(int arg1);
    void on_spinBoxCoursesToSemester_valueChanged(int arg1);
    void on_labelMadeBy_linkActivated(const QString &link);

    //Setting dock
    void on_langButton_clicked();

    void on_creditButton_clicked();

    void on_howtoButton_clicked();

private:
    void checkLocale();
    bool checkIfValidDates();

    bool isBusy();
    void lock();
    void unlock();

    Ui::MainScreen *ui;

    bool isBlocked;

    QPixmap iconPix;
    user *userLoginSetting;
    SaveData *data;

    CalendarManager * calendar;
    coursesTableManager *courseTableMgr;
    loginHandler *loginHandel;

    jceStatusBar *statusBar;

};

#endif // MAINSCREEN_H
