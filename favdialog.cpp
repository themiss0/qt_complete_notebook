#include "favdialog.h"
#include "ui_favdialog.h"
#include "idatabase.h"
#include <QDialogButtonBox>
#include <QMessageBox>

FavDialog::FavDialog(QWidget *parent, const QString &path)
    : QDialog(parent), ui(new Ui::FavDialog), filepath(path)
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
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &FavDialog::on_bt_open_clicked);
}
//  打开文件
void FavDialog::on_bt_open_clicked()
{
    if (ui->listWidget->selectedItems().isEmpty())
    {
        return;
    }
    
    QString s = ui->listWidget->selectedItems().first()->text();
    qDebug() << s;
    emit openFile(s);
    this->close();
}
//  删除文件
void FavDialog::on_bt_delete_clicked()
{
    if (ui->listWidget->selectedItems().isEmpty())
    {
        return;
    }

    if (QMessageBox::question(
            this,
            "Delete",
            "确定要删除此收藏吗?",
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
    {
        return;
    }

    QString s = ui->listWidget->selectedItems().first()->text();

    qDebug() << s;
    ui->listWidget->removeItemWidget(ui->listWidget->selectedItems().first());
    IDataBase::getInstance().deleteFavoriteFilePath(s);

    this->showEvent(nullptr);
}
//  关闭窗口
void FavDialog::on_bt_close_clicked()
{
    this->close();
}
// 添加收藏
void FavDialog::on_bt_add_clicked()
{

    if (filepath.isEmpty() || filepath == "")
    {
        return;
    }
    qDebug() << "not null " + filepath;
    IDataBase::getInstance().addFavoriteFilePath(filepath);
    this->showEvent(nullptr);
}
