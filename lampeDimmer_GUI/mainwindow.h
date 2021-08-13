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

    void boutonManage(int value);

private:
    void createMenus();
    QMenu *toolsMenu;
    QAction *setupSerialAct;

    void setupSerial();
    QSerialPort *serial;
    Ui::MainWindow *ui;

    void sendSerialData();

};
#endif // MAINWINDOW_H
