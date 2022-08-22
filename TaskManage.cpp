#include "TaskManage.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

TaskManage::TaskManage(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<TaskDetail>("TaskDetail");
    qRegisterMetaType<TaskGroup>("TaskGroup");

    TaskGroup m_taskGroup;
    loadFile(ConfigFilePath , m_taskGroup);

    qDebug()<< m_taskGroup;
}

void TaskManage::loadFile(const QString &filePath, TaskGroup &taskGroup)
{
    QFile file(filePath);
    if(!file.open(QFile::Text | QFile::ReadOnly))
    {
        qDebug()<< filePath << "文件打开失败：" << file.errorString();
        return;
    }

    qDebug()<< "文件打开成功";

    QJsonParseError jsonError;
    QJsonDocument   docment = QJsonDocument::fromJson(file.readAll() , &jsonError);
    if(docment.isNull())
    {
        qDebug()<< "Json文件打开失败：" << jsonError.errorString();
        return;
    }

    QJsonObject jsonObject = docment.object();
    m_jsonObject = jsonObject;
    m_jsonString = docment.toJson();

    qDebug()<< QString(docment.toJson());

    taskGroup.groupId = jsonObject.value("groupId").toInt();
    taskGroup.groupName = jsonObject.value("groupName").toString();

    QJsonArray jsonArray = jsonObject.value("tasks").toArray();
    for(int idx = 0 ; idx < jsonArray.size(); ++idx)
    {
        QJsonValue jsonValue = jsonArray.at(idx);
        QJsonObject _jsonObject = jsonValue.toObject();

        TaskDetail _detail;
        _detail.taskId = _jsonObject.value("taskId").toInt();
        _detail.name = _jsonObject.value("name").toString();
        _detail.detail = _jsonObject.value("detail").toString();
        _detail.excuteTime = QTime::fromString(_jsonObject.value("time").toString() , "hh:mm:ss");
        _detail.delay = _jsonObject.value("delay").toInt();
        _detail.unit = _jsonObject.value("TimeUnit").toString();

        taskGroup.tasks.push_back(_detail);
    }

    file.close();

}

void TaskManage::setTaskGroup(const TaskGroup &group)
{
    m_taskGroup = group;
    emit TaskGroupChanged();
}

TaskGroup TaskManage::getTaskGroup()
{
    return m_taskGroup;
}

QJsonObject TaskManage::getJsonObject()
{
    return m_jsonObject;
}

QString TaskManage::getJsonString()
{
    return m_jsonString;
}
