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

Page {
    id: settingsPage
    title: qsTr("Settings")
    header: Header {
        title: parent.title
    }

    StandartView {
        anchors.fill: parent
        ListDelegate {
            text: qsTr("Language:")
            ComboBox {
                model: [
                    { name: qsTr("English"), value: "en_US" },
                    { name: qsTr("Russian"), value: "ru_RU" }
                ]
                textRole: "name"
                valueRole: "value"
                currentIndex: model.findIndex(x => x.value === Settings.language)
                onCurrentValueChanged: {
                    Settings.language = currentValue
                }
                Layout.fillWidth: true
                Layout.preferredWidth: 200
            }
        }

        ListDelegate {
            text: qsTr("Theme:")
            ComboBox {
                model: [
                    { name: qsTr("Light"), value: AppStyle.Light },
                    { name: qsTr("Dark"), value: AppStyle.Dark }
                ]
                textRole: "name"
                valueRole: "value"
                currentIndex: model.findIndex(x => x.value === AppStyle.theme )
                onCurrentValueChanged: {
                    AppStyle.theme = currentValue
                }
                Layout.fillWidth: true
                Layout.preferredWidth: 200
            }
        }

        Switch {
            text: qsTr("Run on system startup")
            enabled: false
            font: AppStyle.textFont
            padding: 0
            checked: Settings.autostart
            width: parent.width
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }
        Switch {
            text: qsTr("Show notifications")
            font: AppStyle.textFont
            padding: 0
            enabled: Settings.notificationsAvailable
            checked: Settings.showNotifications && Settings.notificationsAvailable
            width: parent.width
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }
    }
}
