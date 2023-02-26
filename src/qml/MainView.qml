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
import QtQuick.Window
import vol2com as VC

Rectangle {
    id: mainView
    anchors.fill: parent
    color: VC.AppStyle.background

    VC.ToolbarViewModel {
        id: viewModel
    }

    VerticalToolBar {
        id: toolbar
        width: 50
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        onChangeView: {
            swipeView.currentIndex = index
        }
    }

    TitleBar {
        id: viewTitle
        height: 40
        anchors {
            top: parent.top
            left: toolbar.right
            right: parent.right
        }
    }

    Loader {
        id: swipeView

        function getSource(page){
            switch(page){
            case VC.Controller.ConnectPage:
                return "qrc:/vol2com/qml/ConnectView.qml";
            case VC.Controller.WorkModePage:
                return "qrc:/vol2com/qml/WorkModeView.qml";
            case VC.Controller.EqualizerPage:
                return "qrc:/vol2com/qml/EqualizerView.qml";
            case VC.Controller.SettingsPage:
                return "qrc:/vol2com/qml/Settings.qml";
            case VC.Controller.AboutPage:
                return "qrc:/vol2com/qml/AboutView.qml";
            default:
                return "qrc:/vol2com/qml/Settings.qml";
            }
        }

        clip: true
        anchors {
            top: viewTitle.bottom
            bottom: parent.bottom
            left: toolbar.right
            right: parent.right
        }
        source: getSource(VC.Controller.page)
    }

    Rectangle {
        color: "transparent"
        anchors.fill: parent
        border {
            width: 1
            color: VC.AppStyle.accent
        }
        visible: mainWindow.visibility === Window.Windowed
    }

    Component.onCompleted: {
        if(Settings.photosensitiveWarningAccepted === false)
        {
            let comp = Qt.createComponent("qrc:/vol2com/qml/EpilepsyWarningPopup.qml");
            if (comp && comp.status === Component.Ready) {
                let warning = comp.createObject(mainView);
                if (warning) {
                    warning.rejected.connect(function(){
                        Controller.exit();
                    });
                    warning.open()
                }
            }
        }
    }

    //Connections {
    //    target: Controller
    //    function onShowMessage(title, text) {
    //        mainPopup.title = title
    //        mainPopup.text = text
    //        mainPopup.open()
    //    }
    //}
}
