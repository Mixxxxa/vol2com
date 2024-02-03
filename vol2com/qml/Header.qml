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
import QtQuick.Layouts
import vol2com

Rectangle {
    id: control
    property alias title: titleText.text
    default property alias contentItem: layout.children

    color: AppStyle.accentDark1
    width: parent.width
    height: 80

    RowLayout {
        id: layout
        anchors {
            fill: parent
            leftMargin: AppStyle.pageMargins
            rightMargin: AppStyle.pageMargins
        }
        Text {
            id: titleText
            text: "Title"
            font {
                family: "Segoe UI"
                weight: Font.DemiBold
                pixelSize: 40
            }
            elide: Text.ElideRight
            bottomPadding: 12
            color: "#FFFFFF"
            Layout.fillWidth: true
        }
    }
}
