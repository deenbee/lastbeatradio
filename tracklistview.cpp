#include "tracklistview.h"
#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QDebug>

TrackListView::TrackListView(QWidget *parent) :
    QTreeView(parent)
{
    //setSelectionMode(QAbstractItemView::ExtendedSelection);
    setAcceptDrops(true);

    model_tracks = new QStandardItemModel;
    setModel(model_tracks);

    QStringList list;
    list << "Position" << "Title" << "Artist Name" <<"Duration"<<"Format";
    model_tracks->setHorizontalHeaderLabels(list);

    setColumnWidth(0,50);
    setColumnWidth(1,250);
    setColumnWidth(2,150);
    setColumnWidth(3,75);
    setColumnWidth(4,50);
}

void TrackListView::dragEnterEvent(QDragEnterEvent *event)
 {
    qDebug()<<"Drag Enter Event tracklistview";
     if (event->mimeData()->hasFormat("text/plain"))
         event->accept();
     else
         event->ignore();
 }

void TrackListView::dragLeaveEvent(QDragLeaveEvent *event)
{
    qDebug()<<"Drag Leave Event tracklistview";
    event->accept();
}

void TrackListView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain")) {
         event->setDropAction(Qt::MoveAction);
         event->accept();
     } else {
         event->ignore();
     }
}

void TrackListView::dropEvent(QDropEvent *event)
{
     qDebug()<<"Drop Event tracklistview";

     if (event->mimeData()->hasFormat("text/plain")) {
         QString listfile = event->mimeData()->text();
         QStringList files = listfile.split(";");

         for(int i=0; i<files.size();i++){
            if(check_is_audio(files.at(i))){
                 //qDebug()<<"Is audio";
                emit add_drop_track(files.at(i));
                add_track(files.at(i),i+model_tracks->rowCount());
            }
         }
         event->setDropAction(Qt::MoveAction);
         event->accept();
     } else {
         event->ignore();
     }
}

bool TrackListView::check_is_audio(QString filename){
    QFileInfo file(filename);

    if(file.suffix()=="mp3" || file.suffix()=="wav" || file.suffix()=="aiff"
       || file.suffix()=="wma" || file.suffix()=="ogg" || file.suffix()=="flac"){
        return true;
    }
    else{
        return false;
    }
}

void TrackListView::add_track(QString filename, int row){
    #ifdef Q_OS_WIN
            const wchar_t * encodedName = reinterpret_cast<const wchar_t*>(filename.utf16());
    #else
            const char * encodedName = QFile::encodeName(filename).constData();
    #endif

    TagLib::MPEG::File file(encodedName);
    TagLib::ID3v2::Tag *tag = file.ID3v2Tag();

    TagLib::FileRef fr(encodedName, true, TagLib::AudioProperties::Accurate);

    model_tracks->setItem(row, 0, new QStandardItem(QString("%1").arg(row, 3, 10, QLatin1Char('0'))));
    model_tracks->setItem(row, 1, new QStandardItem(filename.remove(0,filename.lastIndexOf("/")+1)));
    model_tracks->setItem(row, 2, new QStandardItem(QString::fromStdString(tag->artist().to8Bit())));
    model_tracks->setItem(row, 3, new QStandardItem(convert_time(fr.audioProperties()->length())));
    model_tracks->setItem(row, 4, new QStandardItem(filename.remove(0,filename.lastIndexOf(".")+1)));
}

void TrackListView::remove_track(){
    model_tracks->removeRow(currentIndex().row());

    for (int i = currentIndex().row(); i<model_tracks->rowCount();i++){
        model_tracks->setItem(i,0,new QStandardItem(QString("%1").arg(i, 3, 10, QLatin1Char('0'))));
    }
}

void TrackListView::changeRowColor(int idx, QColor color){
    for(int i=0; i<5;i++){
        model_tracks->item(idx,i)->setBackground(QBrush(color));
    }
}

QString TrackListView::convert_time(qint64 secs){
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



