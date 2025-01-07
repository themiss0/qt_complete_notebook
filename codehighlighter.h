#ifndef CODEHIGHLIGHTER_H
#define CODEHIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QRegularExpression>
#include "highlightrulereader.h"

class CodeHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit CodeHighlighter(QTextDocument *parent = nullptr);

    // 设置高亮规则
    void setHighlightingRules(const QVector<HighlightingRule> &rules);

protected:
    void highlightBlock(const QString &text) override;

private:
    QVector<HighlightingRule> highlightingRules;
};



#endif // CODEHIGHLIGHTER_H
