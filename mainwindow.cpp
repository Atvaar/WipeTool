#include "mainwindow.h"
#include "ui_mainwindow.h"

AutoMonitor *Milton;
QThread *workerBee;
QString myDrive;
wiperClass *driveWiper;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString zero = "/dev/disk/by-path/pci-0000:00:1a.7-usb-0:3.1:1.0-scsi-0:0:0:0";
    QString one = "/dev/disk/by-path/pci-0000:00:1a.7-usb-0:3.1:1.0-scsi-0:0:0:1";
    autoStartFlag = false;//wipe as soon as detected?
    //sudo dmidecode -s system-serial-number
    //load config file and set mahConfig
    //update
    ui->setupUi(this);
    Milton = new AutoMonitor(this);
    //qDebug("butz mi stahpler");
    //get bays from config,  replace below with thread and test object for each bay
    //*****************************************
    workerBee = new QThread(this);
    //qDebug("pardon me sir");
    driveWiper = new wiperClass();
    //qDebug("dats mi rud swinline stahpler");
    driveWiper->moveToThread(workerBee);
    //qDebug("i burn dis place");
    workerBee->start();
    //******************************************

    //QObject::connect(Milton, &AutoMonitor::tellMainDriveDetect, this, &MainWindow::getDriveInfo);
    //QObject::connect(Milton, &AutoMonitor::tellMainDriveRemoved, this, &MainWindow::leaveDriveInfo);
    QObject::connect(ui->pushButton, &QPushButton::clicked, this , &MainWindow::wipeDrive);
    QObject::connect(driveWiper,&wiperClass::clearStatus,ui->textBrowser, &QTextBrowser::clear);
    QObject::connect(driveWiper, &wiperClass::statusUpdate, ui->textBrowser, &QTextBrowser::setText);
    QObject::connect(this, &MainWindow::startWipe, driveWiper, &wiperClass::startDrive);
}

MainWindow::~MainWindow()
{
    //add disconnects
    //QObject::disconnect(Milton, &AutoMonitor::tellMainDriveDetect, this, &MainWindow::getDriveInfo);
    //QObject::disconnect(Milton, &AutoMonitor::tellMainDriveRemoved, this, &MainWindow::leaveDriveInfo);
    QObject::disconnect(ui->pushButton, &QPushButton::clicked, this , &MainWindow::wipeDrive);
    QObject::disconnect(driveWiper,&wiperClass::clearStatus,ui->textBrowser, &QTextBrowser::clear);
    QObject::disconnect(driveWiper, &wiperClass::statusUpdate, ui->textBrowser, &QTextBrowser::setText);
    QObject::disconnect(this, &MainWindow::startWipe, driveWiper, &wiperClass::startDrive);
    delete driveWiper;
    workerBee->quit();
    delete Milton;
    delete workerBee;
    //update config file with any updates.
    delete ui;
}

void MainWindow::getDriveInfo(QString drive){
    qDebug() << "Drive Detected:  " << drive;
    //udisksctl info -b /dev/sdb | grep /by-path/
    if (autoStartFlag == false){
        myDrive = drive;
        ui->pushButton->setEnabled(true);
        ui->pushButton->setText("Start Wipe on drive: " + drive + "?");
    }
    else if (autoStartFlag == true){
        //emit autostart signal and have it kick of the correct workerbee with drive name.
    }
}

void MainWindow::leaveDriveInfo(QString drive){
    qDebug() << "Drive Removed:  " << drive;
    myDrive = drive;
    ui->pushButton->setEnabled(false);
    ui->textBrowser->clear();
    ui->pushButton->setText("Start Wipe on drive: ?");
}

void MainWindow::wipeDrive(){
    qDebug() << "Begining wipe on drive: " << myDrive;
    emit startWipe("sdb");
    ui->pushButton->setEnabled(false);
}


bool MainWindow::loadConfig(){
    //open config file
    //read geometry values
    //dynamically set size and hardware desigs for mahConfig and geometry
    //read test threshholds and update
    //read update server config and update config.
    return true;
}
