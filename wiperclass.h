#ifndef WIPERCLASS_H
#define WIPERCLASS_H

#include <QObject>
#include <QWidget>
#include <QAction>
#include <QString>
#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace std;

struct validData{
    int indexNum;
    int smartNum;
    int weight;
    int value;
};

struct TestBundle{
    QString TestName;
    int TestStatus;//0 = not completed, 1 = passed, 2 = failed
};

struct driveData{
    QString driveSerial;
    QString driveModel;
    QString driveFW;
    QString driveSize;
    QString TestDate;
    TestBundle *driveTest;
    QString DriveStatus;
};

class wiperClass : public QObject
{
    Q_OBJECT
public:
    explicit wiperClass(QObject *parent = nullptr, validData *thisDrive = nullptr);
    ~wiperClass();
    void recieveDrive(QString drive);
    void removeDrive(QString drive);

signals:
    void statusUpdate(int, QString, int, bool);
    void clearStatus();
    void error(QString);
    void finished(QString);
    void testFinished(bool);
    void recieved();
    void removed();

public slots:
    void killProcess();
    void startDrive();

private:
    bool smartDrive1();//smartctl -a "First SMART check"
    bool rmDCO();//hdparm "remove DCO if present"
    bool rmHPA();//hdparm "remove HPA if present"
    bool initWipe();//time hdparm security-secure-erase "Begin wipe of drive" halts waiting for finish
    bool initWipe(QString);//for other wipe methods
    bool smartDrive2();//smartctl -a "Second SMART check"
    bool testStatus();//final passfail status
    void idDrive();//hdparm -I
    int critVals = 9;
    bool autoStart;
    QString whichDrive;
    validData *critical;
    driveData thisDrive;
    QProcess *toolBox;
    std::fstream *file;
};

#endif // WIPERCLASS_H
