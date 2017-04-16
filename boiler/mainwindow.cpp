#include "mainwindow.h"
#include "ui_mainwindow.h"

char shm_fn[] = "IP_CC_shm";
char sem_fn[] = "IP_CC_sem";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(InitializeIPC())
        QMessageBox::critical(this,"Błąd","Inicjalizacja pamieci dzilonej nieudana");
    timer= new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateall())); //adding update to event loop
    connect(ui->powerButton,SIGNAL(clicked()),this, SLOT(poweroff()));
    timer->start(1000);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene2 = new QGraphicsScene(this);
    ui->graphicsView_2->setScene(scene2);
    //pump init
    ui->waterComboBox->addItem("ON");
    ui->waterComboBox->addItem("OFF");
    ui->waterComboBox->setCurrentIndex(1);
    ui->waterComboBox->setEnabled(false);
    //valve init
    ui->valveComboBox->addItem("OPEN");
    ui->valveComboBox->addItem("CLOSED");
    ui->valveComboBox->setCurrentIndex(1);
    ui->valveComboBox->setEnabled(false);
    //alarm init
    ui->alarmComboBox->addItem("ON");
    ui->alarmComboBox->addItem("OFF");
    ui->alarmComboBox->setCurrentIndex(1);
    ui->alarmComboBox->setEnabled(false);
//obsługa animacji zbiornika
       QBrush greenBrush(Qt::green);
       QBrush whiteBrush(Qt::white);
       QBrush blueBrush(Qt::blue);
       QPen outlinePen(Qt::black);
       outlinePen.setWidth(2);
       QPen zeroPen(Qt::black);
       zeroPen.setWidth(1);
       //wizualizacja zbiornika wody
       int hmax_wat=200;
       int htemp_wat=0; //ustawienie z slidera*2
       int width_wat=100;
       background_rectangle_wat = scene->addRect(0, 0, width_wat, hmax_wat, outlinePen, whiteBrush);
       rectangle_wat = scene->addRect(0,200-htemp_wat,width_wat,htemp_wat,zeroPen,blueBrush);
        //wizualizacja zbiornika cisnienia
       int width_pre = 200;
       int width_pre_act = 0;
       int height_pre = 50;
       background_rectangle_pre = scene2->addRect(0,0,width_pre,height_pre,outlinePen,whiteBrush);
       rectangle_pre = scene2->addRect(0,0,width_pre_act,height_pre,zeroPen,greenBrush);

}

MainWindow::~MainWindow()
{
    delete ui;
    munmap(shmptr, SHM_SIZE);

     /* Close the shared memory object */

     ::close(shmdes);

     /* Close&Delete the Semaphore */

     sem_close(sem_des);
     sem_unlink(sem_fn);

     /* Delete the shared memory object */

     shm_unlink(shm_fn);

}
int MainWindow::InitializeIPC()
{
    qDebug()<<"Inicjalizacja";
    mode = S_IRWXU|S_IRWXG;

    /* Open the shared memory object */

    if ( (shmdes = shm_open(shm_fn,O_CREAT|O_RDWR|O_TRUNC, mode)) == -1 ) {
       perror("shm_open failure");
       return 1;
     }

  /*  Preallocate a shared memory area   */

    SHM_SIZE = sysconf(_SC_PAGESIZE);
  /* The size equal to one OS page e.g. 4kb */

    if(ftruncate(shmdes, SHM_SIZE) == -1){
      perror("ftruncate failure");
      return 1;
    }

    if((shmptr = mmap(0, SHM_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED,
                  shmdes,0)) == (void*)-1){
      perror("mmap failure");
      return 1;
    }
    /* Create a semaphore in locked state */

   sem_des = sem_open(sem_fn, O_CREAT, 0644, 0);

   if(sem_des == (void*)-1){
     perror("sem_open failure");
     return 1;
   }

  /* Access to the shared memory area */
  /* initialises the temp values - no CS (semaphore closed) */
  /* cast  the *void pointer in order to use as integer - we use only 2 places (2 sensor values)*/

  shm_ptr = (int*)shmptr;

  shm_ptr[0]=0;  /* shutdown */
  shm_ptr[1]=0;  //water level
  shm_ptr[2]=0;  //preassure level
  shm_ptr[3]=0;  //temp level



  sem_post(sem_des);/* Release the semaphore lock */
  return 0;
}

void MainWindow::updateall()
{
    if(sem_wait(sem_des)){perror("sem_op failure!!");_exit(-1);}
    water_level = shm_ptr[1];
    preassure_level = shm_ptr[2];
    temperature_level = shm_ptr[3];
    sem_post(sem_des);
    UpdateGauges(water_level,preassure_level,temperature_level);

}
void MainWindow::poweroff()
{
    shm_ptr[0]=1;
    QApplication::quit();
}

void MainWindow::UpdateGauges(int water, int preassure, int temperature)
{
    QBrush greenBrush(Qt::green);
    QBrush whiteBrush(Qt::white);
    QBrush blueBrush(Qt::blue);
    QPen zeroPen(Qt::black);
    zeroPen.setWidth(1);

    //wizualizacja zbiornika cisnienia
   int width_pre_act = preassure;
   int height_pre = 50;
   rectangle_pre->setRect(0,0,width_pre_act,height_pre);

   //wizualizacja zbiornika wody
   int htemp_wat=water*2; //ustawienie z slidera*2
   int width_wat=100;
   rectangle_wat->setRect(0,200-htemp_wat,width_wat,htemp_wat);
   if(water<30 || water>70)
       rectangle_wat->setBrush(QBrush(Qt::red));
   else if(water>30 && water<=50)
       rectangle_wat->setBrush(QBrush(Qt::yellow));
   else if(water>50 && water<=70)
       rectangle_wat->setBrush(QBrush(Qt::green));
   //wizualizacja temperatury:
   ui->lcdNumber->display(temperature);
   //electric heater logic:
   if(temperature>90)
       ui->progressBar->setValue(0);
   else if(temperature<30)
       ui->progressBar->setValue(100);
   else if(temperature>=30 && temperature <50)
       ui->progressBar->setValue(90);
   else if(temperature>=50 && temperature<70)
       ui->progressBar->setValue(80);
   else if(temperature>=70 && temperature<=100)
       ui->progressBar->setValue(75);
   //water pump logic
   if(water<50)
       ui->waterComboBox->setCurrentIndex(0);
   else if(water>70) ui->waterComboBox->setCurrentIndex(1);
    //valve logic
   if(preassure>180)
       ui->valveComboBox->setCurrentIndex(0);
   else
       ui->valveComboBox->setCurrentIndex(1);
   //alarm logic
   if(preassure>200 || water<30 || water>80)
   {
       ui->alarmComboBox->setCurrentIndex(0); //TODO PPOTW
   //security valve logic
      ui->securityValveComboBox->setCurrentIndex(0);
      //turning off electric heater
      ui->progressBar->setValue(0);
   }



}
