#include "testsql.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDirIterator>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniObject>
#endif
#include <QCoreApplication>
TestSql::TestSql(QObject *parent)
    : QObject{parent}
{

    // writebleAppLocation();
    copyDB();
    // TestSql::nativeLibraryDir();
    // TestSql::directoryDownLoad();
    const auto folder = dataLocation();;
    const QStringList paths = QCoreApplication::libraryPaths();
    // const QMap<QString,QString>map={
    //     {"libfbclient.so","libfbclient.so.2"},
    //     {"libicui18n.63.1.so","libicui18n.so.63"},
    //     {"libicuuc.63.1.so","libicuuc.so.63"},
    //     {"libicudata.63.1.so","libicudata.so.63"}
    // };
    QFile file(QString("%1/writelib.txt").arg(folder));
    if(!file.exists())
    {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            file.write("pier");
            file.close();
        }
    }
    for (int i = 0; i < paths.count(); ++i) {
        const QString &pluginDir = paths.at(i);
        qCritical() << "libraryPath qt" << pluginDir;
        QDirIterator it(pluginDir, QDirIterator::Subdirectories);
        // const auto libs = map.keys();
        while (it.hasNext()) {
            const auto libpath = it.next();
            qCritical()<<"libraryPath file" <<libpath;
            if(libpath.endsWith("libfbclient.so")){
                m_pathFbclient = libpath;
                break;
            }
        }

    }
    qCritical() << folder;
    QCoreApplication::addLibraryPath(folder);
}

QString TestSql::openPsql()
{
    // connection not found we add the connection
    //https://ib-aid.com/download/docs/fb4migrationguide.html#_installing_embedded
    const auto folder = dataLocation();
    const auto pathdb = QStringLiteral("%1/firebird/database.fdb").arg(folder);
    qCritical() << "pathdb" << pathdb;
    QSqlDatabase m_database = QSqlDatabase::addDatabase("QIBASE", "DB");
    //Check Valid database.;
    if (!m_database.isValid())
    {
        QString lastError = m_database.lastError().text();
        qCritical() << "isValid driver"<<lastError << qgetenv("FIREBIRD")<< getenv("FIREBIRD_TMP");
        return "Database driver is not valid";
    }
    const QString serverType = "1";
    m_database.setDatabaseName(pathdb);
    m_database.setHostName("");
    m_database.setUserName("SYSDBA");
    m_database.setPassword("firebird");
    bool embedded =true;
    if(embedded)
    {
        m_database.setHostName("");//TODO force ""
        m_database.setConnectOptions(QString("Database=%1;" \
                                             "User=%2;" \
                                             "Password=%3;" \
                                             "ServerType=%4;" \
                                             "Pooling=true;MinPoolSize=0;MaxPoolSize=50;" \
                                             "ClientLibrary=%5;")
                                         .arg(pathdb)
                                         .arg("SYSDBA")
                                         .arg("firebird")
                                         .arg(serverType)
                                         .arg(m_pathFbclient));
    }
    QFile file(pathdb);
    if(!file.exists()) return QString("database not exists %1").arg(pathdb);
    bool ok = m_database.open();
    if(!ok)
        qCritical() << m_database.lastError().text() << m_database.lastError().databaseText() << m_database.lastError().driverText() << m_database.lastError().nativeErrorCode();
    return ok?"OK":m_database.lastError().text();
}

bool TestSql::closePsql()
{
    QSqlDatabase db = QSqlDatabase::database("DB");
    if(!db.isValid()){
        qCritical() << "db not valid";
    }
    if(!db.isOpen())
        return true;
    db.close();
    return true;
}

QString TestSql::versionPsql()
{
    QSqlDatabase db = QSqlDatabase::database("DB");
    if(!db.isValid()){
        qCritical() << "db not valid";
    }
    if(!db.isOpen())
        db.open();
    QString v{};
    QSqlQuery query(db);
    if(!query.exec("SELECT rdb$get_context('SYSTEM', 'ENGINE_VERSION') as version from rdb$database;")){
        qCritical() << query.lastError().text();
        v = query.lastError().text();
        return v;
    }
    while (query.next()) {
        v = query.value(0).toString();
    }

    return v;
}

