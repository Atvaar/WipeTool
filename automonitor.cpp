#include "automonitor.h"
#include <QProcess>
#include <stdio.h>
#include <iostream>
#include <QString>
#include <QDebug>

AutoMonitor::AutoMonitor(QObject *parent) : QObject(parent)
{
    driveStatFlag = false;
    monitor = new QProcess;
    //monitor->setProgram("udisksctl");
    //monitor->setArguments(QStringList() << "monitor");
    QObject::connect(monitor,&QProcess::readyReadStandardOutput, this, &AutoMonitor::DriveChange);
    GehtSie();
}

AutoMonitor::~AutoMonitor(){
    HaltSie();
    monitor->close();
    delete monitor;
}

void AutoMonitor::GehtSie(){
    //qDebug() << "starting";
    //monitor->startDetached("udisksctl", QStringList("monitor"));
    monitor->start("udisksctl", QStringList("monitor"));
}

void AutoMonitor::HaltSie(){
    //qDebug() << "stopping";
    monitor->kill();
}

//Signals
void tellMainDriveDetect(QString drive);
void tellMainDriveRemoved(QString drive);

//Slots
void AutoMonitor::DriveChange(){
    QString driveDesig;
    driveDesig = "sdb";
    QString Data = monitor->readAllStandardOutput();

    //is the drive inserted or removed
    if (Data.contains("Added") and driveStatFlag==false and !Data.contains("jobs")){
        driveStatFlag = true;
        //find drive designation
        //find hardware address
        emit tellMainDriveDetect(driveDesig);
    }
    else if (Data.contains("Removed") and driveStatFlag==true and !Data.contains("jobs")){
        driveStatFlag = false;
        emit tellMainDriveRemoved(driveDesig);
    }
    //else qDebug() << "OREO!";
    //is this a drive or a partition
    //if inserted and a drive emit tellMainDriveDetct(QString drive)
    //if removed and a drive emit tellMainDriveRemoved(QString drive)
    //qDebug() << "emitted yarp!";
}
