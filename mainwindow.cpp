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

    directoryViewer();
    createActions();

    AudioPlayer audioplayer;
    playlist = new QMediaPlaylist;
    QStringList playlist_names;
}

void MainWindow::createActions()
{
    connect(&audioplayer,SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(&audioplayer,SIGNAL(durationChanged(qint64)), this, SLOT(currentMediaChanged(qint64)));
}

void MainWindow::updateClockLabel(QString text_time){
   // QString text = "hh:mm:ss";
    ui->label_clock->setText(text_time);
}

void MainWindow::directoryViewer(){
    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::rootPath());
    ui->treeView_directoryviewer->setModel(model);
    ui->treeView_directoryviewer->hideColumn(1);
    ui->treeView_directoryviewer->hideColumn(2);
    ui->treeView_directoryviewer->hideColumn(3);
}

void MainWindow::on_pushButton_AddFile_clicked()
{
    if(playlist_names.empty()){
        playlist_names = QFileDialog::getOpenFileNames(this, tr("Selecciona los archivos de audio"),QDir::rootPath(),"Multimedia (*.mp3 *.wav *.aiff *.flac *.ogg *.wma)");
        for (int i = 0; i < playlist_names.size(); ++i){
            QString file = playlist_names.at(i);
            playlist->addMedia(QUrl(file));
            ui->listWidget->addItem(file.remove(0,file.lastIndexOf("/")+1));
        }
    }
    else{
        QStringList aux = QFileDialog::getOpenFileNames(this, tr("Selecciona los archivos de audio"),QDir::rootPath(),"Multimedia (*.mp3 *.wav *.aiff *.flac *.ogg *.wma))");
        playlist_names = playlist_names+aux;
        for (int i = 0; i < aux.size(); ++i){
            QString file = aux.at(i);
            playlist->addMedia(QUrl(file));
            ui->listWidget->addItem(file.remove(0,file.lastIndexOf("/")+1));
        }
    }

    playlist->setCurrentIndex(1);
    audioplayer.setPlaylist(playlist);
}

void MainWindow::on_pushButton_RemoveFile_clicked()
{
    if(!playlist_names.empty()){
        playlist_names.removeAt(ui->listWidget->currentRow());
        playlist->removeMedia(ui->listWidget->currentRow());
        delete ui->listWidget->takeItem(ui->listWidget->currentRow());
    }
    audioplayer.setPlaylist(playlist);
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
    ui->horizontalSlider_audio_time_position->setValue(time);
    ui->label_audio_time_position->setText(audioplayer.getPosition());
    ui->label_remain_duration->setText(audioplayer.getRemainingTime());
}

void MainWindow::currentMediaChanged(qint64 duration){
    ui->horizontalSlider_audio_time_position->setMaximum(duration);
    ui->label_remain_duration->setText(audioplayer.getRemainingTime());

    if(playlist->currentIndex()!= -1){
        QString track = playlist_names.at(playlist->currentIndex());
        if(playlist->currentIndex()>0){
            ui->listWidget->item(playlist->currentIndex()-1)->setBackground(Qt::white);
        }
        ui->listWidget->item(playlist->currentIndex())->setBackground(Qt::green);
        ui->textBrowser_playing_track->setText(track.remove(0,track.lastIndexOf("/")+1));

        if(playlist->nextIndex()!= -1){
            track = playlist_names.at(playlist->nextIndex());
            ui->listWidget->item(playlist->nextIndex())->setBackground(QColor(255,100,20));
            ui->textBrowser_next_track->setText(track.remove(0,track.lastIndexOf("/")+1));
        }
        else{
            ui->textBrowser_next_track->setText("");
        }
    }
    else{
        ui->listWidget->item(playlist_names.count()-1)->setBackground(Qt::white);
        ui->textBrowser_playing_track->setText("-");
        ui->pushButton_Play->setChecked(false);
        audioplayer.stop();
    }

}

void MainWindow::on_pushButton_NextTrack_clicked()
{
    audioplayer.playlist()->next();
}

void MainWindow::on_pushButton_PreviousTrack_clicked()
{
    audioplayer.playlist()->previous();
}

void MainWindow::on_dial_volume_valueChanged(int value)
{
    audioplayer.setVolume(value);
}
