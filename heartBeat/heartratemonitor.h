#ifndef HEARTRATEMONITOR_H
#define HEARTRATEMONITOR_H

#include <QObject>
#include <QTimer>

class HeartRateMonitor : public QObject
{
  Q_OBJECT
public:
  explicit HeartRateMonitor(QObject *parent = 0);

  Q_PROPERTY(int upperLimit READ getUpperLimit WRITE setUpperLimit NOTIFY upperLimitChanged)
  Q_PROPERTY(int lowerLimit READ getLowerLimit WRITE setLowerLimit NOTIFY lowerLimitChanged)

  int getUpperLimit();
  int getLowerLimit();

signals:
  void upperLimitExceeded();
  void lowerLimitExceeded();
  void upperLimitChanged();
  void lowerLimitChanged();

public slots:
  void setUpperLimit(int value);
  void setLowerLimit(int value);
  void start();
  void stop();
  void setHeartRate(int value);

private slots:
  void checkHeartRateRange();

private:
  int     _upperLimit;
  int     _lowerLimit;
  int     _heartRate;
  QTimer* _timer;
};

#endif // HEARTRATEMONITOR_H
