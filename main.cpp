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

#include <QApplication>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QLocale>
#include <QTranslator>
#include <QtQml/QQmlExtensionPlugin>

#include "vol2com/v2c.h"
#include "vol2com/controller.h"

Q_IMPORT_QML_PLUGIN(vol2comPlugin)

int main(int argc, char *argv[])
{
  QCoreApplication::setApplicationName(V2C_APP_NAME);
  QCoreApplication::setApplicationVersion(V2C_APP_VERSION_STR);

  QQuickStyle::setStyle(QLatin1String("Universal"));
  QApplication app(argc, argv);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "vol2com_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      app.installTranslator(&translator);
      break;
    }
  }

  auto& controller = vol2com::Controller::getInstance();
  controller.openUI();
  return app.exec();
}
