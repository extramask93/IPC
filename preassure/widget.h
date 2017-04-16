#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QTimer>
#include <string>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    ////////////
    void* shmptr;
    int *shm_ptr;
    int shmdes, index;
    sem_t *semdes;
    int SHM_SIZE;
    std::string shm_fn;
    std::string sem_fn;
     //////////////
    Ui::Widget *ui;
    QTimer *timer;
private slots:
    void WriteToSharedMemory(int);
    void poweroff();
};

#endif // WIDGET_H
