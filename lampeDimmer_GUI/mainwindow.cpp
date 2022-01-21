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

    settings = new QSettings("./settings.ini", QSettings::IniFormat);
    settings->beginGroup("Info");
    settings->setValue("Author", "Thomas Desrosiers");
    settings->setValue("App", "LampeDimmer");
    settings->endGroup();

    ui->spinBox->hide();
    timerSpin = new QTimer();
    connect(timerSpin, SIGNAL(timeout()), this, SLOT(updateCountDown()));

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(recepTimer()));
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
    sliderModif = false;
    dialModif = false;
    ui->lbIntensiteValue->setText(QString::number(value)); //La valeur du slider est affichée dans le label sous le slider.
    ui->dialIntensite->setSliderPosition(value);           //Modifie la position du slider en fonction de la valeur obtenue par le slider.
    ui->horizontalSliderIntensite->setSliderPosition(value);
    ui->statusBar->showMessage(QString::number((value / 2.55), 'f', 0) + '%'); //conversion de la valeur actuelle en pourcentage.
    ui->comboBoxSleep->setCurrentIndex(veilleState);
    qDebug() << "SET_VAL : " << QString::number(value);

    QPixmap pixmapOff("/images/off.png");
    QIcon ButtonIcon(pixmapOff);
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
    sliderModif = true;
    dialModif = true;
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
        qDebug() << "VAL_ACTU : " << QString::number(valueOut);
        break;
    case VAL_INIT:
        /*// ACQUISITION //*/
        valueOut = rxData[0];
        valueAdc = rxData[1];
        veilleState = rxData[2];
        valueModeSys = rxData[3];

        /*// CHANGEMENTS GUI //*/
        boutonManage(valueOut);
        boutonEnabler();

        if (!veilleState) //Si le mode de veille actuel est "NONE"...
        {
            veilleState = settings->value("Veille/Mode").toInt(); //Récupération du mode veille sauvegardé dans le fichier .ini.
            ui->comboBoxSleep->setCurrentIndex(veilleState);      //Mise à jour du comboBox
            txCommande = SET_SLEEP_MODE;                          //Envoi du mode veille au Arduino.
            execTxCommand();
        }
        else //Sinon (si le mode de veille actuel est autre que "NONE")...
        {
            settings->setValue("Veille/Mode", QString::number(veilleState)); //Les valeurs sauvegardés son actualisés.
            settings->setValue("Veille/Description", ui->comboBoxSleep->currentText());
        }

        /*// DEBUG //*/
        qDebug() << "VAL_ACTU : " << QString::number(valueOut);
        qDebug() << "SLEEP_MODE : " << ui->comboBoxSleep->currentText();
        qDebug() << "SYS_MODE : " << (valueModeSys ? "NORMAL" : "VEILLE");

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
        char txData[6];
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
            txData[1] = 2;
            txData[2] = SET_SLEEP_MODE;
            txData[3] = veilleState;
            txData[4] = customVal;
            txData[5] = '>';
            serial->write(txData, 6);
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

/**
 * @brief  Fonction de lecture du port série..
 */
void MainWindow::readSerialData(void)
{
    QByteArray tmpRx;

    if (serial->bytesAvailable())
    {
        if (recepAvailable)
        {
            recepAvailable = false;
            tmpRx.resize(serial->bytesAvailable());
            serial->read(tmpRx.data(), tmpRx.size());
            for (uint16_t i = 0; i < tmpRx.size(); i++)
                parseRXData(tmpRx[i]);
        }
        else
        {
            serial->clear();
            tmpRx.clear();
        }
    }
    else
    {
        recepAvailable = false;
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
    veilleState = index;
    if (!serialRxIn)
    {
        settings->setValue("Veille/Mode", QString::number(index));
        settings->setValue("Veille/Description", ui->comboBoxSleep->currentText());
    }

    boutonEnabler();
    if (ui->comboBoxSleep->currentIndex() == (ui->comboBoxSleep->findText("CUSTOM", Qt::MatchExactly)))
    {
        ui->spinBox->show();
        ui->spinBox->setStyleSheet("#spinBox { background-color: #9FD4A3; }");
    }
    else
    {
        if (settings->contains("Veille/ValueOut"))
        {
            settings->remove("Veille/ValueOut");
        }
    }
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
    if (dialModif)
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
    if (sliderModif)
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

void MainWindow::on_spinBox_valueChanged(void)
{
    timerSpin->start(1000);
    countDown = 0;
    ui->spinBox->setStyleSheet("#spinBox { background-color: #9FD4A3; }");
}

void MainWindow::updateCountDown()
{
    QString myColor = "blue";
    QString styleSheet = "#spinBox { background-color: %1 ; }";
    switch (countDown) {
    case 0:
        myColor = "#AEDCAE";
        break;
    case 1:
        myColor = "#CDEBC5";
        break;
    case 2:
        myColor = "#DDF2D1";
        break;
    case 3:
        myColor = "#ECFADC";
        break;
    case 4:
        myColor = "#FFFFFF";
        break;
    }
    qDebug() << countDown++;
    ui->spinBox->setStyleSheet(styleSheet.arg(myColor));
    if (countDown >= 5)
    {
        timerSpin->stop();
        ui->spinBox->hide();
        customVal = ui->spinBox->value();
        ui->comboBoxSleep->setItemText(ui->comboBoxSleep->currentIndex(), ("CUSTOM | " + QString::number(customVal)));
        settings->setValue("Veille/ValueOut", customVal);
        txCommande = SET_SLEEP_MODE;
        execTxCommand();
    }
}
