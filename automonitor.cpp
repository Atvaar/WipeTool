#include "automonitor.h"

AutoMonitor::AutoMonitor(QObject *parent) : QObject(parent)
{
    //driveStatFlag = false;
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
    qDebug() << "starting";
    //monitor->startDetached("udisksctl", QStringList("monitor"));
    monitor->start("udisksctl", QStringList("monitor"));
}

void AutoMonitor::HaltSie(){
    qDebug() << "stopping";
    monitor->kill();
}

//Signals
//void tellMainDriveDetect(QString drive);
//void tellMainDriveRemoved(QString drive);

//Slots
void AutoMonitor::DriveChange(){
    QString driveDesig = "/dev/sdb";
    QString driveSlot;
    QString Data = monitor->readAllStandardOutput();

    //is the drive inserted or removed
    if (Data.contains("Added /org/freedesktop/UDisks2/block_devices/") /*and driveStatFlag==false */and !Data.contains("jobs")){
        //qDebug() << "drive added \n";
        int snipStart = Data.indexOf("block_devices") + 14;
        int snipEnd = Data.indexOf("\n") - snipStart;
        QString snipIt = Data.mid(snipStart,snipEnd);
        driveDesig = "/dev/" + snipIt;
        //qDebug() << snipIt;

        //QProcess udisksctl info -b "driveDesig"
        QString getMe = QString("udisksctl info -b ") + driveDesig;
        //qDebug() << getMe;
        QProcess findSlot;
        findSlot.start(getMe);
        findSlot.waitForFinished(10000);
        getMe = findSlot.readAllStandardOutput();
        snipStart = getMe.indexOf("/dev/disk/by-path/");
        snipEnd = getMe.indexOf("\n",snipStart,Qt::CaseInsensitive) - snipStart;
        driveSlot = getMe.mid(snipStart, snipEnd);
        //qDebug() << snipStart;
        //qDebug() << snipEnd;
        //qDebug() << "drive in slot: " << driveSlot;

        emit tellMainDriveDetect(driveDesig, driveSlot);
    }
    else if (Data.contains("Removed /org/freedesktop/UDisks2/block_devices/") /*and driveStatFlag==true */and !Data.contains("jobs")){
        //qDebug() << "drive removed";
        int snipStart = Data.indexOf("block_devices") + 14;
        int snipEnd = Data.indexOf("\n") - snipStart;
        QString snipIt = Data.mid(snipStart,snipEnd);
        //qDebug() << snipIt;
        driveDesig = "/dev/" + snipIt;
        emit tellMainDriveRemoved(driveDesig);
    }
}
