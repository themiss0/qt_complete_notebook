#include "codeeditor.h"
#include <QObject>
#include <QPlainTextEdit>
#include <QPaintEvent>
#include <QPainter>
#include <QTextBlock>
#include <QTextEdit>
#include <QApplication>
#include <QCursor>
#include "qmenu.h"
#include <QInputDialog>

// 构造函数:初始化行号区域并连接信号槽
CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    this->filepath = filepath;
    lineNumberArea = new LineNumberArea(this);
    labelDialog = new LabelDialog(this);
    labelDialog->hide();

    // 连接信号槽
    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(labelDialog, &LabelDialog::jumpToLineRequested, this, &CodeEditor::jumpToRow);

    setMouseTracking(true);
    updateLineNumberAreaWidth(0);
}

//
void CodeEditor::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu(); // 创建标准右键菜单

    // 获取当前光标所在行
    QTextCursor cursor = cursorForPosition(event->pos());
    int currentLine = cursor.blockNumber() + 1;

    // 检查当前行是否已有标签

    // 添加 "Add Label" 或 "Delete Label" 菜单项
    if (labelDialog->isRowBookmarked(currentLine))
    {
        QAction *deleteLabelAction = menu->addAction("删除书签");
        connect(deleteLabelAction, &QAction::triggered, this, [this, currentLine]()
                {
                    this->labelDialog->removeBookmark(currentLine); // 删除当前行的标签
                });
    }
    else
    {
        QAction *addLabelAction = menu->addAction("添加书签");
        connect(addLabelAction, &QAction::triggered, this, [this, currentLine]()
                {
                    QString message = QInputDialog::getText(this, "添加书签", "输入书签注释："); // 获取用户输入的标签信息
                    if (message.isEmpty())
                    {
                        return;
                    }
                    this->labelDialog->addBookmark(currentLine, message); // 添加当前行的标签
                });
    }

    menu->exec(event->globalPos()); // 显示菜单
    delete menu;                    // 释放菜单内存
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
    int space = 8 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

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
void CodeEditor::jumpToRow(int row)
{
    QTextBlock block = document()->findBlockByLineNumber(row - 1);
    if (!block.isValid())
    {
        return; // 如果行号无效，直接返回
    }

    // 将光标移动到指定行
    QTextCursor cursor(block);
    setTextCursor(cursor);

    setTextCursor(cursor);
    ensureCursorVisible();
}
// 绘制行号区域
void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    // 初始化画笔
    QPainter painter(lineNumberArea);
    // painter.fillRect(event->rect(), Qt::lightGray);

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
            painter.setPen(Qt::green);

            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
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
            if (anchor.contains("@"))
            {
                // 处理邮箱链接
                anchor = "mailto:" + anchor;
            }
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
    // 检测ftp连接、http连接和邮箱
    QRegularExpression urlRegex(R"((https?://\S+)|([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})|(ftp://\S+))");
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
        format.setForeground(QColor("#ce9178"));
        format.setFontUnderline(true);
        cursor.setCharFormat(format);
    }
    cursor.endEditBlock();
}
// 清除全局样式
void CodeEditor::cleanAllFormat()
{
    QTextCursor cursor(document());          // 获取文档的光标
    cursor.movePosition(QTextCursor::Start); // 将光标移动到文档开头

    // 选择整个文档
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);

    // 清除样式
    QTextCharFormat defaultFormat;       // 默认格式
    cursor.setCharFormat(defaultFormat); // 应用默认格式
}
// 读取高亮规则
void CodeEditor::setHightligter(const QString &language, const QString &theme)
{
    if (highlighter != nullptr)
    {
        delete highlighter;
    }
    cleanAllFormat();
    highlighter = new CodeHighlighter(document());
    QVector<HighlightingRule> rules = RuleReader::readRules("../../highlightRule/" + theme + ".json", language);
    highlighter->setHighlightingRules(rules);
}

void CodeEditor::showLabelDialog()
{
    labelDialog->show();
}

void CodeEditor::hideLabelDialog()
{
    labelDialog->hide();
}
