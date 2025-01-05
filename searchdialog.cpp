#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QPlainTextEdit>
#include <QMessageBox>
#include <iostream>

SearchDialog::SearchDialog(QWidget *parent, QPlainTextEdit *textEdit)
    : QDialog(parent), ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    this->textEdit = textEdit;

    ui->searchButton->setEnabled(ui->lineEdit->text() == "" ? false : true);
    ui->down->setChecked(true);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_cancelButton_clicked()
{
    close();
}

void SearchDialog::on_searchButton_clicked()
{

    QString source = textEdit->toPlainText();
    QString target = ui->lineEdit->text();

    QTextCursor c = textEdit->textCursor();
    long index = -1;

    index = ui->down->isChecked() ? source.indexOf(target, c.position(), ui->sensativecase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive)
                                  : index = source.lastIndexOf(target, c.position() - source.length() - 1, ui->sensativecase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

    if (index >= 0)
    {

        c.setPosition(ui->down->isChecked() ? index : index + target.length());
        c.setPosition(ui->down->isChecked() ? index + target.length() : index, QTextCursor::KeepAnchor);

        textEdit->setTextCursor(c);
        return;
    }

    QMessageBox box(this);
    box.setWindowTitle("文本编辑器");
    box.setContentsMargins(20, 10, 20, 0);
    box.setText("找不到字符串\n" + target);
    box.exec();
}

void SearchDialog::on_lineEdit_textChanged(const QString &arg1)
{
    if (ui->lineEdit->text() == "")
    {
        ui->searchButton->setEnabled(false);
    }
    else
    {
        ui->searchButton->setEnabled(true);
    }
}
