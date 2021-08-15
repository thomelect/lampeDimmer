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

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /*
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendGetState()));
    timer->start(100); // vous pouvez réduire l'interval durant les tests.
    */

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
                        "}"
                        );
}

MainWindow::~MainWindow()
{
    if(serial->isOpen())
        serial->close();
    delete serial;
    delete ui;
}

void MainWindow::boutonManage(int value)
{
    QPixmap pixmapOff("/images/off.png");
    QIcon ButtonIcon(pixmapOff);
    if (value)
    {
        //ui->pushBottonOnOff->setText("OFF");
        ui->pushBottonOnOff->setIcon(QIcon(":/images/on.png"));
        ui->pushBottonOnOff->setIconSize(QSize(65, 65));
        boutonState = 0;
    }
    else
    {
        //ui->pushBottonOnOff->setText("ON ");
        ui->pushBottonOnOff->setIcon(QIcon(":/images/off.png"));
        ui->pushBottonOnOff->setIconSize(QSize(65, 65));
        boutonState = 1;
    }
}

void MainWindow::createMenus()
{
    setupSerialAct = new QAction(tr("&Configuration du port série"), this);
    connect(setupSerialAct, &QAction::triggered, this, &MainWindow::setupSerial);
    toolsMenu = menuBar()->addMenu(tr("&Outils"));
    toolsMenu->addAction(setupSerialAct);
}

/**
 * @brief Traitement et exécution d'une commande reçue
 */
void MainWindow::execRxCommand()
{
    /*
    if (rxCommande == VAL_POT)
    {
        //intensite = rxData[0];
        ui->horizontalSliderIntensite->setSliderPosition(rxData[0]); //Modifie la position du slider en fonction de la valeur obtenue par le dial.
        ui->dialIntensite->setSliderPosition(rxData[0]); //Modifie la position du slider en fonction de la valeur obtenue par le slider.
        ui->lbIntensiteValue->setText(QString::number(rxData[0]));
    }
    */
}

/**
 * @brief Sépare les données reçues sur le port série.
 * @param data la donnée à traiter
 */
void MainWindow::parseRXData(uint8_t data)
{

    switch(rxState)
    {
    default :
        if(data == '<')
        {
            rxState = RXSIZE;
            rxDataCnt = 0;
        }
        break;
    case RXSIZE:
        rxDataSize = data;
        if(rxDataSize >= _MAX_RXDATASIZE_)
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
        if(rxDataSize)
        {

            rxState = RXDATA;
        }
        else
            rxState = VALIDATE;
        break;
    case RXDATA:
        rxData[rxDataCnt++] = data;
        if(rxDataCnt == rxDataSize)
            rxState = VALIDATE;
        break;
    case VALIDATE :
        rxState = WAIT;
        if(data == '>')
        {

            execRxCommand();
        }
        else
        {
            rxErrorCommCnt++;
            qDebug() << " \n\r Error RX TRAME Error = " <<rxErrorCommCnt<<"   "<< " \n\r";
        }
        break;
    }
}

/**
* @brief Fonction de lecture du port série..
*/
void MainWindow::readSerialData() {

    QByteArray tmpRx;

    if(serial->bytesAvailable())
    {
        tmpRx.resize(serial->bytesAvailable());
        serial->read(tmpRx.data(), tmpRx.size());
        for(uint16_t i = 0 ; i < tmpRx.size() ; i++ )
            parseRXData(tmpRx[i]);
    }
}

/**
 * @brief Envoi la commande GET_STATE. Déclenchée par le timer.
 */
/*
void MainWindow::sendGetState()
{
    if(serial->isOpen())
    {
        char txData[4];
        txData[0] = '<';
        txData[1] = 0;
        txData[2] = GET_ETAT;
        txData[3] = '>';
        serial->write(txData,4);
    }
*/

/**
* @brief Fonction de lecture du'envoie sur le port série..
*/
void MainWindow::sendSerialData()
{
    char txData[5];
    txData[0] = '<';
    txData[1] = 1;
    txData[2] = SEND_VAL;
    txData[3] = intensite;
    txData[4] = '>';
    serial->write(txData, 5);
}

void MainWindow::setupSerial()
{
    SetupSerialDialog setupDia(serial);
    setupDia.setWindowTitle("Configuration du port série");
    setupDia.setWindowFlags( Qt::WindowSystemMenuHint); // Pour retirer le ?
    setupDia.setModal(1);
    setupDia.exec();
}

void MainWindow::on_comboBoxSleep_activated(int index)
{
    char txData[5];
    txData[0] = '<';
    txData[1] = 1;
    txData[2] = SEND_SLEEP_MODE;
    txData[3] = index;
    txData[4] = '>';
    serial->write(txData, 5);
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

void MainWindow::on_horizontalSliderIntensite_valueChanged(void)
{
    intensite = ui->horizontalSliderIntensite->value(); //On récupère la valeur du slider.
    ui->lbIntensiteValue->setText(QString::number(intensite)); //La valeur du slider est affichée dans le label sous le slider.
    ui->dialIntensite->setSliderPosition(intensite); //Modifie la position du slider en fonction de la valeur obtenue par le slider.
    boutonManage(intensite);
    sendSerialData();  //Appel de la fonction sendData();
    qDebug() << intensite;
}

void MainWindow::on_pushBottonOnOff_clicked()
{
    //boutonState = !boutonState;
    if (boutonState) //Met la lumière à "ON" et le bouton affiche maintenant "OFF".
    {
        boutonState = !boutonState;
        intensite = 255;
        ui->lbIntensiteValue->setText(QString::number(intensite));
    }
    else //Met la lumière à "OFF" et le bouton affiche maintenant "ON".
    {
        boutonState = !boutonState;
        intensite = 0;
        ui->lbIntensiteValue->setText(QString::number(intensite));
    }
    ui->horizontalSliderIntensite->setSliderPosition(intensite); //Modifie la position du slider.
    ui->dialIntensite->setSliderPosition(intensite); //Modifie la position du dial.
    sendSerialData();
}