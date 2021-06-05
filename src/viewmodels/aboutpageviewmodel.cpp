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

#include "aboutpageviewmodel.h"
#include <QCoreApplication>
#include <array>

using namespace vol2com;

namespace
{
    struct ThirdPartyEntry
    {
        const wchar_t * const name;
        const wchar_t * const link;
    };

    constexpr std::array<ThirdPartyEntry, 2> ThirdPartyEntries =
    {{
         {L"Bass library", L"https://www.un4seen.com/"},
         {L"WinToast", L"https://github.com/mohabouje/WinToast/"}
    }};

    constexpr auto GitHubProjectUrl = "https://github.com/Mixxxxa/vol2com";

    QString getCommitLink()
    {
        return QLatin1String("<a href=\"%1/commit/%2\">%2</a>")
                .arg(GitHubProjectUrl)
                .arg(GIT_COMMIT);
    }

    QString getCompilerInfo()
    {
#if defined(Q_CC_CLANG)
        return QLatin1String("Clang " ) + QString::number(__clang_major__)
                     + QLatin1Char('.') + QString::number(__clang_minor__);
#elif defined(Q_CC_GNU)
        return QLatin1String("GCC ") + QLatin1String(__VERSION__);
#elif defined(Q_CC_MSVC)
        if(_MSC_VER > 1999)
            return QLatin1String("MSVC (unknown)");
        if(_MSC_VER >= 1920)
            return QLatin1String("MSVC 2019");
        if(_MSC_VER >= 1910)
            return QLatin1String("MSVC 2017");
        if(_MSC_VER >= 1900)
            return QLatin1String("MSVC 2015");
#endif
        return QLatin1String("<unknown compiler version>");
    }
}

QString AboutPageViewModel::aboutText() const
{
    return tr("<b>%1</b> - simple open source LED strip management software.")
            .arg(QCoreApplication::applicationName());
}

QString AboutPageViewModel::versionText() const
{
    return tr("Version: %1")
            .arg(QCoreApplication::applicationVersion());
}

QString AboutPageViewModel::commitText() const
{
    return tr("Build based on commit: %1 [%2]")
            .arg(getCommitLink(), QLatin1String(GIT_BRANCH));
}

QString AboutPageViewModel::qtInfoText() const
{
    return tr("App based on Qt version %1 (%2, %3 bit)")
            .arg(QLatin1String(QT_VERSION_STR),
                 getCompilerInfo(),
                 QString::number(QSysInfo::WordSize, 10));
}

QString AboutPageViewModel::disclaimerText() const
{
    return tr("The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.");
}

QString AboutPageViewModel::thirdPartyText() const
{
    QString result;
    for(size_t i = 0; i < ::ThirdPartyEntries.size(); ++i)
    {
        result += QStringLiteral("%1) %2 - <a href=\"%3\">%3</a>")
                .arg(static_cast<int>(i + 1))
                .arg(::ThirdPartyEntries[i].name, ::ThirdPartyEntries[i].link);
        if(i != ::ThirdPartyEntries.size() - 1)
            result += QStringLiteral("<br></br>");
    }
    return result;
}

QString AboutPageViewModel::githubLink() const
{
    return QLatin1String(::GitHubProjectUrl);
}

QString AboutPageViewModel::copyrightText() const
{
    return "Copyright (C) 2020-2021 Mixxxxa <gelvikhmikhail@gmail.com>";
}
