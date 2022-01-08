/*
 * Fichier: mainwindow.cpp
 * Auteur: Thomas Desrosiers
 * Date: 2021 03 23
 * Desc.: Laboratoire #5 du cours d'intégration de systèmes.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "setupserialdialog.h"
#include <QTimer>
#include <QDebug>
#include <QMenuBar>
#include <QString>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    /*
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendGetState()));
    timer->start(100); // vous pouvez réduire l'interval durant les tests.
    */

    intensite = 0;
    serialRxIn = false;
    boutonState = true;

    ui->setupUi(this);
    serial = new QSerialPort(this);
    connect(serial, SIGNAL(readyRead()), this, SLOT(readSerialData()));
    createMenus();
    ui->pushBottonOnOff->setIcon(QIcon(":/images/off.png"));
    ui->pushBottonOnOff->setIconSize(QSize(65, 65));

    //Feuille de style des boutons de la de l'interface MainWindow.
    this->setStyleSheet("#pushBottonOnOff {"
                        "background-color: none;"
                        "border: 0px"
                        "}");
}

MainWindow::~MainWindow()
{
    if (serial->isOpen())
        serial->close();
    delete serial;
    delete ui;
}

void MainWindow::boutonManage(int value)
{
    ui->lbIntensiteValue->setText(QString::number(intensite)); //La valeur du slider est affichée dans le label sous le slider.
    ui->dialIntensite->setSliderPosition(intensite);           //Modifie la position du slider en fonction de la valeur obtenue par le slider.
    ui->horizontalSliderIntensite->setSliderPosition(intensite);
    ui->statusBar->showMessage(QString::number((intensite / 2.55), 'f', 0) + '%');
    qDebug() << "SET_VAL : " + QString::number(intensite);

    QPixmap pixmapOff("/images/off.png");
    QIcon ButtonIcon(pixmapOff);
    if (value)
    {
        ui->pushBottonOnOff->setIcon(QIcon(":/images/on.png"));
        ui->pushBottonOnOff->setIconSize(QSize(65, 65));
        boutonState = 0;
    }
    else
    {
        ui->pushBottonOnOff->setIcon(QIcon(":/images/off.png"));
        ui->pushBottonOnOff->setIconSize(QSize(65, 65));
        boutonState = 1;
    }
}

void MainWindow::createMenus(void)
{
    setupSerialAct = new QAction(tr("&Configuration du port série"), this);
    connect(setupSerialAct, &QAction::triggered, this, &MainWindow::setupSerial);
    toolsMenu = menuBar()->addMenu(tr("&Outils"));
    toolsMenu->addAction(setupSerialAct);
}

/**
 * @brief Traitement et exécution d'une commande reçue
 */
void MainWindow::execRxCommand(void)
{
    switch (rxCommande)
    {
    case VAL_ACTU:
        /*// ACQUISITION //*/
        valueOut = rxData[0];

        /*// DEBUG //*/
        qDebug() << "VAL_ACTU : " + QString::number(valueOut);
        break;
    case VAL_INIT:
        /*// ACQUISITION //*/
        valueOut = rxData[0];
        valueVeilleMode = rxData[1];

        /*// DEBUG //*/
        qDebug() << "VAL_ACTU : " + QString::number(valueOut);
        qDebug() << "SLEEP_MODE : " + ui->comboBoxSleep->currentText();

        /*// CHANGEMENTS GUI //*/
        ui->horizontalSliderIntensite->setSliderPosition(valueAdc); //Modifie la position du slider en fonction de la valeur obtenue par le dial.
        ui->dialIntensite->setSliderPosition(valueAdc);             //Modifie la position du slider en fonction de la valeur obtenue par le slider.
        ui->lbIntensiteValue->setText(QString::number(valueAdc));
        ui->comboBoxSleep->setCurrentIndex(valueVeilleMode);

        /*// FLAG DE RÉCEPTION //*/
        serialRxIn = false;
        break;
    case VAL_POT:
        /*// ACQUISITION //*/
        valueAdc = rxData[0];

        /*// DEBUG //*/
        qDebug() << "VAL_POT : " + QString::number(valueAdc);

        /*// CHANGEMENTS GUI //*/
        ui->horizontalSliderIntensite->setSliderPosition(valueAdc); //Modifie la position du slider en fonction de la valeur obtenue par le dial.
        ui->dialIntensite->setSliderPosition(valueAdc);             //Modifie la position du slider en fonction de la valeur obtenue par le slider.
        ui->lbIntensiteValue->setText(QString::number(valueAdc));

        /*// FLAG DE RÉCEPTION //*/
        serialRxIn = false;
        break;
    }
}

/**
 * @brief Sépare les données reçues sur le port série.
 * @param data la donnée à traiter
 */
