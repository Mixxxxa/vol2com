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
import vol2com as VC

Rectangle {
    id: viewTitle
    readonly property int windowControlWidth: 58
    height: 40
    color: VC.AppStyle.accentDark1

    MouseArea {
        property point clickPos: Qt.point(1,1)

        onPressed: function(mouse) {
            clickPos = Qt.point(mouse.x,mouse.y)
        }
        onPositionChanged: function(mouse) {
            mainWindow.x = mainWindow.x + mouse.x - clickPos.x
            mainWindow.y = mainWindow.y + mouse.y - clickPos.y
        }
        anchors.fill: parent;
    }

    RowLayout {
        spacing: 0
        anchors.fill: parent
        anchors.leftMargin: VC.AppStyle.pageMargins

        Text {
            text: Qt.application.name
            color: "#FFFFFF"
            bottomPadding: 6
            font {
                family: "Segoe UI"
                pixelSize: 15
            }
            Layout.fillWidth: true
        }

        TitleBarButton {
            id: minimizeButton
            text: "\uE921"
            Layout.preferredWidth: viewTitle.windowControlWidth
            Layout.preferredHeight: viewTitle.height
            onClicked: mainWindow.showMinimized()
        }

        TitleBarButton {
            id: closeButton
            text: "\uE8BB"
            hoveredColor: "red"
            Layout.preferredWidth: viewTitle.windowControlWidth
            Layout.preferredHeight: viewTitle.height
            onClicked: VC.Controller.exit()
        }
    }
}
