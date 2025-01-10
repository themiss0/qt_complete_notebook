#include "filetabwidget.h"
#include "qfileinfo.h"
#include "ui_filetabwidget.h"
#include "codeeditor.h"

#include <QMessageBox>

FileTabWidget::FileTabWidget(QWidget *parent)
    : QTabWidget(parent), ui(new Ui::FileTabWidget)
{
    ui->setupUi(this);
    connect(this, &QTabWidget::tabCloseRequested, this, &FileTabWidget::closeTab);
}

FileTabWidget::~FileTabWidget()
{
    delete ui;
}

void FileTabWidget::closeTab(int index)
{
    QWidget *wg = this->widget(index);
    if (isCloseAble(wg->findChild<CodeEditor *>()))
    {
        removeTab(index);
        delete wg;
    }
    else
    {
    }
}

void FileTabWidget::closeCurrentTab()
{
    closeTab(currentIndex());
}

void FileTabWidget::closeAllTabs()
{
    while (count() > 0)
    {
        closeTab(0);
    }
}

bool FileTabWidget::isCloseAble(const CodeEditor *editor)
{

    if (editor->isChanged)
    {

        QString path = editor->filepath != "" ? editor->filepath : "无标题";
        QMessageBox box(this);
        box.setWindowTitle("文本编辑器");
        box.setIcon(QMessageBox::Question);
        box.setWindowFlag(Qt::Drawer);
        box.setText(QString("你想将更改保存到\n") + path + "吗?");
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        int result = box.exec();
        switch (result)
        {
        case QMessageBox::Yes:
            setCurrentWidget(editor->parentWidget());
            emit save();
            return true;
        case QMessageBox::No:
            return true;
            break;
        default:
            return false;
        }
    }
    return true;
}
CodeEditor *FileTabWidget::newTab(const QString &pathName)

{
    CodeEditor *editor = new CodeEditor(this, pathName);
    addTab(editor, pathName.isEmpty() ? "新建文件" : QFileInfo(pathName).fileName());
    setCurrentWidget(editor);
    editors.insert(pathName, editor);
    return editor;
}

CodeEditor *FileTabWidget::getCurrentEditor() const
{
    return currentWidget()->findChild<CodeEditor *>();
}

CodeEditor *FileTabWidget::getEditorByPath(const QString &path)
{
    return editors.value(path, nullptr);
}

QMap<QString, CodeEditor *> *FileTabWidget::getMap()
{
    return &editors;
}
