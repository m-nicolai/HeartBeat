#ifndef HEARTRATEMONITOR_H
#define HEARTRATEMONITOR_H

#include <QObject>

class HeartRateMonitor : public QObject
{
  Q_OBJECT
public:
  explicit HeartRateMonitor(QObject *parent = 0);

signals:

public slots:
};

#endif // HEARTRATEMONITOR_H
