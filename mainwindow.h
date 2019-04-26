#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QAction>
#include <QDebug>
#include <QThread>
#include "automonitor.h"
#include "wiperclass.h"
#include "baypanel.h"
#include <QGridLayout>
#include <QLayout>

namespace Ui {

class MainWindow;
}

struct guiGeometry //item for each grouping of bays... like server and pods.
{
    int section;
    int rows;
    int columns;
};

struct bayConfig //structure for instanciating the hdd bay configuration
{
    int bayNum;
    int section;
    bool occupiedFlag;
    QString ataAddress;
    QString scsiAddress;
    QString resident;
    BayPanel *workBay;
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void getDriveInfo(QString drive, QString path);
    void leaveDriveInfo(QString);
    void wipeDrive();

signals:
    void startWipe(QString);
    void autoStart(int);

private:
    Ui::MainWindow *ui;
    bool autoStartFlag;
    bool loadConfig();
    int bays;
    int sections;
    guiGeometry *mahGeometry;
    bayConfig *mahConfig;
    QString sysSerial;
    QString sysManu;
    QString sysModel;
};

#endif // MAINWINDOW_H
