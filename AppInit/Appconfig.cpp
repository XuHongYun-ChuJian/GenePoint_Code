#include "Appconfig.h"
#include "Common.h"
#include <QFontDatabase>

#define GroupNameColor "Color"
#define KeyColor "color"

#define GroupNameFont "Font"
#define KeyFamily "font_family"
#define KeyBold "font_bold"
#define KeyPixelSize "font_pixelSize"


AppConfig::AppConfig(QObject *parent) : QObject(parent)
{
    m_color = Qt::green;
    m_font = QFont("微软雅黑");

    readFile(AppConfigFilePath);
}

AppConfig::~AppConfig()
{
    writeFile(AppConfigFilePath);
    fwInfo()<< "写入配置文件完成";
}

QColor AppConfig::color() const
{
    return m_color;
}

void AppConfig::setColor(const QColor &c)
{
    if( c == m_color)
        return;

    m_color = c;
    emit colorChanged();
}

QFont AppConfig::font() const
{
    return m_font;
}

void AppConfig::setFont(const QFont &f)
{
    if(f == m_font)
        return;

    m_font = f;
    emit fontChanged();
}

void AppConfig::readFile(const QString &filePath)
{
    if(!QFile::exists(filePath))
    {
        fwErro() << "filePath" <<"文件不存在,使用默认值";
        return;
    }
    QSettings settings(filePath , QSettings::Format::IniFormat);

    settings.beginGroup(GroupNameColor);
    m_color = QColor(settings.value(KeyColor).toString());
    settings.endGroup();

    settings.beginGroup(GroupNameFont);
    m_font.setFamily(settings.value(KeyFamily).toString());
    m_font.setBold(settings.value(KeyBold).toBool());
    m_font.setPointSize(settings.value(KeyPixelSize).toInt());
    settings.endGroup();
}

void AppConfig::writeFile(const QString &filePath)
{
    QSettings settings(filePath , QSettings::Format::IniFormat);

    settings.beginGroup(GroupNameColor);
    settings.setValue(KeyColor , m_color);
    settings.endGroup();

    settings.beginGroup(GroupNameFont);
    settings.setValue(KeyFamily , m_font.family());
    settings.setValue(KeyBold , m_font.bold());
    settings.setValue(KeyPixelSize , m_font.pointSize());
    settings.endGroup();
}
