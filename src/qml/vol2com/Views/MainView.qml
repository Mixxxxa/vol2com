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
import QtQuick.Window 2.15
import vol2com 1.0
import vol2com.Popups 1.0

Rectangle {
    id: mainView
    anchors.fill: parent
    color: AppStyle.background

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
            case Controller.ConnectPage:
                return "qrc:/qml/vol2com/Views/Connect/ConnectView.qml";
            case Controller.WorkModePage:
                return "qrc:/qml/vol2com/Views/WorkMode/WorkModeView.qml";
            case Controller.EqualizerPage:
                return "qrc:/qml/vol2com/Views/EqualizerView.qml";
            case Controller.SettingsPage:
                return "qrc:/qml/vol2com/Views/Settings.qml";
            case Controller.AboutPage:
                return "qrc:/qml/vol2com/Views/AboutView.qml";
            default:
                return "qrc:/Views/Settings.qml";
            }
        }

        clip: true
        anchors {
            top: viewTitle.bottom
            bottom: parent.bottom
            left: toolbar.right
            right: parent.right
        }
        source: getSource(Controller.page)
    }

    Rectangle {
        color: "transparent"
        anchors.fill: parent
        border {
            width: 1
            color: AppStyle.accent
        }
        visible: mainWindow.visibility === Window.Windowed
    }

    Component.onCompleted: {
        if(Settings.photosensitiveWarningAccepted === false)
        {
            let comp = Qt.createComponent("qrc:/qml/vol2com/Popups/EpilepsyWarningPopup.qml");
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
