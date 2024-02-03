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

Slider {
    id: control

    property alias gradient: backgroundRect.gradient

    background: Rectangle {
        id: backgroundRect
        x: control.leftPadding
        y: control.topPadding + control.availableHeight / 2 - height / 2
        height: 14
        width: control.availableWidth
        border {
            color: AppStyle.accentDark1
            width: 1
        }
    }

    handle: Rectangle {
        x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: 10
        implicitHeight: 30
        radius: 5
        color: AppStyle.foreground
    }

    stepSize: 1
    snapMode: Slider.SnapOnRelease
}
