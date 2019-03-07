#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegularExpression>
#include <QObject>
#include <QAction>
#include <QDebug>
#include <QThread>
#include "automonitor.h"
#include "wiperclass.h"

namespace Ui {

class MainWindow;
}
struct bayConfig //structure for instanciating the hdd bay configuration
{
    int bayNum;
    bool occupiedFlag;
    QString ataAddress;
    QString scsiAddress;
    QString resident;
    //bayConfig() {}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void getDriveInfo(QString);
    void leaveDriveInfo(QString);
    void wipeDrive();

signals:
    void startWipe(QString);

private:
    Ui::MainWindow *ui;
    bool autoStartFlag;
    bool loadConfig();
    bayConfig *mahConfig;
};

#endif // MAINWINDOW_H
