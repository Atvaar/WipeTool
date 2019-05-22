#include "baypanel.h"
#include "ui_baypanel.h"

BayPanel::BayPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BayPanel)
{
    ui->setupUi(this);

    //set part in order
    wiperThread = new QThread;
    thisWiper = new wiperClass;
    thisWiper->moveToThread(wiperThread);
    //wiperThread->start();
    QObject::connect(this, &BayPanel::sendWiperDrive, thisWiper, &wiperClass::recieveDrive);
    QObject::connect(this, &BayPanel::takeWiperDrive, thisWiper, &wiperClass::removeDrive);
    QObject::connect(thisWiper, &wiperClass::statusUpdate, this, &BayPanel::updateUI);
    QObject::connect(this->ui->pushButton, &QPushButton::clicked, thisWiper, &wiperClass::startDrive);
    wiperThread->start();
    //updateUI(0, "Empty", 4, false);
}

BayPanel::~BayPanel()
{
    QObject::disconnect(this, &BayPanel::sendWiperDrive, thisWiper, &wiperClass::recieveDrive);
    QObject::disconnect(this, &BayPanel::takeWiperDrive, thisWiper, &wiperClass::removeDrive);
    QObject::disconnect(thisWiper, &wiperClass::statusUpdate, this, &BayPanel::updateUI);
    QObject::disconnect(this->ui->pushButton, &QPushButton::clicked, thisWiper, &wiperClass::startDrive);
    delete thisWiper;
    wiperThread->quit();
    wiperThread->wait();
    delete wiperThread;
    delete ui;
}

void BayPanel::setThreshHolds(validData steve[]){

}

void BayPanel::catchDrive(QString drive){
    //thisWiper->recieveDrive(drive);
    //activeDrive = drive;
    qDebug()<< drive;
    emit sendWiperDrive(drive);

    //if autostart enabled then emit start wipe
}

void BayPanel::takeDrive(QString drive){
    emit takeWiperDrive(drive);
    activeDrive = "";
    //kill drive processes
    //emit takeWiperDrive(drive);
}

void BayPanel::updateUI(int percent, QString labelMessage,int messageColor, bool buttonEnabled){
   ui->progressBar->setValue(percent);
   ui->label->setText(labelMessage);
   switch (messageColor){
        case 0://set netrual blue detected not running
            ui->label->setStyleSheet("QLabel { background-color : blue; color : white; }");
            ui->pushButton->setText("Test");
            ui->pushButton->setEnabled(true);
            ui->tab_2->setEnabled(false);//change to true when tab_2 has function
            ui->tab_2->setVisible(true);
           break;
        case 1://set yellow inprogress
            ui->label->setStyleSheet("QLabel { background-color : yellow; color : black; }");
            ui->pushButton->setText("Running...");
            ui->pushButton->setEnabled(false);
            ui->tab_2->setEnabled(false);
            ui->tab_2->setVisible(false);
           break;
        case 2: //set green finished passed
            ui->label->setStyleSheet("QLabel { background-color : green; color : white; }");
            ui->pushButton->setText("Retest?");
            ui->pushButton->setEnabled(true);
            ui->tab_2->setEnabled(false);
            ui->tab_2->setVisible(true);
           break;
        case 3: //set red finish failed
            ui->label->setStyleSheet("QLabel { background-color : red; color : yellow; }");
            ui->pushButton->setText("Retest?");
            ui->pushButton->setEnabled(true);
            ui->tab_2->setEnabled(false);
            ui->tab_2->setVisible(true);
           break;
        case 4: //set gray empty status
            ui->label->setStyleSheet("QLabel { background-color : grey; color : white; }");
            ui->pushButton->setText("Empty");
            ui->pushButton->setEnabled(false);
            ui->tab_2->setEnabled(false);
            ui->tab_2->setVisible(false);
           break;
   }
   ui->progressBar->setValue(percent);
   ui->label->setText(labelMessage);
   ui->pushButton->setEnabled(buttonEnabled);
   ui->tab_2->setEnabled(false);//remove later when tab_2 has function
}
