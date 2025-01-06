#ifndef FAVDIALOG_H
#define FAVDIALOG_H

#include <QDialog>

namespace Ui {
class FavDialog;
}

class FavDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FavDialog(QWidget *parent = nullptr);
    ~FavDialog();

private:
    Ui::FavDialog *ui;
};

#endif // FAVDIALOG_H
