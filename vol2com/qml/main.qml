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

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Universal
import vol2com

Window {
    id: mainWindow
    width: 652
    height: 480
    visible: true
    title: "vol2com"
    flags: Qt.Window | Qt.FramelessWindowHint
    Universal.theme: AppStyle.theme
    Universal.background: AppStyle.background
    Universal.foreground: AppStyle.foreground
    Universal.accent: AppStyle.accent

    MainView {
        id: mainView
        anchors.fill: parent
    }
}
