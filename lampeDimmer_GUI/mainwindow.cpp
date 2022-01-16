/**
 * @file    mainwindow.cpp
 * @author  Thomas Desrosiers
 * @date    2022/01/12
 * @brief   Programme utilisé pour la communication avec le contrôleur de la lampe.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "setupserialdialog.h"
#include <QDebug>
#include <QMenuBar>
#include <QString>
#include <QtSerialPort/QSerialPortInfo>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    intensite = 0;
    serialRxIn = false;
    boutonState = true;

    ui->setupUi(this);
    serial = new QSerialPort(this);
    connect(serial, SIGNAL(readyRead()), this, SLOT(readSerialData()));
    createMenus();
    ui->pushBottonOnOff->setIcon(QIcon(":/images/off.png"));
    ui->pushBottonOnOff->setIconSize(QSize(65, 65));

    settings = new QSettings("./settings.ini", QSettings::IniFormat);
    settings->beginGroup("Info");
    settings->setValue("Author", "Thomas Desrosiers");
    settings->setValue("App", "LampeDimmer");
    settings->endGroup();

    statusLabel = new QLabel(this);
    ui->statusBar->addPermanentWidget(statusLabel);

    autoSetupSerial(); //Appel de la fonction de connexion automatique.

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

void MainWindow::autoSetupSerial(void)
{
    settings->beginGroup("Serial");
    if (settings->contains("SerialNumber")) //Si le numéro de série lu dans le fichier n'est pas null...
    {
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) //Pour tout les ports disponibles...
        {
            if (info.serialNumber() == settings->value("SerialNumber")) //Si le numéro de série sauvegardé dans le fichier correspond à un des périphériques connectés...
            {
                portConfig = (info.portName() + " " + info.description()); //Information utilisées pour la connexion.
            }
            qDebug() << portConfig;
        }
        if (!portConfig.isEmpty()) //Si les information utilisées pour la connexion sont existantes...
        {
            foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) //Pour tout les ports disponibles...
            {
                QString infoTag = info.portName() + " " + info.description();
                if (infoTag == portConfig)
                {
                    serial->setPort(info);             //Assignation du port à connecter.
                    qDebug() << "try  " << portConfig; //Indique à l'utilisateur qu'une tentative de connexion est en cours.
                    if (serial->open(QIODevice::ReadWrite))
                    {
                        qDebug() << "open " << portConfig; //Indique à l'utilisateur qu'une tentative de connexion est en cours.
                        if (serial->setBaudRate(settings->value("BaudRate").toInt()) && serial->setDataBits(QSerialPort::Data8) && serial->setParity(QSerialPort::NoParity) && serial->setStopBits(QSerialPort::OneStop) && serial->setFlowControl(QSerialPort::NoFlowControl))
                        {
                            statusLabel->setText("Connecté " + info.portName()); //Indique à l'utilisateur que la connexion est réussie ainsi que le nom du port.
                            statusLabel->setToolTip(infoTag);                    //Indique à l'utilisateur le nom du port et sa description.
                        }
                    }
                }
            }
        }
        if (serial->isOpen()) //Si le port est ouvert...
        {
            qDebug() << "GET_VAL_INIT";
            txCommande = GET_VAL_INIT;
            execTxCommand();
        }
        boutonEnabler();
    }
    settings->endGroup();
}

void MainWindow::boutonEnabler()
{
    if (serial->isOpen())
    {
        ui->comboBoxSleep->setEnabled(true);
        ui->dialIntensite->setEnabled(true);
        ui->pushBottonOnOff->setEnabled(true);
        ui->horizontalSliderIntensite->setEnabled(true);
    }
    else
    {
        statusLabel->setText("Non connecté");
        statusLabel->setToolTip("");
        ui->comboBoxSleep->setDisabled(true);
        ui->dialIntensite->setDisabled(true);
        ui->pushBottonOnOff->setDisabled(true);
        ui->horizontalSliderIntensite->setDisabled(true);
    }
    if (!valueModeSystem)
    {
        ui->dialIntensite->setDisabled(true);
        ui->pushBottonOnOff->setDisabled(true);
        ui->horizontalSliderIntensite->setDisabled(true);
    }
    else
    {
        ui->dialIntensite->setEnabled(true);
        ui->pushBottonOnOff->setEnabled(true);
        ui->horizontalSliderIntensite->setEnabled(true);
    }
}

void MainWindow::boutonManage(int value)
{
    ui->lbIntensiteValue->setText(QString::number(intensite)); //La valeur du slider est affichée dans le label sous le slider.
    ui->dialIntensite->setSliderPosition(intensite);           //Modifie la position du slider en fonction de la valeur obtenue par le slider.
    ui->horizontalSliderIntensite->setSliderPosition(intensite);
    ui->statusBar->showMessage(QString::number((intensite / 2.55), 'f', 0) + '%'); //conversion de la valeur actuelle en pourcentage.
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
        valueModeVeille = rxData[1];
        valueModeSystem = rxData[2];

        /*// DEBUG //*/
        qDebug() << "VAL_ACTU : " + QString::number(valueOut);
        qDebug() << "SLEEP_MODE : " + ui->comboBoxSleep->currentText();

        /*// CHANGEMENTS GUI //*/
        ui->horizontalSliderIntensite->setSliderPosition(valueOut); //Modifie la position du slider en fonction de la valeur obtenue par le dial.
        ui->dialIntensite->setSliderPosition(valueOut);             //Modifie la position du slider en fonction de la valeur obtenue par le slider.
        ui->lbIntensiteValue->setText(QString::number(valueOut));
        ui->comboBoxSleep->setCurrentIndex(valueModeVeille);

        if (!valueModeVeille)
        {
            valueModeVeille = settings->value("veille/mode").toInt();
            ui->comboBoxSleep->setCurrentIndex(valueModeVeille);
            txCommande = SET_SLEEP_MODE;
            execTxCommand();
        }
        else
        {
            settings->setValue("veille/mode", QString::number(valueModeVeille));
            settings->setValue("veille/Description", ui->comboBoxSleep->currentText());
        }

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
    case VAL_MODE:
        /*// ACQUISITION //*/
        valueModeSystem = rxData[0];

        /*// DEBUG //*/
        qDebug() << "VAL_MODE : " + QString::number(valueModeSystem);

        /*// CHANGEMENTS GUI //*/
        boutonEnabler();

        /*// FLAG DE RÉCEPTION //*/
        serialRxIn = false;
        break;
    }
}

