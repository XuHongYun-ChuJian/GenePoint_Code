#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QColor>
#include <QFont>
#include <QObject>
#include <QSettings>

class AppConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
public:
    explicit AppConfig(QObject *parent = nullptr);
    ~AppConfig();

    QColor color() const;
    void setColor(const QColor &c);

    QFont font() const;
    void setFont(const QFont &f);

    void readFile(const QString& filePath);
    void writeFile(const QString& filePath);

signals:
    void colorChanged();
    void fontChanged();

private:
    QColor m_color;
    QFont m_font;

};

#endif // APPCONFIG_H
