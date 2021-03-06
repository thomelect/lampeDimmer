/**
 * @file    mainwindow.cpp
 * @author  Thomas Desrosiers
 * @date    2022/01/12
 * @brief   Programme utilisé pour la communication avec le contrôleur de la lampe.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "setupserialdialog.h"
#include <QPixmap>
#include <QMainWindow>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QSettings>
#include <QTimer>
#include <QDateTime>

#define MAX_RXDATASIZE 16

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* Enums étapes réception: */
    enum RX_STATES
    {
        WAIT,
        RXSIZE,
        RXCOMMANDE,
        RXDATA,
        VALIDATE
    };
    /* Enums commandes réception: */
    enum RX_COMMANDES
    {
        VAL_ACTU,
        VAL_INIT,
        VAL_POT,
        VAL_MODE
    };
    /* Enums commandes transmission: */
    enum TX_COMMANDES
    {
        GET_VAL_ACTU,
        GET_VAL_INIT,
        GET_VAL_POT,
        SET_SLEEP_MODE,
        SET_VAL
    };

    enum VEILLE_STATE
    {
        VEILLE_NONE,
        VEILLE_OFF,
        VEILLE_ON,
        VEILLE_BREATHING,
        VEILLE_VEILLE
    };
    /* Déclaration enums: */
    RX_STATES rxState;
    RX_COMMANDES rxCommande;
    TX_COMMANDES txCommande;
    VEILLE_STATE veilleState;

    /* Déclarations variables: */
    QByteArray tmpRxData;
    QString portConfig;
    QString *connectInfo;
    uint8_t rxDataSize;
    uint8_t rxDataCnt;
    uint8_t rxData[MAX_RXDATASIZE];
    uint16_t rxErrorCommCnt;
    uint8_t valueAdc;
    uint8_t valueOut;
    bool valueModeSys;
    int intensite;
    bool serialRxIn;
    bool boutonState;
    bool recepAvailable;
    bool sliderModif;
    bool dialModif;

    /* Déclarations classes: */
    QLabel *statusLabel;
    QSerialPort *serial;
    QMenu *toolsMenu;
    QTimer *timer;
    QAction *setupSerialAct;
    QPixmap *pixmapOff();
    QIcon *ButtonIcon();
    QSettings *settings;
    Ui::MainWindow *ui;

private slots:

    /**
     * @brief  Fonction de lecture du port série..
     */
    void readSerialData(void);

    /**
     * @brief  Fonction appelée à caque fois que le décompte du timer arrive à 0.
     */
    void recepTimer(void);

    /**
     * @brief        Fonction appelée lorsque l'utilisateur choisi une option dans la liste.
     * @param index  Index des options de la liste.
     */
    void on_comboBoxSleep_activated(int index);

    /**
     * @brief  Fonction appelée lorsque le bouton on/off est appuyé.
     */
    void on_pushBottonOnOff_pressed();

    /**
     * @brief  Fonction appelée lorsque le bouton on/off est relachée.
     */
    void on_pushBottonOnOff_released();

    /**
     * @brief  Fonction appelée lorsque la valeur du slider est changée.
     */
    void on_horizontalSliderIntensite_valueChanged();

    /**
     * @brief  Fonction appelée lorsque la valeur du dial est changée.
     */
    void on_dialIntensite_valueChanged();

private:
    /**
     * @brief  Fonction utilisée afin de créer le menu "Outils".
     */
    void createMenus(void);

    /**
     * @brief  Fonction utilisée afin de gérer la fenêtre de connexion.
     */
    void setupSerial(void);

private:
    /**
     * @brief  Fonction utilisée afin de connecter automatiquement l'application au port série correspondant aux informations lut dans le fichier.
     */
    void autoSetupSerial(void);

    /**
     * @brief  Fonction qui sert à activer / désactiver les boutons en fonction d'une connexion ou non à un port série.
     */
    void boutonEnabler();

    /**
     * @brief       Fonction qui modifie la variable boutonState et l'affichage sur le bouton en fonction de la valeur des sliders.
     * @param data  Valeurs des sliders.
     */
    void boutonManage(int value);

    /**
     * @brief  Fonction de traitement des données et commandes reçues.
     */
    void execRxCommand(void);

    /**
     * @brief  Fonction d'envoie des commandes sur le port série.
     */
    void execTxCommand();

    /**
     * @brief       Fonction qui remplis la structure de donnés avec les paramètres correspondants qui lui sont envoyés en paramètre par la fonction usartRemRxData.
     * @n           Le switch case commence à l'état WAIT qui attend la réception de "<".
     * @n           RXDATA place alors les donnés reçus dans l'union de structure jusqu'à ce que la dernière donnée soit reçue.
     *              (ici, la longueur de la trame à été spécifié manuellement à 7 puisque nous n'envoyons pas l'octet qui contient la longueur de la trame.).
     * @n           Finalement, VALIDATE s'assure que la trame se termine par ">"
     * @param data  Octet reçu par la fonction usartRemRxData.
     */
    void parseRXData(uint8_t data);
};
#endif // MAINWINDOW_H
