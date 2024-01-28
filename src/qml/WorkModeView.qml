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
    title: qsTr("Select mode")
    header: Header {
        id: pageHeader
        title: viewModel.currentMode ? viewModel.currentMode.name
                                     : parent.title

        ComboBox {
            model: viewModel.devices
            textRole: "text"
            valueRole: "value"
            currentIndex: model.findIndex(viewModel.currentDevice)
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredWidth: 250
            onCurrentValueChanged: {
                viewModel.currentDevice = currentValue
            }
        }
        Label {
            id: deviceErrorIcon
            text: "\uE783"
            color: "red"
            font {
                family: "Segoe MDL2 Assets"
                bold: true
                pixelSize: 24
            }
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            visible: viewModel.deviceError

            SequentialAnimation {
                loops: Animation.Infinite
                running: viewModel.deviceError
                PropertyAnimation {target: deviceErrorIcon; property: "opacity"; from: 1.0; to: 0.0; duration: 500 }
                PropertyAnimation {target: deviceErrorIcon; property: "opacity"; from: 0.0; to: 1.0; duration: 500 }
            }

            ToolTip.delay: AppStyle.tooltipDelay
            ToolTip.timeout: 5000
            ToolTip.visible: viewModel.deviceError && hovered
            ToolTip.text: qsTr("Failed to use selected device")
            Layout.preferredWidth: 30
        }
    }

    WorkModeViewModel {
        id: viewModel
        onCurrentModeChanged: {
            if(viewModel.currentMode){
                modeStack.push(Qt.resolvedUrl(viewModel.currentMode.qmlDelegate))
            }
        }
    }

    Connections {
        target: toolbar
        function onBackClicked() {
            modeStack.pop()
        }
    }

    Timer {
        id: popTimer
        interval: 0
        onTriggered: {
            viewModel.clearMode()
        }
    }

    StackView {
        id: modeStack
        anchors.fill: parent
        initialItem: "qrc:/qt/qml/vol2com/qml/WorkModeSelector.qml"
        Component.onCompleted: {
            if(viewModel.currentMode){
                push(viewModel.currentMode.qmlDelegate, {}, StackView.Immediate)
            }
        }
        onBusyChanged: {
            if(!busy && depth === 1){
                popTimer.start()
            }
        }
    }
}
