#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect( ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(WriteToSharedMemory(int)) );
    timer= new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(poweroff()));
    timer->start(1000);
    //connect(ui->verticalSlider,SIGNAL(sliderMoved(int),this,)
    //IPC starting:

    shm_fn="IP_CC_shm";
    sem_fn="IP_CC_sem";
    //open the shared memory
    SHM_SIZE = sysconf(_SC_PAGESIZE);

      if ( (shmdes = shm_open(shm_fn.c_str(), O_RDWR, 0)) == -1 ) {
         qFatal("shm_open failure");
         QCoreApplication::exit(-1);
       }

      if((shmptr = mmap(0, SHM_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED,
                   shmdes,0)) == (void*) -1){
         qFatal("mmap failure");
        QCoreApplication::exit(-1);
}
      shm_ptr = (int*)shmptr;
      //open the semaphore
      semdes = sem_open(sem_fn.c_str(), 0, 0644, 0);

      if(semdes == (void*) -1){
        qFatal("sem_open failure");
        QCoreApplication::exit(-1);
      }
}

Widget::~Widget()
{
    munmap(shmptr, SHM_SIZE);

     /* Close the shared memory object */

     ::close(shmdes);

     /* Close the Semaphore */

     sem_close(semdes);
    delete ui;

}
void Widget::poweroff()
{
    if(shm_ptr[0]==1)
        QApplication::quit();
}
void Widget::WriteToSharedMemory(int temperature)
{




   shm_ptr[0]=0; /* shutdown */
   /* shm_ptr[3]  cabin temperature */
   if(sem_wait(semdes)){qFatal("sem_op failure!!");QCoreApplication::exit(-1);}  /*no shared objects cleaning on exit !!!*/

   /*write sensor values*/
   shm_ptr[3]=temperature;

   sem_post(semdes);

}
