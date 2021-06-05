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
#ifndef ABOUTPAGEVIEWMODEL_H
#define ABOUTPAGEVIEWMODEL_H

#include <QObject>

namespace vol2com
{
    class AboutPageViewModel : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString aboutText READ aboutText CONSTANT)
        Q_PROPERTY(QString copyrightText READ copyrightText CONSTANT)
        Q_PROPERTY(QString versionText READ versionText CONSTANT)
        Q_PROPERTY(QString commitText READ commitText CONSTANT)
        Q_PROPERTY(QString qtInfoText READ qtInfoText CONSTANT)
        Q_PROPERTY(QString disclaimerText READ disclaimerText CONSTANT)
        Q_PROPERTY(QString thirdPartyText READ thirdPartyText CONSTANT)
        Q_PROPERTY(QString githubLink READ githubLink CONSTANT)

    public:
        AboutPageViewModel() = default;
        ~AboutPageViewModel() = default;

        QString aboutText() const;
        QString versionText() const;
        QString commitText() const;
        QString qtInfoText() const;
        QString disclaimerText() const;
        QString thirdPartyText() const;
        QString githubLink() const;
        QString copyrightText() const;
    };
}

#endif // ABOUTPAGEVIEWMODEL_H
