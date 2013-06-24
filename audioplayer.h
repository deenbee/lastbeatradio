#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "QMediaPlayer"
#include "QObject"

class AudioPlayer: public QMediaPlayer
{
    Q_OBJECT

public:
    AudioPlayer();
    QString getPosition();
    QString getDuration();
    QString getRemainingTime();
private:
    QString convert_time(qint64 msecs);
};

#endif // AUDIOPLAYER_H