void MainWindow::parseRXData(uint8_t data)
{
    switch (rxState)
    {
    default:
        if (data == '<')
        {
            rxState = RXSIZE;
            rxDataCnt = 0;
        }
        break;
    case RXSIZE:
        rxDataSize = data;
        if (rxDataSize >= _MAX_RXDATASIZE_)
        {
            qDebug() << " \n\r Erreur de trame : Size trop grand. \n\r";
            rxState = WAIT;
        }
        else
        {
            rxState = RXCOMMANDE;
        }
        break;
    case RXCOMMANDE:
        rxCommande = (RX_COMMANDES)data;
        if (rxDataSize)
        {
            rxState = RXDATA;
        }
        else
            rxState = VALIDATE;
        break;
    case RXDATA:
        rxData[rxDataCnt++] = data;
        qDebug() << " ----------------- RXDATA : " + QString::number(data);
        if (rxDataCnt == rxDataSize)
            rxState = VALIDATE;
        break;
    case VALIDATE:
        rxState = WAIT;
        if (data == '>')
        {
            serialRxIn = true;
            execRxCommand();
        }
        else
        {
            rxErrorCommCnt++;
            qDebug() << " \n\r Error RX TRAME Error = " << rxErrorCommCnt << "   "
                     << " \n\r";
        }
        break;
    }
}

/**
* @brief Fonction de lecture du port série..
*/
void MainWindow::readSerialData(void)
{
    QByteArray tmpRx;
    if (serial->bytesAvailable())
    {
        tmpRx.resize(serial->bytesAvailable());
        serial->read(tmpRx.data(), tmpRx.size());
        for (uint16_t i = 0; i < tmpRx.size(); i++)
            parseRXData(tmpRx[i]);
    }
}

/**
* @brief Fonction de lecture du'envoie sur le port série..
*/
void MainWindow::sendSerialData(/*uint8_t cmd, uint8_t data*/)
{
    if (serial->isOpen())
    {
        char txData[5];
        switch (txCommande)
        {
        case GET_VAL_ACTU:
            txData[0] = '<';
            txData[1] = 0;
            txData[2] = GET_VAL_ACTU;
            txData[3] = '>';
            serial->write(txData, 4);
            break;
        case GET_VAL_INIT:
            txData[0] = '<';
            txData[1] = 0;
            txData[2] = GET_VAL_INIT;
            txData[3] = '>';
            serial->write(txData, 4);
            break;
        case GET_VAL_POT:
            txData[0] = '<';
            txData[1] = 0;
            txData[2] = GET_VAL_POT;
            txData[3] = '>';
            serial->write(txData, 4);
            break;
        case SET_SLEEP_MODE:
            txData[0] = '<';
            txData[1] = 1;
            txData[2] = SET_SLEEP_MODE;
            txData[3] = valueVeilleMode;
            txData[4] = '>';
            serial->write(txData, 5);
            break;
        case SET_VAL:
            char txData1[5];
            txData1[0] = '<';
            txData1[1] = 1;
            txData1[2] = SET_VAL;
            txData1[3] = intensite;
            txData1[4] = '>';
            serial->write(txData1, 5);
            break;
        }
    }
}

void MainWindow::setupSerial(void)
{
    SetupSerialDialog setupDia(serial);
    setupDia.setWindowTitle("Configuration du port série");
    setupDia.setWindowFlags(Qt::WindowSystemMenuHint); // Pour retirer le ?
    setupDia.setModal(1);
    setupDia.exec();
    if (serial->isOpen())
    {
        qDebug() << "GET_VAL_INIT";
        txCommande = GET_VAL_INIT;
        sendSerialData();
    }
}

void MainWindow::on_comboBoxSleep_activated(int index)
{
    valueVeilleMode = index;
    txCommande = SET_SLEEP_MODE;
    sendSerialData();
}

void MainWindow::on_dialIntensite_valueChanged(void)
{
    if (!ui->pushBottonOnOff->isChecked())
    {
        intensite = ui->dialIntensite->value();
        boutonManage(intensite);
        if (!serialRxIn)
        {
            txCommande = SET_VAL;
            sendSerialData();
        }
    }
}

void MainWindow::on_horizontalSliderIntensite_valueChanged(void)
{
    if (!ui->pushBottonOnOff->isChecked())
    {
        intensite = ui->horizontalSliderIntensite->value(); //On récupère la valeur du slider.
        boutonManage(intensite);
        if (!serialRxIn)
        {
            txCommande = SET_VAL;
            sendSerialData();
        }
    }
}

void MainWindow::on_pushBottonOnOff_pressed()
{
    if (boutonState) //Met la lumière à "ON" et le bouton affiche maintenant "OFF".
    {
        txCommande = GET_VAL_POT;
        sendSerialData();
        boutonState = !boutonState;
        intensite = valueAdc;
    }
    else //Met la lumière à "OFF" et le bouton affiche maintenant "ON".
    {
        boutonState = !boutonState;
        intensite = 0;
    }
    txCommande = SET_VAL;
    sendSerialData();
}

void MainWindow::on_pushBottonOnOff_released()
{
    boutonManage(intensite);
    if (!serialRxIn)
    {
        txCommande = SET_VAL;
        sendSerialData();
    }
}
