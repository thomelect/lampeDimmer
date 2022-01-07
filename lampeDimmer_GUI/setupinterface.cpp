#include "setupinterface.h"
#include "ui_setupinterface.h"

setupinterface::setupinterface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setupinterface)
{
    ui->setupUi(this);
}

setupinterface::~setupinterface()
{
    delete ui;
}
