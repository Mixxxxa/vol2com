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

#ifndef APPSTYLE_H
#define APPSTYLE_H

#include <QObject>
#include <QColor>
#include <QFont>
#include <winrt/base.h>
#include <winrt/Windows.UI.ViewManagement.h>

namespace vol2com
{
    class AppStyle : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(Theme theme READ theme WRITE setTheme NOTIFY themeChanged)
        Q_PROPERTY(QColor accent READ accent NOTIFY accentChanged)
        Q_PROPERTY(QColor accentLight1 READ accentLight1 NOTIFY accentLight1Changed)
        Q_PROPERTY(QColor accentLight2 READ accentLight2 NOTIFY accentLight2Changed)
        Q_PROPERTY(QColor accentLight3 READ accentLight3 NOTIFY accentLight3Changed)
        Q_PROPERTY(QColor accentDark1 READ accentDark1 NOTIFY accentDark1Changed)
        Q_PROPERTY(QColor accentDark2 READ accentDark2 NOTIFY accentDark2Changed)
        Q_PROPERTY(QColor accentDark3 READ accentDark3 NOTIFY accentDark3Changed)
        Q_PROPERTY(QColor foreground READ foreground NOTIFY foregroundChanged)
        Q_PROPERTY(QColor background READ background NOTIFY backgroundChanged)
        Q_PROPERTY(QColor grayText READ grayText NOTIFY grayTextChanged)
        Q_PROPERTY(QFont textFont READ textFont NOTIFY textFontChanged)
        Q_PROPERTY(QFont iconFont READ iconFont NOTIFY iconFontChanged)
        Q_PROPERTY(int pageMargins READ pageMargins NOTIFY pageMarginsChanged)
        Q_PROPERTY(int tooltipDelay READ tooltipDelay NOTIFY tooltipDelayChanged)

    public:
        enum class Theme
        {
            Light,
            Dark
        };
        Q_ENUM(Theme)

        static AppStyle& getInstance();

        Theme theme() const { return m_theme; }
        QColor accent() const { return m_accent; }
        QColor accentLight1() const { return m_accentLight1; }
        QColor accentLight2() const { return m_accentLight2; }
        QColor accentLight3() const { return m_accentLight3; }
        QColor accentDark1() const { return m_accentDark1; }
        QColor accentDark2() const { return m_accentDark2; }
        QColor accentDark3() const { return m_accentDark3; }
        QColor foreground() const { return m_foreground; }
        QColor background() const { return m_background; }
        QColor grayText() const { return m_grayText; }
        QFont textFont() const { return m_textFont; }
        QFont iconFont() const { return m_iconFont; }
        int pageMargins() const { return m_pageMargins; }
        int tooltipDelay() const { return m_tooltipDelay; }
        void onWindowsColorsChanged(const winrt::Windows::UI::ViewManagement::UISettings& sender,
                                    const winrt::Windows::Foundation::IInspectable& val);

    public slots:
        void setTheme(Theme theme);

    signals:
        void themeChanged(Theme theme);
        void accentChanged(QColor accent);
        void accentLight1Changed(QColor accentLight1);
        void accentLight2Changed(QColor accentLight2);
        void accentLight3Changed(QColor accentLight3);
        void accentDark1Changed(QColor accentDark1);
        void accentDark2Changed(QColor accentDark2);
        void accentDark3Changed(QColor accentDark3);
        void foregroundChanged(QColor foreground);
        void backgroundChanged(QColor background);
        void grayTextChanged(QColor grayText);
        void textFontChanged(QFont textFont);
        void iconFontChanged(QFont iconFont);
        void pageMarginsChanged(int pageMargins);
        void tooltipDelayChanged(int tooltipDelay);

    private:
        constexpr QColor WinRTColorToQColor(const winrt::Windows::UI::Color& color) const;
        void repaint();

        AppStyle(QObject *parent = nullptr);
        AppStyle(const AppStyle&) = delete;
        AppStyle(AppStyle&&) = delete;
        AppStyle& operator=(const AppStyle&) = delete;
        AppStyle& operator=(AppStyle&&) = delete;

        const winrt::Windows::UI::ViewManagement::UISettings m_uiSettings;
        winrt::Windows::UI::ViewManagement::UISettings::ColorValuesChanged_revoker m_colorsChangeToken;

        Theme m_theme;
        QColor m_accent;
        QColor m_accentLight1;
        QColor m_accentLight2;
        QColor m_accentLight3;
        QColor m_accentDark1;
        QColor m_accentDark2;
        QColor m_accentDark3;
        QColor m_foreground;
        QColor m_background;
        QColor m_grayText;
        QFont m_textFont;
        QFont m_iconFont;
        int m_pageMargins;
        int m_tooltipDelay;
    };
}

#endif // APPSTYLE_H
