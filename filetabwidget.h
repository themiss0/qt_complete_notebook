#ifndef FILETABWIDGET_H
#define FILETABWIDGET_H

#include <QTabWidget>
#include <QMap>
#include "codeeditor.h"
#include <QString>

namespace Ui
{
    class FileTabWidget;
}

class FileTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit FileTabWidget(QWidget *parent);
    ~FileTabWidget();

    void closeTab(int index);

    bool isCloseAble(const CodeEditor *editor);

    CodeEditor *newTab(const QString &pathName);
    CodeEditor *getCurrentEditor() const;
    CodeEditor *getEditorByPath(const QString &path);
    CodeEditor *getEditorByWidget(const QWidget &wg);
    QMap<QString, CodeEditor *> *getMap();


signals:
    void save();

private:
    Ui::FileTabWidget *ui;
    QMap<QString, CodeEditor *> editors;

    void closeCurrentTab();
    void closeAllTabs();
};

#endif // FILETABWIDGET_H
