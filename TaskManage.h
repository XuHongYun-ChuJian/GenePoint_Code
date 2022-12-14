#ifndef TASKMANAGE_H
#define TASKMANAGE_H

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QTime>
#include <QTimer>


struct TaskDetail
{
    enum TimeUnit{ms , s , m , h};

    int         taskId;      //<任务Id，唯一标识号
    QString     name;        //<任务名
    QString     detail;      //<详情
    QTime       excuteTime;  //<任务执行时间
    int         delay;       //<休息时间
    QString     unit;        //<时间单位

    TaskDetail():
        taskId(0),name(QString()),detail(QString()),excuteTime(QTime()),delay(-1),unit(TimeUnit::ms)
    {

    }

    inline bool operator==(const TaskDetail& other)
    {
        return other.taskId == this->taskId;
    }
};
Q_DECLARE_METATYPE(TaskDetail)

inline QDebug operator<<(QDebug debug , const TaskDetail &taskDetail)
{
    debug.space() << "\n"
                  << taskDetail.taskId
                  << "\n"
                  << taskDetail.name
                  << "\n"
                  << taskDetail.detail
                  << "\n"
                  << taskDetail.excuteTime.toString()
                  << "\n"
                  << taskDetail.delay
                  << "\n"
                  << taskDetail.unit
                  << "\n";
    return debug.maybeSpace();
}

struct TaskGroup
{
    QString groupName;       //<组名
    int     groupId;         //<组Id，唯一标识号
    QList<TaskDetail> tasks; //<任务集合
};
Q_DECLARE_METATYPE(TaskGroup)

inline QDebug operator<<(QDebug debug , const TaskGroup &taskGroup)
{
    debug.space() << "groupName:" << taskGroup.groupName << "groupId：" << taskGroup.groupId << "\n" << "tasks:" << taskGroup.tasks;
    return debug.maybeSpace();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The TaskManage class
///////////////////////////////////////////////////////////////////////////////////////////////////////
class TaskManage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TaskGroup m_taskGroup READ getTaskGroup WRITE setTaskGroup NOTIFY TaskGroupChanged)
public:
    explicit TaskManage(QObject *parent = nullptr);
    ~TaskManage();

public:
    void loadFile(const QString &filePath , TaskGroup &taskGroup);

    void setTaskGroup(const TaskGroup& group);
    TaskGroup getTaskGroup();

    Q_INVOKABLE QJsonObject getJsonObject(); //Json文件转换为QJsonObject，传递到QML  弃用
    Q_INVOKABLE QString     getJsonString(); //Json文件转换为String，传递到QML解析    弃用
    Q_INVOKABLE void        excuteTask(); //执行任务

signals:
    void TaskGroupChanged();

    void sig_SendTaskDetail( QString str);
    void sig_SendTaskTaskName( QString str);
    void sig_SendTaskExcuteTime( int seconds);

private:
    TaskGroup   m_taskGroup;
    QJsonObject m_jsonObject;
    QString     m_jsonString;
    QTimer      m_timer;
    QList<TaskDetail> m_willExcuteTasks;
};

#endif // TASKMANAGE_H
