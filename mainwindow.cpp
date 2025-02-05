#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include "idatabase.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <qcolordialog.h>
#include <QPlainTextEdit>
#include <QFontDialog>
#include <QFileInfo>

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

    favDialog = new FavDialog(this, filepath);                                // 初始化收藏夹窗口
    favDialog->hide();                                                        // 默认隐藏
    connect(favDialog, &FavDialog::openFile, this, &MainWindow::favOpenFile); // 连接打开文件信号
}
// 析构函数
MainWindow::~MainWindow()
{
    delete ui;
}

// 提取的功能函数

// 保存文件
void MainWindow::saveToFile(const QString &filename)
{
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

    setHighlighter(QFileInfo(filename).suffix(), theme);
    filepath = filename;
    ischanged = false;
    this->setWindowTitle(filepath);
    ui->TextEdit->labelDialog->save(filepath);
}
// 打开文件
bool MainWindow::openFile(const QString &pathName)
{
    QFile file(pathName);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return false;
    }
    // 添加最近打开文件路径到数据库
    try
    {
        IDataBase::getInstance().addLastOpenFilePath(pathName);
    }
    catch (QException *e)
    {
        qDebug() << e->what();
    }
    // 读取文件内容
    QTextStream in(&file);
    QString text = in.readAll();
    ui->TextEdit->clear();
    ui->TextEdit->insertPlainText(text);
    file.close();
    // 更新窗口标题
    this->setWindowTitle(QFileInfo(pathName).absoluteFilePath());
    // 设置高亮规则
    setHighlighter(QFileInfo(pathName).suffix(), theme);
    // 更新文件路径
    filepath = pathName;
    //读取书签
    ui->TextEdit->labelDialog->loadBookmarksFromDatabase(filepath);
    // 重置更改状态
    ischanged = false;
    return true;
}
// 设置高亮规则
void MainWindow::setHighlighter(const QString &language, const QString &theme)
{
    disconnect(ui->TextEdit, &QPlainTextEdit::textChanged, this, &MainWindow::on_TextEdit_textChanged);
    ui->TextEdit->setHightligter(language, theme);
    connect(ui->TextEdit, &QPlainTextEdit::textChanged, this, &MainWindow::on_TextEdit_textChanged);
}

// 是否保存
int MainWindow::isSave()
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

        return box.exec();
    }
    return QMessageBox::No;
}

// 槽函数

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

// 新建文件操作处理
void MainWindow::on_actionNew_triggered()
{

    // 处理用户选择
    int re = isSave();
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

    // 重置编辑器状态
    ui->TextEdit->clear();                        // 清空文本
    ischanged = false;                            // 重置更改标志
    filepath = "";                                // 清空文件路径
    this->setWindowTitle("新建文本文件--编辑器"); // 更新窗口标题
}
// 打开文件操作处理
void MainWindow::on_actionOpen_triggered()
{
    int re = isSave();
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

    QString filename = QFileDialog::getOpenFileName(this, "打开文件", ".", tr("Text files (*.txt);;All(*.*)"));
    openFile(filename);
}

// 保存文件操作处理
void MainWindow::on_actionSave_triggered()
{
    if (filepath.isEmpty())
    {
        on_actionSaveAs_triggered();
    }
    else
    {
        saveToFile(filepath);
        IDataBase::getInstance().addLastOpenFilePath(filepath);
    }
}

// 另存为文件操作处理
void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".", tr("Text files (*.txt);;All(*.*)"));
    if (!filename.isEmpty())
    {
        saveToFile(filename);
        IDataBase::getInstance().addLastOpenFilePath(filename);
    }
}

// 编辑功能相关函数

