#ifndef IDATABASE_H
#define IDATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QDataWidgetMapper>

class IDataBase : public QObject
{
    Q_OBJECT
public:
    static IDataBase &getInstance()
    {
        static IDataBase instance;
        return instance;
    }

    // 最近打开文件路径
    QStringList getLastOpenFilePaths();
    bool addLastOpenFilePath(const QString &filePath);
    bool deleteAllLastOpenFilePaths();

    // 
    bool addFavoriteFilePath(const QString &filePath);
    bool deleteFavoriteFilePath(const QString &filePath);
    QStringList getFavoriteFilePaths();

private:
    explicit IDataBase(QObject *parent = nullptr);
    IDataBase(IDataBase const &) = delete;
    void operator=(IDataBase const &) = delete;

    QSqlDatabase database;

    void initDataBase();

signals:
};

#endif // IDATABASE_H
