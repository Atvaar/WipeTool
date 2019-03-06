#ifndef AUTOMONITOR_H
#define AUTOMONITOR_H

#include <QRegularExpression>
#include <QProcess>
#include <QObject>
#include <QString>
#include <QDebug>
#include <regex>
#include <iostream>
#include <stdio.h>


//need to add apperatus for loading the number and designation of hdd bays
class AutoMonitor : public QObject
{
    Q_OBJECT
public:
    explicit AutoMonitor(QObject *parent = nullptr);
    ~AutoMonitor();

signals:
    void tellMainDriveDetect(QString drive);
    void tellMainDriveRemoved(QString drive);

public slots:
    void DriveChange();

private:
    QProcess *monitor;
    void GehtSie();
    void HaltSie();
    QString ChangeDecipher();
    bool driveStatFlag;
};

#endif // AUTOMONITOR_H
