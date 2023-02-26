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
import QtQuick.Layouts
import vol2com

Item {
    property alias text: _text.text
    default property alias contentItem: _layout.children

    height: 30
    width: parent.width

    RowLayout {
        id: _layout
        anchors.fill: parent
        Label {
            id: _text
            font: AppStyle.textFont
            Layout.preferredWidth: 160
        }
    }
}
