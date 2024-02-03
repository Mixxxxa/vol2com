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
    id: equalizerView
    title: qsTr("Equalizer")
    header: Header {
        title: parent.title
        ComboBox {
            model: [
                { name: qsTr("Enabled"), value: true },
                { name: qsTr("Disabled"), value: false }
            ]
            textRole: "name"
            valueRole: "value"
            currentIndex: model.findIndex(x => x.value === viewModel.enabled)
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredWidth: 150
            onCurrentValueChanged: {
                viewModel.enabled = currentValue
            }
        }
    }

    function formatNumbers(value){
        return (value >= viewModel.defaultValue ? "+" : "-") + (value / 100).toPrecision(3)
    }

    EqualizerViewModel {
        id: viewModel
    }

    StandartView {
        spacing: 10
        anchors.fill: parent
        enabled: viewModel.enabled
        ListDelegate {
            text: qsTr("Overall amplification:")
            Slider {
                Layout.fillWidth: true
                from: viewModel.minValue
                to: viewModel.maxValue
                stepSize: 10
                value: viewModel.overallValue
                onValueChanged: {
                    viewModel.overallValue = value
                }
            }
            Label {
                id: overallLabel
                text: formatNumbers(viewModel.overallValue)
                horizontalAlignment: Text.AlignRight
                Layout.minimumWidth: 50
            }
        }

        Label {
            text: qsTr("Per band amplification (select and change value below):")
            font: AppStyle.textFont
        }
        EqualizerViewer {
            width: parent.width
            active: true
            selectedBand: viewModel.selectedBand
            onSelectedBandChanged: function(selectedBand){
                viewModel.selectedBand = selectedBand
            }
            Layout.preferredHeight: 160
            Layout.fillWidth: true
        }
        ListDelegate {
            text: qsTr("Band #") + viewModel.selectedBand
            Slider {
                Layout.fillWidth: true
                from: viewModel.minValue
                to: viewModel.maxValue
                stepSize: 10
                value: viewModel.selectedBandValue
                onValueChanged: {
                    viewModel.selectedBandValue = value
                }
            }
            Label {
                id: bandLabel
                horizontalAlignment: Text.AlignRight
                text: formatNumbers(viewModel.selectedBandValue)
                Layout.minimumWidth: 50
            }
        }
    }
}
