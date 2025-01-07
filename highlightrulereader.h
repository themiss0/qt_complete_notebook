#include "highlightingrule.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextCharFormat>
#include <QRegularExpression>

class RuleReader
{
public:
    static QVector<HighlightingRule> readRules(const QString &filePath, const QString &language);

private:
    static QTextCharFormat parseFormat(const QJsonObject &formatObj);
};
