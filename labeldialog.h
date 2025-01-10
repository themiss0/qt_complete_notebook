#ifndef LABELDIALOG_H
#define LABELDIALOG_H

#include "qlistwidget.h"
#include <QDialog>
#include <QVector>
#include "bookmark.h"

namespace Ui
{
    class LabelDialog;
}

class LabelDialog : public QDialog
{
    Q_OBJECT

public:
    LabelDialog(QWidget *parent);

    void addBookmark(int row, const QString &message); // 添加书签
    void removeBookmark(int row);                      // 删除书签
    void clearBookmarks();                             // 清空书签
    QSet<int> getBookmarkRows();                       // 获取所有书签行号
    void save(const QString &filepath);
    bool isRowBookmarked(int row); // 判断行是否有书签
    void loadBookmarksFromDatabase(const QString &filepath); // 从数据库加载书签信息

signals:
    void jumpToLineRequested(int row); // 跳转到指定行的信号

private slots:
    void onItemClicked(QListWidgetItem *item);            // 点击书签项
    void onCustomContextMenuRequested(const QPoint &pos); // 右键菜单

private:
    Ui::LabelDialog *ui;
    QVector<Bookmark> labelList;
    bool isChange;
    QSet<int> bookmarkRows;

};

#endif // LABELDIALOG_H
