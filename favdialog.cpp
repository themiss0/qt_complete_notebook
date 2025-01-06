#include "favdialog.h"
#include "ui_favdialog.h"
#include "idatabase.h"

FavDialog::FavDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::FavDialog)
{
    ui->setupUi(this);
}

FavDialog::~FavDialog()
{
    delete ui;
}
// 每次显示时刷新
void FavDialog::showEvent(QShowEvent *event)
{
    ui->listWidget->clear();
    QStringList filePaths = IDataBase::getInstance().getFavoriteFilePaths();
    for (int i = 0; i < filePaths.size(); i++)
    {
        ui->listWidget->addItem(filePaths[i]);
    }
}

void FavDialog::on_bt_open_clicked()
{
    QString s = ui->listWidget->selectedItems().first()->text();
    emit openFile(s);
    this->close();
}

void FavDialog::on_bt_delete_clicked()
{
    QString s = ui->listWidget->selectedItems().first()->text();
    ui->listWidget->removeItemWidget(ui->listWidget->selectedItems().first());
}

void FavDialog::on_bt_close_clicked()
{
    this->close();
}
