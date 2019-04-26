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