// 文本内容变更处理
void MainWindow::on_TextEdit_textChanged()
{
    static bool isProcessing = false;

    if (isProcessing)
    {
        return;
    }

    isProcessing = true;
    if (ischanged == false)
    {
        ischanged = true;
        this->setWindowTitle("*" + this->windowTitle());
    }

    if (ui->actionAutoSave->isChecked() && !filepath.isEmpty())
    {
        on_actionSave_triggered();
    }
    ui->TextEdit->cleanAllFormat();
    ui->TextEdit->detectHyperlink();

    isProcessing = false;
}
// 剪切
void MainWindow::on_actionCut_triggered()
{
    ui->TextEdit->cut();
}
// 复制
void MainWindow::on_actionCopy_triggered()
{
    ui->TextEdit->copy();
}
// 粘贴
void MainWindow::on_actionPaste_triggered()
{
    ui->TextEdit->paste();
}
// 重做
void MainWindow::on_actionRedo_triggered()
{
    ui->TextEdit->redo();
}
// 撤销
void MainWindow::on_actionUndo_triggered()
{
    ui->TextEdit->undo();
}
// 复制可用状态变化
void MainWindow::on_TextEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}
// 重做可用状态变化
void MainWindow::on_TextEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}
// 撤销可用状态变化
void MainWindow::on_TextEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}
// 应用样式表更改
void MainWindow::submitStyle(const QString &style)
{
    setStyleSheet(style);
}
// 收藏夹打开文件
bool MainWindow::favOpenFile(const QString &style)
{
    int re = isSave();
    switch (re)
    {
    case QMessageBox::Yes:
        on_actionSave_triggered();
        break;
    case QMessageBox::No:
        break;
    case QMessageBox::Cancel:
        return true;
    }

    return openFile(style);
}

// 自动换行设置
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
// 退出时
void MainWindow::on_actionExit_triggered()
{

    close();
}
// 窗口关闭事件处理
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

// 光标位置改变时
void MainWindow::on_TextEdit_cursorPositionChanged()
{
}
// 全选功能
void MainWindow::on_actionSelectAll_triggered()
{
    ui->TextEdit->textCursor().setPosition(0);
    ui->TextEdit->textCursor().setPosition(-1, QTextCursor::KeepAnchor);
}
// 字体功能
void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, this);
    if (ok)
    {
        ui->TextEdit->setFont(font);
    }
}

// 最近打开文件功能
void MainWindow::on_openLastFiles_aboutToShow()
{
    QAction *action;
    QStringList filePaths = IDataBase::getInstance().getLastOpenFilePaths();

    // 清空最近打开文件菜单
    for (auto i : ui->openLastFiles->actions())
    {
        i->deleteLater();
    }
    ui->openLastFiles->clear();

    // 重新添加最近打开文件
    for (int i = 0; i < filePaths.size(); i++)
    {
        action = new QAction(this);
        action->setText(filePaths[i]);
        action->setData(filePaths[i]);
        connect(action, &QAction::triggered, this, [this, action]()
                { openFile(action->text()); });
        ui->openLastFiles->addAction(action);
    }
    ui->openLastFiles->addSeparator();
    auto deleteAll = new QAction("清空最近打开文件", this);
    connect(deleteAll, &QAction::triggered, &IDataBase::getInstance(), &IDataBase::deleteAllLastOpenFilePaths);
    deleteAll->setCheckable(false);
    ui->openLastFiles->addAction(deleteAll);
}
// 主题功能
void MainWindow::on_theme_aboutToShow()
{
    QString themeDirPath = "../../theme";
    // 清空主题菜单
    for (auto i : ui->theme->actions())
    {
        i->deleteLater();
    }
    ui->theme->clear();
    // 检测主题文件夹内容
    QDir themeDir(themeDirPath);
    if (!themeDir.exists())
    {
        return;
    }
    QStringList filters;
    filters << "*.qss";
    QStringList themes = themeDir.entryList(filters, QDir::Files);

    // 重新添加主题
    QAction *ac;
    for (int i = 0; i < themes.size(); i++)
    {
        QFile styleFile(themeDirPath + "/" + themes[i]);
        QString style = "";
        if (styleFile.open(QFile::ReadOnly))
        {
            style = styleFile.readAll();
            styleFile.close();
        }
        ac = new QAction(ui->theme);
        ac->setText(QFileInfo(themes[i]).baseName());

        connect(ac, &QAction::triggered, this, [style, this, ac]()
                {
            theme = ac->text();
            setStyleSheet(style);
            // 设置高亮规则
            setHighlighter(QFileInfo(filepath).suffix(), ac->text()); });
        ui->theme->addAction(ac);
    }
}
// 打开收藏夹
void MainWindow::on_actionFav_triggered()
{
    favDialog->show();
}

void MainWindow::on_actionLabel_triggered()
{
    ui->TextEdit->showLabelDialog();
}

