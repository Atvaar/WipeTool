#include "mainwindow.h"
#include "ui_mainwindow.h"

AutoMonitor *Milton;
//QThread *workerBee;
QString myDrive;
//wiperClass *driveWiper;

MainWindow::MainWindow(int argc, char** argv, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //look for command line arguments to set url and confing locations. user and password to use

    if (argc >= 2){
        //qDebug() << "entered: " << argv[2];
        if (QString(argv[1]) == "-h"){
        qDebug("Welcome to the Echo Environmental HDD Test and Wipe Tool!");
        qDebug("Please take note of the options available");
        qDebug("    -h this help Menu");
        qDebug("    -a (0 or 1): sets auto start to false or true... default false");
        qDebug("    -n (url or ip for network location of config and health perameters)");
        qDebug("    -l (file location of local config file)");
        qDebug() << "Argument entered:  " << argc-1;//displays number of command line arguments.  #1 is program name so if >2 there is a parameter
        }
    }

    Milton = new AutoMonitor(this);
    bool didItWork = loadConfig();
    if (didItWork != true){
        qDebug("failed to load this system's config");
        //dynamic load bays on insert
    } else {
        //load static gui on config file
        //change this section to load off of the dynamic info out of loadConfig();
        QGridLayout *mainLayout;
        QString baysATA[2] = {"/dev/disk/by-path/pci-0000:00:1a.7-usb-0:3.1:1.0-scsi-0:0:0:0", "/dev/disk/by-path/pci-0000:00:1a.7-usb-0:3.2:1.0-scsi-0:0:0:0"};
        autoStartFlag = true;//wipe as soon as detected?
        bays = 2;
        sections = 1;
        mahConfig = new bayConfig[bays];
        mahConfig[0] = { 0, 1, false, baysATA[0],baysATA[0],"",new BayPanel};
        mahConfig[1] = { 1, 1, false, baysATA[1],baysATA[1],"",new BayPanel};
        //update
        ui->setupUi(this);
        mainLayout = new QGridLayout(ui->scrollArea);
        mainLayout->setHorizontalSpacing(0);
        mainLayout->addWidget(mahConfig[0].workBay,0,0);
        mainLayout->addWidget(mahConfig[1].workBay,0,1);
    }

    QObject::connect(Milton, &AutoMonitor::tellMainDriveDetect, this, &MainWindow::getDriveInfo);
    QObject::connect(Milton, &AutoMonitor::tellMainDriveRemoved, this, &MainWindow::leaveDriveInfo);
}

MainWindow::~MainWindow()
{
    //add disconnects
    QObject::disconnect(Milton, &AutoMonitor::tellMainDriveDetect, this, &MainWindow::getDriveInfo);
    QObject::disconnect(Milton, &AutoMonitor::tellMainDriveRemoved, this, &MainWindow::leaveDriveInfo);
    //delete driveWiper;
    ///workerBee->quit();
    //delete Milton;
    //delete workerBee;
    //update config file with any updates.
    delete ui;
}

void MainWindow::getDriveInfo(QString drive, QString path){
    qDebug() << "Drive Detected:  " << drive;
    qDebug() << "On Port: " << path;
    if (autoStartFlag == false){
        myDrive = drive;
        //which bay and then notify it that it has a drive and assign bay full and note which bay in mahConfig
    }
    else if (autoStartFlag == true){
        //emit autostart signal and have it kick of the correct workerbee with drive name.
        for (int y = 0; y < bays; y = y + 1){
            qDebug() << "testing bay: " << y;
            if (mahConfig[y].ataAddress == path){
                qDebug() << "Drive inserted in bay " << y;
                mahConfig[y].resident = drive;
                mahConfig[y].workBay->catchDrive(drive);
            }
        }
    }
}

void MainWindow::leaveDriveInfo(QString drive){
    qDebug() << "Drive Removed:  " << drive;
    //myDrive = drive;
    for (int y = 0; y < bays; y = y + 1){
        qDebug() << "testing bay: " << y;
        if (mahConfig[y].resident == drive){
            qDebug() << "Drive removed from bay " << y;
            mahConfig[y].resident = drive;
            mahConfig[y].workBay->takeDrive(drive);
        }
    }
    //which drive from mahConfig to remove and tell bay drive pulled
}


//degraded -- was for the old single wipe
void MainWindow::wipeDrive(){
    qDebug() << "Begining wipe on drive: " << myDrive;
    emit startWipe("sdb");
    //ui->pushButton->setEnabled(false);
}


bool MainWindow::loadConfig(){
    //first get info for the system
    QProcess sysInfo;
    sysInfo.start("sudo dmidecode -s system-serial-number");
    sysInfo.waitForFinished(10000);
    sysSerial = sysInfo.readAllStandardOutput();
    sysSerial = sysSerial.mid(0,sysSerial.length()-1);
    qDebug() << "System: " << sysSerial;
    sysInfo.start("sudo dmidecode -s system-manufacturer");
    sysInfo.waitForFinished(10000);
    sysManu = sysInfo.readAllStandardOutput();
    sysManu = sysManu.mid(0,sysManu.length()-1);
    qDebug() << "manufacturer: " << sysManu;
    sysInfo.start("sudo dmidecode -s system-product-name");
    sysInfo.waitForFinished(10000);
    sysModel = sysInfo.readAllStandardOutput();
    sysModel = sysModel.mid(0, sysModel.length()-1);
    qDebug() << "Model: " << sysModel;
    //open config file
    //read geometry values
    //dynamically set size and hardware desigs for mahConfig and geometry
    //read test threshholds and update
    //read update server config and update config.
    return true;
}
