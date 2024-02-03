
#include "src/testsql.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif
#include <QLocale>
#include <QTranslator>
#include <QQmlContext>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    qputenv("QT_DEBUG_PLUGINS", QByteArray("1"));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

#ifdef Q_OS_ANDROID
    const QStringList permissions = {"android.permission.CAMERA","android.permission.READ_MEDIA_VIDEO","android.permission.READ_MEDIA_IMAGES","android.permission.READ_MEDIA_VISUAL_USER_SELECTED","android.permission.WRITE_EXTERNAL_STORAGE","android.permission.READ_EXTERNAL_STORAGE"};
    foreach (const auto &ipemission, permissions) {
        auto  result = QtAndroid::checkPermission(ipemission);
        if(result == QtAndroid::PermissionResult::Denied){
            QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList(ipemission));
            if(resultHash[ipemission] == QtAndroid::PermissionResult::Denied)
                qCritical() << "Denied permission"  << ipemission;
        }
    }
#endif
    qmlRegisterType<TestSql>("App", 1, 0, "TestSql");
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "QtAppFirebird_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    const auto drivers = QSqlDatabase::drivers();
    engine.rootContext()->setContextProperty("_drivers", drivers.join("-"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    QObject::connect(&engine, &QQmlApplicationEngine::warnings, &app, [](const QList<QQmlError> &warnings){
        QString errorText;
        foreach (auto err, warnings) {
            errorText.append(err.toString());
        }
        qCritical() <<errorText;
    });
    engine.load(url);

    return app.exec();
}
