#include "baypanel.h"
#include "ui_baypanel.h"

BayPanel::BayPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BayPanel)
{
    ui->setupUi(this);
    //thisWiper.moveToThread(&wiperThread);
    ui->progressBar->setValue(0);
    ui->label->setText("Empty");
    //set ui. style sheet
    ui->pushButton->setText("Empty");
    ui->pushButton->setEnabled(false);

    //set part in order
    wiperThread = new QThread;
    thisWiper = new wiperClass;
    thisWiper->moveToThread(wiperThread);
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
}

//void updateUI(int percent, QString labelMessage,int messageColor, bool buttonEnabled){
//   ui->progressBar->setValue(percent);
//   ui->label->setText(labelMessage);
//   switch (messageColor){
//        case 0://set netrual blue
//           break;
//        case 1://set yellow
//           break;
//        case 2: //set green
//           break;
//        case 3: //set red
//           break;
//   }
//   ui->pushButton->setEnabled(buttonEnabled);
//}
