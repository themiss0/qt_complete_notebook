#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr); // 构造函数
    ~AboutDialog(); // 析构函数

private:
    Ui::AboutDialog *ui;  // UI界面对象
};

#endif // ABOUTDIALOG_H
