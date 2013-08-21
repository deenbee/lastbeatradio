#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlaylist>
#include "digitalclock.h"
#include "audioplayer.h"
#include "tracklistview.h"
#include "browseview.h"

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
    void init();
    void add_drop_track(QString filename);


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
    void currentMediaChanged(int index);
    void clear_mark_tracks();

    void calculate_total_length();
    QString convert_time(qint64 secs);

private:
    DigitalClock *clock;
    Ui::MainWindow *ui;
    AudioPlayer audioplayer;

    TrackListView *treeView_tracks;
    BrowseView *browseview;

    QStringList playlist_names;
    bool go_up;
};

#endif // MAINWINDOW_H
