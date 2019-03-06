#ifndef WIPERCLASS_H
#define WIPERCLASS_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace std;

struct validData{
    int indexNum;
    int weight;
    int value;
};

class wiperClass : public QObject
{
    Q_OBJECT
public:
    explicit wiperClass(QObject *parent = nullptr);
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
    bool idDrive();
    bool smartDrive1();
    bool initWipe();
    bool smartDrive2();
    bool testStatus();
    int critVals = 9;
    validData *critical;
    QProcess *toolBox;
    std::fstream *file;
};

#endif // WIPERCLASS_H
