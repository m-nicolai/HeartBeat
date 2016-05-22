#include "heartratemonitor.h"

HeartRateMonitor::HeartRateMonitor(QObject *parent) : QObject(parent)
{
  _upperLimit = 0;
  _lowerLimit = 0;
  _timer = new QTimer();
  _timer->setInterval(3000);
  connect(_timer, SIGNAL(timeout()), this, SLOT(checkHeartRateRange()));
}


int
HeartRateMonitor::getUpperLimit ()
{
  return _upperLimit;
}



int
HeartRateMonitor::getLowerLimit ()
{
  return _lowerLimit;
}

void
HeartRateMonitor::setUpperLimit(int value)
{
  _upperLimit = value;
  emit upperLimitChanged();
}



void
HeartRateMonitor::setLowerLimit(int value)
{
  _lowerLimit = value;
  emit lowerLimitChanged();
}



void
HeartRateMonitor::start()
{
  _timer->start();
}



void
HeartRateMonitor::stop()
{
  _timer->stop();
}



void
HeartRateMonitor::setHeartRate(int value)
{
  _heartRate = value;
}


void
HeartRateMonitor::checkHeartRateRange()
{
  if (_heartRate > _upperLimit)
    emit upperLimitExceeded();
  if (_heartRate < _lowerLimit)
    emit lowerLimitExceeded();
}
