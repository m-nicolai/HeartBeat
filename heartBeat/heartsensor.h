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

#ifndef HEARTSENSOR_H
#define HEARTSENSOR_H

#include <QObject>
#include <QTimer>

#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>

/*!
 * \brief The HeartSensor class manages the connection to the heart rate sensor
 * and provides the characteristics as Q_PROPERTY for use in QML.
 */
class HeartSensor : public QObject
{
  Q_OBJECT
public:
  /*!
   * \brief batterySoc is the state of charge of the sensor battery in percent
   */
  Q_PROPERTY(unsigned int batterySoc READ getBatterySoc NOTIFY batterySocChanged)
  /*!
   * \brief heartRate is the measured heartrate in beats per minute
   */
  Q_PROPERTY(unsigned int heartRate  READ getHeartRate  NOTIFY heartRateChanged)
  /*!
   * \brief HeartSensor
   * \param parent
   */
  explicit HeartSensor(QObject *parent = 0);
  /*!
   * \brief HeartSensor
   * \param address
   * \param parent
   */
  explicit HeartSensor(QBluetoothAddress address, QObject *parent = 0);
  /*!
   * \brief init
   */
  void init();
  /*!
   * \brief getBatterySoc getter for battery state of charge
   * \return batterySoc_ as unsigend char
   */
  unsigned int getBatterySoc       ();
  /*!
   * \brief getHeartRate getter for heart rate
   * \return heartRate_ as unsigned char
   */
  unsigned int getHeartRate        ();

signals:
  /*!
   * \brief batterySocChanged is emitted when an update of the battery state of
   * charge is received
   * \param batterySoc_ as unsigned char
   */
  void batterySocChanged    (unsigned int Soc);
  /*!
   * \brief heartRateChanged is emitted when an update of the heart rate is
   * received
   * \param heartRate_ as unsigned char
   */
  void heartRateChanged     (unsigned int rate);

private slots:
  /*!
   * \brief deviceConnected is called when a connection to a bluetooth lowercasebase
   * energy device is established and initiates service discovery.
   */
  void deviceConnected             ();
  /*!
   * \brief deviceError prints the error string of newError to the debug console
   * \param newError is the error object
   */
  void deviceError                 (QLowEnergyController::Error newError);
  /*!
   * \brief addService adds the discovered service to the list of known
   * services
   * \param newService is the bluetooth uuid of the discovered service
   */
  void addService                  (const QBluetoothUuid &newService);
  /*!
   * \brief inspectServices is called after service discovery is finished to
   * go through the list of services and find the BatteryService and HeartRate
   * services.
   */
  void inspectServices             ();
  /*!
   * \brief heartRateDetailsDiscovered is called when the service details
   * discovery for the HeartRate service is finished
   * \param state is the state of the service
   */
  void heartRateDetailsDiscovered  (QLowEnergyService::ServiceState state);
  /*!
   * \brief batteryDetailsDiscovered is called when the service details
   * discovery of the BatteryService service is finished
   * \param state is the state of the service
   */
  void batteryDetailsDiscovered    (QLowEnergyService::ServiceState state);
  /*!
   * \brief connectDevice is called to connect to the bluetooth low energy
   * device.
   */
  void connectDevice               ();
  /*!
   * \brief updateBatterySoc is called by the internal timer to update the
   * state of charge indication in case the bluetooth device does not support
   * notifications for the BatteryService
   */
  void updateBatterySoc            ();
  /*!
   * \brief updateBatterySoc is called when a notification is received from the
   * bluetooth device that indicates the state of charge of the battery changed
   * \param chr is the characteristic that sent the notification
   * \param value is the raw array of values for the characteristic
   */
  void updateBatterySoc            (QLowEnergyCharacteristic chr, QByteArray value);
  /*!
   * \brief updateHeartRate is called when a notification is received from the
   * bluetooth device that indicates a new measurement of the heartrate is
   * available.
   * \param chr is the characteristic that sent the notification
   * \param value is the raw array of values for the characteristic
   */
  void updateHeartRate             (QLowEnergyCharacteristic chr, QByteArray value);

private:
  unsigned int                 batterySoc_;
  unsigned int                 heartRate_;
  QLowEnergyController*        control_;
  QList<QBluetoothUuid>        services_;
  QLowEnergyService*           heartRateService_;
  QLowEnergyService*           batteryService_;
  QTimer*                      timer_;
};

#endif // HEARTSENSOR_H
