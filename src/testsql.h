#ifndef TESTSQL_H
#define TESTSQL_H

#include <QObject>

class TestSql : public QObject
{
    Q_OBJECT
public:
    explicit TestSql(QObject *parent = nullptr);
public slots:
    QString openPsql();
    bool closePsql();
    QString versionPsql();
    QString copyDB();
    QString dataLocation();
    bool writebleAppLocation();
    QString directoryDownLoad();
    QString nativeLibraryDir();
signals:
private:
    QString m_pathFbclient;
};

#endif // TESTSQL_H
