#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();

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
