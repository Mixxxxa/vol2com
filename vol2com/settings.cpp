// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/****************************************************************************
**
** Copyright (C) 2020-2021 Mixxxxa <gelvikhmikhail@gmail.com>.
**
** This file is part of vol2com.
**
** vol2com is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** any later version.
**
** vol2com is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with vol2com. If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "settings.h"
#include <QCoreApplication>
#include <array>
#include <QJSEngine>

using namespace vol2com;

namespace
{
    // Maybe will be used in further versions
    static constexpr int CurrentVersion = 1;

    QString getSettingsFilename()
    {
        return QStringLiteral("%1/settings.cfg").arg(QCoreApplication::applicationDirPath());
    }
}

Settings::Settings(QObject *parent) :
    QObject(parent),
    m_settings(getSettingsFilename(), QSettings::IniFormat, this),
    m_language(""),
    m_autostart(false),
    m_showNotifications(true),
    m_notificationsAvailable(false),
    m_photosensitiveWarningAccepted(false)
{
    load();
}

Settings *Settings::create(QQmlEngine *, [[maybe_unused]] QJSEngine *engine) // TODO check engine use
{
  Settings *result = &(Settings::getInstance());
  QJSEngine::setObjectOwnership(result, QJSEngine::ObjectOwnership::CppOwnership);
  return result;
}

Settings& Settings::getInstance()
{
    static Settings instance;
    return instance;
}

Settings::~Settings()
{
    save();
}

void Settings::save()
{
    set(Keys::Version, ::CurrentVersion);
    set(Keys::Language, m_language);
    set(Keys::Autostart, m_autostart);
    set(Keys::ShowNotifications, m_showNotifications);
    set(Keys::PhotosensitiveWarningAccepted, m_photosensitiveWarningAccepted);
    m_settings.sync();
}

void Settings::load()
{
    constexpr int UnknownSettingsVersion = -1;

    const int settingsFileVersion = get(Keys::Version, UnknownSettingsVersion).toInt();
    if(settingsFileVersion != ::CurrentVersion)
    {
        save();
        return;
    }

    m_language = get(Keys::Language, "").toString();
    m_autostart = get(Keys::Autostart, false).toBool();
    m_showNotifications = get(Keys::ShowNotifications, true).toBool();
    m_photosensitiveWarningAccepted = get(Keys::PhotosensitiveWarningAccepted, false).toBool();
}

void Settings::sync()
{
    m_settings.sync();
}

QVariant Settings::get(const Settings::Keys &key, const QVariant &defaultValue)
{
    const auto a = QVariant::fromValue(key).toString();
    return m_settings.value(a, defaultValue);
}

void Settings::set(const Settings::Keys &key, const QVariant &value)
{
    const auto a = QVariant::fromValue(key).toString();
    m_settings.setValue(a, value);
}

void Settings::setLanguage(const QString &language)
{
    if (m_language == language)
        return;

    m_language = language;
    emit languageChanged(m_language);
}

void Settings::setAutostart(bool autostart)
{
    if (m_autostart == autostart)
        return;

    m_autostart = autostart;
    emit autostartChanged(m_autostart);
}

void Settings::setShowNotifications(bool showNotifications)
{
    if (m_showNotifications == showNotifications)
        return;

    m_showNotifications = showNotifications;
    emit showNotificationsChanged(m_showNotifications);
}

void Settings::setNotificationsAvailable(bool notificationsAvailable)
{
    if (m_notificationsAvailable == notificationsAvailable)
        return;

    m_notificationsAvailable = notificationsAvailable;
    emit notificationsAvailableChanged(m_notificationsAvailable);
}

void Settings::setPhotosensitiveWarningAccepted(bool photosensitiveWarning)
{
    if (m_photosensitiveWarningAccepted == photosensitiveWarning)
        return;

    m_photosensitiveWarningAccepted = photosensitiveWarning;
    emit photosensitiveWarningAcceptedChanged(m_photosensitiveWarningAccepted);
}
