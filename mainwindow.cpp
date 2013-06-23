#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileSystemModel"
#include "QTreeWidget"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    directoryViewer();

    clock = new DigitalClock();
    connect(clock,SIGNAL(updateTime(QString)),this,SLOT(updateClockLabel(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
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
