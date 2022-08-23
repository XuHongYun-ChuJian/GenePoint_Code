#include "SystemTray.h"

////////////////////////////////////////////////////////
/// class CustomAction implementation
////////////////////////////////////////////////////////
CustomAction::CustomAction(QObject *parent):
    QAction(parent)
{

}

QUrl CustomAction::icon()
{
    return m_icon;
}

void CustomAction::setIcon(const QUrl &_icon)
{
    if(_icon == m_icon)
    {
        return;
    }

    QString str = _icon.toLocalFile();
    if(str == "") str = _icon.toString();     //如果转换失败
    if( str.mid (0, 3) == "qrc")
        str = str.mid (3, str.count() - 3);
    QAction::setIcon(QIcon(str));

    m_icon = _icon;
    emit iconChanged();
}


////////////////////////////////////////////////////////
/// class SystemTray implementation
////////////////////////////////////////////////////////
SystemTray::SystemTray(QObject *parent) : QObject(parent)
{
    init();
}

void SystemTray::init()
{
    m_SystemTrayIcon = new QSystemTrayIcon (this);

    m_SystemTrayIcon->setToolTip("大兄弟，你好！");
    m_SystemTrayIcon->setIcon(QIcon(":/image/时间.png"));

    m_SystemTrayIcon->show();
}


