#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlaylist>
#include <QTreeWidgetItem>
#include "digitalclock.h"
#include "audioplayer.h"

#include "tag.h"
#include "taglib/mpeg/mpegfile.h"
#include "tbytevectorstream.h"
#include "taglib/mpeg/id3v2/id3v2tag.h"
#include "taglib/mpeg/id3v2/id3v2framefactory.h"
#include "taglib/fileref.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void createActions();
    void updateClockLabel(QString text_time);
    void directoryViewer();
    void init();

private slots:
    void on_actionQuit_triggered();
    void on_dial_volume_valueChanged(int value);
    void on_pushButton_AddFile_clicked();
    void on_pushButton_NextTrack_clicked();
    void on_pushButton_PreviousTrack_clicked();
    void on_pushButton_Play_toggled(bool checked);
    void on_pushButton_RemoveFile_clicked();
    void on_pushButton_Stop_clicked();
    void positionChanged(qint64 time);
    void currentMediaChanged(qint64 duration);

    void get_audio_metadata(QString filename, int i);
    void calculate_total_length();
    QString convert_time(qint64 secs);

private:
    DigitalClock *clock;
    Ui::MainWindow *ui;
    AudioPlayer audioplayer;

    QMediaPlaylist *playlist;
    QStringList playlist_names;

};

#endif // MAINWINDOW_H
