#ifndef BROWSEVIEW_H
#define BROWSEVIEW_H

#include <QTreeView>
#include <QFileSystemModel>

class BrowseView : public QTreeView
{
    Q_OBJECT
public:
    explicit BrowseView(QWidget *parent = 0);
    
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void startDrag(Qt::DropActions supportedActions);
    
public slots:

private:
    QFileSystemModel *model;
    
};

#endif // BROWSEVIEW_H
