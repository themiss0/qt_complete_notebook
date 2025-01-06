#include "favdialog.h"
#include "ui_favdialog.h"

FavDialog::FavDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FavDialog)
{
    ui->setupUi(this);
}

FavDialog::~FavDialog()
{
    delete ui;
}
