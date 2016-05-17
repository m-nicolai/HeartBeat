/***************************************************************************
**
** This file is part of the heartBeat app for bluetooth heartrate measuring
** Copyright (C) 2016 Matthijs Nicolai
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.0
import Ubuntu.Components 1.3

Rectangle {
    property int maxValue: 220
    property int minValue: 60
    property int pointerHeight: units.gu(3)
    property int upperLimit: 0
    property int lowerLimit: 0

    width: parent.width/2
    height: parent.height

    Rectangle {
        id: verticalBar
        anchors.centerIn: parent
        width: units.gu(4)
        height: parent.height*0.8
        color: "grey"
    }
    Rectangle {
        id: upperLimitSelector
        anchors.right: verticalBar.left
        width: units.gu(4)
        height: pointerHeight
        color: "grey"


        MouseArea {
            anchors.fill : parent
            drag.target: parent
            drag.axis: Drag.YAxis
            drag.minimumY: verticalBar.y - (pointerHeight/2)
            drag.maximumY: verticalBar.y + verticalBar.height - (pointerHeight/2)

        }
        onYChanged: {
            if (upperLimitSelector.y >= lowerLimitSelector.y-1) {
                upperLimitSelector.y = lowerLimitSelector.y-1;
            }
            upperLimit = getValue(upperLimitSelector.y);
            console.log(upperLimit);
        }
    }
    Rectangle {
        property int value
        id: lowerLimitSelector
        anchors.right: verticalBar.left
        width: units.gu(4)
        height: pointerHeight
        color: "grey"

        y: verticalBar.y + (verticalBar.height*0.6)

        MouseArea {
            anchors.fill : parent
            drag.target: parent
            drag.axis: Drag.YAxis
            drag.minimumY: verticalBar.y - (pointerHeight/2)
            drag.maximumY: verticalBar.y + verticalBar.height - (pointerHeight/2)
        }

        onYChanged: {
            if (lowerLimit.y <= upperLimitSelector.y+1) {
                lowerLimit.y = upperLimitSelector.y+1;
            }
            lowerLimit = getValue(lowerLimitSelector.y);
            console.log(lowerLimit);
        }
    }

    function getValue ( pos) {
        var range = maxValue-minValue;
        var p = pos+(pointerHeight/2);
        var q = (p-verticalBar.y) / (verticalBar.height);
        var v = ( (1-q) * range ) + minValue;
        return v;
    }

    Component.onCompleted: {

        lowerLimitSelector.y = verticalBar.y + (verticalBar.height*0.6);
        upperLimitSelector.y = verticalBar.y + (verticalBar.height*0.4);
        lowerLimit = getValue(lowerLimitSelector.y);
        upperLimit = getValue(upperLimitSelector.y);
    }
}

