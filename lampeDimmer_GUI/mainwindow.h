/*
 * Fichier: mainwindow.h
 * Auteur: Thomas Desrosiers
 * Date: 2021 03 23
 * Desc.: Laboratoire #5 du cours d'intégration de systèmes.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
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
    void readSerialData();

    void on_horizontalSliderIntensite_valueChanged(void);

    void on_dialIntensite_valueChanged(void);

    void on_pushBottonOnOff_clicked();

//    void sendGetState();

    void execRxCommand();

    void parseRXData(uint8_t data);

    void boutonManage(int value);

    void on_comboBoxSleep_activated(int index);

private:

#define _MAX_RXDATASIZE_    16
#define _SEQ_SIZE_          10

    void createMenus();
    QMenu *toolsMenu;
    QAction *setupSerialAct;

    void setupSerial();
    QSerialPort *serial;
    QTimer *timer;
    Ui::MainWindow *ui;

    void sendSerialData();

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

};
#endif // MAINWINDOW_H
