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
    // 添加书签
    bool addLabel(const QString &filePath, int row, const QString &message);
    // 删除书签
    bool deleteLabel(int id);
    // 根据文件路径获取书签列表
    QList<QMap<QString, QVariant>> getLabelsByFilePath(const QString &filePath);
    // 删除所有行
    bool deleteAllLabels(const QString &filename);

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
