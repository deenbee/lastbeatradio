#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileSystemModel"
#include "QTreeWidget"
#include "QDateTime"
#include "QDebug"
#include "QFileDialog"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    init();
    go_up = false;
    stopaftercurrent = false;
    total_length = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init(){

    //get current date
    QDate date = QDate::currentDate();
    QString dateString = date.toString("dddd, d 'de' MMMM 'de' yyyy");
    ui->label_date->setText(dateString);

    //get current time and clock start
    clock = new DigitalClock();
    connect(clock,SIGNAL(updateTime(QString)),this,SLOT(updateClockLabel(QString)));

    AudioPlayer audioplayer;
    QStringList playlist_names;

    treeView_tracks = new TrackListView(ui->verticalLayoutWidget_5);
    ui->verticalLayout_6->addWidget(treeView_tracks);

    browseview = new BrowseView(this);
    ui->verticalLayout_browsemusic->addWidget(browseview);

    createActions();
}

void MainWindow::createActions()
{
    connect(&audioplayer,SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(&audioplayer,SIGNAL(update_current_track(int)), this, SLOT(currentMediaChanged(int)));
    connect(treeView_tracks,SIGNAL(add_drop_track(QString)),this, SLOT(add_drop_track(QString)));
}

void MainWindow::updateClockLabel(QString text_time)
{
   // QString text = "hh:mm:ss";
    ui->label_clock->setText(text_time);
}

void MainWindow::on_pushButton_AddFile_clicked()
{
    QMediaResource mediaresource;

    if(playlist_names.empty()){
        playlist_names = QFileDialog::getOpenFileNames(this, tr("Audio"),QDir::rootPath(),"Audio (*.mp3 *.wav *.aiff *.flac *.ogg *.wma)");
        for (int i = 0; i < playlist_names.size(); ++i){
            QString file = playlist_names.at(i);
            mediaresource = QMediaResource(QUrl(file));
            audioplayer.addMedia(file);
            treeView_tracks->add_track(file,i);
        }
        audioplayer.setCurrentIndex(0);
    }
    else{
        QStringList aux = QFileDialog::getOpenFileNames(this, tr("Audio"),QDir::rootPath(),"Audio (*.mp3 *.wav *.aiff *.flac *.ogg *.wma))");
        int prev_numtracks = playlist_names.count();
        playlist_names = playlist_names+aux;
        for (int i = 0; i < aux.size(); ++i){
            QString file = aux.at(i);
            audioplayer.addMedia(file);
            treeView_tracks->add_track(file,i+prev_numtracks);
        }
    }

    calculate_total_length();
}

void MainWindow::add_drop_track(QString filename){
    playlist_names.append(filename);
    audioplayer.addMedia(filename);
}

void MainWindow::on_pushButton_RemoveFile_clicked()
{
    if(!playlist_names.empty()){
        playlist_names.removeAt(treeView_tracks->currentIndex().row());
        audioplayer.removeMedia(treeView_tracks->currentIndex().row());
        treeView_tracks->remove_track();
        calculate_total_length();
    }   
}

void MainWindow::on_pushButton_Stop_clicked()
{
    if (ui->pushButton_Play->isChecked()){
        ui->pushButton_Play->toggle();
    }
    audioplayer.stop();
}

void MainWindow::on_pushButton_Play_toggled(bool checked)
{
    if(checked){
        audioplayer.setVolume(ui->dial_volume->value());
        audioplayer.play();
    }
    else{
        audioplayer.pause();
    }
}

void MainWindow::positionChanged(qint64 time){
    ui->horizontalSlider_audio_track_position->setValue(time);
    ui->label_audio_time_position->setText(audioplayer.getPosition());
    ui->label_remain_duration->setText(audioplayer.getRemainingTime());
}

void MainWindow::currentMediaChanged(int index){
    if(stopaftercurrent){
       on_pushButton_Stop_clicked();
       ui->pushButton_stopaftercurrent->setChecked(false);
       stopaftercurrent = false;
       clear_mark_tracks();
    }
    else{
        if(index > -1){
            QString track = playlist_names.at(index);
            ui->label_remain_duration->setText(audioplayer.getDuration_formated());
            ui->horizontalSlider_audio_track_position->setMaximum(audioplayer.getDuration());

            if(index>0){
                treeView_tracks->changeRowColor(index-1,Qt::white);
            }

            if(go_up & index+2 < playlist_names.count()){
                go_up = false;
                treeView_tracks->changeRowColor(index+2,Qt::white);
            }

            treeView_tracks->changeRowColor(index,Qt::green);
            QString name = track.remove(0,track.lastIndexOf("/")+1);
            ui->lineEdit_playing_track->setText(name.remove(track.lastIndexOf("."),track.size()));

            if(audioplayer.getNextIndex()!= -1){
                track = playlist_names.at(index+1);
                treeView_tracks->changeRowColor(index+1,QColor(100,149,237));
                QString name = track.remove(0,track.lastIndexOf("/")+1);
                ui->lineEdit_next_track->setText(name.remove(track.lastIndexOf("."),track.size()));
            }
            else{
                ui->lineEdit_next_track->setText("");
            }
        }
        else{
            clear_mark_tracks();
            ui->lineEdit_playing_track->setText("");
            ui->pushButton_Play->setChecked(false);
            audioplayer.stop();
        }
    }

}

void MainWindow::clear_mark_tracks(){
    for(int i=0; i<playlist_names.count();i++){
        treeView_tracks->changeRowColor(i,Qt::white);
    }
}

void MainWindow::on_pushButton_NextTrack_clicked()
{
    audioplayer.next_track();
}

void MainWindow::on_pushButton_PreviousTrack_clicked()
{
    go_up = true;
    audioplayer.previous_track();
}

void MainWindow::on_dial_volume_valueChanged(int value)
{
    audioplayer.setVolume(value);
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::calculate_total_length(){
    for(int i =0; i<playlist_names.count(); i++){
        QString filename = playlist_names.at(i);

        #ifdef Q_OS_WIN
                const wchar_t * encodedName = reinterpret_cast<const wchar_t*>(filename.utf16());
        #else
                const char * encodedName = QFile::encodeName(filename).constData();
        #endif

        TagLib::FileRef fr(encodedName, true, TagLib::AudioProperties::Accurate);
        total_length += fr.audioProperties()->length();
    }
    ui->label_totallength->setText(convert_time(total_length));
}

QString MainWindow::convert_time(qint64 secs){
    QString formattedTime;

    int hours, minutes, seconds;

    seconds = secs;
    hours = seconds / (60*60);
    seconds %= 60*60;
    minutes = seconds / 60;
    seconds %= 60;

    formattedTime.append(QString("%1").arg(hours, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));
    //QString("%1").arg(hours, 2, 10, QLatin1Char('0')) + ":" +
    //QString( "%1" ).arg(milliseconds, 3, 10, QLatin1Char('0'))

    return formattedTime;
}

void MainWindow::on_horizontalSlider_audio_track_position_sliderMoved(int position)
{
    audioplayer.setPosition(position);
}

void MainWindow::on_pushButton_stopaftercurrent_toggled(bool checked)
{
    stopaftercurrent = checked;
}
