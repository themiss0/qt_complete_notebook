#ifndef CODEEDITOR_H
#define CODEEDITOR_H
#include <QObject>
#include <QPlainTextEdit>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QRegularExpression>
#include "CodeHighlighter.h"

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr); // 构造函数

    // 行号区域相关函数
    void lineNumberAreaPaintEvent(QPaintEvent *event); // 绘制行号
    int lineNumberAreaWidth();                         // 计算行号区域宽度
    void hideLineNumberArea(bool flag);                // 显示/隐藏行号
    void detectHyperlink(); // 检测超链接
    void cleanAllFormat(); // 清除所有超链接
    void setHightligter(const QString &language); // 读取高亮规则

protected:
    void resizeEvent(QResizeEvent *event) override; // 重写大小调整事件
    void mousePressEvent(QMouseEvent *event) override;// 鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;// 鼠标移动事件
    void insertHyperlink(const QString &text, const QString &url); // 插入超链接
    void keyPressEvent(QKeyEvent *event) override; // 键盘按下事件
    void keyReleaseEvent(QKeyEvent *event) override; // 键盘释放事件

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);    // 更新行号区域宽度
    void highlightCurrentLine();                          // 高亮当前行
    void updateLineNumberArea(const QRect &rect, int dy); // 更新行号区域

private:
    QWidget *lineNumberArea; // 行号显示区域
    CodeHighlighter *highlighter = nullptr;
};

// 行号区域类
class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor)
    {
    }

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor; // 所属编辑器
};
#endif
