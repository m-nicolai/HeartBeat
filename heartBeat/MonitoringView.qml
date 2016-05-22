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
import QtMultimedia 5.6
import Ubuntu.Components 1.3
import "./items"

Item {

    property var sensor: deviceManager.sensor

    Connections {
        target: heartRateMonitor
        onUpperLimitExceeded: { alarmGenerator.playUpperAlarm(); }
        onLowerLimitExceeded: { alarmGenerator.playLowerAlarm(); }
    }

    Connections {
        target: sensor
        onHeartRateChanged: { heartRateMonitor.setHeartRate(sensor.heartRate); }
    }

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
                id: monitoringView
                objectName: "monitoringView"
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
                        }
                    }
                    Rectangle {
                        width: parent.width
                        height: parent.height-infoBar.height
                        DoubleSlider {
                            id: rangeSelector
                            height: parent.height
                            width: parent.width/2
                            anchors.top: parent.top
                            anchors.left: parent.left
                            onUpperLimitChanged: heartRateMonitor.setUpperLimit(upperLimit)
                            onLowerLimitChanged: heartRateMonitor.setLowerLimit(lowerLimit)
                        }

                        Rectangle {
                            height: parent.height/2
                            width: parent.width/2
                            anchors.top: parent.top
                            anchors.right: parent.right

                            Text {
                                id: upperLimitText
                                anchors.centerIn: parent
                                font.pointSize: units.gu(1)
                                text: "upper limit:<br/>" + rangeSelector.upperLimit
                            }
                        }

                        Rectangle {
                            height: parent.height/2
                            width: parent.width/2
                            anchors.bottom: parent.bottom
                            anchors.right: parent.right
                            Text {
                                id: lowerLimitText
                                anchors.centerIn: parent
                                font.pointSize: units.gu(1)
                                text: "lower limit:<br/>" + rangeSelector.lowerLimit
                            }
                        }
                    }

                    //height: units.gu(20)
                }
            }

            Button {
                id: clickButton
                objectName: "button"
                width: parent.width * 0.8
                height: units.gu(8)
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                text: i18n.tr("Start")

                SoundEffect {
                    id: playSound
                    source: "sounds/short_beep.wav"
                }

                onClicked: {
                    if (clickButton.text === "Start") {
                        // get range from double slider
                        console.log(rangeSelector.upperLimit);
                        console.log(rangeSelector.lowerLimit);

                        rangeSelector.enabled = false;
                        //playSound.play();
                        //alarmGenerator.playUpperAlarm();
                        heartRateMonitor.setLowerLimit(rangeSelector.lowerLimit);
                        heartRateMonitor.setUpperLimit(rangeSelector.upperLimit);
                        heartRateMonitor.start();

                        clickButton.text = "Stop"
                    } else {
                        heartRateMonitor.stop();
                        rangeSelector.enabled = true;
                        clickButton.text = "Start"
                    }
                }
            }

            Item {
                id: alarmGenerator
                property bool running: false
                property int repeats: 2

                Item {
                    id: internal
                    property int repeatsLeft: 0
                }

                function playLowerAlarm() {
                    playSound.play();  // play single beep
                }

                function playUpperAlarm() {
                    repeats = 2;
                    running = true;
                    playSound.play();
                    repeatTimer.start();
                }

                Timer {
                    id: repeatTimer
                    interval: 350


                    onRunningChanged: {
                        if (running === true) {
                            internal.repeatsLeft = alarmGenerator.repeats-1;
                        }
                    }

                    onTriggered: {
                        playSound.play();
                        internal.repeatsLeft--;
                        if (internal.repeatsLeft === 0) {
                            running = false;
                            repeatTimer.stop();
                        }
                    }
                }

            }
        }
    }
}
