/**
 * @file    mainwindow.cpp
 * @author  Thomas Desrosiers
 * @date    2022/01/12
 * @brief   Programme utilisé pour la communication avec le contrôleur de la lampe.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMenuBar>
#include <QString>
#include <QtSerialPort/QSerialPortInfo>

using namespace std;

int const MainWindow::EXIT_CODE_REBOOT = -1;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    intensite = 0;
    serialRxIn = false;
    boutonState = true;
    recepAvailable = false;
    sliderModif = true;
    dialModif = true;

    ui->setupUi(this);
    serial = new QSerialPort(this);
    connect(serial, SIGNAL(readyRead()), this, SLOT(readSerialData()));
    createMenus();
    ui->pushBottonOnOff->setIcon(QIcon(":/images/off.png"));
    ui->pushBottonOnOff->setIconSize(QSize(65, 65));

    systemTray = new QSystemTrayIcon(this);
    QIcon icon("on.png");
    systemTray->setIcon(icon);
    systemTray->setVisible(true);
    connect(systemTray, &QSystemTrayIcon::activated, this, &MainWindow::handleClick);
    systemTray->setToolTip("TEST");

    settings = new QSettings("./settings.ini", QSettings::IniFormat);
    settings->beginGroup("Info");
    settings->setValue("Author", "Thomas Desrosiers");
    settings->setValue("App", "LampeDimmer");
    settings->endGroup();

    timer = new QTimer();                                           //Timer utilisé pour le lecture du port série.
    connect(timer, SIGNAL(timeout()), this, SLOT(recepTimer()));    //Connexion du timer avec la fonction recepTimer.
    timer->start(10);

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
    if (serial->isOpen()) //Si le port série est ouvert...
    {
        ui->comboBoxSleep->setEnabled(true); //Les éléments du GUI sont activés.
        ui->dialIntensite->setEnabled(true);
        ui->pushBottonOnOff->setEnabled(true);
        ui->horizontalSliderIntensite->setEnabled(true);
    }
    else //Sinon (si le port série n'est pas ouvert)...
    {
        statusLabel->setText("Non connecté"); //Les éléments du GUI sont désactivés et le système affiche "Non connecté".
        statusLabel->setToolTip("");
        ui->comboBoxSleep->setDisabled(true);
        ui->dialIntensite->setDisabled(true);
        ui->pushBottonOnOff->setDisabled(true);
        ui->horizontalSliderIntensite->setDisabled(true);
    }
    if (!valueModeSys) //Si le système est en mode veille...
    {
        ui->dialIntensite->setDisabled(true); //Les éléments du GUI (sauf le comboBox) sont désactivés.
        ui->pushBottonOnOff->setDisabled(true);
        ui->horizontalSliderIntensite->setDisabled(true);
        ui->statusBar->showMessage("VEILLE | " + ui->comboBoxSleep->currentText()); //conversion de la valeur actuelle en pourcentage.
    }
    else //Sinon (si le système n'est pas en mode veille)...
    {
        ui->dialIntensite->setEnabled(true); //Les éléments du GUI sont activés.
        ui->pushBottonOnOff->setEnabled(true);
        ui->horizontalSliderIntensite->setEnabled(true);
    }
}

void MainWindow::boutonManage(int value)
{
    sliderModif = false; //Indique que la position du slider est modifiée par la fonction boutonManage.
    dialModif = false;   //Indique que la position du dial est modifiée par la fonction boutonManage.

    ui->comboBoxSleep->setCurrentIndex(veilleState);

    if (!valueModeSys) //Si le système est en mode veille...
    {
        ui->lbIntensiteValue->setText(QString::number(0)); //La valeur du slider est affichée dans le label sous le slider.
        ui->dialIntensite->setSliderPosition(0);           //Modifie la position du slider en fonction de la valeur obtenue par le slider.
        ui->horizontalSliderIntensite->setSliderPosition(0);
        ui->statusBar->showMessage(QString::number(0)); //conversion de la valeur actuelle en pourcentage.
        ui->comboBoxSleep->setCurrentIndex(veilleState);
    }
    else //Sinon (si le système n'est pas en mode veille)...
    {
        ui->lbIntensiteValue->setText(QString::number(value)); //La valeur du slider est affichée dans le label sous le slider.
        ui->dialIntensite->setSliderPosition(value);           //Modifie la position du slider en fonction de la valeur obtenue par le slider.
        ui->horizontalSliderIntensite->setSliderPosition(value);
        ui->statusBar->showMessage(QString::number((value / 2.55), 'f', 0) + '%'); //conversion de la valeur actuelle en pourcentage.
        ui->comboBoxSleep->setCurrentIndex(veilleState);
    }

    qDebug() << "SET_VAL : " << QString::number(value);

    if (value) //Si la valeur est plus grande que 0...
    {
        ui->pushBottonOnOff->setIcon(QIcon(":/images/on.png"));
        ui->pushBottonOnOff->setIconSize(QSize(65, 65));
        boutonState = 0;
    }
    else //Sinon (si la valeur est égale à 0)...
    {
        ui->pushBottonOnOff->setIcon(QIcon(":/images/off.png"));
        ui->pushBottonOnOff->setIconSize(QSize(65, 65));
        boutonState = 1;
    }
    sliderModif = true; //Indique que la position du slider est prête à être modifiée.
    dialModif = true;   //Indique que la position du dial est prête à être modifiée.
}

void MainWindow::createMenus(void)
{
    fichierMenu = menuBar()->addMenu(tr("&Fichier"));
    setupPrefAct = new QAction(tr("&Préférences"), this);
    quitterAct = new QAction(tr("&Quitter"), this);
    connect(setupPrefAct, &QAction::triggered, this, &MainWindow::setupPreference);
    connect(quitterAct, &QAction::triggered, this, &MainWindow::quitter);
    fichierMenu->addAction(setupPrefAct);
    fichierMenu->addAction(quitterAct);

    outilsMenu = menuBar()->addMenu(tr("&Outils"));
    setupSerialAct = new QAction(tr("&Configuration du port série"), this);
    connect(setupSerialAct, &QAction::triggered, this, &MainWindow::setupSerial);
    outilsMenu->addAction(setupSerialAct);
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
        qDebug() << "VAL_ACTU : " << QString::number(valueOut);
        break;
    case VAL_INIT:
        /*// ACQUISITION //*/
        valueOut = rxData[0];
        valueAdc = rxData[1];
        veilleState = VEILLE_STATE(rxData[2]);
        valueModeSys = rxData[3];

        /*// CHANGEMENTS GUI //*/
        boutonManage(valueOut);
        boutonEnabler();

        if (veilleState == VEILLE_NONE) //Si le mode de veille actuel est "NONE"...
        {
            veilleState = VEILLE_STATE(settings->value("Veille/Mode").toInt()); //Récupération du mode veille sauvegardé dans le fichier .ini.
            ui->comboBoxSleep->setCurrentIndex(veilleState);                    //Mise à jour du comboBox
            txCommande = SET_SLEEP_MODE;                                        //Envoi du mode veille au Arduino.
            execTxCommand();
        }
        else //Sinon (si le mode de veille actuel est autre que "NONE")...
        {
            settings->setValue("Veille/Mode", QString::number(veilleState)); //Les valeurs sauvegardés son actualisés.
            settings->setValue("Veille/Description", ui->comboBoxSleep->currentText());
        }

        /*// DEBUG //*/
        qDebug() << " - OUT_INI :    " << valueOut;
        qDebug() << " - ADC_INI :    " << valueAdc;
        qDebug() << " - VEILLE_INI : " << veilleState;
        qDebug() << " - SYS_INI :    " << valueModeSys;

        /*// FLAG DE RÉCEPTION //*/
        serialRxIn = false;
        break;
    case VAL_POT:
        /*// ACQUISITION //*/
        valueAdc = rxData[0];

        /*// CHANGEMENTS GUI //*/
        boutonManage(valueAdc);

        /*// DEBUG //*/
        qDebug() << "VAL_POT : " << QString::number(valueAdc);

        /*// FLAG DE RÉCEPTION //*/
        serialRxIn = false;
        break;
    case VAL_MODE:
        /*// ACQUISITION //*/
        valueModeSys = rxData[0];

        /*// CHANGEMENTS GUI //*/
        boutonEnabler();

        /*// DEBUG //*/
        qDebug() << "VAL_MODE : " + QString::number(valueModeSys);

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
            txData[3] = veilleState;
            txData[4] = '>';
            serial->write(txData, 5);
            break;
        case SET_VAL:
            txData[0] = '<';
            txData[1] = 1;
            txData[2] = SET_VAL;
            txData[3] = char(intensite);
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
        if (rxDataSize >= MAX_RXDATASIZE)
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
        rxCommande = RX_COMMANDES(data);
        if (rxDataSize)
        {
            rxState = RXDATA;
        }
        else
            rxState = VALIDATE;
        break;
    case RXDATA:
        rxData[rxDataCnt++] = data;
        qDebug() << " ----------------- RXDATA : " << QString::number(data);
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

