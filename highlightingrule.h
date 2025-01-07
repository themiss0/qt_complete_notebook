#ifndef HIGHLIGHTINGRULE_H
#define HIGHLIGHTINGRULE_H

#include "qregularexpression.h"
#include <QTextCharFormat>

struct HighlightingRule
{
    QRegularExpression pattern;
    QTextCharFormat format;
};
#endif // HIGHLIGHTINGRULE_H
