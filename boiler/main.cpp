#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    a.exec();
    //////////////////////////////////////////////////////////////////////////////////




//    while(!shm_ptr[0]){
//   /*CS*/
//   if(sem_wait(sem_des)){perror("sem_op failure!!");_exit(-1);}  /*no shared objects cleaning on exit !!!*/

//   /*read sensor values*/
//   water_level=shm_ptr[1];
//   preassure_level=shm_ptr[2];

//   sem_post(sem_des);
//   /*end of CS*/
//   perror("sadasdadas");
//   sleep(5);}







}
