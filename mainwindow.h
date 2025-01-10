#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "favdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr); // 主窗口构造函数
    ~MainWindow();  // 析构函数
    void submitStyle(const QString &style); // 应用样式表更改
    bool favOpenFile(const QString &style); // 收藏夹打开文件
    int isSave(); // 是否保存




private slots:
    // 菜单栏动作槽函数
    void on_actionAbout_triggered();  // "关于"对话框
    void on_actionFind_triggered();   // 查找对话框
    void on_actionReplace_triggered(); // 替换对话框
    void on_actionOpen_triggered();   // 打开文件
    void on_actionNew_triggered();    // 新建文件
    void on_actionSave_triggered();   // 保存文件
    void on_actionSaveAs_triggered(); // 另存为文件

    // 编辑功能槽函数 
    void on_TextEdit_textChanged();   // 文本变更处理
    void on_actionCut_triggered();    // 剪切
    void on_actionCopy_triggered();   // 复制
    void on_actionPaste_triggered();  // 粘贴
    void on_actionRedo_triggered();   // 重做
    void on_actionUndo_triggered();   // 撤销
    
    // 界面显示相关槽函数
    void on_TextEdit_copyAvailable(bool b);      // 复制可用状态变化
    void on_TextEdit_redoAvailable(bool b);      // 重做可用状态变化
    void on_TextEdit_undoAvailable(bool b);      // 撤销可用状态变化
    void on_actionLineWrap_triggered();          // 自动换行开关
    void on_actionExit_triggered();              // 退出程序
    void on_TextEdit_cursorPositionChanged();    // 光标位置改变
    void on_actionSelectAll_triggered();         // 全选
    void on_openLastFiles_aboutToShow();        // 打开最近文件
    void on_theme_aboutToShow();             // 主题设置
    void on_actionFav_triggered();          // 收藏夹
    void on_actionFont_triggered();        // 字体设置

    void closeEvent(QCloseEvent *e);             // 关闭事件处理器

    void on_actionLabel_triggered();

private:
    Ui::MainWindow *ui;           // UI界面对象
    QLabel statusLabel;           // 状态栏标签(显示文本信息)
    QLabel statusCursorLabel;     // 状态栏标签(显示光标位置) 
    QLabel autherLabel;           // 状态栏标签(显示作者信息)
    QString textEditFontColor;    // 字体颜色
    QString textEditBgColor;      // 背景颜色 
    QString theme = "light";              // 主题
    bool ischanged;              // 文件是否被修改

    void saveToFile(const QString &filename); // 保存文件
    bool openFile(const QString &filePath); // 打开文件
    void setHighlighter(const QString &language, const QString &theme); // 设置高亮规则
    

    FavDialog *favDialog;         // 收藏夹窗口
    QString filepath;             // 当前文件路径
};
#endif // MAINWINDOW_H
