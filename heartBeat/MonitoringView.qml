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
import "./items"

Item {

    property var sensor: deviceManager.sensor

    Page {
        title: i18n.tr("heartBeat")
        head.title: "Set target range"



        Rectangle {
            //spacing: units.gu(1)
            anchors {
                margins: units.gu(2)
                fill: parent
            }




            Rectangle {
                id: deviceList
                objectName: "deviceList"
                anchors.top: parent.top
                anchors.bottom: clickButton.top
                width: parent.width
                anchors.bottomMargin: units.gu(2)

                color: "green"

                Column {
                    anchors.fill: parent

                    Rectangle {
                        width: parent.width
                        height: units.gu(12)
                        color: "white"

                        Row {
                            id: infoBar
                            anchors.fill: parent
                            HeartBeatBox {heartRate: sensor.heartRate}
                            BatteryBox {soc: sensor.batterySoc}
//                            Rectangle {
//                                width: parent.width / 2
//                                height: parent.height
//                                color: "red"
//                            }
//                            Rectangle {
//                                width: parent.width/2
//                                height: parent.height
//                                color: "orange"
//                            }
                        }
                    }

                }

                //height: units.gu(20)
            }

            Button {
                id: clickButton
                objectName: "button"
                width: parent.width

                anchors.bottom: parent.bottom
                text: i18n.tr("Start")

                onClicked: {
                    clickButton.text = "Stop"
                }
            }
        }
    }
}

