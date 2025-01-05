#include "replacedialog.h"
#include "ui_replacedialog.h"
#include <QPlainTextEdit>
#include <iostream>

replaceDialog::replaceDialog(QWidget *parent, QPlainTextEdit *edit)
    : QDialog(parent), ui(new Ui::replaceDialog)
{
    ui->setupUi(this);

    ui->buttonReplace->setEnabled(false);
    ui->buttonReplaceAll->setEnabled(false);

    textEdit = edit;
}

replaceDialog::~replaceDialog()
{
    delete ui;
}

void replaceDialog::on_sourceInput_textChanged(const QString &arg1)
{
    if (ui->sourceInput->text() == "")
    {
        ui->buttonFindNext->setEnabled(false);
        ui->buttonReplace->setEnabled(false);
        ui->buttonReplaceAll->setEnabled(false);
    }
    else
    {
        ui->buttonFindNext->setEnabled(true);
        ui->buttonReplace->setEnabled(true);
        ui->buttonReplaceAll->setEnabled(true);
    }
}

void replaceDialog::on_targetInput_textChanged(const QString &arg1)
{
}

int replaceDialog::on_buttonReplace_clicked()
{
    QString target = ui->sourceInput->text();
    QString to = ui->targetInput->text();

    QString seltext = textEdit->textCursor().selectedText();

    if (seltext == target)
    {
        textEdit->insertPlainText(to);
    }
    return on_buttonFindNext_clicked();
}

void replaceDialog::on_buttonReplaceAll_clicked()
{
    QString text = textEdit->toPlainText();
    QString target = ui->sourceInput->text();
    QString to = ui->targetInput->text();

    text.replace(target, to);
    textEdit->setPlainText(text);
}

void replaceDialog::on_buttonCancel_clicked()
{
    close();
}

int replaceDialog::on_buttonFindNext_clicked()
{

    QString source = textEdit->toPlainText();
    QString target = ui->sourceInput->text();

    QTextCursor c = textEdit->textCursor();
    std::cout << c.position();
    long index = -1;

    index = source.indexOf(target, c.position(), ui->sensativecase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

    if (index >= 0)
    {
        c.setPosition(index);
        c.setPosition(index + target.length(), QTextCursor::KeepAnchor);

        textEdit->setTextCursor(c);
        return index;
    }

    index = source.indexOf(target, 0, ui->sensativecase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
    if (index >= 0)
    {
        c.setPosition(index);
        c.setPosition(index + target.length(), QTextCursor::KeepAnchor);

        textEdit->setTextCursor(c);
        return index;
    }
    return index;
}
