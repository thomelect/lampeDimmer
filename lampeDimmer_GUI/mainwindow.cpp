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
    etatOnOff = true;
    recepAvailable = false;
    sliderModif = true;
    dialModif = true;
    intensiteByPass = 255;
    bypassMode = true;

    iconOn = new QIcon(":/images/on.png");
    iconOff = new QIcon(":/images/off.png");

    ui->setupUi(this);

    serial = new QSerialPort(this);
    connect(serial, SIGNAL(readyRead()), this, SLOT(readSerialData()));
    createMenus();
    ui->pushBottonOnOff->setIcon(*iconOff);
    ui->pushBottonOnOff->setIconSize(QSize(65, 65));

    sysTrayMenu = new QMenu;
    action1 = new QAction(tr("&On/Off"), this);
    connect(action1, &QAction::triggered, this, &MainWindow::toggleLampe);
    sysTrayMenu->addAction(action1);
    ///////////////////////////////
    sysTrayMenu1 = new QMenu;
    sysTrayMenu1->setTitle("&Réglage rapides");
    sysTrayMenu->addMenu(sysTrayMenu1);
    (full = new QAction(tr("&100%"), this))->setProperty("maValeur", 255);
    (demi = new QAction(tr("&50%"), this))->setProperty("maValeur", 127);
    (quart = new QAction(tr("&25%"), this))->setProperty("maValeur", 63);
    connect(full, &QAction::triggered, this, &MainWindow::mySlot);
    connect(demi, &QAction::triggered, this, &MainWindow::mySlot);
    connect(quart, &QAction::triggered, this, &MainWindow::mySlot);
    sysTrayMenu1->addAction(full);
    sysTrayMenu1->addAction(demi);
    sysTrayMenu1->addAction(quart);
    ///////////////////////////////
    action2 = new QAction(tr("&Préférences"), this);
    action3 = new QAction(tr("&Configuration série"), this);
    action4 = new QAction(tr("&Quitter"), this);
    connect(action2, &QAction::triggered, this, &MainWindow::setupPreference);
    connect(action3, &QAction::triggered, this, &MainWindow::setupSerial);
    connect(action4, &QAction::triggered, this, &exit);
    sysTrayMenu->addSeparator();
    sysTrayMenu->addAction(action2);
    sysTrayMenu->addAction(action3);
    sysTrayMenu->addSeparator();
    sysTrayMenu->addAction(action4);

    systemTray = new QSystemTrayIcon(this);
    systemTray->setIcon(*iconOn);
    systemTray->setVisible(true);
    connect(systemTray, &QSystemTrayIcon::activated, this, &MainWindow::handleClick);
    systemTray->setContextMenu(sysTrayMenu);

    settings = new QSettings("Thomas Desrosiers", "Lampe Dimmer");
    settings->setValue("Author", "Thomas Desrosiers");
    settings->setValue("App", "Lampe Dimmer");

    settings = new QSettings("Thomas Desrosiers", "Lampe Dimmer\\Settings");

    timer = new QTimer();                                        // Timer utilisé pour le lecture du port série.
    connect(timer, SIGNAL(timeout()), this, SLOT(recepTimer())); // Connexion du timer avec la fonction recepTimer.
    timer->start(10);

    statusLabel = new QLabel(this);
    ui->statusBar->addPermanentWidget(statusLabel);

    autoSetupSerial(); // Appel de la fonction de connexion automatique.

    // Feuille de style des boutons de la de l'interface MainWindow.
    this->setStyleSheet("#pushBottonOnOff {"
                        "background-color: none;"
                        "border: 0px"
                        "}");
}

MainWindow::~MainWindow()
{
    //bypassMode = false;
    txCommande = SET_BYPASS_VAL;
    execTxCommand();

    if (serial->isOpen())
        serial->close();
    delete serial;
    delete ui;
}

void MainWindow::mySlot()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (!action)
        return;
    int myValue = action->property("maValeur").toInt();
    intensite = myValue;
    qDebug() << "mySlot - maValeur" << myValue;
    if (!serialRxIn)
    {
        txCommande = SET_VAL;
        execTxCommand();
    }
    boutonManage(intensite);
    // emit mySignal(myValue);
}

