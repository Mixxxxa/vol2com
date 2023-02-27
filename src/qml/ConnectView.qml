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

import QtQuick.Controls
import QtQuick.Layouts
import vol2com

Page {
    title: qsTr("Connection")
    header: Header {
        title: parent.title
        ComboBox {
            model: [
                { name: "Serial", value: Controller.Serial },
                { name: "Websocket", value: Controller.WebSocket }
            ]
            textRole: "name"
            valueRole: "value"
            currentIndex: model.findIndex(x => x.value === viewModel.connectType)
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredWidth: 180
            onCurrentValueChanged: {
                viewModel.connectType = currentValue
            }
        }
    }

    ConnectPageViewModel {
        id: viewModel
    }

    StackView {
        id: connectStack
        anchors.fill: parent
        initialItem: viewModel.qmlDelegate
    }

    ProgressBar {
        indeterminate: true
        enabled: viewModel.state === ConnectMethodBase.Connecting
        visible: viewModel.state === ConnectMethodBase.Connecting
        z: 1

        anchors {
            top: connectStack.top
            left: parent.left
            right: parent.right
        }
    }
}
