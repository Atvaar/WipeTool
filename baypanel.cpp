#include "baypanel.h"
#include "ui_baypanel.h"

BayPanel::BayPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BayPanel)
{
    ui->setupUi(this);
}

BayPanel::~BayPanel()
{
    delete ui;
}