void MainWindow::autoSetupSerial(void)
{
    settings->beginGroup("Serial");
    if (settings->contains("SerialNumber")) // Si le numéro de série lu dans le fichier n'est pas null...
    {
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) // Pour tout les ports disponibles...
        {
            if (info.serialNumber() == settings->value("SerialNumber")) // Si le numéro de série sauvegardé dans le fichier correspond à un des périphériques connectés...
            {
                portConfig = (info.portName() + " " + info.description()); // Information utilisées pour la connexion.
            }
            qDebug() << portConfig;
        }
        if (!portConfig.isEmpty()) // Si les information utilisées pour la connexion sont existantes...
        {
            foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) // Pour tout les ports disponibles...
            {
                QString infoTag = info.portName() + " " + info.description();
                if (infoTag == portConfig)
                {
                    serial->setPort(info);             // Assignation du port à connecter.
                    qDebug() << "try  " << portConfig; // Indique à l'utilisateur qu'une tentative de connexion est en cours.
                    if (serial->open(QIODevice::ReadWrite))
                    {
                        qDebug() << "open " << portConfig; // Indique à l'utilisateur qu'une tentative de connexion est en cours.
                        if (serial->setBaudRate(settings->value("BaudRate").toInt()) && serial->setDataBits(QSerialPort::Data8) && serial->setParity(QSerialPort::NoParity) && serial->setStopBits(QSerialPort::OneStop) && serial->setFlowControl(QSerialPort::NoFlowControl))
                        {
                            statusLabel->setText("Connecté " + info.portName()); // Indique à l'utilisateur que la connexion est réussie ainsi que le nom du port.
                            statusLabel->setToolTip(infoTag);                    // Indique à l'utilisateur le nom du port et sa description.
                        }
                    }
                }
            }
        }
        if (serial->isOpen()) // Si le port est ouvert...
        {

            if (!serialRxIn)
            {
                txCommande = GET_VAL_INIT;
                execTxCommand();
            }
            if (!serialRxIn)
            {
                txCommande = SET_BYPASS_VAL;
                execTxCommand();
            }
        }


        boutonEnabler();
    }
    settings->endGroup();
}

void MainWindow::boutonEnabler()
{
    if (serial->isOpen()) // Si le port série est ouvert...
    {
        ui->comboBoxSleep->setEnabled(true); // Les éléments du GUI sont activés.
        ui->dialIntensite->setEnabled(true);
        ui->pushBottonOnOff->setEnabled(true);
        ui->horizontalSliderIntensite->setEnabled(true);
    }
    else // Sinon (si le port série n'est pas ouvert)...
    {
        statusLabel->setText("Non connecté"); // Les éléments du GUI sont désactivés et le système affiche "Non connecté".
        statusLabel->setToolTip("");
        ui->comboBoxSleep->setDisabled(true);
        ui->dialIntensite->setDisabled(true);
        ui->pushBottonOnOff->setDisabled(true);
        ui->horizontalSliderIntensite->setDisabled(true);
    }
    if (!valueModeSys) // Si le système est en mode veille...
    {
        ui->dialIntensite->setDisabled(true); // Les éléments du GUI (sauf le comboBox) sont désactivés.
        ui->pushBottonOnOff->setDisabled(true);
        ui->horizontalSliderIntensite->setDisabled(true);
        ui->statusBar->showMessage("VEILLE | " + ui->comboBoxSleep->currentText()); // conversion de la valeur actuelle en pourcentage.
        systemTray->setToolTip(ui->comboBoxSleep->currentText());
    }
    else // Sinon (si le système n'est pas en mode veille)...
    {
        ui->dialIntensite->setEnabled(true); // Les éléments du GUI sont activés.
        ui->pushBottonOnOff->setEnabled(true);
        ui->horizontalSliderIntensite->setEnabled(true);
    }
}

void MainWindow::toggleLampe()
{
    etatOnOff = !etatOnOff;
    qDebug() << "toggleLampe : " << QString::number(etatOnOff);

    if (etatOnOff) // Met la lumière à "ON" et le bouton affiche maintenant "OFF".
    {
        intensite = valueAdc;
    }
    else // Met la lumière à "OFF" et le bouton affiche maintenant "ON".
    {
        intensite = 0;
    }
    if (!serialRxIn)
    {
        txCommande = SET_VAL;
        execTxCommand();
    }
    boutonManage(intensite);
}

