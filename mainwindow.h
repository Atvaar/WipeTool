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
};

#endif // MAINWINDOW_H