void MainWindow::quitter(void)
{
    exit(1);
}

void MainWindow::readSerialData(void)
{
    QByteArray tmpRx;

    if (serial->bytesAvailable()) //Si des données sont disponibles...
    {
        if (recepAvailable) //Si recepAvailable est à 1...
        {
            recepAvailable = false;                         //recepAvailable est remis à 0.
            tmpRx.resize(int(serial->bytesAvailable()));    //tmpRx est tronqué avec le nombre de bits qui sont actuellement disponible sur le port série.
            serial->read(tmpRx.data(), tmpRx.size());       //Les données dans le port série sont placés dans tmpRx. 
            for (uint16_t i = 0; i < tmpRx.size(); i++)
                parseRXData(uint8_t(tmpRx[i])); //Les bits qui se trouvent dans tmpRx sont envoyés un à uns à parseRXDate.
        }
        else //Sinon...
        {
            serial->clear();    //Le port série est vidé.
            tmpRx.clear();      //tmpRx est vidé.
        }
    }
    else //Si aucune données est disponible...
    {
        recepAvailable = false; //recepAvailable est remis à 0.
    }
}

void MainWindow::reboot(void)
{
    qDebug() << "Performing application reboot...";
    qApp->exit(MainWindow::EXIT_CODE_REBOOT);
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
        txCommande = GET_VAL_INIT;
        execTxCommand();
    }
    boutonEnabler();
}

