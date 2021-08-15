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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    /**
    * @brief  Fonction de lecture du port série..
    */
    void readSerialData();

    void on_comboBoxSleep_activated(int index);

    void on_dialIntensite_valueChanged(void);

    void on_horizontalSliderIntensite_valueChanged(void);

    void on_pushBottonOnOff_clicked();

private:

    /*//////////////////-Communication série-///////////////////*/

    void createMenus();
    QMenu *toolsMenu;
    QAction *setupSerialAct;

    void setupSerial();
    QSerialPort *serial;
    QTimer *timer;
    Ui::MainWindow *ui;

    /**
    * @brief  Fonction d'envoie sur le port série..
    */
    void sendSerialData();

    /*///////////////-Protocole de communication-////////////////*/

    /**
    *@brief  Fonction de traitement des données et commandes reçues.
    */
    void execRxCommand();

    /**
    *@brief  Fonction qui remplis la structure de donnés avec les paramètres correspondants qui lui sont envoyés en paramètre par la fonction usartRemRxData. Le switch case commence à l'état WAIT qui attend la réception de "<". RXDATA place alors les donnés reçus dans l'union de structure jusqu'à ce que la dernière donnée (ici, la longueur de la trame à été spécifié manuellement à 7 puisque nous n'envoyons pas l'octet qui contient la longueur de la trame. Finalement, VALIDATE s'assure que la trame se termine par ">"
    *@param  data L'octet reçu par la fonction usartRemRxData
    */
    void parseRXData(uint8_t data);

    #define _MAX_RXDATASIZE_    16
    #define _SEQ_SIZE_          10

    enum RX_STATES {WAIT, RXSIZE, RXCOMMANDE, RXDATA, VALIDATE};
    enum RX_COMMANDES {VAL_POT};
    enum TX_COMMANDES {GET_ETAT, SEND_VAL, SEND_SLEEP_MODE};
    RX_STATES rxState;
    RX_COMMANDES rxCommande;
    QByteArray tmpRxData;
    uint8_t rxDataSize;
    uint8_t rxDataCnt;
    uint8_t rxData[_MAX_RXDATASIZE_];
    uint16_t rxErrorCommCnt;
    uint8_t seqCnt;

    /*/////////////////////-Autres fonctions-/////////////////////*/

    /**
    *@brief  Fonction qui modifie la variable boutonState et l'affichage sur le bouton en fonction de la valeur des sliders.
    *@param  data Valeurs des sliders.
    */
    void boutonManage(int value);

    uint8_t intensite = 0;
    bool boutonState = 1;

    QPixmap *pixmapOff();
    QIcon *ButtonIcon();

};
#endif // MAINWINDOW_H