void MainWindow::boutonManage(int value)
{
    sliderModif = false; // Indique que la position du slider est modifiée par la fonction boutonManage.
    dialModif = false;   // Indique que la position du dial est modifiée par la fonction boutonManage.

    ui->comboBoxSleep->setCurrentIndex(veilleState);

    if (!valueModeSys) // Si le système est en mode veille...
    {
        ui->lbIntensiteValue->setText(QString::number(0)); // La valeur du slider est affichée dans le label sous le slider.
        ui->dialIntensite->setSliderPosition(0);           // Modifie la position du slider en fonction de la valeur obtenue par le slider.
        ui->horizontalSliderIntensite->setSliderPosition(0);
        ui->statusBar->showMessage(QString::number(0)); // conversion de la valeur actuelle en pourcentage.
        ui->comboBoxSleep->setCurrentIndex(veilleState);
    }
    else // Sinon (si le système n'est pas en mode veille)...
    {
        ui->lbIntensiteValue->setText(QString::number(value)); // La valeur du slider est affichée dans le label sous le slider.
        ui->dialIntensite->setSliderPosition(value);           // Modifie la position du slider en fonction de la valeur obtenue par le slider.
        ui->horizontalSliderIntensite->setSliderPosition(value);
        ui->statusBar->showMessage(QString::number((value / 2.55), 'f', 0) + '%'); // conversion de la valeur actuelle en pourcentage.
        ui->comboBoxSleep->setCurrentIndex(veilleState);
        systemTray->setToolTip(QString::number((value / 2.55), 'f', 0) + '%');
    }

    qDebug() << "SET_VAL : " << QString::number(value);

    if (value) // Si la valeur est plus grande que 0...
    {
        ui->pushBottonOnOff->setIcon(*iconOn);
        action1->setText("&Off");
        ui->pushBottonOnOff->setIconSize(QSize(65, 65));
        systemTray->setIcon(*iconOn); // On assigne une image à notre icône
        etatOnOff = 1;
    }
    else // Sinon (si la valeur est égale à 0)...
    {
        ui->pushBottonOnOff->setIcon(*iconOff);
        action1->setText("&On");
        ui->pushBottonOnOff->setIconSize(QSize(65, 65));
        systemTray->setIcon(*iconOff); // On assigne une image à notre icône
        etatOnOff = 0;
    }
    sliderModif = true; // Indique que la position du slider est prête à être modifiée.
    dialModif = true;   // Indique que la position du dial est prête à être modifiée.
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

        /*// FLAG DE RÉCEPTION //*/
        serialRxIn = false;
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

        if (veilleState == VEILLE_NONE) // Si le mode de veille actuel est "NONE"...
        {
            veilleState = VEILLE_STATE(settings->value("Veille/Mode").toInt()); // Récupération du mode veille sauvegardé dans le fichier .ini.
            ui->comboBoxSleep->setCurrentIndex(veilleState);                    // Mise à jour du comboBox
            txCommande = SET_SLEEP_MODE;                                        // Envoi du mode veille au Arduino.
            execTxCommand();
        }
        else // Sinon (si le mode de veille actuel est autre que "NONE")...
        {
            settings->beginGroup("Veille");
            settings->setValue("Mode", QString::number(veilleState)); // Les valeurs sauvegardés son actualisés.
            settings->setValue("Description", ui->comboBoxSleep->currentText());
            settings->endGroup();
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
    case VAL_BYPASS_VAL:
        /*// ACQUISITION //*/


        /*// DEBUG //*/
        qDebug() << "VAL_BYPASS_VALVAL_BYPASS_VAL : ";

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
    serialRxIn = true;
    if (serial->isOpen())
    {
        char txData[6];
        switch (txCommande)
        {
        case GET_VAL_ACTU:
            qDebug() << "EXEC-TX | GET_VAL_ACTU";
            txData[0] = '<';
            txData[1] = 0;
            txData[2] = GET_VAL_ACTU;
            txData[3] = '>';
            serial->write(txData, 4);
            break;
        case GET_VAL_INIT:
            qDebug() << "EXEC-TX | GET_VAL_INIT";
            txData[0] = '<';
            txData[1] = 0;
            txData[2] = GET_VAL_INIT;
            txData[3] = '>';
            serial->write(txData, 4);
            break;
        case GET_VAL_POT:
            qDebug() << "EXEC-TX | GET_VAL_POT";
            txData[0] = '<';
            txData[1] = 0;
            txData[2] = GET_VAL_POT;
            txData[3] = '>';
            serial->write(txData, 4);
            break;
        case SET_SLEEP_MODE:
            qDebug() << "EXEC-TX | SET_SLEEP_MODE";
            txData[0] = '<';
            txData[1] = 1;
            txData[2] = SET_SLEEP_MODE;
            txData[3] = veilleState;
            txData[4] = '>';
            serial->write(txData, 5);
            break;
        case SET_VAL:
            qDebug() << "EXEC-TX | SET_VAL";
            txData[0] = '<';
            txData[1] = 1;
            txData[2] = SET_VAL;
            txData[3] = char(intensite);
            txData[4] = '>';
            serial->write(txData, 5);
            break;
        case SET_BYPASS_VAL:
            qDebug() << "EXEC-TX | SET_BYPASS_VAL";
            txData[0] = '<';
            txData[1] = 2;
            txData[2] = SET_BYPASS_VAL;
            txData[3] = char(bypassMode);
            txData[4] = char(intensiteByPass);
            txData[5] = '>';
            serial->write(txData, 6);
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
            //serialRxIn = true;
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
    bypassMode = 1;
        txCommande = SET_BYPASS_VAL;
        execTxCommand();
    while (serialRxIn)
        ;
    exit(1);
}

void MainWindow::readSerialData(void)
{
    QByteArray tmpRx;

    if (serial->bytesAvailable()) // Si des données sont disponibles...
    {
        if (recepAvailable) // Si recepAvailable est à 1...
        {
            recepAvailable = false;                      // recepAvailable est remis à 0.
            tmpRx.resize(int(serial->bytesAvailable())); // tmpRx est tronqué avec le nombre de bits qui sont actuellement disponible sur le port série.
            serial->read(tmpRx.data(), tmpRx.size());    // Les données dans le port série sont placés dans tmpRx.
            for (uint16_t i = 0; i < tmpRx.size(); i++)
                parseRXData(uint8_t(tmpRx[i])); // Les bits qui se trouvent dans tmpRx sont envoyés un à uns à parseRXDate.
        }
        else // Sinon...
        {
            serial->clear(); // Le port série est vidé.
            tmpRx.clear();   // tmpRx est vidé.
        }
    }
    else // Si aucune données est disponible...
    {
        recepAvailable = false; // recepAvailable est remis à 0.
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
        statusLabel->setText("Connecté " + serial->portName()); // Indique à l'utilisateur que la connexion est réussie ainsi que le nom du port.
        statusLabel->setToolTip(setupDia.getInfoPort());
        if (!serialRxIn)
        {
            txCommande = GET_VAL_INIT;
            execTxCommand();
        }
    }

    if (!serialRxIn)
    {
        txCommande = SET_BYPASS_VAL;
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

    settings->beginGroup("Veille");
    settings->setValue("Mode", QString::number(index)); // Les valeurs sauvegardés son actualisés.
    settings->setValue("Description", ui->comboBoxSleep->currentText());
    settings->endGroup();

    boutonEnabler();
    txCommande = SET_SLEEP_MODE;
    execTxCommand();
}

void MainWindow::on_pushBottonOnOff_pressed()
{
    toggleLampe();
}

void MainWindow::recepTimer(void)
{
    recepAvailable = true;
}

void MainWindow::on_dialIntensite_valueChanged()
{
    /* Cette condition permet de différencier un changement de la valeur du dial par l'utilisateur du modification en fonction des valeurs reçues par le potentiomètre */
    if (dialModif) // Si dialModif est vrai (si la position du slider n'est pas modifier actuellement)...
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
    if (sliderModif) // Si sliderModif est vrai (si la position du slider n'est pas modifier actuellement)...
    {
        if (!ui->pushBottonOnOff->isChecked())
        {
            intensite = ui->horizontalSliderIntensite->value(); // On récupère la valeur du slider.
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
        sysTrayMenu->setVisible(1);
        break;
    case QSystemTrayIcon::DoubleClick:
        qDebug() << "Double Click";
        break;
    case QSystemTrayIcon::Trigger:
        qDebug() << "Trigger - Left Click";
        show();
        break;
    case QSystemTrayIcon::MiddleClick:
        qDebug() << "MiddleClick";
        break;
    }
}

void MainWindow::on_pushButton_clicked()
{
    //bypassMode = !bypassMode;
    intensiteByPass = 255;
    txCommande = SET_BYPASS_VAL;
    execTxCommand();
}

