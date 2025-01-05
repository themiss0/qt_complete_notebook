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

// 主窗口构造函数:初始化UI和各项设置
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口标题
    this->setWindowTitle("新建文本文件--编辑器");

    // 初始化状态栏
    // 显示文档长度和行数
    statusLabel.setText("Length: " + QString::number(0) + "   Lines: " + QString::number(0));
    statusLabel.setMaximumWidth(150);

    // 显示光标位置
    statusCursorLabel.setText("Ln: " + QString::number(0) + "   Col: " + QString::number(0));
    statusCursorLabel.setMaximumWidth(150);

    // 显示作者信息
    autherLabel.setText("陈卓煜");
    autherLabel.setMaximumWidth(150);

    // 添加状态栏组件
    ui->statusbar->addPermanentWidget(&statusLabel);
    ui->statusbar->addPermanentWidget(&statusCursorLabel);
    ui->statusbar->addPermanentWidget(&autherLabel);

    // 初始化编辑功能状态
    ui->actionRedo->setEnabled(false); // 重做按钮初始禁用
    ui->actionUndo->setEnabled(false); // 撤销按钮初始禁用
    ui->actionCopy->setEnabled(false); // 复制按钮初始禁用
    ui->actionCut->setEnabled(false);  // 剪切按钮初始禁用

    // 设置文本编辑器初始属性
    ui->TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap); // 默认不自动换行

    // 设置界面元素初始状态
    ui->actionShowStatusBar->setChecked(true); // 状态栏默认显示
    ui->actionShowToolBar->setChecked(true);   // 工具栏默认显示
    ui->actionShowRowNum->setChecked(true);    // 行号默认显示

    ischanged = false; // 初始化文档修改状态
}

MainWindow::~MainWindow()
{

    delete ui;
}


// 关于对话框的槽函数
void MainWindow::on_actionAbout_triggered()
{
    AboutDialog aboutDialog;
    aboutDialog.exec();
}
// 查找对话框的槽函数
void MainWindow::on_actionFind_triggered()
{
    SearchDialog searchDialog(this, ui->TextEdit);
    searchDialog.exec();
}
// 替换对话框的槽函数
void MainWindow::on_actionReplace_triggered()
{
    replaceDialog replacedialog(this, ui->TextEdit);
    replacedialog.exec();
}

// 文件操作相关函数

/**
 * 新建文件操作处理
 * 1. 检查当前文档是否已修改
 * 2. 如已修改则提示保存
 * 3. 重置编辑器状态
 */
void MainWindow::on_actionNew_triggered()
{
    // 如果文档已修改,提示保存
    if (ischanged)
    {
        // 获取文件路径,如果未保存则显示"无标题"
        QString path = filepath != "" ? filepath : "无标题";

        // 创建保存提示对话框
        QMessageBox box(this);
        box.setWindowTitle("文本编辑器");
        box.setIcon(QMessageBox::Question);
        box.setWindowFlag(Qt::Drawer);
        box.setText(QString("你想将更改保存到\n") + path + "吗?");
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        // 处理用户选择
        int re = box.exec();
        switch (re)
        {
        case QMessageBox::Yes: // 选择保存
            on_actionSave_triggered();
            break;
        case QMessageBox::No: // 选择不保存
            break;
        case QMessageBox::Cancel: // 取消操作
            return;
        }
    }

    // 重置编辑器状态
    ui->TextEdit->clear();                        // 清空文本
    ischanged = false;                            // 重置更改标志
    filepath = "";                                // 清空文件路径
    this->setWindowTitle("新建文本文件--编辑器"); // 更新窗口标题
}

/**
 * 打开文件操作处理
 * 1. 检查当前文档是否需要保存
 * 2. 打开文件选择对话框
 * 3. 读取文件内容并显示
 * 4. 更新窗口标题和文件状态
 */
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

// 编辑功能相关函数

/**
 * 文本内容变更处理
 * 1. 更新修改状态标志
 * 2. 更新窗口标题(添加*号)
 * 3. 更新状态栏信息
 */
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

/**
 * 字体颜色设置
 * 1. 打开颜色选择对话框
 * 2. 应用所选颜色到文本
 */
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

/**
 * 状态栏显示控制
 * 1. 获取当前显示状态
 * 2. 切换显示/隐藏
 * 3. 更新菜单项选中状态
 */
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

/**
 * 窗口关闭事件处理
 * 1. 检查文档是否需要保存
 * 2. 提示用户选择操作
 * 3. 根据用户选择执行相应操作
 */
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
