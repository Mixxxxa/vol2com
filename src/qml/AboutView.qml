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

Page {
    id: aboutPageView
    title: qsTr("About")
    header: Header {
        title: parent.title
    }

    AboutPageViewModel {
        id: viewmodel
    }

    StandartView {
        anchors.fill: parent
        layoutSpacing: 10
        Text {
            font: AppStyle.textFont
            color: AppStyle.foreground
            text: viewmodel.aboutText
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignJustify
            Layout.fillWidth: true
        }

        Text {
            font: AppStyle.textFont
            color: AppStyle.foreground
            text: viewmodel.versionText
            Layout.fillWidth: true
        }

        Text {
            font: AppStyle.textFont
            color: AppStyle.foreground
            text: viewmodel.commitText
            linkColor: AppStyle.accent
            Layout.fillWidth: true
            onLinkActivated: {
                Qt.openUrlExternally(link)
            }
        }

        Text {
            font: AppStyle.textFont
            color: AppStyle.foreground
            text: viewmodel.qtInfoText
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        Text {
            font: AppStyle.textFont
            color: AppStyle.foreground
            text: qsTr("Third party components:")
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        Text {
            font: AppStyle.textFont
            color: AppStyle.foreground
            text: viewmodel.thirdPartyText
            wrapMode: Text.WordWrap
            lineHeight: 1.5
            linkColor: AppStyle.accent
            Layout.fillWidth: true
            onLinkActivated: {
                Qt.openUrlExternally(link)
            }
        }

        Text {
            font: AppStyle.textFont
            color: AppStyle.foreground
            text: viewmodel.copyrightText
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignJustify
            Layout.fillWidth: true
        }

        Text {
            font: AppStyle.textFont
            color: AppStyle.foreground
            text: viewmodel.disclaimerText
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignJustify
            Layout.fillWidth: true
        }

        RowLayout {
            Layout.fillWidth: true
            Button {
                text: "GitHub"
                onClicked: {
                    Qt.openUrlExternally(viewmodel.githubLink)
                }
                Layout.fillWidth: true
            }
            Button {
                text: qsTr("Help and wiki")
                onClicked: {
                    Qt.openUrlExternally(viewmodel.githubLink + "/wiki/")
                }
                Layout.fillWidth: true
            }
        }
    }
}
