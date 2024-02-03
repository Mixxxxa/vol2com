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

Dialog {
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: parent.width * 0.8
    title: qsTr("Photosensivity Warning")
    closePolicy: Popup.NoAutoClose
    modal: true
    contentItem: Item {
        ColumnLayout {
            anchors.fill: parent
            Label {
                id: popupText
                text: qsTr("A small percentage of people may experience seizures when exposed to certain lights, patterns or images, even with no history of epilepsy or seizures. If you experience any of the following symptoms while using this app - dizziness, altered vision, eye or muscle twitches, loss of awareness, disorientation, any involuntary movement, or convulsions - <b>IMMEDIATELY</b> discontinue to use it!")
                font: AppStyle.textFont
                lineHeight: 1.25
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignJustify
                Layout.fillWidth: true
            }
            CheckBox {
                text: qsTr("Don't show this again")
                checked: Settings.photosensitiveWarningAccepted
                font: AppStyle.textFont
                Layout.fillWidth: true
                onCheckedChanged: {
                    Settings.photosensitiveWarningAccepted = checked
                }
            }
        }
    }
    footer: DialogButtonBox {
        Button {
            text: qsTr("Confirm")
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
        }
        Button {
            text: qsTr("Exit")
            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
        }
    }
}
