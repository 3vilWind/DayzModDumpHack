#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QMap>
#include <QSet>
#include <QString>
#include <QVariant>
#include <QSettings>


class SettingsManager
{
public:
    SettingsManager();
    ~SettingsManager();
    static SettingsManager& instance();
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
    void setValue(const QString &key, const QVariant &value);

    SettingsManager(SettingsManager const&) = delete;
    SettingsManager& operator= (SettingsManager const&) = delete;
private:
    QMap<QString, QVariant> data;
    QSettings settings;
};

#endif // SETTINGSMANAGER_H
