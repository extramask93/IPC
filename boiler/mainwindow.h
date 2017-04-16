#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMessageBox>
#include <QTimer>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsScene *scene2;
    QGraphicsRectItem *rectangle_wat;
    QGraphicsRectItem *background_rectangle_wat;
    QGraphicsTextItem *text;
    QGraphicsRectItem *rectangle_pre;
    QGraphicsRectItem *background_rectangle_pre;
    QTimer *timer;
    void* shmptr;
    unsigned int mode;
    int shmdes, index;
    sem_t *sem_des;
    int SHM_SIZE;
    int *shm_ptr;
    int water_level;
    int preassure_level;
    int temperature_level;
    //functions
    int InitializeIPC();
    void UpdateGauges(int water, int preassure, int temperature);
public slots:
    void updateall();
    void poweroff();
};

#endif // MAINWINDOW_H
