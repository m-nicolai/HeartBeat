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

#include "heartsensor.h"

HeartSensor::HeartSensor(QObject *parent) : QObject(parent)
{
  control_    = 0;
}



HeartSensor::HeartSensor(QBluetoothAddress address,
                         QObject *parent) : QObject(parent)
{
  control_ = new QLowEnergyController(address, this);
  init();

}



void
HeartSensor::init()
{
  heartRate_  = 0;
  batterySoc_ = 0;

  // connect to signals of QLowEnergyController for service detection
  if (control_ != 0){
    connect(control_,
            SIGNAL(connected()),
            this,
            SLOT(deviceConnected()));
    connect(control_,
            SIGNAL(error(QLowEnergyController::Error)),
            this,
            SLOT(deviceError(QLowEnergyController::Error)));
    connect(control_,
            SIGNAL(serviceDiscovered(QBluetoothUuid)),
            this,
            SLOT(addService(QBluetoothUuid)));
    connect(control_,
            SIGNAL(discoveryFinished()),
            this,
            SLOT(inspectServices()));
    connectDevice();
  }
}



unsigned int
HeartSensor::getBatterySoc()
{
  return batterySoc_;
}



unsigned int
HeartSensor::getHeartRate()
{
  return heartRate_;
}



void
HeartSensor::connectDevice()
{
  if (!control_)
    return; // No valid control object available
  /*!
    \todo create error handling
    */
  control_->connectToDevice();
}



void
HeartSensor::deviceConnected()
{
  // Start service scan
  control_->discoverServices();
}



void
HeartSensor::deviceError(QLowEnergyController::Error newError)
{
  qDebug() << newError;
}



void
HeartSensor::addService(const QBluetoothUuid& newService)
{
  services_.append(newService);
}



void
HeartSensor::inspectServices()
{
  QListIterator<QBluetoothUuid> i(services_);
  while (i.hasNext()) {
    QBluetoothUuid o = i.next();

    if(o == QBluetoothUuid::BatteryService) {
      // battery service found. Start discovery of details.
      batteryService_ = control_->createServiceObject(o);
      connect(batteryService_,
              SIGNAL(stateChanged(QLowEnergyService::ServiceState)),
              this,
              SLOT(batteryDetailsDiscovered(QLowEnergyService::ServiceState)));

      batteryService_->discoverDetails();
    }

    if (o == QBluetoothUuid::HeartRate) {
      // heartrate service found. Start discovery of details.
      heartRateService_ = control_->createServiceObject(o);
      connect(heartRateService_,
              SIGNAL(stateChanged(QLowEnergyService::ServiceState)),
              this,
              SLOT(heartRateDetailsDiscovered(QLowEnergyService::ServiceState)));
      heartRateService_->discoverDetails();
    }
  }
}



void
HeartSensor::batteryDetailsDiscovered (QLowEnergyService::ServiceState state)
{
  if (state != QLowEnergyService::ServiceDiscovered) return;

  QLowEnergyCharacteristic soc = batteryService_->characteristic(QBluetoothUuid::BatteryLevel);

  if (!soc.isValid())
    return;

  QLowEnergyDescriptor d = soc.descriptor(
        QBluetoothUuid::ClientCharacteristicConfiguration);
  if (!d.isValid()) {
    // Notifications not available use timer to periodically update battery
    // state of charge
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateBatterySoc()));
    timer_->setInterval(60000); // one minute
    timer_->start();
    updateBatterySoc();
  }
  else {
    // establish hook into notifications
    connect(batteryService_,
            SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),
            this,
            SLOT(updateBatterySoc(QLowEnergyCharacteristic,QByteArray)));

    // enable notification
    batteryService_->writeDescriptor(d, QByteArray::fromHex("0100"));
  }
}



void
HeartSensor::updateBatterySoc()
{
  QLowEnergyCharacteristic c =
      batteryService_->characteristic(QBluetoothUuid::BatteryLevel);

  batterySoc_ = c.value()[0];
  emit batterySocChanged(batterySoc_);
}



void
HeartSensor::updateBatterySoc(QLowEnergyCharacteristic chr, QByteArray value)
{
  if (chr.name().compare("Battery Level")) {
    batterySoc_ = value[0];
    emit batterySocChanged(batterySoc_);
  }
}



void
HeartSensor::heartRateDetailsDiscovered(QLowEnergyService::ServiceState state)
{
  if (state != QLowEnergyService::ServiceDiscovered) return;

  QLowEnergyCharacteristic c = heartRateService_->characteristic(
        QBluetoothUuid::HeartRateMeasurement);

  if (!c.isValid())
    return;

  QLowEnergyDescriptor heartRateNotification = c.descriptor(
        QBluetoothUuid::ClientCharacteristicConfiguration);
  if (!heartRateNotification.isValid())
    return;

  // establish hook into notifications
  connect(heartRateService_,
          SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),
          this,
          SLOT(updateHeartRate(QLowEnergyCharacteristic,QByteArray)));

  // enable notification
  heartRateService_->writeDescriptor(heartRateNotification,
                                     QByteArray::fromHex("0100"));

}



void
HeartSensor::updateHeartRate (QLowEnergyCharacteristic chr, QByteArray value)
{
  /// \todo Enter complete description of characteristic and/or link
  if(chr.name().compare("Heart Rate")) {
    // Process heartrate characteristic
    if(value[0] & 0x01)
    {
      // 16 bit measurement
      heartRate_ = (unsigned char)((unsigned int)value[1]);
    } else {
      // 8 bit measurement
      heartRate_ = (unsigned char)value[1];
    }
    emit heartRateChanged(heartRate_);
  }
}
