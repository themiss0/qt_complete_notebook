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

    bool initPatientModel();
    QStringList getLastOpenFilePaths();
    bool addLastOpenFilePath(const QString &filePath);
    int userLogin(QString userName, QString password);
    int addPatient();
    bool submitPatient();
    bool deletePatient();
    void revertPatient();
    bool searchPatient(QString filter);

    QSqlTableModel *patientTabModel;
    QItemSelectionModel *thePatientSelection;

private:
    explicit IDataBase(QObject *parent = nullptr);
    IDataBase(IDataBase const &) = delete;
    void operator=(IDataBase const &) = delete;

    QSqlDatabase database;

    void initDataBase();

signals:
};

#endif // IDATABASE_H
