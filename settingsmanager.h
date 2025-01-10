#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>
#include <QVariant>

class SettingsManager : public QObject
{
    Q_OBJECT

public:
    // 获取单例实例
    static SettingsManager& getInstance();

    // 读取设置值
    QVariant getValue(const QString &key, const QVariant &defaultValue = QVariant()) const;

    // 写入设置值
    void setValue(const QString &key, const QVariant &value);

    // 删除设置值
    void remove(const QString &key);

    // 检查键是否存在
    bool contains(const QString &key) const;

    // 同步设置到磁盘
    void sync();

private:
    // 私有构造函数和析构函数
    explicit SettingsManager(QObject *parent = nullptr);
    ~SettingsManager();

    // 禁用拷贝构造函数和赋值运算符
    SettingsManager(const SettingsManager &) = delete;
    SettingsManager &operator=(const SettingsManager &) = delete;

    // QSettings 对象
    QSettings *m_settings;
};

#endif // SETTINGSMANAGER_H
