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
    explicit FavDialog(QWidget *parent = nullptr, const QString &path = nullptr);
    ~FavDialog();

signals:
    void openFile(const QString &s);

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void on_bt_open_clicked();

    void on_bt_delete_clicked();

    void on_bt_close_clicked();

    void on_bt_add_clicked();

private:
    Ui::FavDialog *ui;
    const QString &filepath;
};

#endif // FAVDIALOG_H
