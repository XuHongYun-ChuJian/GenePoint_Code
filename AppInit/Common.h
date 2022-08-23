#ifndef COMMON_H
#define COMMON_H

#include <QTime>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>


#define TaskConfigFilePath QCoreApplication::applicationDirPath() + QDir::separator() + "TaskConfig.json"
#define AppConfigFilePath QCoreApplication::applicationDirPath() + QDir::separator() + "app.conf"

#define TIMEDATEFORMAT QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") + " "
#define qlog() qDebug() << TIMEDATEFORMAT << Q_FUNC_INFO

#define fwInfo() qDebug() << TIMEDATEFORMAT << QStringLiteral("信息") << Q_FUNC_INFO
#define fwErro() qDebug() << TIMEDATEFORMAT << QStringLiteral("错误") << Q_FUNC_INFO
#define fwarning() qDebug() << TIMEDATEFORMAT << QStringLiteral("警告") << Q_FUNC_INFO

#endif // COMMON_H
