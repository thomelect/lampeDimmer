/*
 * Fichier: mainwindow.h
 * Auteur: Thomas Desrosiers
 * Date: 2021 03 23
 * Desc.: Laboratoire #5 du cours d'intégration de systèmes.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QPixmap>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include "setupserialdialog.h"

#define _MAX_RXDATASIZE_ 16

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    // enums communication

    enum RX_STATES
    {
        WAIT,
        RXSIZE,
        RXCOMMANDE,
        RXDATA,
        VALIDATE
    };
    enum RX_COMMANDES
    {
        VAL_POT
    };
    enum TX_COMMANDES
    {
        GET_ETAT,
        SEND_VAL,
        SEND_SLEEP_MODE
    };
    RX_STATES rxState;
    RX_COMMANDES rxCommande;

    QByteArray tmpRxData;
    uint8_t rxDataSize;
    uint8_t rxDataCnt;
    uint8_t rxData[_MAX_RXDATASIZE_];
    uint16_t rxErrorCommCnt;
    uint8_t valuePot;
    uint8_t intensite;
    bool serialRxIn;
    bool boutonState;

    QPixmap *pixmapOff();
    QIcon *ButtonIcon();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    /**
     * @brief  Fonction de lecture du port série..
     */
    void readSerialData(void);

    /**
     * @brief        Fonction appelée lorsque l'utilisateur choisi une option dans la liste.
     * @param index  Index des options de la liste.
     */
    void on_comboBoxSleep_activated(int index);

    /**
     * @brief  Fonction appelée lorsque la valeur du "potentiomètre" est changée.
     */
    void on_dialIntensite_valueChanged(void);

    /**
     * @brief  Fonction appelée lorsque la valeur du "slider" est changée.
     */
    void on_horizontalSliderIntensite_valueChanged(void);

    /**
     * @brief  Fonction appelée lorsque le bouton on/off est appuyé.
     */
    void on_pushBottonOnOff_pressed();

    /**
     * @brief  Fonction appelée lorsque le bouton on/off est relachée.
     */
    void on_pushBottonOnOff_released();

private:
    /*//////////////////-Communication série-///////////////////*/
    /**
     * @brief  Fonction utilisée afin de créer le menu "Outils".
     */
    void createMenus(void);
    QMenu *toolsMenu;
    QAction *setupSerialAct;
    QAction *setupLumiere;
    QAction *setupInterface;

    /**
     * @brief  Fonction utilisée afin de gérer la fenêtre de connexion.
     */
    void setupSerial(void);
    QSerialPort *serial;
    QTimer *timer;
    Ui::MainWindow *ui;

    /**
     * @brief       Fonction d'envoie sur le port série.
     * @param cmd   Commande envoyée
     * @param data  Donnée à envoyer.
     */
    void sendSerialData(uint8_t cmd, uint8_t data = 0);

    /*//////////////////////////////////////////////////////////*/

    /*///////////////-Protocole de communication-////////////////*/

    /**
     * @brief  Fonction de traitement des données et commandes reçues.
     */
    void execRxCommand(void);

    /**
     * @brief       Fonction qui remplis la structure de donnés avec les paramètres correspondants qui lui sont envoyés en paramètre par la fonction usartRemRxData.
     *              Le switch case commence à l'état WAIT qui attend la réception de "<".
     *              RXDATA place alors les donnés reçus dans l'union de structure jusqu'à ce que la dernière donnée soit reçue.
     *              (ici, la longueur de la trame à été spécifié manuellement à 7 puisque nous n'envoyons pas l'octet qui contient la longueur de la trame.).
     *              Finalement, VALIDATE s'assure que la trame se termine par ">"
     * @param  data Octet reçu par la fonction usartRemRxData.
     */
    void parseRXData(uint8_t data);

    /*//////////////////////////////////////////////////////////*/

    /*/////////////////////-Autres fonctions-/////////////////////*/

    /**
     * @brief       Fonction qui modifie la variable boutonState et l'affichage sur le bouton en fonction de la valeur des sliders.
     * @param  data Valeurs des sliders.
     */
    void boutonManage(int value);
};
#endif // MAINWINDOW_H
