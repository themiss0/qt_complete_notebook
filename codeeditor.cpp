#include "codeeditor.h"
#include <QObject>
#include <QPlainTextEdit>
#include <QPaintEvent>
#include <QPainter>
#include <QTextBlock>
#include <QTextEdit>
#include <QApplication>
#include <QCursor>

// 构造函数:初始化行号区域并连接信号槽
CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    // 连接信号槽
    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    setMouseTracking(true);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

// 计算行号区域宽度
int CodeEditor::lineNumberAreaWidth()
{
    // 计算最大行号的位数
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    // 计算所需空间宽度
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

// 更新行号区域宽度
void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

// 更新行号区域显示
void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    // 如果是垂直滚动,则滚动行号区域
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        // 否则更新行号区域对应的矩形区域
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    // 如果视口矩形发生变化,更新行号区域宽度
    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

// 处理窗口大小改变事件
void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}
// 鼠标按下事件
void CodeEditor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && (QApplication::keyboardModifiers() & Qt::ControlModifier))
    {
        QTextCursor cursor = cursorForPosition(event->pos());
        QString anchor = cursor.charFormat().anchorHref();
        if (!anchor.isEmpty())
        {
            QDesktopServices::openUrl(QUrl(anchor));
        }
    }
    QPlainTextEdit::mousePressEvent(event);
}
// 鼠标移动事件
void CodeEditor::mouseMoveEvent(QMouseEvent *event)
{

    if ((QApplication::keyboardModifiers() & Qt::ControlModifier))
    {
        QTextCursor cursor = cursorForPosition(event->pos());
        QString anchor = cursor.charFormat().anchorHref();
        if (!anchor.isEmpty())
        {
            viewport()->setCursor(Qt::PointingHandCursor);
        }
        else
        {
            viewport()->setCursor(Qt::IBeamCursor);
        }
    }
    else
    {
        viewport()->setCursor(Qt::IBeamCursor);
    }
    QPlainTextEdit::mouseMoveEvent(event);
}
// 插入超链接
void CodeEditor::insertHyperlink(const QString &text, const QString &url)
{
    QTextCharFormat format;
    format.setAnchor(true);
    format.setAnchorHref(url);
    format.setForeground(Qt::blue);
    format.setFontUnderline(true);

    QTextCursor cursor = textCursor();
    cursor.insertText(text, format);
}
// 键盘按下事件
void CodeEditor::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
    {
        QTextCursor cursor = cursorForPosition(QCursor::pos());
        QString anchor = cursor.charFormat().anchorHref();
        if (!anchor.isEmpty())
        {
            viewport()->setCursor(Qt::PointingHandCursor);
        }
        else
        {
            viewport()->setCursor(Qt::IBeamCursor);
        }
    }
    else
    {
        viewport()->setCursor(Qt::IBeamCursor);
    }
    QPlainTextEdit::keyPressEvent(event);
}
// 键盘释放事件
void CodeEditor::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
    {

            viewport()->setCursor(Qt::IBeamCursor);
    }
    QPlainTextEdit::keyReleaseEvent(event);
}

// 检测超链接
void CodeEditor::detectHyperlink()
{
    QString text = toPlainText();
    QRegularExpression urlRegex(R"((https?://\S+))");
    QRegularExpressionMatchIterator it = urlRegex.globalMatch(text);

    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();
    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        QString url = match.captured(0);
        cursor.setPosition(match.capturedStart());
        cursor.setPosition(match.capturedEnd(), QTextCursor::KeepAnchor);
        QTextCharFormat format;
        format.setAnchor(true);
        format.setAnchorHref(url);
        format.setForeground(Qt::blue);
        format.setFontUnderline(true);
        cursor.setCharFormat(format);
    }
    cursor.endEditBlock();
}

void CodeEditor::cleanAllHyperlink()
{
    QTextCursor cursor(document());          // 获取文档的光标
    cursor.movePosition(QTextCursor::Start); // 将光标移动到文档开头

    // 选择整个文档
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);

    // 清除样式
    QTextCharFormat defaultFormat;         // 默认格式
    cursor.setCharFormat(defaultFormat); // 应用默认格式
}

// 高亮当前行
void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    // 仅在非只读模式下高亮当前行
    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        // 设置高亮颜色为淡黄色
        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}

// 绘制行号区域
void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    // 初始化画笔
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    // 获取第一个可见文本块
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    // 绘制行号
    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

// 控制行号区域显示/隐藏
void CodeEditor::hideLineNumberArea(bool flag)
{
    if (flag)
    {
        lineNumberArea->hide();
        setViewportMargins(0, 0, 0, 0);
    }
    else
    {
        lineNumberArea->show();
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    }
}
