#include <QtWidgets>
#include <QMediaPlaylist>
#include "audioplayer.h"
#include "taglib/tag.h"
#include "taglib/fileref.h"
#include <QDebug>

AudioPlayer::AudioPlayer()
{
    current_length = 0;
    playlist = new QMediaPlaylist;
    this->setPlaylist(playlist);
    connect(playlist,SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
    //connect(this, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
}

void AudioPlayer::currentIndexChanged(int index){
    emit update_current_track(index);
}

void AudioPlayer::previous_track(){
    playlist->previous();
}

void AudioPlayer::next_track(){
    playlist->next();
}

void AudioPlayer::addMedia(QString file){
    playlist->addMedia(QUrl(file));
}

void AudioPlayer::removeMedia (int index){
    playlist->removeMedia(index);
}

void AudioPlayer::setCurrentIndex(int index){
    playlist->setCurrentIndex(index);
}

QString AudioPlayer::getPosition(){
    return convert_time(this->position());
}

QString AudioPlayer::getDuration_formated(){
    QString filename = playlist->currentMedia().canonicalUrl().toString(QUrl::PreferLocalFile);

    #ifdef Q_OS_WIN
            const wchar_t * encodedName = reinterpret_cast<const wchar_t*>(filename.utf16());
    #else
            const char * encodedName = QFile::encodeName(filename).constData();
    #endif

    TagLib::FileRef fr(encodedName, true, TagLib::AudioProperties::Accurate);

    current_length = fr.audioProperties()->length();
    return convert_time(fr.audioProperties()->length());
}

int AudioPlayer::getDuration(){
    return current_length*1000;
}

QString AudioPlayer::getRemainingTime(){
    return "- "+convert_time((current_length*1000)-this->position());
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

int AudioPlayer::getNextIndex(){
    return playlist->nextIndex();
}
