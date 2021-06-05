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

import QtQuick 2.15
import QtQuick.Controls 2.15
import vol2com 1.0

Button {
    property alias backgroundColor: backgroundRect.color

    background: Rectangle {
        id: backgroundRect
        border {
            width: checked ? 2 : 0
            color: AppStyle.foreground
        }

        Label {
            width: 25
            height: 25
            anchors {
                right: parent.right
                top: parent.top
            }
            visible: checked
            background: Rectangle {
                color: AppStyle.foreground
            }
            font {
                family: "Segoe MDL2 Assets"
                pixelSize: 24
                bold: true
            }
            color: AppStyle.background
            text: "\uE73E" //check mark
            horizontalAlignment: Text.AlignVCenter | Text.AlignHCenter
        }
    }
    flat: true
    implicitWidth: 52
    implicitHeight: 52
    autoExclusive: true
    checkable: true
}
