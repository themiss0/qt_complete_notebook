#ifndef CODEEDITOR_H
#define CODEEDITOR_H
#include <QObject>
#include <QPlainTextEdit>
#include <QPaintEvent>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);  // 构造函数

    // 行号区域相关函数
    void lineNumberAreaPaintEvent(QPaintEvent *event);   // 绘制行号
    int lineNumberAreaWidth();                           // 计算行号区域宽度
    void hideLineNumberArea(bool flag);                  // 显示/隐藏行号

protected:
    void resizeEvent(QResizeEvent *event) override;     // 重写大小调整事件

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);   // 更新行号区域宽度
    void highlightCurrentLine();                        // 高亮当前行
    void updateLineNumberArea(const QRect &rect, int dy); // 更新行号区域

private:
    QWidget *lineNumberArea;                           // 行号显示区域
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
    CodeEditor *codeEditor;             // 所属编辑器
};
#endif
