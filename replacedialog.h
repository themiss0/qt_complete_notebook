#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
class replaceDialog;
}

class replaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit replaceDialog(QWidget *parent = nullptr, QPlainTextEdit *edit = nullptr); // 构造函数 
    ~replaceDialog(); // 析构函数

private slots:
    void on_sourceInput_textChanged(const QString &arg1);  // 查找文本变更处理
    void on_targetInput_textChanged(const QString &arg1);  // 替换文本变更处理
    int on_buttonReplace_clicked();     // 替换按钮点击处理
    void on_buttonReplaceAll_clicked(); // 全部替换按钮点击处理 
    void on_buttonCancel_clicked();     // 取消按钮点击处理
    int on_buttonFindNext_clicked();    // 查找下一个按钮点击处理

private:
    Ui::replaceDialog *ui;      // UI界面对象
    QPlainTextEdit *textEdit;   // 编辑器对象指针

};

#endif // REPLACEDIALOG_H
