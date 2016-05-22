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

#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QQuickView>
#include <QQmlContext>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QBluetoothLocalDevice>
#include <QDebug>

#include "heartsensor.h"
#include "deviceinfo.h"

class DeviceManager : public QObject
{
  Q_OBJECT
public:
  Q_PROPERTY(QString statusMessage READ getStatusMessage NOTIFY statusMessageChanged)
  Q_PROPERTY(QVariant deviceList READ deviceList NOTIFY deviceListChanged)
  Q_PROPERTY(QVariant sensor READ sensor)
  /*!
   * \brief DeviceManager default constructor
   * \param parent as QObject
   */
  explicit DeviceManager(QObject *parent = 0);
  /*!
   * \brief getStatusMessage is the getter for the statusmessage property.
   * \return status message as QString
   */
  QString getStatusMessage();
  /*!
   * \brief setStatusMessage is the setter for the statusmessage property
   * \param string, the new message as QString
   */
  void    setStatusMessage(const QString string);
  /*!
   * \brief deviceList returns the list of found bluetooth devices
   * \return QList of QOjbect pointers to devices, returned as QVariant
   */
  QVariant deviceList();
  /*!
   * \brief sensor returns the currently selected HeartSensor
   * \return pointer to HeartSensor
   */
  QVariant sensor();

signals:
  /*!
   * \brief statusMessageChanged indicates the content of the statusmessage has
   * been updated.
   */
  void statusMessageChanged();
  /*!
   * \brief deviceListChanged indicates devices were added or removed.
   */
  void deviceListChanged();
  /*!
   * \brief heartRateSensorConnected indicates a connection to a heart rate
   * sensor has been established.
   */
  void heartRateSensorConnected();

public slots:
  /*!
   * \brief scanForDevices slot to trigger scanning for bluetooth devices.
   */
  void scanForDevices();
  /*!
   * \brief connectToDevice slot to instruct devicemanager to connect to the
   * device that corresponds to the parameter 'address'
   * \param address is the address of the bluetooth device to connect to as Q
   * String
   */
  void connectToDevice(const QString &address);

private slots:
  /*!
   * \brief addDevice adds the device that corresponds to parameter 'dev' to
   * the list of know devices
   * \param dev as QBluetoothDeviceInfo
   */
  void addDevice(QBluetoothDeviceInfo dev);
  /*!
   * \brief endScan is called when the scan for bluetooth devices has finished
   */
  void endScan();
  /*!
   * \brief onError prints the errorstring of newError to the debug console
   * \param newError is the error object
   */
  void onError(QLowEnergyController::Error newError);

private:
  DeviceInfo                      activeDevice_;
  QList<QObject*>                 devices_;
  QLowEnergyController*           control_;
  QBluetoothDeviceDiscoveryAgent  agent_;
  QString                         message_;
  HeartSensor*                    sensor_;
};

#endif // DEVICEMANAGER_H
