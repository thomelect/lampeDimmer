#ifndef SETUPPREFERENCEDIALOG_H
#define SETUPPREFERENCEDIALOG_H

#include <QDialog>
#include <QDir>
#include <QtSerialPort/QSerialPort>
#include <QSettings>

namespace Ui
{
    class SetupPreferenceDialog;
}

class SetupPreferenceDialog : public QDialog
{
    Q_OBJECT

public:
    SetupPreferenceDialog(QDialog *parent = nullptr);
    ~SetupPreferenceDialog();

private slots:
    void on_btAnnuler_clicked();

    void on_btEnregistre_clicked();

    void on_cbOption_4_clicked();

    void on_pbReboot_clicked();

    void on_cbOption_2_clicked();

private:
    /* Déclarations variables: */
    bool needReboot;

    /*Déclarations objets : */
    QSettings *settingsPref;
    Ui::SetupPreferenceDialog *ui;

private:
    void boutonManage(void);

    void setAppToStartAutomatically(bool startAutomatically);

    QString windowsAppPath();
};

#endif // SETUPPREFERENCEDIALOG_H
