#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui
{
    class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr, QPlainTextEdit *textEdit = nullptr); // 构造函数
    ~SearchDialog(); // 析构函数

private slots:
    void on_cancelButton_clicked();    // 取消按钮点击处理
    void on_searchButton_clicked();    // 搜索按钮点击处理
    void on_lineEdit_textChanged(const QString &arg1);  // 搜索文本变更处理

private:
    Ui::SearchDialog *ui;       // UI界面对象
    QPlainTextEdit *textEdit;   // 编辑器对象指针
};

#endif // SEARCHDIALOG_H
