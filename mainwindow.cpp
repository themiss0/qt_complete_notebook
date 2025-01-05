#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <qcolordialog.h>
#include <iostream>
#include <QPlainTextEdit>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("新建文本文件--编辑器");

    statusLabel.setText("Length: " + QString::number(0) + "   Lines: " + QString::number(0));
    statusLabel.setMaximumWidth(150);

    statusCursorLabel.setText("Ln: " + QString::number(0) + "   Col: " + QString::number(0));
    statusCursorLabel.setMaximumWidth(150);

    autherLabel.setText("陈卓煜");
    autherLabel.setMaximumWidth(150);

    ui->statusbar->addPermanentWidget(&statusLabel);
    ui->statusbar->addPermanentWidget(&statusCursorLabel);
    ui->statusbar->addPermanentWidget(&autherLabel);

    ui->actionRedo->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

    ui->actionShowStatusBar->setChecked(true);
    ui->actionShowToolBar->setChecked(true);
    ui->actionShowRowNum->setChecked(true);

    ischanged = false;
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog aboutDialog;
    aboutDialog.exec();
}

void MainWindow::on_actionFind_triggered()
{
    SearchDialog searchDialog(this, ui->TextEdit);
    searchDialog.exec();
}

void MainWindow::on_actionReplace_triggered()
{
    replaceDialog replacedialog(this, ui->TextEdit);
    replacedialog.exec();
}

void MainWindow::on_actionNew_triggered()
{

    // 如果已有更改
    if (ischanged)
    {
        QString path = filepath != "" ? filepath : "无标题";
        QMessageBox box(this);
        box.setWindowTitle("文本编辑器");
        box.setIcon(QMessageBox::Question);
        box.setWindowFlag(Qt::Drawer);
        box.setText(QString("你想将更改保存到\n") + path + "吗?");
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        int re = box.exec();
        switch (re)
        {
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;

        case QMessageBox::No:
            break;
        case QMessageBox::Cancel:
            return;
        }
    }

    // 重置编辑器状态
    ui->TextEdit->clear();
    ischanged = false;
    this->setWindowTitle("新建文本文件--编辑器");
    filepath = "";
}

void MainWindow::on_actionOpen_triggered()
{

    if (ischanged)
    {
        QString path = filepath != "" ? filepath : "无标题";
        QMessageBox box(this);
        box.setWindowTitle("文本编辑器");
        box.setIcon(QMessageBox::Question);
        box.setWindowFlag(Qt::Drawer);
        box.setText(QString("你想将更改保存到\n") + path + "吗?");
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        int re = box.exec();
        switch (re)
        {
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;

        case QMessageBox::No:
            break;
        case QMessageBox::Cancel:
            return;
        }
    }

    QString filename = QFileDialog::getOpenFileName(this, "打开文件", ".", tr("Text files (*.txt);;All(*.*)"));
    QFile file(filename);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    ui->TextEdit->insertPlainText(text);
    file.close();

    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());
    filepath = filename;
    ischanged = false;
}

void MainWindow::on_actionSave_triggered()
{
    QFile file(filepath);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        on_actionSaveAs_triggered();
        return;
    }
    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();

    out << text;
    file.flush();
    file.close();

    ischanged = false;
    this->setWindowTitle(filepath);
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".", tr("Text files (*.txt);;All(*.*)"));
    QFile file(filename);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        return;
    }

    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();

    out << text;
    file.flush();
    file.close();

    filepath = filename;
    ischanged = false;
    this->setWindowTitle(filepath);
}

void MainWindow::on_TextEdit_textChanged()
{
    if (ischanged == false)
    {
        ischanged = true;
        this->setWindowTitle("*" + this->windowTitle());
    }

    statusLabel.setText("Length: " + QString::number(ui->TextEdit->toPlainText().length()) + "   Lines: " + QString::number(ui->TextEdit->document()->lineCount()));
}

void MainWindow::on_actionCut_triggered()
{
    ui->TextEdit->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->TextEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->TextEdit->paste();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->TextEdit->redo();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->TextEdit->undo();
}

void MainWindow::on_TextEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}

void MainWindow::on_TextEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}

void MainWindow::on_TextEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}

void MainWindow::sumbitStyleSheet()
{
    ui->TextEdit->setStyleSheet(textEditFontColor + "; " + textEditBgColor);
}

void MainWindow::on_actionFontColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");
    textEditFontColor = "color:" + color.name();
    sumbitStyleSheet();
}

void MainWindow::on_actionBgColor_triggered()
{

    QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");
    textEditBgColor = "background-color:" + color.name();
    sumbitStyleSheet();
}

void MainWindow::on_actionLineWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->TextEdit->lineWrapMode();

    if (mode == QPlainTextEdit::NoWrap)
    {
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);

        ui->actionLineWrap->setChecked(true);
    }
    else
    {
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

        ui->actionLineWrap->setChecked(false);
    }
}

void MainWindow::on_actionShowStatusBar_triggered()
{
    bool check = ui->statusbar->isVisible();
    std::cout << check << std::endl;

    ui->actionShowStatusBar->setChecked(!check);
    ui->statusbar->setVisible(!check);
}

void MainWindow::on_actionShowToolBar_triggered()
{
    bool check = ui->toolBar->isVisible();
    std::cout << check << std::endl;

    ui->actionShowToolBar->setChecked(!check);
    ui->toolBar->setVisible(!check);
}

void MainWindow::on_actionExit_triggered()
{

    close();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (ischanged)
    {
        QString path = filepath != "" ? filepath : "无标题";
        QMessageBox box(this);
        box.setWindowTitle("文本编辑器");
        box.setIcon(QMessageBox::Question);
        box.setWindowFlag(Qt::Drawer);
        box.setText(QString("你想将更改保存到\n" + path + "吗？"));
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        int re = box.exec();
        switch (re)
        {
        case QMessageBox::Yes:
            on_actionSaveAs_triggered();
            break;
        case QMessageBox::No:
            exit(0);
        default:
            e->ignore();
        }
    }
}

void MainWindow::on_TextEdit_cursorPositionChanged()
{
    statusCursorLabel.setText("Ln: " + QString::number(ui->TextEdit->textCursor().blockNumber()) + "   Col: " + QString::number(ui->TextEdit->textCursor().columnNumber()));
}

void MainWindow::on_actionShowRowNum_triggered()
{
    if (ui->actionShowRowNum->isChecked())
    {
        ui->TextEdit->hideLineNumberArea(false);
    }
    else
    {
        ui->TextEdit->hideLineNumberArea(true);
    }
}

void MainWindow::on_actionSelectAll_triggered()
{
    ui->TextEdit->textCursor().setPosition(0);
    ui->TextEdit->textCursor().setPosition(-1, QTextCursor::KeepAnchor);
}

void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, this);
    if (ok)
    {
        ui->TextEdit->setFont(font);
    }
}

