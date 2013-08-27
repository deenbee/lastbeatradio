#include "browseview.h"
#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>

BrowseView::BrowseView(QWidget *parent) :
    QTreeView(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setMaximumWidth(500);
    viewport()->setAcceptDrops(true);

    model = new QFileSystemModel;
    model->setRootPath(QDir::rootPath());
    setModel(model);

    hideColumn(1);
    hideColumn(2);
    hideColumn(3);
}

void BrowseView::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug()<<"Drag Enter Event browseview";
    if (event->mimeData()->hasFormat("text/plain"))
        event->accept();
    else
        event->ignore();
}

void BrowseView::dragMoveEvent(QDragMoveEvent *event)
{
    //qDebug()<<"Drag Move Event browseview";
    if (event->mimeData()->hasFormat("text/plain")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void BrowseView::dropEvent(QDropEvent *event)
{
    qDebug()<<"Drop Event browseview";
    if (event->mimeData()->hasFormat("text/plain")) {
        QByteArray pieceData = event->mimeData()->data("text/plain");
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void BrowseView::startDrag(Qt::DropActions /*supportedActions*/)
{
    qDebug()<<"Start Drag browseview";
    QModelIndexList list = selectionModel()->selectedIndexes();

    QMimeData * mimeData = new QMimeData();
    QString file;
    int row = -1;

    foreach (QModelIndex index, list)
    {
        if (index.row()!=row && index.column()==0)
        {
            qDebug() << model->filePath(index)<< '\n';
            QString filename(model->filePath(index));
            file += filename+";";
            row = index.row();
        }
    }

   mimeData->setText(file);

    /*
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(model->filePath(currentIndex()));
    //mimeData = model->mimeData(this->currentIndex());;
    */
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    if (drag->exec(Qt::MoveAction) == Qt::MoveAction){
        //delete takeItem(row(item));
    }
}