QString TestSql::copyDB()
{
    const auto folder = dataLocation();
    bool ok = true;
    QString error;

    //create directory
    const QStringList dbDirs = {"firebird","firebird/tmp","firebird/lock"};
    for (int i = 0; i < dbDirs.length(); ++i) {
        const auto idbdir = dbDirs.at(i);
        const auto pathdb = QStringLiteral("%1/%2").arg(folder,idbdir);
        QDir dir(pathdb);
        if(!dir.exists()){
            bool ok = dir.mkpath(".");
            if(!ok){
                qCritical() << "error to create "<<idbdir << "to" << pathdb;
                error.append(pathdb);
                break;
            }
            qDebug() <<idbdir << "to" << pathdb;
        }

        switch (i) {
        case 0:
            ok = qputenv("FIREBIRD",pathdb.toLatin1());
            setenv("FIREBIRD",pathdb.toLatin1(),1);
            qDebug() << "FIREBIRD:" << qgetenv("FIREBIRD")<< getenv("FIREBIRD");
            break;
        case 1:
            ok= qputenv("FIREBIRD_TMP",pathdb.toLatin1());
            setenv("FIREBIRD_TMP",pathdb.toLatin1(),1);
            qDebug() << "FIREBIRD_TMP:" << qgetenv("FIREBIRD_TMP")<< getenv("FIREBIRD_TMP");
            break;
        case 2:
            ok = qputenv("FIREBIRD_LOCK",pathdb.toLatin1());
            setenv("FIREBIRD_LOCK",pathdb.toLatin1(),1);
            qDebug() << "FIREBIRD_LOCK:" << qgetenv("FIREBIRD_LOCK") << getenv("FIREBIRD_LOCK");
            break;
        default:
            break;
        }

        if(!ok){
            qCritical() << "error to set evirement "<<idbdir << "to" << pathdb;
            error.append(pathdb);
            break;
        }
    }
    //copy file in folder
    QStringList fieldsDB= {};
    QDirIterator it(":/assets/firebird", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        const auto file = it.next();
        fieldsDB << file;
    }

    foreach (const auto fileDB, fieldsDB) {
        QFileInfo fileInfo(fileDB);
        const auto pathdb = QStringLiteral("%1/firebird/%2").arg(folder,fileInfo.fileName());
        QFile file(pathdb);
        ok=true;
        if(!file.exists())
            (ok =  QFile::copy(fileDB , pathdb));
        if(!ok){
            qCritical() << "error to copy"<<fileDB << "to" << pathdb;
            error.append(pathdb);
            break;
        }
        if(fileInfo.fileName().compare("database.fdb",Qt::CaseInsensitive)==0){
            (ok=QFile::setPermissions(pathdb, QFileDevice::ReadOwner|QFileDevice::WriteOwner));
        }
        if(!ok){
            qCritical() << "error to setPermissions"<<fileDB << "to" << pathdb;
            error.append(pathdb);
            break;
        }
        qDebug() <<fileDB << "to" << pathdb;
    }

    return ok?"OK":error;
}
QString TestSql::directoryDownLoad()
{
#ifdef Q_OS_ANDROID
    QAndroidJniObject string = QAndroidJniObject::callStaticObjectMethod<jstring>("it/piervalli/NotificationClient", "directoryDownLoad");
    QString sharedFilesPath =  QString(string.toString());
    qCritical() << "download" << sharedFilesPath;
    return sharedFilesPath;
#endif
    return QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
}
QString TestSql::nativeLibraryDir()
{
#ifdef Q_OS_ANDROID
    QAndroidJniObject string = QAndroidJniObject::callStaticObjectMethod<jstring>("it/piervalli/NotificationClient", "nativeLibraryDir");
    QString sharedFilesPath =  QString(string.toString());
    qCritical() << "nativeLibraryDir" << sharedFilesPath;
    return sharedFilesPath;
#endif
    return QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
}
QString TestSql::dataLocation()
{
    return  QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

bool TestSql::writebleAppLocation()
{
    const auto folder = dataLocation();
    QFile file(QStringLiteral("%1/test.txt").arg(folder));
    QDir dir;
    if(!dir.exists())
        dir.mkdir(folder);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    file.write("TEST\n");
    return dir.exists();
}