void MainWindow::setupPreference(void)
{
    SetupPreferenceDialog setupDia;
    setupDia.setWindowTitle("Préférences");
    setupDia.setWindowFlags(Qt::WindowSystemMenuHint); // Pour retirer le ?
    setupDia.setModal(1);
    setupDia.exec();
}

void MainWindow::on_comboBoxSleep_activated(int index)
{
    veilleState = VEILLE_STATE(index);
    settings->setValue("Veille/Mode", QString::number(index));
    settings->setValue("Veille/Description", ui->comboBoxSleep->currentText());
    boutonEnabler();
    txCommande = SET_SLEEP_MODE;
    execTxCommand();
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

void MainWindow::recepTimer(void)
{
    recepAvailable = true;
}

void MainWindow::on_dialIntensite_valueChanged()
{
    /* Cette condition permet de différencier un changement de la valeur du dial par l'utilisateur du modification en fonction des valeurs reçues par le potentiomètre */
    if (dialModif) //Si dialModif est vrai (si la position du slider n'est pas modifier actuellement)...
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
}

void MainWindow::on_horizontalSliderIntensite_valueChanged()
{
    /* Cette condition permet de différencier un changement de la valeur du slider par l'utilisateur du modification en fonction des valeurs reçues par le potentiomètre */
    if (sliderModif) //Si sliderModif est vrai (si la position du slider n'est pas modifier actuellement)...
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
}

void MainWindow::handleClick(QSystemTrayIcon::ActivationReason reason)
{

    switch (reason)
    {
    case QSystemTrayIcon::Unknown:
        qDebug() << "Unknown";
        break;
    case QSystemTrayIcon::Context:
        qDebug() << "Context - Right Click";
        exit(1);
        intensite = 0;
        break;
    case QSystemTrayIcon::DoubleClick:
        qDebug() << "Double Click";
        //show();
        break;
    case QSystemTrayIcon::Trigger:
        qDebug() << "Trigger - Left Click";
        show();
        break;
    case QSystemTrayIcon::MiddleClick:
        qDebug() << "MiddleClick";
        //txCommande = GET_VAL_POT;
        execTxCommand();
        break;
    }

    txCommande = SET_VAL;
    execTxCommand();
    boutonManage(intensite);
}

void MainWindow::on_pbReboot_clicked()
{
    reboot();
}
