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

//#pragma comment(lib, "WindowsApp")
#include "appstyle.h"
#include <QGuiApplication>
#include <QJSEngine>

//using namespace winrt;
//using namespace Windows::Foundation;
//using namespace Windows::UI::ViewManagement;
using namespace vol2com;

AppStyle& AppStyle::getInstance()
{
    static AppStyle instance;
    return instance;
}

//void AppStyle::onWindowsColorsChanged(const UISettings& sender, const IInspectable& val)
//{
//    Q_UNUSED(sender)
//    Q_UNUSED(val)
//    repaint();
//}

AppStyle *AppStyle::create(QQmlEngine *, QJSEngine *engine)
{
  auto *result = &(AppStyle::getInstance());
  QJSEngine::setObjectOwnership(result, QJSEngine::ObjectOwnership::CppOwnership);
  return result;
}

void AppStyle::setTheme(AppStyle::Theme theme)
{
    if (m_theme == theme)
        return;

    m_theme = theme;
    emit themeChanged(m_theme);
}

//constexpr QColor AppStyle::WinRTColorToQColor(const Windows::UI::Color& color) const
//{
//    return QColor(color.R, color.G, color.B);
//}

void AppStyle::repaint()
{
    //m_accent = WinRTColorToQColor(m_uiSettings.GetColorValue(UIColorType::Accent));
    //m_accentLight1 = WinRTColorToQColor(m_uiSettings.GetColorValue(UIColorType::AccentLight1));
    //m_accentLight2 = WinRTColorToQColor(m_uiSettings.GetColorValue(UIColorType::AccentLight2));
    //m_accentLight3 = WinRTColorToQColor(m_uiSettings.GetColorValue(UIColorType::AccentLight3));
    //m_accentDark1 = WinRTColorToQColor(m_uiSettings.GetColorValue(UIColorType::AccentDark1));
    //m_accentDark2 = WinRTColorToQColor(m_uiSettings.GetColorValue(UIColorType::AccentDark2));
    //m_accentDark3 = WinRTColorToQColor(m_uiSettings.GetColorValue(UIColorType::AccentDark3));
    ////m_foreground = WinRTColorToQColor(m_uiSettings.GetColorValue(UIColorType::Foreground));
    ////m_background = WinRTColorToQColor(m_uiSettings.GetColorValue(UIColorType::Background));
    m_foreground = QColor(m_theme == Theme::Dark ? Qt::white : Qt::black);
    m_background = QColor(m_theme == Theme::Dark ? Qt::black : Qt::white);
    //m_grayText = WinRTColorToQColor(m_uiSettings.UIElementColor(UIElementType::GrayText));
    //
    emit accentChanged(m_accent);
    emit accentDark1Changed(m_accentDark1);
    emit accentDark2Changed(m_accentDark2);
    emit accentDark3Changed(m_accentDark3);
    emit accentLight1Changed(m_accentLight1);
    emit accentLight2Changed(m_accentLight2);
    emit accentLight3Changed(m_accentLight3);
    emit foregroundChanged(m_foreground);
    emit backgroundChanged(m_background);
    emit grayTextChanged(m_grayText);
}

AppStyle::AppStyle(QObject *parent) :
    QObject(parent),
    m_theme(Theme::Dark),
    m_accent(234, 0, 94),
    m_accentLight1(247, 82, 148),
    m_accentLight2(247, 131, 178),
    m_accentLight3(255, 168, 203),
    m_accentDark1(172, 0, 70),
    m_accentDark2(136, 0, 55),
    m_accentDark3(107, 0, 43),
    m_foreground(Qt::white),
    m_background(Qt::black),
    m_grayText(109, 109, 109),
    m_textFont(QFont(QGuiApplication::font())),
    m_iconFont("Segoe MDL2 Assets"),
    m_pageMargins(20),
    m_tooltipDelay(500)
{
    m_textFont.setPixelSize(16);
    m_textFont.setWeight(QFont::Normal);
    m_iconFont.setPixelSize(18);
    //m_colorsChangeToken = m_uiSettings.ColorValuesChanged(winrt::auto_revoke, {this, &AppStyle::onWindowsColorsChanged});

    QObject::connect(this, &AppStyle::themeChanged,
                     this, &AppStyle::repaint);

    //repaint();
}
