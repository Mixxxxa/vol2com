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

StandartView {
    data: SerialViewModel {
        id: viewModel
    }

    ListDelegate {
        text: qsTr("Port:")
        enabled: viewModel.availableForEdit
        ComboBox {
            id: portCombo
            model: viewModel.portsModel
            textRole: "text"
            valueRole: "value"
            enabled: viewModel.arePortsAvailable
            currentIndex: model.findIndex(viewModel.port)
            Layout.fillWidth: true
            onCurrentValueChanged: {
                if(currentValue !== undefined)
                    viewModel.port = portCombo.currentValue
            }
        }
        StandartButton {
            text: "\uE72C"
            font.pixelSize: 16
            Layout.fillHeight: true
            ToolTip.text: qsTr("Refresh list")
            ToolTip.delay: AppStyle.tooltipDelay
            ToolTip.visible: hovered
            onClicked: {
                const lastValue = portCombo.currentValue
                viewModel.updatePortsList()
                const currentValue = portCombo.indexOfValue(lastValue)
                if(currentValue === -1)
                    portCombo.currentIndex = 0
                else
                    portCombo.currentIndex = currentValue
            }
        }
    }

    ListDelegate {
        text: qsTr("Baudrate:")
        enabled: viewModel.availableForEdit
        ComboBox {
            model: [
                { name: "9600",   value: QSerialPort.Baud9600 },
                { name: "19200",  value: QSerialPort.Baud19200 },
                { name: "38400",  value: QSerialPort.Baud38400 },
                { name: qsTr("57600 (recommended)"), value: QSerialPort.Baud57600 },
                { name: "115200", value: QSerialPort.Baud115200 }
            ]
            textRole: "name"
            valueRole: "value"
            currentIndex: model.findIndex(x => x.value === viewModel.baudrate)
            Layout.fillWidth: true
            onCurrentValueChanged: {
                viewModel.baudrate = currentValue
            }
        }
    }

    GridLayout {
        id: hertest
        enabled: viewModel.availableForEdit
        columns: 3
        width: parent.width
        columnSpacing: AppStyle.pageMargins
        rowSpacing: AppStyle.pageMargins / 2
        Label {
            text: qsTr("DataBits:")
            font: AppStyle.textFont
            Layout.fillWidth: true
        }
        Label {
            text: qsTr("Parity:")
            font: AppStyle.textFont
            Layout.fillWidth: true
        }
        Label {
            text: qsTr("Stop Bits:")
            font: AppStyle.textFont
            Layout.fillWidth: true
        }
        ComboBox {
            model: [
                { name: "5", value: QSerialPort.Data5 },
                { name: "6", value: QSerialPort.Data6 },
                { name: "7", value: QSerialPort.Data7 },
                { name: "8", value: QSerialPort.Data8 }
            ]
            textRole: "name"
            valueRole: "value"
            currentIndex: model.findIndex(x => x.value === viewModel.dataBits)
            Layout.fillWidth: true
            onCurrentValueChanged: {
                viewModel.dataBits = currentValue
            }
        }
        ComboBox {
            model: [
                { name: "No",    value: QSerialPort.NoParity },
                { name: "Even",  value: QSerialPort.EvenParity },
                { name: "Odd",   value: QSerialPort.OddParity },
                { name: "Space", value: QSerialPort.SpaceParity },
                { name: "Mark",  value: QSerialPort.MarkParity }
            ]
            textRole: "name"
            valueRole: "value"
            currentIndex: model.findIndex(x => x.value === viewModel.parity)
            Layout.fillWidth: true
            onCurrentValueChanged: {
                viewModel.parity = currentValue
            }
        }
        ComboBox {
            model: [
                { name: "1",   value: QSerialPort.OneStop },
                { name: "1.5", value: QSerialPort.OneAndHalfStop },
                { name: "2",   value: QSerialPort.TwoStop }
            ]
            textRole: "name"
            valueRole: "value"
            currentIndex: model.findIndex(x => x.value === viewModel.stopBits)
            Layout.fillWidth: true
            onCurrentValueChanged: {
                viewModel.stopBits = currentValue
            }
        }
    }

    ColumnLayout {
        spacing: 10
        Label {
            text: qsTr("Reconnect automaticaly")
            font: AppStyle.textFont
        }
        Switch {
            text: checked ? qsTr("On") : qsTr("Off")
            font: AppStyle.textFont
            checked: viewModel.autoReconnect
            Layout.preferredHeight: 30
            onCheckedChanged: {
                viewModel.autoReconnect = checked
            }
            padding: 0
        }
    }

    Button {
        id: processButton
        Layout.preferredHeight: 40
        Layout.preferredWidth: 200

        states: [
            State {
                name: "idle"
                when: viewModel.state === Serial.Idle
                PropertyChanges {
                    target:processButton
                    text: qsTr("Connect")
                    enabled: true
                    onClicked: {
                        viewModel.connectDevice()
                    }
                }
            },
            State {
                name: "connecting"
                when: viewModel.state === Serial.Connecting
                PropertyChanges {
                    target: processButton
                    text: qsTr("Connecting...")
                    enabled: false
                    onClicked: undefined
                }
            },
            State {
                name: "connected"
                when: viewModel.state === Serial.Connected
                PropertyChanges {
                    target: processButton
                    text: qsTr("Disconnect")
                    enabled: true
                    onClicked: {
                        viewModel.disconnectDevice()
                    }
                }
            },
            State {
                name: "reconnecting"
                when: viewModel.state === Serial.Reconnecting
                PropertyChanges {
                    target: processButton
                    text: qsTr("Trying to reconnect")
                    enabled: false
                    onClicked: undefined
                }
            }
        ]
    }
}
