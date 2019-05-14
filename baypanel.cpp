#include "baypanel.h"
#include "ui_baypanel.h"

BayPanel::BayPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BayPanel)
{
    ui->setupUi(this);
    //thisWiper.moveToThread(&wiperThread);
    //ui->progressBar->setValue(0);
    //ui->label->setText("Empty");
    //ui->label->setEnabled(false);
    //set ui. style sheet
    //ui->pushButton->setText("Empty");
    //ui->pushButton->setEnabled(false);

    //set part in order
    wiperThread = new QThread;
    thisWiper = new wiperClass;
    thisWiper->moveToThread(wiperThread);
    QObject::connect(this, &BayPanel::sendUIUpdate, this, &BayPanel::updateUI);
    //QObject::connect(Milton, &AutoMonitor::tellMainDriveDetect, this, &MainWindow::getDriveInfo);
    emit sendUIUpdate(0, "Empty", 4, false);
}

BayPanel::~BayPanel()
{
    delete thisWiper;
    delete wiperThread;
    delete ui;
}

void BayPanel::setThreshHolds(validData steve[]){

}

void BayPanel::catchDrive(QString drive){
    ui->label->setEnabled(true);
    //get drive info
    //update label with serial number
    activeDrive = drive;
    getDriveInfo();
    emit updateUI(0, driveSerial, 0, true);
    //if autostart enabled then emit start wipe
}

void BayPanel::takeDrive(QString drive){
    //kill drive processes
    emit sendUIUpdate(0, "Empty", 4, false);
}

void BayPanel::getDriveInfo(){
    int snipStart;
    int snipEnd;
    QString getMe = QString("sudo hdparm -I ") + activeDrive;
    //qDebug() << getMe;
    QProcess findSlot;
    findSlot.start(getMe);
    findSlot.waitForFinished(10000);
    getMe = findSlot.readAllStandardOutput();
    snipStart = getMe.indexOf("Serial Number:      ");
    snipEnd = getMe.indexOf("\n",snipStart,Qt::CaseInsensitive) - snipStart;
    driveSerial = getMe.mid(snipStart, snipEnd);
    qDebug() << "Serial Number:      "<< driveSerial;

    //now get drive model
    snipStart = getMe.indexOf("Model Number:       ");
    snipEnd = getMe.indexOf("\n",snipStart,Qt::CaseInsensitive) - snipStart;
    driveModel = getMe.mid(snipStart, snipEnd);
    qDebug() << "Model Number:       " << driveModel;

    //now get drive size
    snipStart = getMe.indexOf("device size with M = 1000*1000:");
    snipEnd = getMe.indexOf("\n",snipStart,Qt::CaseInsensitive) - snipStart;
    driveSize = getMe.mid(snipStart, snipEnd);
    qDebug() << "Drive Size:         " << driveSize;

    //now get FW

    snipStart = getMe.indexOf("Firmware Revision:  ");
    snipEnd = getMe.indexOf("\n",snipStart,Qt::CaseInsensitive) - snipStart;
    driveFW = getMe.mid(snipStart, snipEnd);
    qDebug() << "Firmware Revision:  " << driveFW;
}

void BayPanel::updateUI(int percent, QString labelMessage,int messageColor, bool buttonEnabled){
   ui->progressBar->setValue(percent);
   ui->label->setText(labelMessage);
   switch (messageColor){
        case 0://set netrual blue detected not running

            ui->label->setStyleSheet("QLabel { background-color : blue; color : white; }");
            ui->pushButton->setText("Test");
            ui->tab_2->setEnabled(false);//change to true when tab_2 has function
            ui->tab_2->setVisible(true);
           break;
        case 1://set yellow inprogress
            ui->label->setStyleSheet("QLabel { background-color : yellow; color : black; }");
            ui->pushButton->setText("Running...");
            ui->tab_2->setEnabled(false);
            ui->tab_2->setVisible(false);
           break;
        case 2: //set green finished passed
            ui->label->setStyleSheet("QLabel { background-color : green; color : white; }");
            ui->pushButton->setText("Retest?");
            ui->tab_2->setEnabled(false);
            ui->tab_2->setVisible(true);
           break;
        case 3: //set red finish failed
            ui->label->setStyleSheet("QLabel { background-color : red; color : yellow; }");
            ui->pushButton->setText("Retest?");
            ui->tab_2->setEnabled(false);
            ui->tab_2->setVisible(true);
           break;
        case 4: //set gray empty status
            ui->label->setStyleSheet("QLabel { background-color : grey; color : white; }");
            ui->pushButton->setText("Empty");
            ui->tab_2->setEnabled(false);
            ui->tab_2->setVisible(false);
           break;
   }
   ui->progressBar->setValue(percent);
   ui->label->setText(labelMessage);
   ui->pushButton->setEnabled(buttonEnabled);
   ui->tab_2->setEnabled(false);//remove later when tab_2 has function
}
