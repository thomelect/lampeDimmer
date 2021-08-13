/*
 * Fichier: mainwindow.cpp
 * Auteur: Thomas Desrosiers
 * Date: 2021 03 23
 * Desc.: Laboratoire #5 du cours d'intégration de systèmes.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "setupserialdialog.h"
#include <QDebug>
#include <QMenuBar>

using namespace std;

uint8_t intensite = 0;
bool boutonState = 1;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    connect(serial, SIGNAL(readyRead()), this, SLOT(readSerialData()));
    createMenus();
}

MainWindow::~MainWindow()
{
    if(serial->isOpen())
        serial->close();
    delete serial;
    delete ui;
}

void MainWindow::createMenus()
{
    setupSerialAct = new QAction(tr("&Configuration du port série"), this);
    connect(setupSerialAct, &QAction::triggered, this, &MainWindow::setupSerial);
    toolsMenu = menuBar()->addMenu(tr("&Outils"));
    toolsMenu->addAction(setupSerialAct);
}

void MainWindow::setupSerial()
{
    SetupSerialDialog setupDia(serial);
    setupDia.setWindowTitle("Configuration du port série");
    setupDia.setWindowFlags( Qt::WindowSystemMenuHint); // Pour retirer le ?
    setupDia.setModal(1);
    setupDia.exec();
}

/**
* @brief Fonction de lecture du port série..
*/
void MainWindow::readSerialData()
{
    QByteArray tmpRx;

    if(serial->bytesAvailable())
    {
        tmpRx.resize(serial->bytesAvailable());
        serial->read(tmpRx.data(), tmpRx.size());
        foreach (uint8_t data, tmpRx)
        {
            qDebug() << data;
        }
    }
}

/**
* @brief Fonction d'e lecture du'envoie sur le port série..
*/
void MainWindow::sendSerialData()
{
    char txData[3];
    txData[0] = '<';
    txData[1] = intensite;
    txData[2] = '>';
    serial->write(txData, 3);
}

void MainWindow::on_horizontalSliderIntensite_valueChanged(void)
{
    intensite = ui->horizontalSliderIntensite->value(); //On récupère la valeur du slider.
    ui->lbIntensiteValue->setText(QString::number(intensite)); //La valeur du slider est affichée dans le label sous le slider.
    ui->dialIntensite->setSliderPosition(intensite); //Modifie la position du slider en fonction de la valeur obtenue par le slider.
    boutonManage(intensite);
    sendSerialData();  //Appel de la fonction sendData();
    qDebug() << intensite;
}

void MainWindow::on_dialIntensite_valueChanged(void)
{
    intensite = ui->dialIntensite->value();
    ui->lbIntensiteValue->setText(QString::number(intensite));
    ui->horizontalSliderIntensite->setSliderPosition(intensite); //Modifie la position du slider en fonction de la valeur obtenue par le dial.
    boutonManage(intensite);
    sendSerialData();
    qDebug() << intensite;
}

void MainWindow::on_pushBottonOnOff_clicked()
{
    //boutonState = !boutonState;
    if (boutonState) //Met la lumière à "ON" et le bouton affiche maintenant "OFF".
    {
        boutonState = !boutonState;
        ui->pushBottonOnOff->setText("OFF");
        intensite = 255;
        ui->lbIntensiteValue->setText(QString::number(intensite));
    }
    else //Met la lumière à "OFF" et le bouton affiche maintenant "ON".
    {
        boutonState = !boutonState;
        ui->pushBottonOnOff->setText("ON");
        intensite = 0;
        ui->lbIntensiteValue->setText(QString::number(intensite));
    }
    ui->horizontalSliderIntensite->setSliderPosition(intensite); //Modifie la position du slider.
    ui->dialIntensite->setSliderPosition(intensite); //Modifie la position du dial.
    sendSerialData();
}

void MainWindow::boutonManage(int value)
{
    if (value)
    {
        ui->pushBottonOnOff->setText("OFF");
        boutonState = 0;
    }
    else
    {
        ui->pushBottonOnOff->setText("ON");
        boutonState = 1;
    }
}
