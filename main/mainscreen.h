#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QMainWindow>
#include <QApplication>
#include <QString>
#include <QMessageBox>
#include <QTextEdit>

#include <string>
#include <iostream>

#include "coursestablemanager.h"
#include "loginhandler.h"

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

    void on_spinBoxFromYear_editingFinished();

    void on_spinBoxFromSem_editingFinished();

    void on_spinBoxToYear_editingFinished();

    void on_spinBoxToSemester_editingFinished();

    void on_loginButton_clicked();

    void on_checkBox_toggled(bool checked);

    void on_usrnmLineEdit_editingFinished();

    void on_actionCredits_triggered();

    void on_clearTableButton_clicked();

    void on_actionExit_triggered();

    void on_coursesTable_itemChanged(QTableWidgetItem *item);

    void on_keepLogin_clicked();

private:

    void updateDates();
    void uiSetDisconnectMode();
    void uiSetConnectMode();
    void setLabelConnectionStatus(jceLogin::jceStatus statusDescription);

    Ui::MainScreen *ui;

    user *userLoginSetting;

    coursesTableManager *courseTableMgr;
    loginHandler *loginHandel;

    QLabel *ButtomStatusLabel;
    QLabel *statusLabel;

};

#endif // MAINSCREEN_H
