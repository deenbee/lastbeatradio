#ifndef TRACKLISTWIDGET_H
#define TRACKLISTWIDGET_H

#include <QTreeView>
#include <QStandardItemModel>

#include "tag.h"
#include "taglib/mpeg/mpegfile.h"
#include "tbytevectorstream.h"
#include "taglib/mpeg/id3v2/id3v2tag.h"
#include "taglib/mpeg/id3v2/id3v2framefactory.h"
#include "taglib/fileref.h"

class QDragEnterEvent;
class QDropEvent;

class TrackListView : public QTreeView
{
    Q_OBJECT
public:
    explicit TrackListView(QWidget *parent = 0);

signals:
    void add_drop_track(QString filename);

public slots:
    void add_track(QString filename, int row);
    void remove_track();
    void changeRowColor(int idx, QColor color);

protected:
    bool check_is_audio(QString filename);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    QString convert_time(qint64 secs);

private:
    QStandardItemModel *model_tracks;
};

#endif // WORKSPACEWIDGET_H
