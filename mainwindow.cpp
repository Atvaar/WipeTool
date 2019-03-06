#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "automonitor.h"
#include "wiperclass.h"
#include <QObject>
#include <QAction>
#include <QDebug>
#include <QThread>

AutoMonitor *Milton;
QThread *workerBee;
QString myDrive;
wiperClass *driveWiper;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Milton = new AutoMonitor(this);
    //qDebug("swingline");
    workerBee = new QThread(this);
    //qDebug("pardon me sir");
    driveWiper = new wiperClass();
    //qDebug("dats my red swingline stapler");
    driveWiper->moveToThread(workerBee);
    //qDebug("i burn dis place");
    workerBee->start();

    QObject::connect(Milton, &AutoMonitor::tellMainDriveDetect, this, &MainWindow::getDriveInfo);
    QObject::connect(Milton, &AutoMonitor::tellMainDriveRemoved, this, &MainWindow::leaveDriveInfo);
    QObject::connect(ui->pushButton, &QPushButton::clicked, this , &MainWindow::wipeDrive);
    QObject::connect(driveWiper,&wiperClass::clearStatus,ui->textBrowser, &QTextBrowser::clear);
    QObject::connect(driveWiper, &wiperClass::statusUpdate, ui->textBrowser, &QTextBrowser::setText);
    QObject::connect(this, &MainWindow::startWipe, driveWiper, &wiperClass::startDrive);
}

MainWindow::~MainWindow()
{

    delete driveWiper;
    workerBee->quit();
    delete Milton;
    delete workerBee;
    delete ui;
}

void MainWindow::getDriveInfo(QString drive){
    qDebug() << "Drive Detected:  " << drive;
    myDrive = drive;
    ui->pushButton->setEnabled(true);
    ui->pushButton->setText("Start Wipe on drive: " + drive + "?");
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
