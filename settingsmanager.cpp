#include "settingsmanager.h"

// 初始化静态成员变量
SettingsManager& SettingsManager::getInstance() {
    static SettingsManager instance; // 静态局部变量，确保唯一实例
    return instance;
}

// 构造函数
SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent), m_settings(new QSettings("MyCompany", "MyApp", this)) {
    // 初始化 QSettings
}

// 析构函数
SettingsManager::~SettingsManager() {
    // 自动释放 QSettings
}

// 读取设置值
QVariant SettingsManager::getValue(const QString &key, const QVariant &defaultValue) const {
    return m_settings->value(key, defaultValue);
}

// 写入设置值
void SettingsManager::setValue(const QString &key, const QVariant &value) {
    m_settings->setValue(key, value);
    sync();
}

// 删除设置值
void SettingsManager::remove(const QString &key) {
    m_settings->remove(key);
}

// 检查键是否存在
bool SettingsManager::contains(const QString &key) const {
    return m_settings->contains(key);
}

// 同步设置到磁盘
void SettingsManager::sync() {
    m_settings->sync();
}
