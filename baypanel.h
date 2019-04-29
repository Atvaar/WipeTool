#ifndef BAYPANEL_H
#define BAYPANEL_H

#include <QWidget>
#include <QThread>
#include <QString>
#include <QProcess>
#include "wiperclass.h"

namespace Ui {
class BayPanel;
}

class BayPanel : public QWidget
{
    Q_OBJECT

public:
    explicit BayPanel(QWidget *parent = 0);
    ~BayPanel();
    //void setID(QString[], int, QString);//String array for hardware location, int bay number, QString for other orientation info

signals:
    void sendReport(QString);
public slots:
    void setThreshHolds(validData[]);//so that the pass fail criteria can be changed on the fly
    void catchDrive(QString);//if drive detect QString drive letter, QString drive address... is it mine
    //void updateUI(int, QString, int,bool);

private:
    Ui::BayPanel *ui;
    void updateThreshHolds(validData[]);
    QThread *wiperThread;
    wiperClass *thisWiper;
    void writeLog();
};

#endif // BAYPANEL_H
