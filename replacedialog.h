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
    explicit replaceDialog(QWidget *parent = nullptr, QPlainTextEdit *edit = nullptr);
    ~replaceDialog();

private slots:
    void on_sourceInput_textChanged(const QString &arg1);

    void on_targetInput_textChanged(const QString &arg1);

    int on_buttonReplace_clicked();

    void on_buttonReplaceAll_clicked();

    void on_buttonCancel_clicked();

    int on_buttonFindNext_clicked();

private:
    Ui::replaceDialog *ui;
    QPlainTextEdit *textEdit;

};

#endif // REPLACEDIALOG_H
