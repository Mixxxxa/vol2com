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
import vol2com as VC

Button {
    id: control
    property color hoveredColor: VC.AppStyle.accent
    flat: true
    background: Rectangle {
        visible: enabled && (control.hovered || control.down)
        color: control.hoveredColor
        opacity: control.down ? 0.85 : 1.0
    }
    font {
        family: "Segoe MDL2 Assets"
        pixelSize: 13
    }
}
