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

StandartView {
    ListDelegate {
        text: qsTr("Imitation type:")
        ComboBox {
            model: [
                { name: qsTr("Fixed"), value: 0 },
                { name: qsTr("Fading"), value: 1 }
            ]
            textRole: "name"
            valueRole: "value"
            currentIndex: model.findIndex(x => x.value === viewModel.currentMode.subMode)
            onCurrentValueChanged: {
                viewModel.currentMode.subMode = currentValue
            }
            Layout.fillWidth: true
            Layout.preferredWidth: 200
        }
    }

    ListDelegate {
        text: qsTr("Speed level:")
        Slider {
            id: speedSlider
            from: viewModel.currentMode.uprate.min
            to: viewModel.currentMode.uprate.max
            value: viewModel.currentMode.uprate.value
            snapMode: Slider.SnapOnRelease
            stepSize: 1
            Layout.fillWidth: true
            Layout.preferredWidth: 200
            onValueChanged: {
                viewModel.currentMode.speedLevel = value
            }
        }
        Text {
            text: speedSlider.value
            font: AppStyle.textFont
            color: AppStyle.foreground
            horizontalAlignment: Text.AlignRight
            Layout.preferredWidth: 30
        }
    }
}
