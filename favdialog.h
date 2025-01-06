#ifndef FAVDIALOG_H
#define FAVDIALOG_H

#include <QDialog>

namespace Ui
{
    class FavDialog;
}

class FavDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FavDialog(QWidget *parent = nullptr);
    ~FavDialog();

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void openFile(QString s);

private slots:
    void on_bt_open_clicked();

    void on_bt_delete_clicked();

    void on_bt_close_clicked();

private:
    Ui::FavDialog *ui;
};

#endif // FAVDIALOG_H