/**
 * @brief  Fonction de lecture du'envoie sur le port série..
 */
void MainWindow::execTxCommand()
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
            txData[3] = valueModeVeille;
            txData[4] = '>';
            serial->write(txData, 5);
            break;
        case SET_VAL:
            txData[0] = '<';
            txData[1] = 1;
            txData[2] = SET_VAL;
            txData[3] = intensite;
            txData[4] = '>';
            serial->write(txData, 5);
            break;
        }
    }
}

/**
 * @brief       Sépare les données reçues sur le port série.
 * @param data  Donnée à traiter
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
 * @brief  Fonction de lecture du port série..
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

void MainWindow::setupSerial(void)
{
    SetupSerialDialog setupDia(serial);
    setupDia.setWindowTitle("Configuration du port série");
    setupDia.setWindowFlags(Qt::WindowSystemMenuHint); // Pour retirer le ?
    setupDia.setModal(1);
    setupDia.exec();
    if (serial->isOpen())
    {
        statusLabel->setText("Connecté " + serial->portName()); //Indique à l'utilisateur que la connexion est réussie ainsi que le nom du port.
        statusLabel->setToolTip(setupDia.getInfoPort());
        qDebug() << "GET_VAL_INIT";
        txCommande = GET_VAL_INIT;
        execTxCommand();
    }
    boutonEnabler();
}

void MainWindow::on_comboBoxSleep_activated(int index)
{
    valueModeVeille = index;
    settings->setValue("veille/mode", QString::number(index));
    settings->setValue("veille/Description", ui->comboBoxSleep->currentText());
    txCommande = SET_SLEEP_MODE;
    execTxCommand();
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
            execTxCommand();
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
            execTxCommand();
        }
    }
}

void MainWindow::on_pushBottonOnOff_pressed()
{
    if (boutonState) //Met la lumière à "ON" et le bouton affiche maintenant "OFF".
    {
        txCommande = GET_VAL_POT;
        execTxCommand();
        boutonState = !boutonState;
        intensite = valueAdc;
    }
    else //Met la lumière à "OFF" et le bouton affiche maintenant "ON".
    {
        boutonState = !boutonState;
        intensite = 0;
    }
    txCommande = SET_VAL;
    execTxCommand();
}

void MainWindow::on_pushBottonOnOff_released()
{
    boutonManage(intensite);
    if (!serialRxIn)
    {
        txCommande = SET_VAL;
        execTxCommand();
    }
}
