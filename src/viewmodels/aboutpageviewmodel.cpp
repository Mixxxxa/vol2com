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
#include <QUrl>
#include <QVersionNumber>
#include "utils/gitinfo.h"

namespace
{
  QString CreateHTMLLink(QString name, QString link)
  {
    const auto url = QUrl{ link };
    Q_ASSERT(url.isValid());
    const auto escapedName = name.toHtmlEscaped();
    Q_ASSERT(name == escapedName);

    return QStringLiteral(R"delim(<a href="%2">%1</a>)delim").arg(escapedName, url.toEncoded());
  }

  QString getCompilerInfo()
  {
    using namespace Qt::StringLiterals;

#if defined(Q_CC_CLANG)
    const auto clangVersion = QVersionNumber(__clang_major__, __clang_minor__, __clang_patchlevel__);
    return u"Clang %1"_s.arg(clangVersion.normalized().toString());
#elif defined(Q_CC_GNU)
    return u"GCC %1"_s.arg(__VERSION__);
#elif defined(Q_CC_MSVC)
    QString msvcRelease;
    if constexpr(_MSC_VER >= 1930 && _MSC_VER < 1940)
    {
      msvcRelease = u"2022"_s;
    }
    else if constexpr(_MSC_VER >= 1920)
    {
      msvcRelease = u"2019"_s;
    }
    else if constexpr(_MSC_VER >= 1910)
    {
      msvcRelease = u"2017"_s;
    }
    else if constexpr(_MSC_VER >= 1900)
    {
      msvcRelease = u"2015"_s;
    }
    else
    {
      msvcRelease = u"Unknown"_s;
    }

    const auto check = u"MSVC %1 (%2)"_s.arg(msvcRelease).arg(_MSC_VER);

    return u"MSVC %1 (%2)"_s.arg(msvcRelease).arg(_MSC_VER);
#endif
    return u"Unknown compiler"_s;
  }
}

namespace vol2com
{
  QString AboutPageViewModel::aboutText() const
  {
    return tr("<b>%1</b> - simple open source LED strip management software.")
        .arg(QCoreApplication::applicationName());
  }

  QString AboutPageViewModel::versionText() const
  {
    return tr("Version: %1").arg(QCoreApplication::applicationVersion());
  }

  QString AboutPageViewModel::commitText() const
  {
    using namespace Qt::StringLiterals;

    const auto &branch   = QStringLiteral(GIT_BRANCH);
    const auto &shortRev = QStringLiteral(GIT_SHORT_REV);
    const auto &fullRev  = QStringLiteral(GIT_FULL_REV);

    if(fullRev.isEmpty() || shortRev.isEmpty())
    {
      return u"Build based on custom build"_s;
    }

    const auto &commitLink = u"%1/commit/%2"_s.arg(githubLink(), fullRev);
    auto result = tr("Build based on commit: %1").arg(CreateHTMLLink(shortRev, commitLink));

    if(!branch.isEmpty())
    {
      const auto &branchLink = u"%1/tree/%2"_s.arg(githubLink(), branch);
      result += u" [%1]"_s.arg(CreateHTMLLink(branch, branchLink));
    }

    return result;
  }

  QString AboutPageViewModel::qtInfoText() const
  {
    return tr("App based on Qt version %1 (%2, %3 bit)")
        .arg(QStringLiteral(QT_VERSION_STR),
             getCompilerInfo(),
             QString::number(QSysInfo::WordSize, 10));
  }

  QString AboutPageViewModel::disclaimerText() const
  {
    return tr("The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.");
  }

  QString AboutPageViewModel::thirdPartyText() const
  {
    using namespace Qt::StringLiterals;
    using ThirdPartyEntry = std::pair<QLatin1StringView,
                                      QLatin1StringView>;

    static constexpr std::array<ThirdPartyEntry, 2> ThirdPartyEntries =
    {{
        { "Bass library"_L1, "https://www.un4seen.com/"_L1 }
      , { "WinToast"_L1,     "https://github.com/mohabouje/WinToast/"_L1 }
    }};

    QString result;
    for(auto &&[name, link] : ThirdPartyEntries)
    {
      if(!result.isEmpty())
      {
        result += QStringLiteral("<br></br>");
      }
      result += u"%1 - %2"_s.arg(name, CreateHTMLLink(link, link));
    }

    return result;
  }

  QString AboutPageViewModel::githubLink() const
  {
    using namespace Qt::StringLiterals;
    return u"https://github.com/Mixxxxa/vol2com"_s;
  }

  QString AboutPageViewModel::copyrightText() const
  {
    using namespace Qt::StringLiterals;
    return u"Copyright (C) 2020-2023 Mixxxxa"_s;
  }

  QString AboutPageViewModel::wikiUrl() const
  {
    using namespace Qt::StringLiterals;
    return u"%1/wiki"_s.arg(githubLink());
  }
}
