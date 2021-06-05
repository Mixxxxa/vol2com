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
    ListDelegate {
        text: qsTr("Hue:")
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
            Layout.fillWidth: true
            onValueChanged: {
                viewModel.currentMode.hue.value = value
            }
        }
        Text {
            text: hueControl.value
            font: AppStyle.textFont
            color: AppStyle.foreground
            horizontalAlignment: Text.AlignRight
            Layout.preferredWidth: 30
        }
    }

    ListDelegate {
        text: qsTr("Update rate:")
        Slider {
            id: uprateSlider
            from: viewModel.currentMode.uprate.min
            to: viewModel.currentMode.uprate.max
            value: viewModel.currentMode.uprate.value
            snapMode: Slider.SnapOnRelease
            stepSize: 1
            Layout.fillWidth: true
            Layout.preferredWidth: 200
            onValueChanged: {
                viewModel.currentMode.uprate.value = value
            }
        }
        Text {
            text: uprateSlider.value
            font: AppStyle.textFont
            color: AppStyle.foreground
            horizontalAlignment: Text.AlignRight
            Layout.preferredWidth: 30
        }
    }

    BandSelector {
        id: bandSelector
        Layout.preferredHeight: 160
        Layout.fillWidth: true
        width: parent.width
        selectedBand: viewModel.currentMode.selectedBand.value
        active: true
        barsStyle: viewModel.currentMode.barsStyle
        showBandsBackground: true
        colorSource: BandSelector.ActiveMode
        onSelectedBandChanged: viewModel.currentMode.selectedBand.value = selectedBand
        onBarsStyleChanged: viewModel.currentMode.barsStyle = barsStyle
        onShowBandsBackgroundChanged: viewModel.currentMode.showBackground = showBandsBackground
        onColorSourceChanged: viewModel.currentMode.colorSource = colorSource
    }
}
