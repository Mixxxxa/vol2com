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
import vol2com as VC

Rectangle {
    id: toolbar

    property int indicatorMargin: 5
    signal changeView(int index)
    signal backClicked

    width: 50
    color: VC.AppStyle.theme === VC.AppStyle.Dark ? "#1f1f1f" : "#e6e6e6"

    VC.ToolbarViewModel {
        id: viewModel
    }

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        TitleBarButton {
            id: mainButton
            text: viewModel.backarrowActive? "\uE72B" : ""
            hoveredColor: VC.AppStyle.accentDark1
            enabled: viewModel.backarrowActive
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 40
            ToolTip.text: qsTr("Change mode")
            ToolTip.visible: hovered
            ToolTip.delay: VC.AppStyle.tooltipDelay
            onClicked: {
                backClicked()
            }

            Image {
                anchors.fill: parent
                anchors.margins: 6
                visible: !viewModel.backarrowActive
                source: "qrc:/vol2com/res/icon-active.svg"
                sourceSize.width: height
                sourceSize.height: height
                verticalAlignment: Image.AlignVCenter
                horizontalAlignment: Image.AlignHCenter
                fillMode: Image.PreserveAspectFit
            }
        }

        StandartButton {
            id: connectButton
            text: "\uED5C"
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.width
            ToolTip.text: qsTr("Connection")
            ToolTip.visible: hovered
            ToolTip.delay: VC.AppStyle.tooltipDelay
            onClicked: {
                VC.Controller.page = VC.Controller.ConnectPage
            }
        }

        StandartButton {
            id: workModeButton
            text: "\uE71D"
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.width
            ToolTip.text: qsTr("Modes")
            ToolTip.visible: hovered
            ToolTip.delay: VC.AppStyle.tooltipDelay
            onClicked: {
                VC.Controller.page = VC.Controller.WorkModePage
            }
        }

        StandartButton {
            id: equalizerButton
            text: "\uE9E9"
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.width
            ToolTip.text: qsTr("Equalizer")
            ToolTip.visible: hovered
            ToolTip.delay: VC.AppStyle.tooltipDelay
            onClicked: {
                VC.Controller.page = VC.Controller.EqualizerPage
            }
        }

        Item {
            Layout.fillHeight: true
        }

        StandartButton {
            id: settingsButton
            text: "\uE713"
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.width
            ToolTip.text: qsTr("Settings")
            ToolTip.visible: hovered
            ToolTip.delay: VC.AppStyle.tooltipDelay
            onClicked: {
                VC.Controller.page = VC.Controller.SettingsPage
            }
        }

        StandartButton {
            id: aboutButton
            text: "\uE946"
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.width
            ToolTip.text: qsTr("About")
            ToolTip.visible: hovered
            ToolTip.delay: VC.AppStyle.tooltipDelay
            onClicked: {
                VC.Controller.page = VC.Controller.AboutPage
            }
        }
    }

    Rectangle {
        id: pageIndicator
        color: VC.AppStyle.accent
        width: 3
        height: 50 - toolbar.indicatorMargin * 2
        radius: 1
        anchors {
            left: toolbar.left
            leftMargin: 4
        }
    }

    states: [
        State {
            when: viewModel.page === VC.Controller.ConnectPage
            PropertyChanges {
                target: pageIndicator;
                y: connectButton.y + toolbar.indicatorMargin
            }
        },
        State {
            when: viewModel.page === VC.Controller.WorkModePage
            PropertyChanges {
                target: pageIndicator;
                y: workModeButton.y + toolbar.indicatorMargin
            }
        },
        State {
            when: viewModel.page === VC.Controller.EqualizerPage
            PropertyChanges {
                target: pageIndicator;
                y: equalizerButton.y + toolbar.indicatorMargin
            }
        },
        State {
            when: viewModel.page === VC.Controller.SettingsPage
            PropertyChanges {
                target: pageIndicator;
                y: settingsButton.y + toolbar.indicatorMargin
            }
        },
        State {
            when: viewModel.page === VC.Controller.AboutPage
            PropertyChanges {
                target: pageIndicator;
                y: aboutButton.y + toolbar.indicatorMargin
            }
        }
    ]

    transitions: Transition {
        ParallelAnimation {
            PropertyAnimation {
                properties: "y";
                easing.type: Easing.InOutQuad
                duration: 200
            }
        }
    }
}
