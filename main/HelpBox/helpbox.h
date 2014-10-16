#ifndef HELPBOX_H
#define HELPBOX_H

#include <QDialog>

namespace Ui {
class helpBox;
}

class helpBox : public QDialog
{
    Q_OBJECT

public:
    explicit helpBox(QWidget *parent = 0);
    ~helpBox();

private:
    Ui::helpBox *ui;
};

#endif // HELPBOX_H
