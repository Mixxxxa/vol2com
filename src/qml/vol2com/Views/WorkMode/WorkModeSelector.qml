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
import QtQuick.Layouts 1.14
import vol2com 1.0

GridView {
    id: grid
    boundsBehavior: Flickable.StopAtBounds
    ScrollBar.vertical: ScrollBar { }
    cellWidth: 140
    cellHeight: 200
    model: viewModel.modes
    clip: true
    delegate: Component {
        Button {
            id: modeButton
            flat: true
            width: grid.cellWidth
            height: grid.cellHeight
            background: Item {}
            onClicked: viewModel.selectMode(id)

            ColumnLayout {
                anchors {
                    fill: parent
                    leftMargin: AppStyle.pageMargins
                    topMargin: AppStyle.pageMargins
                }

                Image {
                    source: image
                    width: parent.width
                    height: width
                    sourceSize.width: width
                    sourceSize.height: width
                    verticalAlignment: Image.AlignVCenter
                    horizontalAlignment: Image.AlignHCenter
                    fillMode: Image.PreserveAspectFit
                    antialiasing: true
                    opacity: modeButton.hovered ? 0.7 : 1.0
                }
                Text {
                    text: name
                    font {
                        pixelSize: 16
                        weight: Font.Bold
                        family: "Segoe UI"
                    }
                    color: AppStyle.foreground
                    elide: Text.ElideRight
                    Layout.preferredWidth: parent.width
                }
                Text {
                    text: description
                    font {
                        pixelSize: 16
                        weight: Font.Normal
                        family: "Segoe UI"
                    }
                    color: AppStyle.grayText
                    elide: Text.ElideRight
                    Layout.preferredWidth: parent.width
                    Layout.fillHeight: true
                    ToolTip.delay: AppStyle.tooltipDelay
                    ToolTip.timeout: 5000
                    ToolTip.visible: hovered
                    ToolTip.text: description
                }
            }
        }
    }
}
