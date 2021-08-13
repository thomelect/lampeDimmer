/**
 * @file       setupserialdialog.h
 * @brief      Classe qui affiche une boite de dialogue pour configurer le port série
 * @author     Marc Juneau
 * @version    0.01
 * @date       8 février 2019
 */
#ifndef SETUPSERIALDIALOG_H
#define SETUPSERIALDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>

namespace Ui {
class SetupSerialDialog;
}

class SetupSerialDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe SetupSerialDialog
     * @param s Port série à configurer
     */
    explicit SetupSerialDialog(QSerialPort * s);
    ~SetupSerialDialog();

private slots:
    /**
     * @brief Lorsque le bouton Scan est appuyé
     * Fait la mise à jour de la liste des ports disponibles.
     * Si le port série était connecté, il est automatiquement fermé.
     */
    void on_btActualiser_clicked();

    /**
     * @brief Lorsque le bouton Connecter est appuyé
     * Essaie de connecter le port série sélectionné dans la liste.
     * Des messages sont envoyés dans vers QDebug
     */
    void on_btConnection_clicked();

    void on_pbOK_clicked();

    void on_pbCANCEL_clicked();

private:
    QSerialPort *serial;
    Ui::SetupSerialDialog *ui;
    const uint32_t BAUD_RATE[7] = {9600,19200,115200,250000,921600,1000000,2000000};
};

#endif // SETUPSERIALDIALOG_H
