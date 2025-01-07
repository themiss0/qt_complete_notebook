#include "highlightrulereader.h"


QVector<HighlightingRule> RuleReader::readRules(const QString &filePath, const QString &language)
{
    QVector<HighlightingRule> rules;

    // 打开 JSON 文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << filePath;
        return rules;
    }

    // 读取 JSON 数据
    QByteArray jsonData = file.readAll();
    file.close();

    // 解析 JSON 文档
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull()) {
        qWarning() << "Failed to parse JSON file:" << filePath;
        return rules;
    }

    // 获取规则数组
    QJsonObject rootObj = doc.object();
    QJsonArray ruleArray = rootObj.value(language).toArray();

    // 遍历规则数组
    for (const QJsonValue &ruleValue : ruleArray) {
        QJsonObject ruleObj = ruleValue.toObject();

        HighlightingRule rule;
        rule.pattern = QRegularExpression(ruleObj.value("pattern").toString());
        rule.format = parseFormat(ruleObj.value("format").toObject());

        rules.append(rule);
    }

    return rules;
}

QTextCharFormat RuleReader::parseFormat(const QJsonObject &formatObj)
{
    QTextCharFormat format;

    // 解析前景色
    if (formatObj.contains("foreground")) {
        QString colorName = formatObj.value("foreground").toString();
        format.setForeground(QColor(colorName));
    }

    // 解析字体粗细
    if (formatObj.contains("fontWeight")) {
        QString fontWeight = formatObj.value("fontWeight").toString();
        if (fontWeight == "bold") {
            format.setFontWeight(QFont::Bold);
        }
    }

    return format;
}
