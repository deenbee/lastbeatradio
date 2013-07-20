#ifndef DIGITALCLOCK_H
#define DIGITALCLOCK_H

#include <QObject>

class DigitalClock: public QObject
{
    Q_OBJECT
public:
    DigitalClock();

public slots:
    void getTime();

signals:
    void updateTime(QString newValue);

private:
    int m_value;
};

#endif
