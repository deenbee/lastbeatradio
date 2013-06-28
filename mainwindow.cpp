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
    QStringList playlist_names;

    ui->treeWidget_tracks->header()->resizeSection(0,50);
    ui->treeWidget_tracks->header()->resizeSection(1,325);
    ui->treeWidget_tracks->header()->resizeSection(3,75);
    ui->treeWidget_tracks->header()->resizeSection(4,50);
}

void MainWindow::createActions()
{
    connect(&audioplayer,SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(&audioplayer,SIGNAL(update_current_track(int)), this, SLOT(currentMediaChanged(int)));
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
    QMediaResource mediaresource;

    if(playlist_names.empty()){
        playlist_names = QFileDialog::getOpenFileNames(this, tr("Audio"),QDir::rootPath(),"Audio (*.mp3 *.wav *.aiff *.flac *.ogg *.wma)");
        for (int i = 0; i < playlist_names.size(); ++i){
            QString file = playlist_names.at(i);
            mediaresource = QMediaResource(QUrl(file));
            audioplayer.addMedia(file);

            get_audio_metadata(file, i);

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
            get_audio_metadata(file, i+prev_numtracks);
        }
    }

    calculate_total_length();
}

void MainWindow::on_pushButton_RemoveFile_clicked()
{
    if(!playlist_names.empty()){
        playlist_names.removeAt(ui->treeWidget_tracks->currentIndex().row());
        audioplayer.removeMedia(ui->treeWidget_tracks->currentIndex().row());
        delete ui->treeWidget_tracks->currentItem();

        for (int i = ui->treeWidget_tracks->currentIndex().row(); i<playlist_names.count();i++){
            ui->treeWidget_tracks->topLevelItem(i)->setText(0, QString("%1").arg(i, 3, 10, QLatin1Char('0')));
        }
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
    ui->horizontalSlider_audio_time_position->setValue(time);
    ui->label_audio_time_position->setText(audioplayer.getPosition());
    ui->label_remain_duration->setText(audioplayer.getRemainingTime());
}

void MainWindow::currentMediaChanged(int index){
    qDebug()<<"Modifiquem la canço actual"<<index;

    if(index > -1){
        QString track = playlist_names.at(index);
        ui->label_remain_duration->setText(audioplayer.getDuration_formated());
        ui->horizontalSlider_audio_time_position->setMaximum(audioplayer.getDuration());

        if(index>0){
            ui->treeWidget_tracks->topLevelItem(index-1)->setBackgroundColor(0,Qt::white);

        }

        if(go_up & index+2 < playlist_names.count()){
            go_up = false;
            ui->treeWidget_tracks->topLevelItem(index+2)->setBackgroundColor(0,Qt::white);
        }

        ui->treeWidget_tracks->topLevelItem(index)->setBackgroundColor(0,Qt::green);
        ui->textBrowser_playing_track->setText(track.remove(0,track.lastIndexOf("/")+1));

        if(audioplayer.getNextIndex()!= -1){
            track = playlist_names.at(index+1);
            ui->treeWidget_tracks->topLevelItem(index+1)->setBackgroundColor(0,QColor(255,100,20));
            ui->textBrowser_next_track->setText(track.remove(0,track.lastIndexOf("/")+1));
        }
        else{
            ui->textBrowser_next_track->setText("");
        }
    }
    else{
        clear_mark_tracks();
        ui->textBrowser_playing_track->setText("");
        ui->pushButton_Play->setChecked(false);
        audioplayer.stop();
    }
}

void MainWindow::clear_mark_tracks(){
    for(int i=0; i<playlist_names.count();i++){
        ui->treeWidget_tracks->topLevelItem(i)->setBackgroundColor(0,Qt::white);
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

void MainWindow::get_audio_metadata(QString filename, int i)
{
    #ifdef Q_OS_WIN
            const wchar_t * encodedName = reinterpret_cast<const wchar_t*>(filename.utf16());
    #else
            const char * encodedName = QFile::encodeName(filename).constData();
    #endif

    TagLib::MPEG::File file(encodedName);
    TagLib::ID3v2::Tag *tag = file.ID3v2Tag();

    //TagLib::FileName fn(filename.toStdString().c_str());
    TagLib::FileRef fr(encodedName, true, TagLib::AudioProperties::Accurate);

    QTreeWidgetItem *trackItem = new QTreeWidgetItem(ui->treeWidget_tracks);
    trackItem->setText(0, QString("%1").arg(i, 3, 10, QLatin1Char('0')));   //Position
    trackItem->setText(1, filename.remove(0,filename.lastIndexOf("/")+1));  //Title
    trackItem->setText(2, QString::fromStdString(tag->artist().to8Bit()));  //Artist Name
    trackItem->setText(3, convert_time(fr.audioProperties()->length()));    //Duration
    trackItem->setText(4, filename.remove(0,filename.lastIndexOf(".")+1));  //Format
}

void MainWindow::calculate_total_length(){
    int time = 0;

    for(int i =0; i<playlist_names.count(); i++){
        QString filename = playlist_names.at(i);

        #ifdef Q_OS_WIN
                const wchar_t * encodedName = reinterpret_cast<const wchar_t*>(filename.utf16());
        #else
                const char * encodedName = QFile::encodeName(filename).constData();
        #endif

        TagLib::FileRef fr(encodedName, true, TagLib::AudioProperties::Accurate);
        time += fr.audioProperties()->length();
    }

    ui->label_totallength->setText(convert_time(time));
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
