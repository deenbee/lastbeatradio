#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileSystemModel"
#include "QTreeWidget"
#include <QDateTime>

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
