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

Item {


    Page {
        title: i18n.tr("heartBeat")
        head.title: "Select sensor"


        Component {
            id: dev
            ListItem {
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: "Press me"
                }
                onClicked: {
                    container.source = "MonitoringView.qml"
                    console.log("clicked on ListItem")
                }
            }
        }

        Rectangle {
            //spacing: units.gu(1)
            anchors {
                margins: units.gu(2)
                fill: parent
            }




            ListView {
                id: deviceList
                objectName: "deviceList"
                anchors.top: parent.top
                anchors.bottom: clickButton.top
                width: parent.width
                model: deviceManager.deviceList

                delegate: ListItem {
                    Text {
                        anchors.fill: parent
                        text: modelData.deviceName
                    }
                    onClicked: {
                        console.log("Selected ", modelData.deviceName);
                        deviceManager.connectToDevice(modelData.deviceAddress);
                        container.source = "MonitoringView.qml"

                    }
                }

                //height: units.gu(20)
            }

            Text {
                id: progressMessage
                anchors.bottom: clickButton.top
                anchors.horizontalCenter: clickButton.horizontalCenter
                text: deviceManager.statusMessage

            }

            Button {
                id: clickButton
                objectName: "button"
                width: parent.width * 0.8
                height: units.gu(8)
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                text: i18n.tr("Scan")

                onClicked: {
                    deviceManager.scanForDevices();
                }
            }
        }
    }
}

