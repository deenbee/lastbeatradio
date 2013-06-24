#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlaylist>
#include "digitalclock.h"
#include "audioplayer.h"

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
    void on_pushButton_AddFile_clicked();
    void on_pushButton_NextTrack_clicked();
    void on_pushButton_PreviousTrack_clicked();
    void on_pushButton_Play_toggled(bool checked);
    void on_pushButton_RemoveFile_clicked();
    void on_pushButton_Stop_clicked();
    void positionChanged(qint64 time);
    void currentMediaChanged(qint64 duration);

private:
    DigitalClock *clock;
    Ui::MainWindow *ui;
    AudioPlayer audioplayer;

    QMediaPlaylist *playlist;
    QStringList playlist_names;

};

#endif // MAINWINDOW_H
