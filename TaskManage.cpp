#include "TaskManage.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QDateTime>
#include <Common.h>


TaskManage::TaskManage(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<TaskDetail>("TaskDetail");
    qRegisterMetaType<TaskGroup>("TaskGroup");

    TaskGroup m_taskGroup;
    loadFile(TaskConfigFilePath , m_taskGroup);

    fwInfo() << "TaskMange初始化完成！";

    connect(&m_timer, &QTimer::timeout, this , [&](){
        excuteTask();
    });

    m_timer.start(1000);
    fwInfo()<< "任务定时器启动" << m_timer.interval() << m_timer.isActive();
}

TaskManage::~TaskManage()
{
    fwInfo()<< "taskManage 销毁";
}

void TaskManage::loadFile(const QString &filePath, TaskGroup &taskGroup)
{
    QFile file(filePath);
    if(!file.open(QFile::Text | QFile::ReadOnly))
    {
        fwErro()<< filePath << "文件打开失败：" << file.errorString();
        return;
    }

    fwInfo() << "文件打开成功";

    QJsonParseError jsonError;
    QJsonDocument   docment = QJsonDocument::fromJson(file.readAll() , &jsonError);
    if(docment.isNull())
    {
        fwErro()<< "Json文件打开失败：" << jsonError.errorString();
        return;
    }

    QJsonObject jsonObject = docment.object();
    m_jsonObject = jsonObject;
    m_jsonString = docment.toJson();

    fwInfo()<< QString(docment.toJson());

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

    //ToDo
    m_willExcuteTasks = taskGroup.tasks;
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

void TaskManage::excuteTask()
{
    if(m_willExcuteTasks.isEmpty())
    {
        fwInfo()<< "当前没有任务";
        return;
    }

    for(auto task : m_willExcuteTasks)
    {
        if(task.excuteTime.toString("hh:mm:ss") != QTime::currentTime().toString("hh:mm:ss"))
        {
            continue;
        }

        fwInfo()<< "执行任务" << task.taskId << task.excuteTime.toString("hh:mm:ss") << QTime::currentTime().toString("hh:mm:ss");
        emit sig_SendTaskDetail(task.detail);
        emit sig_SendTaskTaskName(task.name);
        emit sig_SendTaskExcuteTime(task.delay);
//        m_willExcuteTasks.removeOne(task);
    }
}
