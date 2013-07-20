#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QMediaPlayer>
#include <QObject>

class AudioPlayer: public QMediaPlayer
{
    Q_OBJECT

public:
    AudioPlayer();
    QString getPosition();
    QString getDuration_formated();
    QString getRemainingTime();
    int  getDuration();
    int  getNextIndex();
    void previous_track();
    void next_track();
    void addMedia(QString file);
    void removeMedia(int index);
    void setCurrentIndex(int index);

signals:
    void update_current_track(int index);

private slots:
    void currentIndexChanged(int index);

private:
    QString convert_time(qint64 msecs);
    int current_length;

    QMediaPlaylist *playlist;
};

#endif // AUDIOPLAYER_H
