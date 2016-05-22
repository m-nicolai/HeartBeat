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

#include "devicemanager.h"


DeviceManager::DeviceManager(QObject *parent) : QObject(parent)
{
  message_ = QString("");
  connect(&agent_, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
          this, SLOT(addDevice(QBluetoothDeviceInfo)));
  connect(&agent_, SIGNAL(finished()),
          this, SLOT(endScan()));
  connect(&agent_, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
          this, SLOT(onError(QLowEnergyController::Error)));

  sensor_ = NULL;
  control_ = NULL;
}



void
DeviceManager::scanForDevices()
{
  QBluetoothLocalDevice d;
  if (d.hostMode() == QBluetoothLocalDevice::HostPoweredOff)
  {
    setStatusMessage("Please switch on bluetooth");
  }
  else
  {
    agent_.start();
    setStatusMessage("Scanning...");
  }
}

void
DeviceManager::addDevice(QBluetoothDeviceInfo dev)
{
  if(dev.coreConfigurations() &
     QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
  {
    DeviceInfo *device = new DeviceInfo(dev);

    devices_.append(device);
    emit deviceListChanged();
  }
}

void
DeviceManager::endScan()
{
  if(devices_.isEmpty())
    setStatusMessage("No devices found");
  else
    setStatusMessage("Finished scan");
}


QString
DeviceManager::getStatusMessage()
{
  return message_;
}


void
DeviceManager::setStatusMessage(const QString string)
{
  message_ = string;
  emit statusMessageChanged();
}


QVariant
DeviceManager::sensor()
{
  return QVariant::fromValue(sensor_);
}


QVariant
DeviceManager::deviceList()
{
  return QVariant::fromValue(devices_);
}

void
DeviceManager::connectToDevice(const QString &address)
{
  bool foundDevice = false;
  for (int i = 0; i < devices_.length(); i++)
  {
    if (static_cast<DeviceInfo*>(devices_.at(i))->getAddress() == address)
    {
      foundDevice = true;
      activeDevice_.setDevice(((DeviceInfo*)devices_.at(i))->getDevice());
      break;
    }
  }
  if (foundDevice)
    qDebug() << "whoohoo!";
  else
    qDebug() << "wheehee!";

  if(sensor_)
  {
    delete sensor_;
    sensor_ = 0;
  }

  sensor_ = new HeartSensor(activeDevice_.getDevice().address(), this);

  static_cast<QQuickView*>(this->parent())->rootContext()->setContextProperty("heartRateSensor", sensor_);

  emit heartRateSensorConnected();
}


void
DeviceManager::onError(QLowEnergyController::Error newError)
{
  // notify user and abort action
  qDebug() << newError;
}


