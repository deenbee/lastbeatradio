#include <QtWidgets>
#include "digitalclock.h"

DigitalClock::DigitalClock(){
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getTime()));
    timer->start(1000);
    getTime();
}

void DigitalClock::getTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    emit updateTime(text);
}
