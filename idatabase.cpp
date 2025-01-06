#include "idatabase.h"
#include <QUuid>
#include <QVariant>

// 数据库操作类构造函数
IDataBase::IDataBase(QObject *parent)
    : QObject{parent}
{
    initDataBase();
}

// 获取最近打开文件的路径列表
QStringList IDataBase::getLastOpenFilePaths()
{
    QStringList filePaths;
    QSqlQuery query("SELECT path FROM lastOpenFile");

    if (!query.exec())
    {
        qDebug() << "Query execution failed:" << query.lastError().text();
        return filePaths;
    }

    while (query.next())
    {
        filePaths << query.value(0).toString();
    }
    return filePaths;
}
// 向lastOpenFiles表写入路径字符串
bool IDataBase::addLastOpenFilePath(const QString &filePath)
{
    QSqlQuery query;

    // 插入新路径
    query.prepare("INSERT INTO lastOpenFile (path) VALUES (:path)");
    query.bindValue(":path", filePath);

    if (!query.exec())
    {
        qDebug() << "Insert query execution failed:" << query.lastError().text();
        return false;
    }

    // 检查表中数据行数
    query.prepare("SELECT COUNT(*) FROM lastOpenFile");
    if (!query.exec() || !query.next())
    {
        qDebug() << "Count query execution failed:" << query.lastError().text();
        return false;
    }

    int rowCount = query.value(0).toInt();

    // 如果行数超过十行，删除id最小的行
    if (rowCount > 10)
    {
        query.prepare("DELETE FROM lastOpenFile WHERE id = (SELECT id FROM lastOpenFile ORDER BY id ASC LIMIT 1)");
        if (!query.exec())
        {
            qDebug() << "Delete query execution failed:" << query.lastError().text();
            return false;
        }
    }

    return true;
}
// 删除所有最近打开文件路径
bool IDataBase::deleteAllLastOpenFilePaths()
{
    QSqlQuery query("DELETE FROM lastOpenFile");
    if (!query.exec())
    {
        qDebug() << "Delete query execution failed:" << query.lastError().text();
        return false;
    }
    return true;
}

// 添加收藏文件路径
bool IDataBase::addFavoriteFilePath(const QString &filePath)
{
    QSqlQuery query;

    // 插入新路径
    query.prepare("INSERT INTO favPath (path) VALUES (:path)");
    query.bindValue(":path", filePath);

    if (!query.exec())
    {
        qDebug() << "Insert query execution failed:" << query.lastError().text();
        return false;
    }
}
// 删除收藏文件路径
bool IDataBase::deleteFavoriteFilePath(const QString &filePath)
{
    QSqlQuery query;
    query.prepare("DELETE FROM favPath WHERE path = :path");
    query.bindValue(":path", filePath);
    if (!query.exec())
    {
        qDebug() << "Delete query execution failed:" << query.lastError().text();
        return false;
    }
    return true;
}
// 获取收藏文件路径
QStringList IDataBase::getFavoriteFilePaths()
{
    QStringList filePaths;
    QSqlQuery query("SELECT path FROM favPath");
    if (!query.exec())
    {
        qDebug() << "Query execution failed:" << query.lastError().text();
        return filePaths;
    }
    while (query.next())
    {
        filePaths << query.value(0).toString();
    }
    return filePaths;
}

// 初始化数据库连接
void IDataBase::initDataBase()
{
    // 创建SQLite数据库连接
    database = QSqlDatabase::addDatabase("QSQLITE");

    // 设置数据库文件路径
    QString aFile = "../../database.db";
    QString context = QDir::currentPath();
    qDebug() << context;

    database.setDatabaseName(aFile);
    if (!database.open())
    {
        qDebug() << "fail to open database";
    }
    else
    {
        qDebug() << "open database succes";
    }
}
