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
import QtQuick.Controls 2.15 as C
import QtQuick.Layouts 1.14
import vol2com 1.0

//Flickable {
//    default property alias items: pageLayout.children
//    property alias spacing: pageLayout.spacing
//    property alias data: view.contentData
//
//    ScrollBar.vertical: ScrollBar { }
//    boundsBehavior: Flickable.StopAtBounds
//    contentHeight: pageLayout.childrenRect.height + (AppStyle.pageMargins * 2)
//    anchors.fill: parent
//
//    Item {
//        width: parent.width
//        ColumnLayout {
//            id: pageLayout
//            spacing: AppStyle.pageMargins
//            anchors {
//                fill: parent
//                margins: AppStyle.pageMargins
//            }
//        }
//    }
//}
//

C.ScrollView {
    id: view
    default property alias items: pageLayout.children
    property alias data: view.contentData
    property alias layoutSpacing: pageLayout.spacing

    spacing: 0
    contentHeight: pageLayout.childrenRect.height + (AppStyle.pageMargins * 2)
    Item {
        width: parent.width
        ColumnLayout {
            id: pageLayout
            spacing: AppStyle.pageMargins
            anchors {
                fill: parent
                margins: AppStyle.pageMargins
            }
        }
    }
}
