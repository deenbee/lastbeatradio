#ifndef TRACKLISTWIDGET_H
#define TRACKLISTWIDGET_H

#include <QTreeWidget>

class QDragEnterEvent;
class QDropEvent;

class TrackListWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit TrackListWidget(QWidget *parent = 0);

signals:
    void add_track(QString filename);
protected:
    bool check_is_audio(QString filename);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

};

#endif // WORKSPACEWIDGET_H
