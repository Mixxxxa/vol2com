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
    Item {
        id: colorPicker
        Layout.preferredHeight: 160
        Layout.fillWidth: true
        GridLayout {
            anchors.fill: parent
            rows: 3
            columns: 4
            columnSpacing: AppStyle.pageMargins

            Rectangle {
                id: resultColor
                color: viewModel.currentMode.color
                Layout.rowSpan: 3
                Layout.row: 0
                Layout.column: 0
                Layout.preferredHeight: colorPicker.height
                Layout.preferredWidth: colorPicker.height /2
                border.color: AppStyle.accent
                border.width: 1
            }

            Label {
                font: AppStyle.textFont
                text: "H"
                Layout.row: 0
                Layout.column: 1
            }

            GradientSlider {
                id: hueControl
                from: viewModel.currentMode.hue.min
                to: viewModel.currentMode.hue.max
                value: viewModel.currentMode.hue.value
                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: "red" }
                    GradientStop { position: 0.166; color: "yellow" }
                    GradientStop { position: 0.332; color: "lime" }
                    GradientStop { position: 0.498; color: "cyan" }
                    GradientStop { position: 0.664; color: "blue" }
                    GradientStop { position: 0.83; color: "magenta" }
                    GradientStop { position: 1.0; color: "red" }
                }
                Layout.row: 0
                Layout.column: 2
                Layout.fillWidth: true
                onValueChanged: {
                    viewModel.currentMode.hue.value = value
                    if(predefinedColors.checked) {
                        predefinedColors.reset()
                    }
                }
            }

            SpinBox {
                from: hueControl.from
                to: hueControl.to
                value: hueControl.value
                editable: true
                Layout.row: 0
                Layout.column: 3
                Layout.minimumWidth: 96
                onValueChanged: {
                    viewModel.currentMode.hue.value = value
                    if(predefinedColors.checked) {
                        predefinedColors.reset()
                    }
                }
            }

            Label {
                text: "S"
                Layout.row: 1
                Layout.column: 1
            }

            GradientSlider {
                id: saturationControl
                from: viewModel.currentMode.saturation.min
                to: viewModel.currentMode.saturation.max
                value: viewModel.currentMode.saturation.value
                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: "white" }
                    GradientStop { position: 1.0; color: viewModel.currentMode.refColor }
                }
                Layout.row: 1
                Layout.column: 2
                Layout.fillWidth: true
                onValueChanged: {
                    viewModel.currentMode.saturation.value = value
                    if(predefinedColors.checked) {
                        predefinedColors.reset()
                    }
                }
            }

            SpinBox {
                from: saturationControl.from
                to: saturationControl.to
                value: saturationControl.value
                editable: true
                Layout.row: 1
                Layout.column: 3
                Layout.minimumWidth: 96
                onValueChanged: {
                    viewModel.currentMode.saturation.value = value
                    if(predefinedColors.checked) {
                        predefinedColors.reset()
                    }
                }
            }

            Label {
                text: "V"
                Layout.row: 2
                Layout.column: 1
            }

            GradientSlider {
                id: valueControl
                from: viewModel.currentMode.value.min
                to: viewModel.currentMode.value.max
                value: viewModel.currentMode.value.value
                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 1.0; color: viewModel.currentMode.refColor }
                }
                Layout.row: 2
                Layout.column: 2
                Layout.fillWidth: true
                onValueChanged: {
                    viewModel.currentMode.value.value = value
                    if(predefinedColors.checked) {
                        predefinedColors.reset()
                    }
                }
            }

            SpinBox {
                from: valueControl.from
                to: valueControl.to
                value: valueControl.value
                editable: true
                Layout.row: 2
                Layout.column: 3
                Layout.minimumWidth: 96
                onValueChanged: {
                    viewModel.currentMode.value.value = value
                    if(predefinedColors.checked) {
                        predefinedColors.reset()
                    }
                }
            }
        }
    }

    Item {
        id: predefinedColors
        property bool checked: false

        function reset(){
            for(let i = 0; i < colorsRepeater.count; ++i){
                let item = colorsRepeater.itemAt(i)
                if(item.checked)
                    item.checked = false
            }
            checked = false
        }

        width: parent.width
        Layout.minimumHeight: 3*56
        GridLayout {
            anchors.fill: parent
            columnSpacing: 4
            rowSpacing: 4
            columns: 10

            Repeater {
                id: colorsRepeater
                model: [
                    "#000000", //Black
                    "#FFFFFF", //White
                    "#800000", //Maroon
                    "#FF0000", //Red
                    "#FF4500", //OrangeRed
                    "#FF8C00", //DarkOrange
                    "#FFA500", //Orange
                    "#FFD700", //Gold
                    "#808000", //Olive
                    "#FFFF00", //Yellow
                    "#7FFF00", //Chartreuse
                    "#008000", //Green
                    "#00FF00", //Lime
                    "#00FF7F", //SpringGreen
                    "#7FFFD4", //Aquamarine
                    "#008080", //Teal
                    "#00FFFF", //Aqua
                    "#00BFFF", //DeepSkyBlue
                    "#000080", //Navy
                    "#0000FF", //Blue
                    "#800080", //Purple
                    "#FF00FF"  //Fuchsia
                ]
                delegate: ColorButton {
                    backgroundColor: modelData
                    onClicked: {
                        viewModel.currentMode.color = modelData
                        predefinedColors.checked = true
                        checked = true
                    }
                }
                Component.onCompleted: {
                    let index = colorsRepeater.model.findIndex(item => item === viewModel.currentMode.color.toString().toUpperCase())
                    if(index !== -1){
                        colorsRepeater.itemAt(index).checked = true
                        predefinedColors.checked = true
                    }
                }
            }
        }
    }
}
