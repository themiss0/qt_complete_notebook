#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void sumbitStyleSheet();

private slots:
    void on_actionAbout_triggered();
    void on_actionFind_triggered();
    void on_actionReplace_triggered();
    void on_actionOpen_triggered();
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_TextEdit_textChanged();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionRedo_triggered();
    void on_actionUndo_triggered();
    void on_TextEdit_copyAvailable(bool b);
    void on_TextEdit_redoAvailable(bool b);
    void on_TextEdit_undoAvailable(bool b);
    void on_actionFontColor_triggered();
    void on_actionBgColor_triggered();
    void on_actionLineWrap_triggered();
    void on_actionShowStatusBar_triggered();
    void on_actionShowToolBar_triggered();
    void on_actionExit_triggered();
    void closeEvent(QCloseEvent *e);
    void on_TextEdit_cursorPositionChanged();
    void on_actionShowRowNum_triggered();
    void on_actionSelectAll_triggered();
    void on_actionFont_triggered();

private:
    Ui::MainWindow *ui;
    QLabel statusLabel;
    QLabel statusCursorLabel;
    QLabel autherLabel;
    QString filepath;
    QString textEditFontColor;
    QString textEditBgColor;
    bool ischanged;
};
#endif // MAINWINDOW_H
