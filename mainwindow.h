#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "digitalclock.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateClockLabel(QString text_time);
    
private:
    DigitalClock *clock;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
