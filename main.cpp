#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <qdatetime.h>

#include "TaskManage.h"
#include "SystemTray.h"
#include <Appconfig.h>
#include <QQmlContext>
#include <Common.h>


int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);


    //注册C++对象给QML
    qmlRegisterType<TaskManage>("TaskManage",1,0,"TaskManage");
    qmlRegisterType<SystemTray>("SystemTray",1,0,"SystemTray");

    QQmlApplicationEngine engine;

    //将实例对象暴露给QML
    AppConfig _appConfig;
    engine.rootContext()->setContextProperty("AppInit" , &_appConfig);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
