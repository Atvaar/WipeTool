#ifndef AUTOMONITOR_H
#define AUTOMONITOR_H

#include <QRegularExpression>
//#include <QRegExp>
#include <QProcess>
#include <QObject>
#include <QString>
#include <QDebug>
#include <iostream>
#include <stdio.h>
//#include <regex>

//need to add apperatus for loading the number and designation of hdd bays
class AutoMonitor : public QObject
{
    Q_OBJECT
public:
    explicit AutoMonitor(QObject *parent = nullptr);
    ~AutoMonitor();

signals:
    void tellMainDriveDetect(QString drive);
    void tellMainWhichDriveDetect(QString);
    void tellMainDriveRemoved(QString drive);

public slots:
    void DriveChange();

private:
    QProcess *monitor;
    void GehtSie();
    void HaltSie();
    QString ChangeDecipher();
    //bool driveStatFlag;
};

#endif // AUTOMONITOR_H
