/**
 * @file       setupserialdialog.h
 * @brief      Classe qui affiche une boite de dialogue pour configurer le port série
 * @author     Base : Marc Juneau
 * @author     Adaptation : Thomas Desrosiers
 * @version    0.01
 * @date       8 février 2019
 */
#ifndef SETUPSERIALDIALOG_H
#define SETUPSERIALDIALOG_H

#include "savereadfile.h"
#include <QDialog>
#include <QtSerialPort/QSerialPort>

#define DEFAULT_BAUD_RATE "1000000"
#define DEFAULT_PORT_DESC "Périphérique série USB"
//"USB-SERIAL CH340"
//"Périphérique série USB"

namespace Ui
{
    class SetupSerialDialog;
}

class SetupSerialDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief    Constructeur de la classe SetupSerialDialog.
     * @param s  Port série à configurer.
     */
    explicit SetupSerialDialog(QSerialPort *s, void *shared);
    ~SetupSerialDialog();

private slots:
    /**
     * @brief  Fonction appelée lorsque le bouton Scan est appuyé.
     * @li     Fait la mise à jour de la liste des ports disponibles.
     * @li     Si le port série est connecté, il utilisé comme un bouton "déconnecter".
     */
    void on_btActualiser_clicked();

    /**
     * @brief  Fonction appelée lorsque le bouton "Annuler" est appuyé.
     *         La fenêtre est alors fermée.
     */
    void on_btAnnuler_clicked();

    /**
     * @brief  Fonction appelée lorsque le bouton Connecter est appuyé.
     * @li     Essaie de connecter le port série sélectionné dans la liste.
     * @li     Des messages sont envoyés dans vers QDebug.
     * @li     Si le port série est connecté, le bouton est désactivé.
     */
    void on_btConnexion_clicked();

    /**
     * @brief  Fonction appelée lorsque le bouton "Ok" est appuyé.
     *         La fenêtre est alors fermée.
     */
    void on_btOk_clicked();

private:
    QSerialPort *serial;
    SaveReadFile *saveRead;
    QString *connectInfo;
    QString connectInfoCom;
    Ui::SetupSerialDialog *ui;
    const uint32_t BAUD_RATE[7] = {9600, 19200, 115200, 250000, 921600, 1000000, 2000000};
};

#endif // SETUPSERIALDIALOG_H
