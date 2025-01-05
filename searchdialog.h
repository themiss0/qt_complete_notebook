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
    explicit SearchDialog(QWidget *parent = nullptr, QPlainTextEdit *textEdit = nullptr);
    ~SearchDialog();

private slots:

    void on_cancelButton_clicked();

    void on_searchButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::SearchDialog *ui;
    QPlainTextEdit *textEdit;
};

#endif // SEARCHDIALOG_H
