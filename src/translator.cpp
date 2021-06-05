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

#include "translator.h"
#include "settings.h"
#include <array>
#include <QLocale>
#include <QGuiApplication>

using namespace vol2com;

namespace
{
    static constexpr std::array<const char * const, 2> SupportedLanguages =
    {
        "en_US",
        "ru_RU"
    };

    bool isSupportedLanguage(const QString& langCode)
    {
        return std::find(::SupportedLanguages.begin(),
                         ::SupportedLanguages.end(),
                         langCode) != ::SupportedLanguages.end();
    }

    QString normalizeUILanguage(const QString& lang)
    {
        if(lang.size() == 5 && lang.contains('-'))
        {
            auto normalized = lang;
            return normalized.replace('-', '_');
        }

        if(lang.compare("ru"))
            return "ru_RU";

        return lang;
    }
}

Translator::Translator(QObject *parent) : QObject(parent)
{

}

void Translator::loadTranslation()
{
    auto& settings = Settings::getInstance();
    const QString language = settings.language();

    if(language.isEmpty())
    {
        const auto uiLanguages = QLocale::system().uiLanguages();
        if(uiLanguages.size() > 0)
        {
            const auto preferedLang = normalizeUILanguage(uiLanguages.constFirst());
            if(loadLanguage(preferedLang))
            {
                settings.setLanguage(preferedLang);
                return;
            }
        }
    }
    else
    {
        if(loadLanguage(language))
            return;
    }

    settings.setLanguage(::SupportedLanguages[0]);
}

bool Translator::loadLanguage(const QString &lang)
{
    if(lang == ::SupportedLanguages[0])
    {
        return true;
    }

    if(isSupportedLanguage(lang))
    {
        const auto translationsPath = qApp->applicationDirPath() + QStringLiteral("/translations");
        if(m_tr.load(QStringLiteral("vol2com2_%1.qm").arg(lang), translationsPath))
        {
            qApp->installTranslator(&m_tr);
            return true;
        }
    }
    return false;
}
