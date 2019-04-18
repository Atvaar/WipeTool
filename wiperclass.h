#ifndef WIPERCLASS_H
#define WIPERCLASS_H

#include <QRegularExpression>
#include <QObject>
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

class wiperClass : public QObject
{
    Q_OBJECT
public:
    explicit wiperClass(QObject *parent = nullptr, validData *thisDrive = nullptr);
    ~wiperClass();

signals:
    void statusUpdate(QString);
    void clearStatus();
    void error(QString);
    void finished(QString);
    void testFinished(bool);

public slots:
    void killProcess();
    void startDrive();

private:
    bool idDrive();//hdparm -I
    bool smartDrive1();//smartctl -a "First SMART check"
    bool rmDCO();//hdparm "remove DCO if present"
    bool rmHPA();//hdparm "remove HPA if present"
    bool initWipe();//time hdparm security-secure-erase "Begin wipe of drive" halts waiting for finish
    bool initWipe(QString);//for other wipe methods
    bool smartDrive2();//smartctl -a "Second SMART check"
    bool testStatus();//final passfail status
    int critVals = 9;
    validData *critical;
    QProcess *toolBox;
    std::fstream *file;
};

#endif // WIPERCLASS_H
