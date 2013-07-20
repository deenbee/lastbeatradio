#include "tracklistwidget.h"
#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QDebug>

TrackListWidget::TrackListWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setAcceptDrops(true);

    QStringList list;
    list << "Position" << "Title" << "Artist Name" <<"Duration"<<"Format";
    setHeaderLabels(list);
    setColumnWidth(0,50);
    setColumnWidth(1,300);
    setColumnWidth(2,150);
    setColumnWidth(3,75);
    setColumnWidth(4,50);
}

void TrackListWidget::dragEnterEvent(QDragEnterEvent *event)
 {
    qDebug()<<"Drag Enter Event tracklistview";
     if (event->mimeData()->hasFormat("text/plain"))
         event->accept();
     else
         event->ignore();
 }

void TrackListWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    qDebug()<<"Drag Leave Event tracklistview";
    event->accept();
}

void TrackListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain")) {
         event->setDropAction(Qt::MoveAction);
         event->accept();
     } else {
         event->ignore();
     }
}

void TrackListWidget::dropEvent(QDropEvent *event)
{
     qDebug()<<"Drop Event tracklistview";

     if (event->mimeData()->hasFormat("text/plain")) {
         QString listfile = event->mimeData()->text();
         QStringList files = listfile.split(";");

         for(int i=0; i<files.size();i++){
            if(check_is_audio(files.at(i))){
                 //qDebug()<<"Is audio";
                 emit add_track(files.at(i));
            }
         }
         event->setDropAction(Qt::MoveAction);
         event->accept();
     } else {
         event->ignore();
     }
}

bool TrackListWidget::check_is_audio(QString filename){
    QFileInfo file(filename);

    if(file.suffix()=="mp3" || file.suffix()=="wav" || file.suffix()=="aiff"
       || file.suffix()=="wma" || file.suffix()=="ogg" || file.suffix()=="flac"){
        return true;
    }
    else{
        return false;
    }
}

