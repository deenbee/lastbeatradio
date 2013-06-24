#include <QtWidgets>
#include "audioplayer.h"
#include "QMediaPlaylist"
#include <QDebug>

AudioPlayer::AudioPlayer()
{
    //connect(this, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    //this->setMedia(QUrl::fromLocalFile("D:\\Users\\Albert\\Desktop\\Celebra los buenos momentos_01.mp3"));
}

QString AudioPlayer::getPosition(){
    return convert_time(this->position());
}

QString AudioPlayer::getDuration(){
    return convert_time(this->duration());
}

QString AudioPlayer::getRemainingTime(){
    return "- "+convert_time(this->duration()-this->position());
}

QString AudioPlayer::convert_time(qint64 msecs){
    QString formattedTime;

    int hours = msecs/(1000*60*60);
    int minutes = (msecs-(hours*1000*60*60))/(1000*60);
    int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60))/1000;
    //int milliseconds = msecs-(seconds*1000)-(minutes*1000*60)-(hours*1000*60*60);

    formattedTime.append(QString("%1").arg(hours, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));
    //QString( "%1" ).arg(milliseconds, 3, 10, QLatin1Char('0'))

    return formattedTime;
}
