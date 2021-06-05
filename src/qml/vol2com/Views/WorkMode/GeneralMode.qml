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
    //Dialog {
    //    id: errorPopup
    //
    //    visible: viewModel.deviceError
    //    x: (parent.width - width) / 2
    //    y: (parent.height - height) / 2
    //    parent: Overlay.overlay
    //    closePolicy: Popup.NoAutoClose
    //    modal: true
    //    standardButtons: Dialog.Ok
    //    title: qsTr("IO Error")
    //    Label {
    //        text: qsTr("Can't get audio samples.\nError: " + viewModel.deviceErrorText)
    //        anchors.fill: parent
    //    }
    //}

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
        width: parent.width
        active: true
        selectedBand: viewModel.currentMode.selectedBand.value
        barsStyle: viewModel.currentMode.barsStyle
        showBandsBackground: viewModel.currentMode.showBackground
        colorSource: viewModel.currentMode.colorSource
        Layout.preferredHeight: 160
        Layout.fillWidth: true
        onRightMouseClicked: {
            contextMenu.popup(pos)
        }
        onSelectedBandChanged: {
            viewModel.currentMode.selectedBand.value = selectedBand
        }

        Menu {
            id: contextMenu

            MenuItem {
                text: qsTr("Show background");
                checkable: true
                checked: viewModel.currentMode.showBackground
                onCheckedChanged: viewModel.currentMode.showBackground = checked
            }

            Menu {
                title: qsTr("Bars paint")
                MenuItem {
                    text: qsTr("All bars")
                    checkable: true
                    autoExclusive: true
                    checked: viewModel.currentMode.barsStyle === BandSelector.AllPainted
                    onTriggered: viewModel.currentMode.barsStyle = BandSelector.AllPainted
                }
                MenuItem {
                    text: qsTr("Active bar")
                    checkable: true
                    autoExclusive: true
                    checked: viewModel.currentMode.barsStyle === BandSelector.ActivePainted
                    onTriggered: viewModel.currentMode.barsStyle = BandSelector.ActivePainted
                }
            }

            Menu {
                title: qsTr("Color source")
                MenuItem {
                    text: qsTr("Default")
                    checkable: true
                    autoExclusive: true
                    checked: viewModel.currentMode.colorSource === BandSelector.Default
                    onTriggered: viewModel.currentMode.colorSource = BandSelector.Default
                }
                MenuItem {
                    text: qsTr("Active mode")
                    checkable: true
                    autoExclusive: true
                    checked: viewModel.currentMode.colorSource === BandSelector.ActiveMode
                    onTriggered: viewModel.currentMode.colorSource = BandSelector.ActiveMode
                }
            }
        }
    }
}
