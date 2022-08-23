#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QAction>
#include <QUrl>

/////////////////////////////////////////////////////////////////////
/// class CustomAction define
/////////////////////////////////////////////////////////////////////
class CustomAction : QAction
{
    Q_OBJECT
    Q_PROPERTY(QUrl icon READ icon WRITE setIcon NOTIFY iconChanged)
public:
    explicit CustomAction(QObject *parent = nullptr);

    QUrl icon();
    void setIcon(const QUrl &_icon);

signals:
    void iconChanged();

private:
    QUrl m_icon;
};


/////////////////////////////////////////////////////////////////////
/// class SystemTray define
/////////////////////////////////////////////////////////////////////
class SystemTray : public QObject
{
    Q_OBJECT
public:
    explicit SystemTray(QObject *parent = nullptr);

    void init();

signals:

private:
    QSystemTrayIcon *m_SystemTrayIcon{nullptr};

};

#endif // SYSTEMTRAY_H
