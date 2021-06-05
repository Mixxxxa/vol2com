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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QVariant>
#include <QMetaEnum>

namespace vol2com
{
    class Settings : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
        Q_PROPERTY(bool autostart READ autostart WRITE setAutostart NOTIFY autostartChanged)
        Q_PROPERTY(bool notificationsAvailable READ notificationsAvailable WRITE setNotificationsAvailable NOTIFY notificationsAvailableChanged)
        Q_PROPERTY(bool showNotifications READ showNotifications WRITE setShowNotifications NOTIFY showNotificationsChanged)
        Q_PROPERTY(bool photosensitiveWarningAccepted READ photosensitiveWarningAccepted WRITE setPhotosensitiveWarningAccepted NOTIFY photosensitiveWarningAcceptedChanged)

    public:
        enum class Keys
        {
            Version,
            Language,
            Autostart,
            ShowNotifications,
            PhotosensitiveWarningAccepted
        };
        Q_ENUM(Keys)

        template <class Section=QString, class Key>
        QVariant get(const Section& section, const Key& key, const QVariant& defaultValue)
        {
            const auto a = QVariant::fromValue(section).toString();
            const auto b = QVariant::fromValue(key).toString();
            return m_settings.value(QStringLiteral("%1/%2").arg(a, b), defaultValue);
        }

        template<>
        QVariant get(const QVariant& section, const QVariant& key, const QVariant& defaultValue)
        {
            return m_settings.value(QStringLiteral("%1/%2").arg(section.toString(), key.toString()), defaultValue);
        }

        template <class Section=QString, class Key>
        int getInt(const Section& section, const Key& key, int min, int max, int defaultValue)
        {
            const auto a = QVariant::fromValue(section).toString();
            const auto b = QVariant::fromValue(key).toString();

            bool check = false;
            const int result = get(a, b, defaultValue).toInt(&check);
            if(check && (result >= min) && (result <= max))
                return result;
            return defaultValue;
        }

        template <class Section=QString, class Key, class EnumT>
        EnumT getEnum(const Section& section, const Key& key, const EnumT& defaultValue)
        {
            const auto ref = QMetaEnum::fromType<EnumT>();
            const auto a = QVariant::fromValue(section).toString();
            const auto b = QVariant::fromValue(key).toString();

            bool check = false;
            const int result = get(a, b, -1).toInt(&check);
            if(check && (result != -1) && (ref.valueToKey(result) != nullptr))
                return static_cast<EnumT>(result);
            return defaultValue;
        }

        template <class Section=QString, class Key>
        void set(const Section& section, const Key& key, const QVariant& value)
        {
            const QString a = QVariant::fromValue(section).toString();
            const QString b = QVariant::fromValue(key).toString();
            m_settings.setValue(QStringLiteral("%1/%2").arg(a, b), value);
        }

        static Settings& getInstance();
        ~Settings();

        QString language() const { return m_language; }
        bool autostart() const { return m_autostart; }
        bool showNotifications() const { return m_showNotifications; }
        bool notificationsAvailable() const { return m_notificationsAvailable; }
        bool photosensitiveWarningAccepted() const { return m_photosensitiveWarningAccepted; }

    public slots:
        void setLanguage(const QString& language);
        void setAutostart(bool autostart);
        void setShowNotifications(bool showNotifications);
        void setNotificationsAvailable(bool notificationsAvailable);
        void setPhotosensitiveWarningAccepted(bool photosensitiveWarning);
        void save();
        void load();
        void sync();

    signals:
        void languageChanged(const QString& language);
        void autostartChanged(bool autostart);
        void showNotificationsChanged(bool showNotifications);
        void notificationsAvailableChanged(bool notificationsAvailable);
        void photosensitiveWarningAcceptedChanged(bool photosensitiveWarning);

    private:
        QVariant get(const Settings::Keys& key, const QVariant& defaultValue);
        void set(const Settings::Keys& key, const QVariant& value);

        Settings(QObject *parent = nullptr);

        QSettings m_settings;
        QString m_language;
        bool m_autostart;
        bool m_showNotifications;
        bool m_notificationsAvailable;
        bool m_photosensitiveWarningAccepted;
    };
}

#endif // SETTINGS_H
